
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
#include "TimerTaskManager.h"

using namespace std;

// *** Global varibles ***************************************************/
extern CLogUtil g_Logger;
extern HANDLE	g_hExitEvent;
extern vector<HANDLE>	g_vhThreadExitEvents;
extern BOOL		g_bKeepWork;
CSysParams		g_SysParams;
// ***********************************************************************/

// *** Methods Declaration ***********************************************/
unsigned __stdcall TimerTaskThread(void* pParameter);
void RunTask(LPCTSTR pcszCommand);
void RunTaskTimely(LPCTSTR pcszCommand, DWORD dwInterval);
void RunTaskAtFixedTime(LPCTSTR pcszCommand, tm &tmFixedTime, INT nFixedDay);
INT Init(CDBUtil &db, COPCClient &OPCClient);
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
		g_Logger.VForceLog(_T("[%s] Error calling OPCClient::Connect(), the OPC server program ID is NULL."), __TFUNCTION__);
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
		delete[] CoServerInfo.pwszName;
	}
	else													// Connect to local OPC server
	{
		pServer = OPCClient.Connect(pProgID, NULL);
	}

	if (!pServer)
		return -1;

	g_Logger.VForceLog(L"[%s] Connected to OPC server [%s].", __FUNCTIONW__, pProgID);

	// Add group to OPC server
	wchar_t wszGroup[50] = { L'\0' };
	swprintf_s(wszGroup, sizeof(wszGroup) / sizeof(wszGroup[0]), L"_Group_%d", GetCurrentThreadId());

	LPGROUPINFO pGroup = new GROUPINFO(wszGroup);
	if (!OPCClient.AddGroup(pGroup, TRUE))
	{
		OPCClient.Disconnect();
		g_Logger.VForceLog(L"[%s] Failed to add group [%s] to OPC server [%s]", __FUNCTIONW__, wszGroup, pProgID);
		return -2;
	}

	// Get items to be monitored from datbase and add them into group
	vector<LPITEMINFO> vItems;
	INT nItemCount = g_SysParams.GetItemList(vItems);
	if (nItemCount < 0)
	{
		OPCClient.Disconnect();
		g_Logger.VForceLog(L"[%s] Failed to call CSysParams.GetItemList(). Return=%d", __FUNCTIONW__, nItemCount);
		return -3;
	}
	else if (0 == nItemCount)
	{
		OPCClient.Disconnect();
		g_Logger.VForceLog(L"[%s] No items found in database. No action required.", __FUNCTIONW__);
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
		g_Logger.VForceLog(L"[%s] Failed to add item group [%s]. No action required.", __FUNCTIONW__, wszGroup);
		return -4;
	}

	TString msg;
	for (vector<LPITEMINFO>::const_iterator it = vItems.begin(); it != vItems.end(); it++)
	{
		LPITEMINFO p = *it;
		msg.append(p->pItemID).append(_T("//")).append(p->pAddress).append(_T(","));
		delete p;
	}
	g_Logger.VForceLog(_T("[%s] %d monitor item(s) found in database, %d item(s) were added successfully.\n%s"), __TFUNCTION__, nItemCount, nAddedItems, msg.c_str());

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
		
	g_Logger.VForceLog(_T("[%s] OPCDataSyncThread Thread started."), __TFUNCTION__);

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
					g_Logger.VForceLog(_T("[%s] Database connected."), __TFUNCTION__);

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
							TString szErr;
							CStrUtil::FormatMsg(NULL, szErr, (INT)OPCClient.GetLastHResult());
							g_Logger.VForceLog(_T("[%s] Cannot connect to database, sleep and try again, HRESULT=%x %s. This log won't be output again until next time connected to database.\n%s")
								, __TFUNCTION__, OPCClient.GetLastHResult(), szErr.c_str(), pMsg);

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

						g_Logger.VForceLog(_T("[%s] %d item(s) to be read, but only %d item(s) read successfully.")
							, __TFUNCTION__, nItemCnt, nRet);
					}
				}	// end if (pGroup)				
			}
			else		// Failed to connect to db
			{
				// Log only when first time failed to connnect to db, no log output if keep get failure while connnecting
				if (bLastDBConnectFlag)
				{
					bLastDBConnectFlag = FALSE;
					TString szErr;
					CStrUtil::FormatMsg(NULL, szErr, (INT)OPCClient.GetLastHResult());
					g_Logger.VForceLog(_T("[%s] Cannot connect to database, sleep and try again, HRESULT=%x %s. This log won't be output again until next time connected to database.\n%s")
						, __TFUNCTION__, OPCClient.GetLastHResult(), szErr.c_str(), db.GetLastErrormsg());
				}
			}
		}
		catch (LPCTSTR pMsg)
		{
			g_Logger.VForceLog(_T("[%s] %s"), __TFUNCTION__, pMsg);
		}
		catch (INT nCode)		// Throw int only when the connection is broken
		{
			if (E_CONNECTION_BROKE == nCode)
			{
				if (bLastOPCConnectFlag)
				{
					bLastOPCConnectFlag = FALSE;
					g_Logger.VForceLog(L"[%s] The connection to OPC server [%s] break down. Will re-connect. This log won't be output again until next time connected to OPC server."
						, __FUNCTIONW__, g_SysParams.GetOPCServerProgID());
				}
			}
			else
			{
				g_Logger.VForceLog(_T("[%s] Error occurred during the read operation, sleep and try again. Return=%d.\n%s")
					, __TFUNCTION__, nCode, db.GetLastErrormsg());
			}
		}
		catch (...)
		{
			g_Logger.VForceLog(_T("[%s] Error occurred during the read operation, sleep and try again.\n%s")
				, __TFUNCTION__, db.GetLastErrormsg());
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

	g_Logger.VForceLog(_T("[%s] Thread exit."), __TFUNCTION__);
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
	CTimerTaskManager::GetTimerTasks(vTasks);

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
* The thread for executing a timer task
* Note: The elements in vector need to be released outside
**************************************************************************/
unsigned __stdcall TimerTaskThread(void* pParameter)
{
	HANDLE hExitEvent = NULL;
	CTimerTask *pTask = (CTimerTask*)pParameter;

	__try
	{
		if ((!pTask) || !pTask->m_bEnabled)
		{
			g_Logger.VForceLog(_T("[%s] Thread will not start since the task is NULL or disabled."), __TFUNCTION__);
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
		g_Logger.VForceLog(_T("[%s] Thread started for timer task [%s]."), __TFUNCTION__, pTask->ToString());

		if (pTime)		// Run at fixed time
		{
			RunTaskAtFixedTime(pTask->m_szRun.c_str(), *pTime, pTask->GetFixedDay());
		}
		else
		{
			// Run one time only
			if (pTask->GetInterval() <= 0)
			{
				RunTask(pTask->m_szRun.c_str());
				return 0;
			}

			// Run timely
			RunTaskTimely(pTask->m_szRun.c_str(), pTask->GetInterval() * 1000);
		}

		return 0;
	}
	__finally
	{
		if (pTask)
		{
			delete pTask;
		}

		g_Logger.VForceLog(_T("[%s] Thread exit."), __TFUNCTION__);
		if (hExitEvent)
			SetEvent(hExitEvent);
	}
}

void RunTask(LPCTSTR pcszCommand)
{
	try
	{
		if (!pcszCommand || _tcslen(pcszCommand) == 0)
		{
			g_Logger.VLog(_T("[%s] RunTask failed because the argument is null or empty."), __TFUNCTION__);
			return;
		}

		TCHAR szProgram[MAX_PATH];
		CStrUtil::GetProgramFromCommandString(pcszCommand, szProgram, MAX_PATH);

		TCHAR szCommand[1000] = { _T('\0') };
		if (pcszCommand[0] == _T('"')
			|| (_tcschr(szProgram, _T('\\')) || _tcschr(szProgram, _T('/'))))
		{
			// Do not combine the absoulute path to command if found double quotation marks
			// Do not combine the absoulute path to command if found the char '\'
			_tcscpy_s(szCommand, sizeof(szCommand) / sizeof(szCommand[0]), pcszCommand);
			if (_taccess(szProgram, 0) == -1)
			{
				g_Logger.VForceLog(_T("[%s] The program '%s' does not exist."), __TFUNCTION__, szProgram);
				// Do not exit in case it is in the system path
			}
		}
		else
		{
			// Need to bombine the absolution path
			TCHAR path[MAX_PATH];
			if (!GetModuleFileName(NULL, path, sizeof(path) / sizeof(path[0])))
			{
				_tcscpy_s(szCommand, sizeof(szCommand) / sizeof(szCommand[0]), pcszCommand);
			}
			else
			{
				// Get the argumens
				LPCTSTR pArguments = pcszCommand + _tcslen(szProgram);

				// Remove file name part
				for (INT pos = _tcslen(path) - 1; pos > 0; pos--)
				{
					if (path[pos] == _T('\\') || path[pos] == _T('/'))
					{
						path[pos] = _T('\0');
						break;
					}
				}

				_stprintf_s(szCommand, sizeof(szCommand) / sizeof(szCommand[0]), _T("\"%s\\%s\" %s"), path, szProgram, pArguments);
				CStrUtil::GetProgramFromCommandString(szCommand, szProgram, MAX_PATH);
				if (_taccess(szProgram, 0) == -1)
				{
					g_Logger.VForceLog(_T("[%s] The program '%s' does not exist."), __TFUNCTION__, szProgram);
					// Do not exit in case it is in the system path
				}
			}
		}

		time_t begin = time(nullptr);
		INT nRet = _tsystem(szCommand);
		time_t end = time(nullptr);

		g_Logger.VLog(_T("[%s] Ran timer task in %d seconds, command = [%s], return=%d."), __TFUNCTION__, (INT)difftime(end, begin), szCommand, nRet);
	}
	catch (INT nCode)		// Throw int only when the connection is broken
	{
		g_Logger.VForceLog(_T("[%s] Error occurred in the timer task, sleep and try again. Return=%d\n\rCommand=[%s]"), __TFUNCTION__, nCode, pcszCommand);
	}
	catch (...)
	{
		g_Logger.VForceLog(_T("[%s] Error occurred in the timer task, sleep and try again.\n\rCommand=[%s]"), __TFUNCTION__, pcszCommand);
	}
}

void RunTaskTimely(LPCTSTR pcszCommand, DWORD dwInterval)
{
	while (TRUE == g_bKeepWork)
	{
		// Sleep
		if (dwInterval > 0)
		{
			// To make sure the thread can exit immediately when the application is about to exit
			if (WaitForSingleObject(g_hExitEvent, dwInterval * 1000) != WAIT_TIMEOUT)
			{
				break;
			}
		}

		RunTask(pcszCommand);
	}	// end while (TRUE == g_bKeepWork)
}

void RunTaskAtFixedTime(LPCTSTR pcszCommand, tm &tmFixedTime, INT nFixedDay)
{
	while (TRUE == g_bKeepWork)
	{
		DWORD seconds = CTimerTaskManager::GetWaitSeconds(tmFixedTime, nFixedDay);

		// To make sure the thread can exit immediately when the application is about to exit
		if (WaitForSingleObject(g_hExitEvent, (DWORD)(seconds * 1000)) != WAIT_TIMEOUT)
		{
			break;
		}

		RunTask(pcszCommand);
	}	// end while (TRUE == g_bKeepWork)
}
