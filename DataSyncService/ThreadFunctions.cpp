
#include "StdAfx.h"
#include <time.h>
#include "ThreadFunctions.h"
#include "StrUtil.h"
#include "LogUtil.h"
#include "MyDB.h"
#include "OPCClient.h"
#include "SysParams.h"
#include "Constants.h"
#include "ProfileOperator.h"
#include "TimerTask.h"
#include "ContainerUtil.h"

using namespace std;

extern CLogUtil g_Logger;
extern HANDLE	g_hExitEvent;
extern vector<HANDLE>	g_vhThreadExitEvents;
extern BOOL		g_bKeepWork;
CSysParams		g_SysParams;
COPCClient		g_OPCClient;

INT GetTimerTasks(vector<CTimerTask*> &vTasks);
unsigned __stdcall TimerTaskThread(void* pParameter);
void RunTask(LPCTSTR pcszCommand);
void RunTaskTimerly(LPCTSTR pcszCommand, DWORD dwInterval);
void RunTaskAtFixedTime(LPCTSTR pcszCommand, tm &time);

INT Init()
{
	// Get system parameters from database
	INT nRet = g_SysParams.RefreshSysParams();
	if (ERR_SUCCESS != nRet)
		return nRet;

	// Connect to OPC server
	g_OPCClient.Clear();
	LPCWSTR pProgID = g_SysParams.GetOPCServerProgID();
	if (!pProgID || (wcslen(pProgID) < 1))
	{
		g_Logger.ForceLog(_T("Error calling OPCClient::Connect(), the OPC server program ID is NULL."));
		return E_INVALIDARG;
	}

	IOPCServer *pServer = NULL;	
	LPCWSTR pRemoteMachine = g_SysParams.GetRemoteMachine();
	DWORD dwLen = wcslen(pRemoteMachine);
	if (pRemoteMachine && (dwLen > 0))		// Connect to remote OPC server
	{
		COSERVERINFO CoServerInfo;
		ZeroMemory(&CoServerInfo, sizeof(COSERVERINFO));
		
		CoServerInfo.pwszName = new wchar_t[dwLen + 1];
		wcscpy_s(CoServerInfo.pwszName, dwLen + 1, pRemoteMachine);
		pServer = g_OPCClient.Connect(pProgID, &CoServerInfo);
		delete [] CoServerInfo.pwszName;
	}
	else													// Connect to local OPC server
	{
		pServer = g_OPCClient.Connect(pProgID, NULL);
	}

	if (!pServer)
	{
		g_Logger.VForceLog(L"Failed to connect OPC server [%s]", pProgID);
		return -1;
	}

	// Add group to OPC server
	wchar_t wszGroup[50] = { L'\0' };
	swprintf_s(wszGroup, sizeof(wszGroup) / sizeof(wszGroup[0]), L"_Group_%d", GetCurrentThreadId());

	LPGROUPINFO pGroup = new GROUPINFO(wszGroup);
	if (!g_OPCClient.AddGroup(pGroup, TRUE))
	{
		g_OPCClient.Disconnect();
		g_Logger.VForceLog(L"Failed to add group [%s] to OPC server [%s]", wszGroup, pProgID);
		return -2;
	}

	// Add items to group
	vector<LPITEMINFO> vItems;
	INT nItemCount = g_SysParams.GetItemList(vItems);
	if (nItemCount < 0)
	{
		g_OPCClient.Disconnect();
		g_Logger.VForceLog(L"Failed to call CSysParams.GetItemList(). Return=%d", nItemCount);
		return -3;
	}
	else if (0 == nItemCount)
	{
		g_OPCClient.Disconnect();
		g_Logger.VForceLog(L"No items found in database. No action required.");
		return -4;
	}

	INT nAddedItems = g_OPCClient.AddItems(vItems);
	if (nAddedItems < 0)
	{
		g_OPCClient.Disconnect();
		g_Logger.VForceLog(L"Failed to call OPCClient.AddItems(). Return=%d", nItemCount);
		return -3;
	}
	else if (0 == nAddedItems)
	{
		g_OPCClient.Disconnect();
		g_Logger.VForceLog(L"Failed to add item group [%s]. No action required.", wszGroup);
		return -4;
	}

	TString msg;
	for (vector<LPITEMINFO>::const_iterator it = vItems.begin(); it != vItems.end(); it++)
	{
		LPITEMINFO p = *it;		
		msg.append(p->pItemID).append(_T(","));
		delete p;
	}
	g_Logger.VForceLog(_T("[%d] item(s) found in database, [%d] item(s) were added successfully.\n%s"), nItemCount, nAddedItems, msg.c_str());

	return nAddedItems;
}

unsigned __stdcall OPCDataSyncThread(void*)
{
	HANDLE hExitEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	g_vhThreadExitEvents.push_back(hExitEvent);

	DWORD dwThreadID = GetCurrentThreadId();
	g_Logger.VForceLog(_T("[OPCDataSyncThread:%d] Thread started."), dwThreadID);

	//// Delete all records in table ItemLatestStatus each time the service is started
	//CMyDB myDB;
	//myDB.RemoveAllItems();
	//myDB.Disconnect();

	BOOL bLastDBConnecctFlag = FALSE;
	BOOL bLastOPCConnectFlag = FALSE;
	CMyDB db;
	while (TRUE == g_bKeepWork)
	{
		try
		{			
			if (db.Connect())
			{
				if (!bLastDBConnecctFlag)
					g_Logger.VForceLog(_T("[OPCDataSyncThread:%d] Database connected."), dwThreadID);

				bLastDBConnecctFlag = TRUE;
				if (!g_OPCClient.IsConnected())
				{
					INT nRet = Init();
					if (nRet < 0)
					{
						if (bLastOPCConnectFlag)
							g_Logger.VForceLog(_T("[OPCDataSyncThread:%d] Init() failed, exit thread. Error Code=%d"), dwThreadID, nRet);

						throw E_CONNECTION_BROKE;
					}
										
					if (!bLastOPCConnectFlag)
						g_Logger.VForceLog(_T("[OPCDataSyncThread]:%d OPC Server connected."), dwThreadID);

					bLastOPCConnectFlag = TRUE;
				}

				LPGROUPINFO pGroup = g_OPCClient.GetGroup();
				if (pGroup)
				{
					g_OPCClient.SetDBPtr(&db);
					vector<COPCItemDef*> *pvList = g_OPCClient.GetItems();
					int nItemCnt = pvList->size();
					int nRet = g_OPCClient.ReadItemValue(pvList, TRUE);
					if (nRet != nItemCnt)
					{
						g_Logger.VForceLog(_T("[OPCDataSyncThread:%d] %d item(s) to be read, but only %d item(s) read successfully."), dwThreadID, nItemCnt, nRet);
					}
				}	// end if (pGroup)

				if (!g_SysParams.IsKeepDbConnection())
				{
					db.Disconnect();
				}
			}
			else		// Failed to connect to db
			{
				if (bLastDBConnecctFlag)
					g_Logger.VForceLog(_T("[OPCDataSyncThread:%d] Cannot connect to database, sleep and try again. This log won't be output again until next time connected to database."), dwThreadID);

				bLastDBConnecctFlag = FALSE;
			}
		}
		catch (LPCTSTR pMsg) 
		{
			g_Logger.ForceLog(pMsg);
		}
		catch (INT nCode)		// Throw int only when the connection is broken
		{
			if (E_CONNECTION_BROKE == nCode)
				g_Logger.VForceLog(L"[OPCDataSyncThread:%d] The connection to OPC server [%s] break down. Will re-connect.pin", dwThreadID, g_SysParams.GetOPCServerProgID());
			else
				g_Logger.VForceLog(_T("[OPCDataSyncThread:%d] Error occurred during the read operation, sleep and try again. Return=%d"), dwThreadID, nCode);
		}
		catch (...)
		{
			g_Logger.VForceLog(_T("[OPCDataSyncThread:%d] Error occurred during the read operation, sleep and try again."), dwThreadID);			
		}

		// Sleep
		if (TRUE == g_bKeepWork)
		{
			// To make sure the thread can exit immediately when the application is about to exit
			if (bLastDBConnecctFlag)	// Should not refresh data from database since cannot connect to db
			{
				if (g_SysParams.IsKeepDbConnection())
				{
					g_SysParams.RefreshSysParams(db);
				}
				else
				{
					g_SysParams.RefreshSysParams();
				}
			}
			if (WaitForSingleObject(g_hExitEvent, g_SysParams.GetQueryInterval()) != WAIT_TIMEOUT)
			{
				break;
			}
		}

	}	// end while (TRUE == g_bKeepWork)

	g_Logger.VForceLog(_T("[OPCDataSyncThread:%d] Thread exit."), dwThreadID);
	SetEvent(hExitEvent);
	return 0;
}

unsigned __stdcall TimerTaskThread(void* pParameter)
{
	HANDLE hExitEvent = NULL;
	DWORD dwThreadID = GetCurrentThreadId();
	CTimerTask *pTask = (CTimerTask*)pParameter;

	__try
	{
		if ((!pTask) || !pTask->m_bEnabled)
		{
			g_Logger.VForceLog(_T("[TimerTaskThread:%d] Thread will not start since the task is NULL or disabled."), dwThreadID);
			return -1;
		}

		hExitEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		g_vhThreadExitEvents.push_back(hExitEvent);

		TCHAR szTimeStr[30] = { _T('\0') };
		tm *pTime = pTask->GetRunAtFixedTimePtr();
		if (pTime)
		{
			_tcsftime(szTimeStr, sizeof(szTimeStr) / sizeof(szTimeStr[0]), _T("Run at time: %H:%M:%S"), pTime);
		}
		g_Logger.VForceLog(_T("[TimerTaskThread:%d] Thread started for timer task [%s], interval=%d. %s")
			, dwThreadID
			, pTask->m_szName.c_str()
			, pTask->GetInterval()
			, szTimeStr);

		if (pTime)		// Run at fixed time
		{
			RunTaskAtFixedTime(pTask->m_szRun.c_str(), *pTime);
		}
		else
		{
			// Run one time only
			if (pTask->GetInterval() <= 0)
			{
				RunTask(pTask->m_szRun.c_str());
				return 0;
			}

			// Run timerly
			RunTaskTimerly(pTask->m_szRun.c_str(), pTask->GetInterval() * 1000);
		}

		return 0;
	}
	__finally
	{
		if (pTask)
		{
			delete pTask;
		}

		g_Logger.VForceLog(_T("[TimerTaskThread:%d] Thread exit."), dwThreadID);
		if (hExitEvent)
			SetEvent(hExitEvent);
	}
}

/**************************************************************************
* Get the tasks to be timerly ran from Windows Registry
* and setup thread for running them
*************************************************************************/
INT SetupTimelyTasks()
{
	vector<CTimerTask*> vTasks;
	GetTimerTasks(vTasks);

	INT nTasks = 0;
	for (vector<CTimerTask*>::const_iterator vi = vTasks.begin(); vi != vTasks.end(); vi++)
	{
		CTimerTask *pTask = *vi;
		if (!pTask)
			continue;

		if (!pTask->m_bEnabled)
		{
			delete pTask;
			continue;
		}

		nTasks++;
		// The pointer to task will be releasd before thread exit
		_beginthreadex(NULL, 0, &TimerTaskThread, (void*)(pTask), 0, NULL);
	}

	return nTasks;
}

void RunTask(LPCTSTR pcszCommand)
{
	DWORD dwThreadID = GetCurrentThreadId();
	try
	{
		INT nRet = _tsystem(pcszCommand);
		if (nRet != 0)
			throw nRet;
	}
	catch (INT nCode)		// Throw int only when the connection is broken
	{
		g_Logger.VForceLog(_T("[TimerTaskThread:%d] Error occurred in the timer task, sleep and try again. Return=%d\n\rCommand=[%s]"), dwThreadID, nCode, pcszCommand);
	}
	catch (...)
	{
		g_Logger.VForceLog(_T("[TimerTaskThread:%d] Error occurred in the timer task, sleep and try again.\n\rCommand=[%s]"), dwThreadID, pcszCommand);
	}
}

void RunTaskTimerly(LPCTSTR pcszCommand, DWORD dwInterval)
{
	while (TRUE == g_bKeepWork)
	{
		RunTask(pcszCommand);

		// Sleep
		if (TRUE == g_bKeepWork)
		{
			// To make sure the thread can exit immediately when the application is about to exit
			if (WaitForSingleObject(g_hExitEvent, dwInterval) != WAIT_TIMEOUT)
			{
				break;
			}
		}

	}	// end while (TRUE == g_bKeepWork)
}

void RunTaskAtFixedTime(LPCTSTR pcszCommand, tm &tmFixedTime)
{	
	while (TRUE == g_bKeepWork)
	{
		time_t now = time(nullptr);
		tm localTime;
		localtime_s(&localTime, &now);

		tmFixedTime.tm_year = localTime.tm_year;
		tmFixedTime.tm_mon = localTime.tm_mon;
		tmFixedTime.tm_mday = localTime.tm_mday;
		time_t target = mktime(&tmFixedTime);

		DOUBLE seconds = difftime(target, now);
		if (seconds < 0)
		{
			// add one day		
			seconds += 24 * 60 * 60;
		}		

		// To make sure the thread can exit immediately when the application is about to exit
		if (WaitForSingleObject(g_hExitEvent, seconds * 1000) != WAIT_TIMEOUT)
		{
			break;
		}

		RunTask(pcszCommand);
	}	// end while (TRUE == g_bKeepWork)
}

/**************************************************************************
* Note: The elements in vector need to be released outside
*
*************************************************************************/
INT GetTimerTasks(vector<CTimerTask*> &vTasks)
{
	ClearVector(vTasks);

	LPCTSTR pPath = CProfileOperator::GetExecutablePath();
	TCHAR szTaskIniFile[MAX_PATH];
	_stprintf_s(szTaskIniFile, MAX_PATH, _T("%s%s"), CProfileOperator::GetExecutablePath(), TimerTasksFileName);
	CProfileOperator po(szTaskIniFile);
	vector<basic_string<TCHAR>> vSections;
	po.GetSections(vSections);

	TCHAR szBuf[1000];
	DWORD dwLen = sizeof(szBuf) / sizeof(szBuf[0]);
	for (vector<basic_string<TCHAR>>::const_iterator vi = vSections.begin(); vi != vSections.end(); vi++)
	{
		CTimerTask *pTask = new CTimerTask();

		basic_string<TCHAR> szSection = *vi;
		pTask->m_szName = szSection;

		po.GetString(szSection.c_str(), _T("Run"), NULL, szBuf, dwLen);
		pTask->m_szRun = szBuf;

		po.GetString(szSection.c_str(), _T("RunAtFixedTime"), NULL, szBuf, dwLen);
		pTask->ParseTimeString(szBuf);

		pTask->SetInterval(po.GetInt(szSection.c_str(), _T("Interval"), 0));				
		pTask->m_bEnabled = po.GetBool(szSection.c_str(), _T("Enable"), FALSE);

		vTasks.push_back(pTask);
	}

	return vTasks.size();
}