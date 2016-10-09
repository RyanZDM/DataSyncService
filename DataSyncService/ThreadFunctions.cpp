
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

// *** Global varibles ***************************************************/
extern CLogUtil g_Logger;
extern HANDLE	g_hExitEvent;
extern vector<HANDLE>	g_vhThreadExitEvents;
extern BOOL		g_bKeepWork;
CSysParams		g_SysParams;
// ***********************************************************************/

// *** Methods Declaration ***********************************************/
INT GetTimerTasks(vector<CTimerTask*> &vTasks);
unsigned __stdcall TimerTaskThread(void* pParameter);
void RunTask(LPCTSTR pcszCommand);
void RunTaskTimerly(LPCTSTR pcszCommand, DWORD dwInterval);
void RunTaskAtFixedTime(LPCTSTR pcszCommand, tm &time);
INT Init(CDBUtil &db);
// ***********************************************************************/

INT Init(CDBUtil &db, COPCClient &OPCClient)
{
	// Get system parameters from database
	INT nRet = g_SysParams.RefreshSysParams(db);
	if (ERR_SUCCESS != nRet)
		return nRet;

	// Connect to OPC server
	OPCClient.Clear();
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
		pServer = OPCClient.Connect(pProgID, &CoServerInfo);
		delete [] CoServerInfo.pwszName;
	}
	else													// Connect to local OPC server
	{
		pServer = OPCClient.Connect(pProgID, NULL);
	}
	
	if (!pServer)
		return -1;

	g_Logger.VForceLog(L"%d: Connected to OPC server [%s].", GetCurrentThreadId(), pProgID);

	// Add group to OPC server
	wchar_t wszGroup[50] = { L'\0' };
	swprintf_s(wszGroup, sizeof(wszGroup) / sizeof(wszGroup[0]), L"_Group_%d", GetCurrentThreadId());

	LPGROUPINFO pGroup = new GROUPINFO(wszGroup);
	if (!OPCClient.AddGroup(pGroup, TRUE))
	{
		OPCClient.Disconnect();
		g_Logger.VForceLog(L"Failed to add group [%s] to OPC server [%s]", wszGroup, pProgID);
		return -2;
	}

	// Get items to be monitored from datbase and add them into group
	vector<LPITEMINFO> vItems;
	INT nItemCount = g_SysParams.GetItemList(vItems);
	if (nItemCount < 0)
	{
		OPCClient.Disconnect();
		g_Logger.VForceLog(L"Failed to call CSysParams.GetItemList(). Return=%d", nItemCount);
		return -3;
	}
	else if (0 == nItemCount)
	{
		OPCClient.Disconnect();
		g_Logger.VForceLog(L"No items found in database. No action required.");
		return -4;
	}

	INT nAddedItems = OPCClient.AddItems(vItems);
	if (nAddedItems < 0)
	{
		OPCClient.Disconnect();
		g_Logger.VForceLog(L"Failed to call OPCClient.AddItems(). Return=%d", nItemCount);
		return -3;
	}
	else if (0 == nAddedItems)
	{
		OPCClient.Disconnect();
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

/**************************************************************************
* Get called once the service started.
* Timely get data from predefined OPC server and update it into database.
**************************************************************************/
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
			
	CMyDB db;
	COPCClient OPCClient;
	BOOL bDbConnectionBroke = TRUE;		// Actively disconnect DB does not affect this variable
	BOOL bLastDBConnectFlag = TRUE;
	BOOL bLastOPCConnectFlag = TRUE;
	while (TRUE == g_bKeepWork)
	{
		try
		{			
			if (db.Connect(FALSE))
			{
				// Log only when successfully connnected to db first time or reconnected after a connection broken
				if (bDbConnectionBroke)
					g_Logger.VForceLog(_T("[OPCDataSyncThread:%d] Database connected."), dwThreadID);
				
				bLastDBConnectFlag = TRUE;
				bDbConnectionBroke = FALSE;

				if (!OPCClient.IsConnected())
				{
					try
					{
						INT nRet = Init(db, OPCClient);
						if (nRet < 0)
						{
							throw E_CONNECTION_BROKE;
						}
					}
					catch (LPCTSTR pMsg)
					{
						if (bLastOPCConnectFlag)
						{
							g_Logger.VForceLog(_T("[OPCDataSyncThread:%d] Cannot connect to database, sleep and try again, HRESULT=%x. This log won't be output again until next time connected to database.\n%s")
								, OPCClient.GetLastHResult(), dwThreadID, pMsg);

							// Update this flag indicates that no need log one more time
							bLastDBConnectFlag = FALSE;
						}

						throw E_CONNECTION_BROKE;
					}					

					bLastDBConnectFlag = TRUE;
				}

				LPGROUPINFO pGroup = OPCClient.GetGroup();
				if (pGroup)
				{
					OPCClient.SetDBPtr(&db);
					vector<COPCItemDef*> *pvList = OPCClient.GetItems();
					int nItemCnt = pvList->size();
					int nRet = OPCClient.ReadAndUpdateItemValue(pvList, TRUE);
					if (nRet != nItemCnt)
					{
						if (!db.IsConnected())
						{
							// Means that the db conection is broken
							bDbConnectionBroke = TRUE;
						}

						g_Logger.VForceLog(_T("[OPCDataSyncThread:%d] %d item(s) to be read, but only %d item(s) read successfully.\n%s")
											, dwThreadID, nItemCnt, nRet, db.GetLastErrormsg());
					}
				}	// end if (pGroup)				
			}
			else		// Failed to connect to db
			{
				// Log only when first time failed to connnect to db, no log output if keep get failure while connnecting
				if (bLastDBConnectFlag)
				{
					bLastDBConnectFlag = FALSE;
					g_Logger.VForceLog(_T("[OPCDataSyncThread:%d] Cannot connect to database, sleep and try again, HRESULT=%x. This log won't be output again until next time connected to database.\n%s")
						, dwThreadID, OPCClient.GetLastHResult(), db.GetLastErrormsg());
				}				
			}
		}
		catch (LPCTSTR pMsg) 
		{
			g_Logger.ForceLog(pMsg);
		}
		catch (INT nCode)		// Throw int only when the connection is broken
		{
			if (E_CONNECTION_BROKE == nCode)
			{
				if (bLastOPCConnectFlag)
				{
					bLastOPCConnectFlag = FALSE;
					g_Logger.VForceLog(L"[OPCDataSyncThread:%d] The connection to OPC server [%s] break down. Will re-connect. This log won't be output again until next time connected to OPC server."
										, dwThreadID, g_SysParams.GetOPCServerProgID());
				}
			}
			else
			{
				g_Logger.VForceLog(_T("[OPCDataSyncThread:%d] Error occurred during the read operation, sleep and try again. Return=%d.\n%s")
									, dwThreadID, nCode, db.GetLastErrormsg());
			}
		}
		catch (...)
		{
			g_Logger.VForceLog(_T("[OPCDataSyncThread:%d] Error occurred during the read operation, sleep and try again.\n%s")
									, dwThreadID, db.GetLastErrormsg());
		}

		// Sleep
		if (TRUE == g_bKeepWork)
		{
			// To make sure the thread can exit immediately when the application is about to exit

			if (db.IsConnected())
			{
				g_SysParams.RefreshSysParams(db);
			}

			if (!g_SysParams.IsKeepDbConnection())
			{
				db.Disconnect();
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

/**************************************************************************
* Get called once the service started.
* Get the task list from ini file and create thread for each of them for
* timerly runing.
**************************************************************************/
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

/**************************************************************************
* Get the task definition of timer task from ini file TimerTasksFileName.
* Note: The elements in vector need to be released outside
**************************************************************************/
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

/**************************************************************************
* The thread for executing a timer task
* Note: The elements in vector need to be released outside
**************************************************************************/
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
		g_Logger.VForceLog(_T("[TimerTaskThread:%d] Thread started for timer task [%s], interval=%d. %s, Command=[%s]")
			, dwThreadID
			, pTask->m_szName.c_str()
			, pTask->GetInterval()
			, szTimeStr
			, pTask->m_szRun.c_str());

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
			// The task will be ran first time immediately
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

void RunTask(LPCTSTR pcszCommand)
{
	DWORD dwThreadID = GetCurrentThreadId();	
	try
	{
		time_t begin = time(nullptr);
		INT nRet = _tsystem(pcszCommand);		
		if (nRet != 0)
			throw nRet;

		time_t end = time(nullptr);
		g_Logger.VLog(_T("Ran timer task in %d seconds, command = [%s]"), difftime(end, begin), pcszCommand);
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
		if (WaitForSingleObject(g_hExitEvent, (DWORD)(seconds * 1000)) != WAIT_TIMEOUT)
		{
			break;
		}

		RunTask(pcszCommand);
	}	// end while (TRUE == g_bKeepWork)
}

