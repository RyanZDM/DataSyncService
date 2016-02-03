      // RegistryOperator.h: interface for the RegistryOperator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REGISTRYOPERATOR_H__E49478AA_691B_4FE7_B42C_48BCAC83A282__INCLUDED_)
#define AFX_REGISTRYOPERATOR_H__E49478AA_691B_4FE7_B42C_48BCAC83A282__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <wtypes.h>
#include <vector>
#include <string>
using namespace std;

#ifndef _DISABLE_LOG_
#include <LogUtil.h>
#endif

class CRegistryOperator  
{
public:
	void Log(LPCTSTR lpMsg, INT nMessageId = 0);
	INT GetRegKeys(vector<basic_string<TCHAR>> &vKeys);
	INT GetRegKeys(HKEY hKey, vector<basic_string<TCHAR>> &vKeys);
	BOOL SetRegDWord(HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueName, DWORD dwData);
	BOOL SetRegDWord(LPCTSTR lpSubKey, LPCTSTR lpValueName, DWORD dwData);
	BOOL SetRegString(HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueName, LPBYTE lpData, DWORD dwDataLen);
	BOOL SetRegString(LPCTSTR lpSubKey, LPCTSTR lpValueName, LPBYTE lpData, DWORD dwDataLen);
	BOOL GetRegDWord(HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueName, DWORD *pData);
	BOOL GetRegDWord(LPCTSTR lpSubKey, LPCTSTR lpValueName, DWORD *pData);
	BOOL GetRegDWord(HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueName, DWORD *pData, DWORD dwDefault);
	BOOL GetRegDWord(LPCTSTR lpSubKey, LPCTSTR lpValueName, DWORD *pData, DWORD dwDefault);
	BOOL GetRegString(HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueName, LPTSTR lpData, DWORD dwDataLen, LPCTSTR pcszDefault = NULL);
	BOOL GetRegString(LPCTSTR lpSubKey, LPCTSTR lpValueName, LPTSTR lpData, DWORD dwDataLen, LPCTSTR pcszDefault = NULL);
	BOOL SetCurrentHKey(HKEY hKey);
	HKEY GetCurrentHKey();
	CRegistryOperator();
	virtual ~CRegistryOperator();

private:
	HKEY m_CurrentHKey;
	BOOL ValidateKey(HKEY);
#ifndef _DISABLE_LOG_
	CLogUtil m_Log;
#endif

};

#endif // !defined(AFX_REGISTRYOPERATOR_H__E49478AA_691B_4FE7_B42C_48BCAC83A282__INCLUDED_)
