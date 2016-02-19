  // LogUtil.cpp: implementation of the CLogUtil class.
//
//////////////////////////////////////////////////////////////////////
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <direct.h>
#include <atlbase.h>
#include <atlconv.h>
#include <process.h>
#include "LogUtil.h"
#include "StrUtil.h"

#include <shellapi.h>
#pragma comment(lib, "shell32.lib")
#include <Winver.h>
#pragma comment(lib, "Version.lib")

#ifndef ARRAYSIZE
#define ARRAYSIZE(A) (sizeof(A)/sizeof((A)[0]))
#endif

#define VALIDATE	if (!m_bEnabled) return TRUE;
const TCHAR *LOG_FLIENAME	= _T("c:\\log\\log.log");

BOOL g_bCheckingLogFileCount = FALSE;

LPCTSTR CInstanceMutex::INSTANCE_MUTEX_NAME = _T("_INSTANCE_MUTEX_NAME_");
UINT CLogUtil::s_nInstanceCount = 0;

void GetProductVersion(FILE_PRODUCT_VERSION &version)
{	
	HRSRC res = FindResource(NULL, MAKEINTRESOURCE(VS_VERSION_INFO), RT_VERSION);
	if (!res)
		return;

	DWORD dwSize = SizeofResource(NULL, res);
	HGLOBAL mem = LoadResource(NULL, res);
	if (!mem)
		return;

	LPVOID pBlock = LockResource(mem);
	if (!pBlock)
		return;

	UINT nLen;
	LPVOID pBuf = NULL;
	BOOL bRet = VerQueryValue(pBlock, _T("\\"), &pBuf, &nLen);
	VS_FIXEDFILEINFO *pVersion = (VS_FIXEDFILEINFO*)pBuf;
	version.wFileVersion1 = HIWORD(pVersion->dwFileVersionMS);
	version.wFileVersion2 = LOWORD(pVersion->dwFileVersionMS);
	version.wFileVersion3 = HIWORD(pVersion->dwFileVersionLS);
	version.wFileVersion4 = LOWORD(pVersion->dwFileVersionLS);
	version.wProductVersion1 = HIWORD(pVersion->dwProductVersionMS);
	version.wProductVersion2 = LOWORD(pVersion->dwProductVersionMS);
	version.wProductVersion3 = HIWORD(pVersion->dwProductVersionLS);
	version.wProductVersion4 = LOWORD(pVersion->dwProductVersionLS);
}

void CheckLogFileCount(LPVOID pFileName)
{	
	typedef std::basic_string<TCHAR> TString;
	LPCTSTR pName = (LPCTSTR)pFileName;
	if (!(!g_bCheckingLogFileCount && pFileName && (_tcslen(pName) > 0)))
		return;
	
	TString szFileName(pName);
	
	TCHAR szFullName[MAX_PATH];
	if (!_tfullpath(szFullName, szFileName.c_str(), MAX_PATH))
		return;

	g_bCheckingLogFileCount = TRUE;
	
	TCHAR szDrive[_MAX_DRIVE];
	TCHAR szDir[_MAX_DIR];
	TCHAR szFname[_MAX_FNAME];
	TCHAR szFNameFilter[_MAX_FNAME];
	TCHAR szExt[_MAX_EXT];
	TCHAR szFilter[MAX_PATH];
	_tsplitpath_s(szFullName, szDrive, _MAX_DRIVE, szDir, _MAX_DIR, szFname, _MAX_FNAME, szExt, _MAX_EXT);
	wsprintf(szFNameFilter, _T("%s%s"), szFname, _T("*"));	// Find file which name is start with the given name
	_tmakepath_s(szFilter, szDrive, szDir, szFNameFilter, szExt);
	
	vector<int> vFileNums;
	DWORD dwTotalFiles = 0;
	_tfinddata_t FindData;
	long int handle = _tfindfirst(szFilter, &FindData);
	if (handle != -1) {		
		DWORD dwNameLen = _tcslen(szFname);
		TCHAR szName[_MAX_FNAME];
		do 
		{
			_tsplitpath_s(FindData.name, NULL, 0, NULL, 0, szName, _MAX_FNAME, NULL, 0);
			if (0 != _tcsicmp(szName, szFname))
			{
				LPCTSTR pNum = szName + dwNameLen;
				if (pNum)
				{
					int nNum = _ttoi(pNum);
					if (nNum > 0)		// It is a number
					{
						vFileNums.push_back(nNum);
						dwTotalFiles++;
					}
				}
			}

		} while(0 == _tfindnext(handle, &FindData));
		
		_findclose(handle);

		if (dwTotalFiles > MAX_LOG_FILES)
		{
			// Remove the older files
			DWORD dwToBeDeleted = dwTotalFiles - MAX_LOG_FILES;
			for (DWORD i = 0; i < dwToBeDeleted; i++)
			{
				wsprintf(szFNameFilter, _T("%s%d"), szFname, vFileNums[i]);
				_tmakepath_s(szFullName, szDrive, szDir, szFNameFilter, szExt);
				DeleteFile(szFullName);
			}

			// Rename the other file's name
			int nOffset = 1;
			TCHAR szNewName[MAX_PATH];
			for (DWORD j = dwToBeDeleted; j < vFileNums.size(); j++)
			{
				wsprintf(szFNameFilter, _T("%s%d"), szFname, vFileNums[j]);
				_tmakepath_s(szFullName, szDrive, szDir, szFNameFilter, szExt);

				wsprintf(szFNameFilter, _T("%s%d"), szFname, nOffset++);
				_tmakepath_s(szNewName, szDrive, szDir, szFNameFilter, szExt);

				MoveFile(szFullName, szNewName);
			}
		}

		vFileNums.clear();
	}

	g_bCheckingLogFileCount = FALSE;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogUtil::CLogUtil(DWORD dwMaxFileSize)
{
	CLogUtil::IncreaseInstanceCount();

	Initialize();
	m_dwMaxFileSize = dwMaxFileSize;
	SetLogFileName(LOG_FLIENAME);
}

CLogUtil::CLogUtil(LPCTSTR pcszLogFileName, DWORD dwMaxFileSize)
{
	CLogUtil::IncreaseInstanceCount();

	Initialize();
	m_dwMaxFileSize = dwMaxFileSize;
	SetLogFileName(pcszLogFileName);
}

CLogUtil::~CLogUtil()
{
	DecreaseInstanceCount();

	if (m_pFile)
		fclose(m_pFile);

	if (m_hEvent)
	{
		SetEvent(m_hEvent);
		CloseHandle(m_hEvent);
	}

}

UINT CLogUtil::IncreaseInstanceCount()
{
	INT nRet = CLogUtil::s_nInstanceCount + 1;
	/*if (m_Mutex.Lock(50))
	{
		__try { 
			nRet = (++CLogUtil::s_nInstanceCount);
		} 
		__finally { 
			// Release ownership of the mutex object
			m_Mutex.Unlock();
		}
	}
	else
	{
		// temp
		MessageBox(NULL, "IncreateInstanceCount failed", "", MB_OK);
	}*/

	return nRet;
}

UINT CLogUtil::DecreaseInstanceCount()
{
	INT nRet = CLogUtil::s_nInstanceCount -1;
	/*if (m_Mutex.Lock(50))
	{
		__try { 
			nRet = (--CLogUtil::s_nInstanceCount);*/
			if (nRet < 1)
			{
				//EnableConsoleOutput(FALSE);
			}
		/*} 
		__finally { 
			// Release ownership of the mutex object
			m_Mutex.Unlock();
		}
	}
	else
	{
		// temp
		char szMsg[100];
		sprintf(szMsg, "DecreaseInstanceCount failed, handle=%d", m_Mutex);
		MessageBox(NULL, szMsg, "", MB_OK);
	}*/

	return nRet;
}

void CLogUtil::Initialize()
{
	m_pFile					= NULL;
	m_dwMilliseconds		= INFINITE;
	m_bNewLineAfterTimeStr	= FALSE;
	m_bShowThreadID			= FALSE;

	// For Tip message
	m_bShowTipMsg			= FALSE;
	m_NotifyData.cbSize				= sizeof(NOTIFYICONDATA);
	m_NotifyData.hWnd				= NULL;
	m_NotifyData.uID				= (UINT)this;
	m_NotifyData.hIcon				= NULL;
	m_NotifyData.uCallbackMessage	= NULL;
	m_NotifyData.uFlags				= NIF_ICON | NIF_TIP | NIF_MESSAGE | 0x00000001/*NIF_INFO*/;	
//	m_NotifyData.dwInfoFlags		= NIIF_INFO;

	TCHAR val[10];

	GetPrivateProfileString(_T("LogCfg"), _T("Enabled"), _T("true"), val, 10, _T("LogCfg.ini"));
	m_bEnabled = ((_tcsicmp(val, _T("true")) == 0) || (_tcscmp(val, _T("1")) == 0));
	GetPrivateProfileString(_T("LogCfg"), _T("AutoShowMsg"), _T("false"), val, 10, _T("LogCfg.ini"));
	m_bAutoShowMsg = ((_tcsicmp(val, _T("true")) == 0) || (_tcscmp(val, _T("1")) == 0));
	GetPrivateProfileString(_T("LogCfg"), _T("ShowTieMsg"), _T("false"), val, 10, _T("LogCfg.ini"));
	m_bShowTipMsg = ((_tcsicmp(val, _T("true")) == 0) || (_tcscmp(val, _T("1")) == 0));
	GetPrivateProfileString(_T("LogCfg"), _T("ShowMsgInConsole"), _T("false"), val, 10, _T("LogCfg.ini"));
	m_bShowInConsole = ((_tcsicmp(val, _T("true")) == 0) || (_tcsicmp(val, _T("1")) == 0));

	TCHAR szEventStr[50];
	SYSTEMTIME systemtime;	
	GetSystemTime(&systemtime);
	_stprintf_s(szEventStr, _T("_LogUtilMutex_%d%d%d%d%d%d%d_")
								, systemtime.wYear
								, systemtime.wMonth
								, systemtime.wDay
								, systemtime.wHour
								, systemtime.wMinute
								, systemtime.wSecond
								, systemtime.wMilliseconds);
	m_hEvent = CreateEvent(NULL, FALSE, TRUE, szEventStr);
}

void CLogUtil::ShowTipMsg(LPCTSTR pMsg, DWORD dwInfoFlag, HICON hNewIcon)
{
	if (!m_bShowTipMsg)
		return;

//	m_NotifyData.dwInfoFlags	= dwInfoFlag;
	if (hNewIcon)
		m_NotifyData.hIcon = hNewIcon;

	_tcsncpy_s(m_NotifyData.szTip, pMsg, ARRAYSIZE(m_NotifyData.szTip));
	//_tcsncpy(m_NotifyData.szInfo, pMsg, ARRAYSIZE(m_NotifyData.szInfo));
	Shell_NotifyIcon(NIM_DELETE, &m_NotifyData);
	Shell_NotifyIcon(NIM_ADD, &m_NotifyData);
}

BOOL CLogUtil::Lock(DWORD dwMilliseconds)
{
	if (!m_hEvent)
		return TRUE;	// If create m_hMutex failed, we do NOT use lock operation

    DWORD dwWaitResult = WaitForSingleObject(m_hEvent, dwMilliseconds); 
    switch (dwWaitResult) 
    {
        // The thread got mutex ownership.
        case WAIT_OBJECT_0: 
            return TRUE;
			
        //case WAIT_TIMEOUT:	// Cannot get mutex ownership due to time-out.
        //case WAIT_ABANDONED:	// Got ownership of the abandoned mutex object.
			
		default:
			return FALSE;
    }
}

BOOL CLogUtil::Unlock()
{
	try
	{
		if (m_hEvent)
			return SetEvent(m_hEvent);
		else
			return TRUE;
	}
	catch (...) {
		return FALSE;
	}
}

BOOL CLogUtil::Log(LPCSTR pszMsg, INT nMessageId, BOOL bWithTime)
{
	VALIDATE
	return ForceLog(pszMsg, nMessageId, bWithTime);
}

BOOL CLogUtil::Log(LPCWSTR pszMsg, INT nMessageId, BOOL bWithTime)
{
	VALIDATE
	return ForceLog(pszMsg, nMessageId, bWithTime);
}

BOOL CLogUtil::VLog(LPCSTR lpcszFormat, ...)
{
	VALIDATE

	try
	{
		char szMsg[MAX_BUFSIZE];
		va_list args;		
		va_start (args, lpcszFormat);
		_vsnprintf_s(szMsg, MAX_BUFSIZE, lpcszFormat, args);
		//string szMsg;
		//szMsg.resize(vsctprintf( lpcszFormat, args ) + 1);
		//vsprintf((char*)szMsg.c_str(), lpcszFormat, args);
		va_end (args);

		return ForceLog(szMsg);
	}
	catch (...) {
		return FALSE;
	}
}

BOOL CLogUtil::VLog(LPCWSTR lpcszFormat, ...)
{
	VALIDATE

	try
	{
		wchar_t szMsg[MAX_BUFSIZE];
		va_list args;		
		va_start (args, lpcszFormat);
		_vsnwprintf_s(szMsg, MAX_BUFSIZE, lpcszFormat, args);
		//TString szMsg;
		//szMsg.resize(_vsctprintf( lpcszFormat, args ) + 1);
		//_vstprintf((TCHAR*)szMsg.c_str(), lpcszFormat, args);
		va_end (args);

		return ForceLog(szMsg);
	}
	catch (...) {
		return FALSE;
	}
}

BOOL CLogUtil::VForceLog(LPCSTR lpcszFormat, ...)
{
	try
	{
		char szMsg[MAX_BUFSIZE];
		//ZeroMemory(szMsg, MAX_BUFSIZE);

		va_list args;		
		va_start (args, lpcszFormat);
		_vsnprintf_s(szMsg, MAX_BUFSIZE, lpcszFormat, args);
		va_end (args);

		return ForceLog(szMsg);
	}
	catch (...) {
		return FALSE;
	}
}

BOOL CLogUtil::VForceLog(LPCWSTR lpcszFormat, ...)
{
	try
	{
		wchar_t szMsg[MAX_BUFSIZE];
		//ZeroMemory(szMsg, MAX_BUFSIZE);

		va_list args;		
		va_start (args, lpcszFormat);
		_vsnwprintf_s(szMsg, MAX_BUFSIZE, lpcszFormat, args);
		va_end (args);

		return ForceLog(szMsg);
	}
	catch (...) {
		return FALSE;
	}
}

BOOL CLogUtil::ForceLog(LPCSTR pszMsg, INT nMessageId, BOOL bWithTime)
{	
	USES_CONVERSION;

	BOOL bRet = FALSE;
	if (Lock(m_dwMilliseconds))
	{
		try
		{
			if (!m_pFile) {
				if (!OpenLogFile(m_szLogFileName))
					return FALSE;
			}
			
			if (TRUE == bWithTime)
				WriteLogTime();
			
			LPCSTR pActMsg = NULL;			
			std::string msg;
			if (!nMessageId) {
				pActMsg = pszMsg;
			}
			else {			
				pActMsg = CStrUtil::FormatMsg(pszMsg, msg, nMessageId).c_str();
			}
			
			LPCTSTR ptActMsg = A2CT(pActMsg);

			fprintf(m_pFile, m_bNewLineAfterTimeStr ? "\n" : "\t");

			fprintf(m_pFile, pActMsg);
			ShowMsgInConsole(ptActMsg);
			
			fflush(m_pFile);
			
			// we should close the opened file as soon as possible
			// because that the other application may be write log into this file
			fclose(m_pFile);
			m_pFile = NULL;
			
			if (m_bAutoShowMsg)
				ShowMessage(ptActMsg);
			
			bRet = TRUE;
		}
		catch (...)
		{
			bRet = FALSE;
		}

		Unlock();
	}

	return bRet;
}

BOOL CLogUtil::ForceLog(LPCWSTR pszMsg, INT nMessageId, BOOL bWithTime)
{
	BOOL bRet = FALSE;
	if (Lock(m_dwMilliseconds))
	{
		try
		{
			if (!m_pFile) {
				if (!OpenLogFile(m_szLogFileName))
					return FALSE;
			}
			
			if (TRUE == bWithTime)
				WriteLogTime();
			
			LPCWSTR pActMsg = NULL;
			std::wstring msg;
			if (!nMessageId) {
				pActMsg = pszMsg;
			}
			else {			
				pActMsg = CStrUtil::FormatMsg(pszMsg, msg, nMessageId).c_str();
			}
			
			fprintf(m_pFile, m_bNewLineAfterTimeStr ? "\n" : "\t"); // Always use ANSI encoding
			//fwprintf(m_pFile, L"%s", pActMsg);
			fprintf(m_pFile, "%S", pActMsg);	// Always use ANSI encoding
			ShowMsgInConsole(pActMsg);

			fflush(m_pFile);
			
			// we should close the opened file as soon as possible
			// because that the other application may be write log into this file
			fclose(m_pFile);
			m_pFile = NULL;
			
			if (m_bAutoShowMsg)
				ShowMessage(pActMsg);
			
			bRet = TRUE;
		}
		catch (...)
		{
			bRet = FALSE;
		}

		Unlock();
	}

	return bRet;
}

void CLogUtil::WriteLogTime(BOOL bNewLine)
{
	if (m_pFile)
	{
		SYSTEMTIME st;
		GetLocalTime(&st);

		//TCHAR szMsg[50];		
		char szMsg[50];
		
		if (bNewLine)
			//_stprintf_s(szMsg, _T("\n%04d/%02d/%02d %02d:%02d:%02d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
			sprintf_s(szMsg, "\n%04d/%02d/%02d %02d:%02d:%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);	// Always use ANSI encoding
		else
			//_stprintf_s(szMsg, _T("%04d/%02d/%02d %02d:%02d:%02d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
			sprintf_s(szMsg, "%04d/%02d/%02d %02d:%02d:%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

		//_ftprintf(m_pFile, szMsg);
		fprintf(m_pFile, szMsg);
		ShowMsgInConsole(szMsg);

		if (m_bShowThreadID)
		{
			//_stprintf_s(szMsg, _T("[%04d]"), GetCurrentThreadId());
			//_ftprintf(m_pFile, szMsg);
			sprintf_s(szMsg, "[%04d]", GetCurrentThreadId());
			fprintf(m_pFile, szMsg);
			ShowMsgInConsole(szMsg);
		}
		
		fflush(m_pFile);
	}
}

BOOL CLogUtil::OpenLogFile(LPCTSTR pcszLogFileName)
{
	if (_tcslen(pcszLogFileName) < 1)
		return FALSE;

	if (m_pFile) {
		fflush(m_pFile);
		fclose(m_pFile);
	}

	TCHAR szFullName[_MAX_PATH];
	if (!_tfullpath(szFullName, pcszLogFileName, MAX_PATH)) {
		return FALSE;
	}

	TCHAR szDrive[_MAX_DRIVE];
	TCHAR szDir[_MAX_DIR];
	TCHAR szFname[_MAX_FNAME];
	TCHAR szExt[_MAX_EXT];
	TCHAR szFullDir[_MAX_PATH];
	_tsplitpath_s(szFullName, szDrive, _MAX_DRIVE, szDir, _MAX_DIR, szFname, _MAX_FNAME, szExt, _MAX_EXT);
	_tmakepath_s(szFullDir, szDrive, szDir, NULL, NULL);

	if ((NULL != _tcschr(pcszLogFileName, _T('\\')) || (NULL != _tcschr(pcszLogFileName, _T('/'))))) {
		// we should create the directory if the directory doesn't exists

		if (-1 == _taccess(szFullDir, 0)) {
			TCHAR szTemp[_MAX_PATH];
			TCHAR *pDir = szTemp + 3;	// the drive letter is ignore, e.g. "c:\"
			do {
				_tcscpy_s(szTemp, _MAX_PATH, szFullDir);

				pDir = _tcschr(pDir, _T('\\'));
				if (pDir)
					*pDir = 0;			// truncate the right part of the szTemp

				if (-1 == _taccess(szTemp, 0))
					if (-1 == _tmkdir(szTemp))
						return FALSE;

				pDir++;

				// if pDir is NULL, it means that we have went to the last directory without the end of '\'
				// if *pDir = 0, it means that we have went to the last directory with the end of '\'
			} while (pDir && (*pDir != 0));
		}
	}
	else {
		// the pcszLogFileName doesn't include directory, 
		// so it should be created in current directory
	}

	_tfinddata_t FindData;
	long int handle = _tfindfirst(szFullName, &FindData);
	if (handle != -1) {
		if (FindData.size > m_dwMaxFileSize) {
			// create a new log file
			TCHAR *pszFileTemplate = _T("%s%s%s%d%s");

			// find a unused file name
			INT i = 1;
			FILE *pTemp = NULL;
			do {
				if (pTemp) {
					fclose(pTemp);
					pTemp = NULL;
				}

				_stprintf_s(szFullDir, _MAX_PATH, pszFileTemplate, szDrive, szDir, szFname, i, szExt);
				_tfopen_s(&pTemp, szFullDir, _T("r"));
				i++;

			} while (pTemp);

			// If the count of log file is larger than MAX_LOG_FILES, then delete some older files
			if (!g_bCheckingLogFileCount && i > MAX_LOG_FILES)
			{
#ifndef _DEBUG
				_beginthread(CheckLogFileCount, 0, szFullName);
#else
				CheckLogFileCount(szFullName);
#endif
			}

			// rename the current log file, for Windows platform only		
			MoveFile(szFullName, szFullDir);
		}

		_findclose(handle);
	}

	_tfopen_s(&m_pFile, szFullName, _T("a+"));	// Sometimes the GetLastError() not equal 0.

	return (m_pFile != NULL);
}

void CLogUtil::SetLogFileName(LPCTSTR pcszLogFileName)
{
	if ( !pcszLogFileName || (_tcslen(pcszLogFileName) == 0) )
		return;

	USES_CONVERSION;	// for W2A or A2W

	_tcscpy_s(m_szLogFileName, MAX_PATH, pcszLogFileName);
}

/**************************************************************************
* Display a message, the method support multi desktop
* and can show message in a service application
*************************************************************************/
void CLogUtil::ShowMessage(LPCTSTR pcszMsg)
{
	if ( (pcszMsg == NULL) || (_tcslen(pcszMsg) < 1) )
		return;

	//////////////////////////
	// rpcrt4.dll API definition

	typedef RPC_STATUS (WINAPI *pRpcImpersonateClient)(RPC_BINDING_HANDLE BindingHandle);
	typedef RPC_STATUS (WINAPI *pRpcRevertToSelf)(VOID);
	HINSTANCE hRPCRT4DLLInst = NULL;									// rpcrt4.dll instance
	pRpcImpersonateClient LPRpcImpersonateClient = NULL;
	pRpcRevertToSelf LPRpcRevertToSelf = NULL;

	hRPCRT4DLLInst = LoadLibrary(_T("rpcrt4.dll"));
	if (hRPCRT4DLLInst != NULL)
	{
		LPRpcImpersonateClient = (pRpcImpersonateClient) GetProcAddress(hRPCRT4DLLInst ,"RpcImpersonateClient");
		LPRpcRevertToSelf = (pRpcRevertToSelf) GetProcAddress(hRPCRT4DLLInst ,"RpcRevertToSelf");
	}

	DWORD dwGuiThreadId = 0;
	DWORD dwThreadId;

	LPTSTR lpszWindowStation = _T("WinSta0");
	LPTSTR lpszDesktop = _T("Default");

	HWINSTA hwinstaSave;
	HDESK hdeskSave;
	HWINSTA hwinstaUser;
	HDESK hdeskUser;

	GetDesktopWindow();
	hwinstaSave = GetProcessWindowStation();
	dwThreadId = GetCurrentThreadId();
	hdeskSave = GetThreadDesktop(dwThreadId);

	if(LPRpcImpersonateClient != NULL)
		(LPRpcImpersonateClient)(0);
	hwinstaUser = OpenWindowStation(lpszWindowStation, FALSE, MAXIMUM_ALLOWED);
	if (hwinstaUser == NULL)
	{
		if(LPRpcRevertToSelf != NULL)
			(LPRpcRevertToSelf)();
		return;
	}
	SetProcessWindowStation(hwinstaUser);
	hdeskUser = OpenDesktop(lpszDesktop, 0, FALSE, MAXIMUM_ALLOWED);
	if(LPRpcRevertToSelf != NULL)
		(LPRpcRevertToSelf)();
	if (hdeskUser == NULL)
	{
		SetProcessWindowStation(hwinstaSave);
		CloseWindowStation(hwinstaUser);
		return;
	}
	SetThreadDesktop(hdeskUser);
	dwGuiThreadId = dwThreadId;

	MessageBox(NULL, pcszMsg, _T("Notice"), MB_OK);

	// resotre the window and desktop
	dwGuiThreadId = 0;
	SetThreadDesktop(hdeskSave);
	SetProcessWindowStation(hwinstaSave);
	CloseDesktop(hdeskUser);
	CloseWindowStation(hwinstaUser);

	if (hRPCRT4DLLInst != NULL)
		FreeLibrary(hRPCRT4DLLInst);
	LPRpcImpersonateClient = NULL;
	LPRpcRevertToSelf = NULL;

	return;
}

void CLogUtil::SetEnable(BOOL bFlag)
{
	if (!bFlag && m_pFile)
		fclose(m_pFile);

	m_bEnabled = bFlag;
}

inline void CLogUtil::DEBUG_LOG(LPCSTR pszMsg, INT nMessageId, BOOL bWithTime)
{
#ifdef _DEBUG
	ForceLog(pszMsg, nMessageId, bWithTime);
#endif
}

inline void CLogUtil::DEBUG_LOG(LPCWSTR pszMsg, INT nMessageId, BOOL bWithTime)
{
#ifdef _DEBUG
	ForceLog(pszMsg, nMessageId, bWithTime);
#endif
}

inline void CLogUtil::DEBUG_VLOG(LPCSTR lpcszFormat, ...)
{
#ifdef _DEBUG
	try
	{
		char szMsg[MAX_BUFSIZE];
		va_list args;		
		va_start (args, lpcszFormat);
		_vsnprintf_s(szMsg, MAX_BUFSIZE, lpcszFormat, args);
		va_end (args);

		ForceLog(szMsg);
	}
	catch (...) {}	
#endif
}

inline void CLogUtil::DEBUG_VLOG(LPCWSTR lpcszFormat, ...)
{
#ifdef _DEBUG
	try
	{
		wchar_t szMsg[MAX_BUFSIZE];
		va_list args;		
		va_start (args, lpcszFormat);
		_vsnwprintf_s(szMsg, MAX_BUFSIZE, lpcszFormat, args);
		va_end (args);

		ForceLog(szMsg);
	}
	catch (...) {}	
#endif
}

inline void CLogUtil::ShowMsgInConsole(LPCSTR pcszMsg)
{
	if (m_bShowInConsole)
		printf("%s", pcszMsg);
}

inline void CLogUtil::ShowMsgInConsole(LPCWSTR pcwszMsg)
{
	if (m_bShowInConsole)
		wprintf(L"%s", pcwszMsg);
}

BOOL WINAPI HandlerRoutine(DWORD dwCtrlType)
{
	switch (dwCtrlType)
	{
	case CTRL_C_EVENT:
	case CTRL_BREAK_EVENT:
	case CTRL_CLOSE_EVENT:
		return TRUE;

	case CTRL_LOGOFF_EVENT:
	case CTRL_SHUTDOWN_EVENT:
		break;
	default:
		break;
	}

	return FALSE;
}

BOOL CLogUtil::OpenConsole()
{
	TCHAR szTitle[100];
	_stprintf_s(szTitle, _T("Log Output [%d]"), GetCurrentThreadId());

	if (m_bShowInConsole)		// We still need to check if the console is created or not
	{
		if (NULL == ::FindWindow(NULL, szTitle))
			m_bShowInConsole = FALSE;
	}

	if (!m_bShowInConsole)
	{
		if (AllocConsole())
		{
			// Redirect stdout to this console
			_tfreopen_s(&m_pFile, _T("CON"), _T("w"), stdout);
			m_pFile = NULL;
			
			// Remove the close button
			SetConsoleTitle(szTitle);
			Sleep(10);	// Sometimes cannot find window if no sleep here 
			HWND hWnd = ::FindWindow(NULL, szTitle);
			if (hWnd != NULL)
			{
				HMENU hMenu = ::GetSystemMenu(hWnd, FALSE);
				DeleteMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);
			}
			
			// Ignore the Ctrl+C and CTRL+BREAK command
			SetConsoleCtrlHandler(HandlerRoutine, TRUE);

			// Adjust the screen buffer size
			HANDLE hHandle = GetStdHandle(STD_OUTPUT_HANDLE);
			if (hHandle > 0)
			{
				COORD coord;
				coord.X = 80;
				coord.Y	= 9999;
				SetConsoleScreenBufferSize(hHandle, coord);
			}

			m_bShowInConsole = TRUE;
		}
	}

	return m_bShowInConsole;
}

BOOL CLogUtil::CloseConsole()
{
	m_bShowInConsole = FALSE;
	FreeConsole();

	return m_bShowInConsole;
}

BOOL CLogUtil::EnableConsoleOutput(BOOL bFlag)
{
	if (bFlag)
		return OpenConsole();
	else
		return CloseConsole();
}