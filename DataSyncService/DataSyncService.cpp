// DataSyncService.cpp : Implementation of WinMain


#include "stdafx.h"
#include <vld.h>

#include "resource.h"
#include "DataSyncService_i.h"
#include <stdio.h>
#include <vector>
#include "LogUtil.h"
#include "Constants.h"
#include "ThreadFunctions.h"
#include "ProfileOperator.h"

using namespace std;
using namespace ATL;

// Global variables
CLogUtil		g_Logger(LOG_FILENAME);
HANDLE			g_hExitEvent = NULL;
vector<HANDLE>	g_vhThreadExitEvents;			// All event handles should have signal by default
BOOL			g_bKeepWork = FALSE;			// Indicate if the monitor thread should running continuously

BOOL IfEnableConsoleOutput();
void Cleanup();

class CDataSyncServiceModule : public ATL::CAtlServiceModuleT< CDataSyncServiceModule, IDS_SERVICENAME >
{
public :
	FILE_PRODUCT_VERSION m_Version;

	DECLARE_LIBID(LIBID_DataSyncServiceLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_DATASYNCSERVICE, "{17676828-C6A4-4C22-82A7-12A3F3F1A0E8}")
		HRESULT InitializeSecurity() throw()
		{
			// TODO : Call CoInitializeSecurity and provide the appropriate security settings for your service
			// Suggested - PKT Level Authentication, 
			// Impersonation Level of RPC_C_IMP_LEVEL_IDENTIFY 
			// and an appropriate Non NULL Security Descriptor.
			
			return S_OK;
		};

		// Override this method since we must call the local versioin of method "Install" of derived class
		inline HRESULT RegisterAppId(_In_ bool bService = false) throw()
		{
			if (!Uninstall())
				return E_FAIL;

			HRESULT hr = UpdateRegistryAppId(TRUE);
			if (FAILED(hr))
				return hr;

			ATL::CRegKey keyAppID;
			LONG lRes = keyAppID.Open(HKEY_CLASSES_ROOT, _T("AppID"), KEY_WRITE);
			if (lRes != ERROR_SUCCESS)
				return AtlHresultFromWin32(lRes);

			ATL::CRegKey key;

			lRes = key.Create(keyAppID, GetAppIdT());
			if (lRes != ERROR_SUCCESS)
				return AtlHresultFromWin32(lRes);

			key.DeleteValue(_T("LocalService"));

			if (!bService)
				return S_OK;

			key.SetStringValue(_T("LocalService"), m_szServiceName);

			// Create service
			if (!Install())
				return E_FAIL;
			return S_OK;
		}

		// Override the base class's implementation since we want install a auto start servcie
		BOOL Install() throw()
		{
			if (IsInstalled())
				return TRUE;

			// Get the executable file path
			TCHAR szFilePath[MAX_PATH + _ATL_QUOTES_SPACE];
			DWORD dwFLen = ::GetModuleFileName(NULL, szFilePath + 1, MAX_PATH);
			if (dwFLen == 0 || dwFLen == MAX_PATH)
				return FALSE;

			// Quote the FilePath before calling CreateService
			szFilePath[0] = _T('\"');
			szFilePath[dwFLen + 1] = _T('\"');
			szFilePath[dwFLen + 2] = 0;

			SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
			if (hSCM == NULL)
			{
				TCHAR szBuf[1024];
				if (AtlLoadString(ATL_SERVICE_MANAGER_OPEN_ERROR, szBuf, 1024) == 0)
#ifdef UNICODE
					Checked::wcscpy_s(szBuf, _countof(szBuf), _T("Could not open Service Manager"));
#else
					Checked::strcpy_s(szBuf, _countof(szBuf), _T("Could not open Service Manager"));
#endif
				MessageBox(NULL, szBuf, m_szServiceName, MB_OK);
				return FALSE;
			}

			SC_HANDLE hService = ::CreateService(
				hSCM, m_szServiceName, DS_SERVICE_DISPLAYNAME,
				SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS,
				SERVICE_AUTO_START, SERVICE_ERROR_NORMAL,
				szFilePath, NULL, NULL, _T("RPCSS\0"), NULL, NULL);

			if (hService == NULL)
			{
				::CloseServiceHandle(hSCM);
				TCHAR szBuf[1024];
				if (AtlLoadString(ATL_SERVICE_START_ERROR, szBuf, 1024) == 0)
#ifdef UNICODE
					Checked::wcscpy_s(szBuf, _countof(szBuf), _T("Could not start service"));
#else
					Checked::strcpy_s(szBuf, _countof(szBuf), _T("Could not start service"));
#endif
				MessageBox(NULL, szBuf, m_szServiceName, MB_OK);
				return FALSE;
			}

			SERVICE_DESCRIPTION srv_descript;
			srv_descript.lpDescription = DS_SERVICE_DESCRIPTION;
			ChangeServiceConfig2(hService, SERVICE_CONFIG_DESCRIPTION, &srv_descript);

			::CloseServiceHandle(hService);
			::CloseServiceHandle(hSCM);
			return TRUE;
		}

		// Override, now know why InitializeCom failed
		HRESULT Start(_In_ int nShowCmd) throw()
		{
			// Are we Service or Local Server
			ATL::CRegKey keyAppID;
			LONG lRes = keyAppID.Open(HKEY_CLASSES_ROOT, _T("AppID"), KEY_READ);
			if (lRes != ERROR_SUCCESS)
			{
				m_status.dwWin32ExitCode = lRes;
				return m_status.dwWin32ExitCode;
			}

			ATL::CRegKey key;
			lRes = key.Open(keyAppID, GetAppIdT(), KEY_READ);
			if (lRes != ERROR_SUCCESS)
			{
				m_status.dwWin32ExitCode = lRes;
				return m_status.dwWin32ExitCode;
			}

			TCHAR szValue[MAX_PATH];
			DWORD dwLen = MAX_PATH;
			lRes = key.QueryStringValue(_T("LocalService"), szValue, &dwLen);

			m_bService = FALSE;
			if (lRes == ERROR_SUCCESS)
				m_bService = TRUE;

			if (m_bService)
			{
				SERVICE_TABLE_ENTRY st[] =
				{
					{ m_szServiceName, _ServiceMain },
					{ NULL, NULL }
				};
				if (::StartServiceCtrlDispatcher(st) == 0)
					m_status.dwWin32ExitCode = GetLastError();
				return m_status.dwWin32ExitCode;
			}
			// local server - call Run() directly, rather than
			// from ServiceMain()	
			/*
#ifndef _ATL_NO_COM_SUPPORT
			HRESULT hr = T::InitializeCom();
			if (FAILED(hr))
			{
				// Ignore RPC_E_CHANGED_MODE if CLR is loaded. Error is due to CLR initializing
				// COM and InitializeCOM trying to initialize COM with different flags.
				if (hr != RPC_E_CHANGED_MODE || GetModuleHandle(_T("Mscoree.dll")) == NULL)
				{
					return hr;
				}
			}
			else
			{
				m_bComInitialized = true;
			}
#endif //_ATL_NO_COM_SUPPORT
			*/
			m_status.dwWin32ExitCode = Run(nShowCmd);
			return m_status.dwWin32ExitCode;
		}

		HRESULT Run(_In_ int nShowCmd = SW_HIDE) throw()
		{
			HRESULT hr = S_OK;
						
			hr = PreMessageLoop(nShowCmd);
			
			// **********************************************************************************************************************
			// Code begin:				
			GetProductVersion(m_Version);			
			g_bKeepWork = TRUE;
			g_hExitEvent = CreateEvent(NULL, TRUE, FALSE, NULL);	// The event should always be nonsignaled until the main thread exit.			
			g_Logger.EnableConsoleOutput(IfEnableConsoleOutput());
			g_Logger.VForceLog(_T("\n\n*** The service [%s v%s] started *************************\nExecutable File Version:%s\n")
								, m_szServiceName
								, m_Version.GetProductVersionString()
								, m_Version.GetFileVersionString());

			SetupTimelyTasks();

			//temp
			//OPCDataSyncThread(NULL);
			_beginthreadex(NULL, 0, &OPCDataSyncThread, (void*)(NULL), 0, NULL);
			// Code end.
			// **********************************************************************************************************************/

			// Call RunMessageLoop only if PreMessageLoop returns S_OK.
			if (hr == S_OK)
			{
				RunMessageLoop();
				// temp for normally exit after 1 minute
				//Sleep(60000);
			}

			// **********************************************************************************************************************
			// Code begin:	
			// Tell threads the service is to be stopped
			g_bKeepWork = FALSE;
			SetEvent(g_hExitEvent);
			// Code end.
			// **********************************************************************************************************************/

			// Call PostMessageLoop if PreMessageLoop returns success.
			if (SUCCEEDED(hr))
			{
				hr = PostMessageLoop();
			}

			// **********************************************************************************************************************
			// Code begin:	
			Cleanup();
			g_Logger.ForceLog(_T("\n*** The service stoped *******************************\n"));
			// Code end.
			// **********************************************************************************************************************/

			ATLASSERT(SUCCEEDED(hr));
			return hr;
		}

		// Override, not knwo why RegisterClassObjects return S_FALSE, we must call InterlockedCompareExchange to let Windows know the service started
		HRESULT PreMessageLoop(_In_ int nShowCmd) throw()
		{
			HRESULT hr = S_OK;

			if (m_bService)
			{
#ifndef _ATL_NO_COM_SUPPORT
				hr = InitializeSecurity();

				if (FAILED(hr))
				{
					return hr;
				}
#endif	// _ATL_NO_COM_SUPPORT
			}

#ifndef _ATL_NO_COM_SUPPORT

			// NOTE: much of this code is duplicated in CAtlExeModuleT::PreMessageLoop above, so if
			// you make changes to either method make sure to change both methods (if necessary).

			hr = RegisterClassObjects(CLSCTX_LOCAL_SERVER, REGCLS_MULTIPLEUSE | REGCLS_SUSPENDED);
			if (FAILED(hr))
			{
				return hr;
			}

			// temp, not knwo why return S_FALSE, we must call InterlockedCompareExchange to let Windows know the service started
			hr = S_OK;

			if (hr == S_OK)
			{
				if (m_bDelayShutdown)
				{
					CHandle h(StartMonitor());
					if (h.m_h == NULL)
					{
						hr = E_FAIL;
					}
					else
					{
						if (m_bService)
						{
							// Make sure that service was not stoped during initialization
							if (::InterlockedCompareExchange(&m_status.dwCurrentState, SERVICE_RUNNING, SERVICE_START_PENDING) == SERVICE_START_PENDING)
							{
								LogEvent(_T("Service started/resumed"));
								::SetServiceStatus(m_hServiceStatus, &m_status);
							}
						}

						hr = CoResumeClassObjects();
						ATLASSERT(SUCCEEDED(hr));
						if (FAILED(hr))
						{
							::SetEvent(m_hEventShutdown); // tell monitor to shutdown
							::WaitForSingleObject(h, m_dwTimeOut * 2);
						}
					}
				}
				else
				{
					if (m_bService)
					{
						// Make sure that service was not stoped during initialization
						if (::InterlockedCompareExchange(&m_status.dwCurrentState, SERVICE_RUNNING, SERVICE_START_PENDING) == SERVICE_START_PENDING)
						{
							LogEvent(_T("Service started/resumed"));
							::SetServiceStatus(m_hServiceStatus, &m_status);
						}
					}

					hr = CoResumeClassObjects();
					ATLASSERT(SUCCEEDED(hr));
				}

				if (FAILED(hr))
					RevokeClassObjects();
			}
			else
			{
				m_bDelayShutdown = false;
			}

#else	// _ATL_NO_COM_SUPPORT

			if (m_bService)
			{
				// Make sure that service was not stoped during initialization
				if (::InterlockedCompareExchange(&m_status.dwCurrentState, SERVICE_RUNNING, SERVICE_START_PENDING) == SERVICE_START_PENDING)
				{
					LogEvent(_T("Service started/resumed"));
					::SetServiceStatus(m_hServiceStatus, &m_status);
				}
			}

#endif	// _ATL_NO_COM_SUPPORT

			ATLASSERT(SUCCEEDED(hr));
			return hr;
		}
};

CDataSyncServiceModule _AtlModule;

//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, 
								LPTSTR /*lpCmdLine*/, int nShowCmd)
{
	return _AtlModule.WinMain(nShowCmd);
}

BOOL IfEnableConsoleOutput()
{
	CProfileOperator po;	
	return po.GetBool(_T("Log"), _T("EnableConsoleOutput"), FALSE);
}

void Cleanup()
{
	if (g_vhThreadExitEvents.size() > 0)
	{
		HANDLE *pHandles = new HANDLE[g_vhThreadExitEvents.size()];
		for (INT i = 0; i < g_vhThreadExitEvents.size(); i++)				{
			pHandles[i] = g_vhThreadExitEvents[i];
		}

		// Wait for all threads exist
		if (WaitForMultipleObjects(g_vhThreadExitEvents.size(), pHandles, TRUE, 3000) == WAIT_TIMEOUT)
		{
			g_Logger.ForceLog(_T("Some threads have no enough time to exit when service stopped."));
		}

		for (INT i = 0; i < g_vhThreadExitEvents.size(); i++)
		{
			CloseHandle(g_vhThreadExitEvents[i]);
		}

		delete[] pHandles;
		g_vhThreadExitEvents.clear();
	}
	CloseHandle(g_hExitEvent);
	g_hExitEvent = NULL;
}