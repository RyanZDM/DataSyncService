// LogUtil.h: interface for the CLogUtil class.
// Description:
// Remark: Once the class is constructed, it will read config info from the file named "LogCfg.ini"
//		   The file format is as followed:
//					[LogCfg]
//					Enabled=true/false/1/0				-- whether write log to file or not
//														-- default is true
//					AutoShowMsg=true/false/1/0			-- whether popup a message window when write log
//														-- default is false
//
// *** The files StrUtil.h/StrUtil.cpp should been added to project for compiling succeed
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//#ifndef _DISABLE_LOG_

#if !defined(AFX_LOGUTIL_H__2B2F8C6F_4E9F_4FB0_888F_C556B8F7A177__INCLUDED_)
#define AFX_LOGUTIL_H__2B2F8C6F_4E9F_4FB0_888F_C556B8F7A177__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <wtypes.h>
#include <STDIO.H>

#include <shellapi.h>
#pragma comment(lib, "shell32.lib")
#ifndef ARRAYSIZE
#define ARRAYSIZE(A) (sizeof(A)/sizeof((A)[0]))
#endif

//#define MAX_FILESIZE	65535
static const DWORD MAX_FILESIZE  = 1048576;		// 1024 * 1024
static const DWORD MAX_BUFSIZE   = 5000;
static const DWORD MAX_LOG_FILES = 30;

class CInstanceMutex
{
	static LPCTSTR INSTANCE_MUTEX_NAME;

public:
	BOOL Lock(DWORD dwMilliseconds) 
	{ 
		if (!m_hMutex)
		{
			m_hMutex = CreateMutex(NULL, FALSE, INSTANCE_MUTEX_NAME);
		}

		DWORD dwWaitResult = WaitForSingleObject(m_hMutex,  dwMilliseconds); 
        switch (dwWaitResult) 
        {
            // The thread got ownership of the mutex
            case WAIT_OBJECT_0: 
                return TRUE;
				 
            // The thread got ownership of an abandoned mutex
            case WAIT_ABANDONED: 
                return FALSE;				
        }

		return FALSE; 
	};

	void Unlock() { if (m_hMutex) ReleaseMutex(m_hMutex); };

	CInstanceMutex() { m_hMutex = CreateMutex(NULL, FALSE, INSTANCE_MUTEX_NAME); }
	~CInstanceMutex() { if (m_hMutex) CloseHandle(m_hMutex); } 

private:
	HANDLE	m_hMutex;
};

class CLogUtil  
{
public:
	BOOL CloseConsole();
	BOOL OpenConsole();
	BOOL EnableConsoleOutput(BOOL bFlag);
	void ShowTipMsg(LPCTSTR pMsg, DWORD dwInfoFlag = 0x00000001/*NIIF_INFO*/, HICON hIcon = NULL);		
	void SetEnable(BOOL bFlag);						// Indicate if log output or not	
	void ShowMessage(LPCTSTR pcszMsg);				// Message output
	void SetLogFileName(LPCTSTR pcszLogFileName);	// Set log file name
	BOOL OpenLogFile(LPCTSTR pcszLogFileName);		// Open log file

	void AutoNewLineAfterTimeStr(BOOL bFlag) { m_bNewLineAfterTimeStr = bFlag; }
	void AutoShowTip(BOOL bFlag) { m_bShowTipMsg = bFlag; }
	void ShowThreadID(BOOL bFlag) { m_bShowThreadID = bFlag; }
	BOOL IsShowThreadID() { return m_bShowThreadID; }
	BOOL IsShowMsgInConsole() { return m_bShowInConsole; }
	void AutoShowMsg(BOOL bAuto) { m_bAutoShowMsg = bAuto; }						// Indicate if pop-up a message window or not
	void SetMaxFileSize(DWORD dwMaxFileSize) { m_dwMaxFileSize = dwMaxFileSize; }	// Set maximum log file size, if file size exceed this value, creat a new one
	void SetLockTimeout(DWORD dwMilliseconds) { m_dwMilliseconds = (dwMilliseconds > 0) ? dwMilliseconds : 0; }

	virtual BOOL VForceLog(LPCSTR lpcszFormat, ...);									// Always output log using the specified format no matter what the value of Enable-switch is
	virtual BOOL VForceLog(LPCWSTR lpcszFormat, ...);									// Always output log using the specified format no matter what the value of Enable-switch is
	virtual BOOL ForceLog(LPCSTR pszMsg, INT nMessageId = 0, BOOL bWithTime = TRUE);	// Always output log no matter what the value of Enable-switch is
	virtual BOOL ForceLog(LPCWSTR pszMsg, INT nMessageId = 0, BOOL bWithTime = TRUE);	// Always output log no matter what the value of Enable-switch is
	virtual BOOL VLog(LPCSTR lpcszFormat, ...);											// Output log using the specified format. No output if the Enable-switch is off
	virtual BOOL VLog(LPCWSTR lpcszFormat, ...);										// Output log using the specified format. No output if the Enable-switch is off
	virtual BOOL Log(LPCSTR pszMsg, INT nMessageId = 0, BOOL bWithTime = TRUE);			// Output log. No output if the Enable-switch is off
	virtual BOOL Log(LPCWSTR pszMsg, INT nMessageId = 0, BOOL bWithTime = TRUE);		// Output log. No output if the Enable-switch is off

	// Only output log in DEBUG mode
	inline void DEBUG_VLOG(LPCSTR lpcszFormat, ...);									// Output log using the specified format in DEBUG mode only
	inline void DEBUG_VLOG(LPCWSTR lpcszFormat, ...);									// Output log using the specified format in DEBUG mode only
	inline void DEBUG_LOG(LPCSTR pszMsg, INT nMessageId = 0, BOOL bWithTime = TRUE);	// Output log in DEBUG mode only
	inline void DEBUG_LOG(LPCWSTR pszMsg, INT nMessageId = 0, BOOL bWithTime = TRUE);	// Output log in DEBUG mode only

	CLogUtil(DWORD dwMaxFileSize = MAX_FILESIZE);
	CLogUtil(LPCTSTR pcszLogFileName, DWORD dwMaxFileSize = MAX_FILESIZE);
	virtual ~CLogUtil();

protected:
	UINT IncreaseInstanceCount();
	UINT DecreaseInstanceCount();

private:
	inline void ShowMsgInConsole(LPCSTR pcszMsg);
	inline void ShowMsgInConsole(LPCWSTR pcwszMsg);

private:
	BOOL	m_bShowThreadID;
	BOOL	m_bNewLineAfterTimeStr;														// Append \r\n at the end of time string
	DWORD	m_dwMilliseconds;															// Time-out for multi-thread lock
	HANDLE	m_hEvent;																	// Event object for multi-thread synchronization
	BOOL	m_bEnabled;																	// Flag to indicate if output log
	DWORD	m_dwMaxFileSize;															// Maximum log file size
	FILE	* m_pFile;																	// Pointer to log file
	TCHAR	m_szLogFileName[MAX_PATH];													// Log file name. The file name in both m_szLogFileName and m_wszLogFileName are the same	
	BOOL	m_bAutoShowMsg;																// if it's true, it will show message window when call the method "Log"
	void	Initialize();																// Initialize member vars
	void	WriteLogTime(BOOL bNewLine = TRUE);											// Write current time as a string into log file
	BOOL	Lock(DWORD dwMilliseconds);													// For multi-thread synchronization
	BOOL	Unlock();																	// For multi-thread synchronization

	BOOL	m_bShowTipMsg;
	BOOL	m_bShowInConsole;
	NOTIFYICONDATA m_NotifyData;

private:
	static UINT				s_nInstanceCount;
	CInstanceMutex			m_Mutex;
};

typedef struct tagFILE_PRODUCT_VERSION{
	WORD	wFileVersion1;
	WORD	wFileVersion2;
	WORD	wFileVersion3;
	WORD	wFileVersion4;
	WORD	wProductVersion1;
	WORD	wProductVersion2;
	WORD	wProductVersion3;
	WORD	wProductVersion4;
	
public:
	LPCTSTR GetFileVersionString()
	{
		size_t len = sizeof(m_szFileVersion) / sizeof(m_szFileVersion[0]);
		ZeroMemory(m_szFileVersion, len);
		_stprintf_s(m_szFileVersion, len, _T("%d.%d.%d.%d"), wFileVersion1, wFileVersion2, wFileVersion3, wFileVersion4);
		return m_szFileVersion;
	}

	LPCTSTR GetProductVersionString()
	{
		size_t len = sizeof(m_szProductVersion) / sizeof(m_szProductVersion[0]);
		ZeroMemory(m_szProductVersion, len);
		_stprintf_s(m_szProductVersion, len, _T("%d.%d.%d.%d"), wProductVersion1, wProductVersion2, wProductVersion3, wProductVersion4);
		return m_szProductVersion;
	}
private:
	TCHAR m_szFileVersion[24];
	TCHAR m_szProductVersion[24];
} FILE_PRODUCT_VERSION, *LPFILE_PRODUCT_VERSION;

void GetProductVersion(FILE_PRODUCT_VERSION &version);

#endif // !defined(AFX_LOGUTIL_H__2B2F8C6F_4E9F_4FB0_888F_C556B8F7A177__INCLUDED_)
