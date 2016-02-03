// RegistryOperator.cpp: implementation of the RegistryOperator class.
//
//////////////////////////////////////////////////////////////////////

#include <Windows.h>
#include <Winreg.h>

#include <stdio.h>
#include <tchar.h>

#include "RegistryOperator.h"
#pragma comment (lib, "Advapi32")

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
#endif

#define MAX_KEY_LENGTH			255
#define MAX_VALUE_NAME_LENGTH	16363

const HKEY REGISTRY_KEYS[] = {HKEY_CLASSES_ROOT,
							  HKEY_CURRENT_CONFIG,
							  HKEY_CURRENT_USER,
							  HKEY_LOCAL_MACHINE,
							  HKEY_USERS,
							  HKEY_PERFORMANCE_DATA,
							  HKEY_DYN_DATA};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRegistryOperator::CRegistryOperator()
{
	m_CurrentHKey = NULL;
#ifndef _DISABLE_LOG_	
	m_Log.SetLogFileName(_T("RegOper.log"));
#endif
}

CRegistryOperator::~CRegistryOperator()
{

}

BOOL CRegistryOperator::ValidateKey(HKEY hKey)
{
	if (!hKey) {
		Log(_T("ValidateKey: The HKEY is invalid"));
		return FALSE;
	}
	
	INT nCount = sizeof(REGISTRY_KEYS) / sizeof(HKEY);
	for (INT i = 0; i < nCount; i++) 
		if (hKey == REGISTRY_KEYS[i])
			return TRUE;

	Log(_T("ValidateKey: The HKEY is invalid"));
	return FALSE;
}

HKEY CRegistryOperator::GetCurrentHKey()
{
	return m_CurrentHKey;
}

BOOL CRegistryOperator::SetCurrentHKey(HKEY hKey)
{
	// temp
	//if (!ValidateKey(hKey))
	//	return FALSE;

	m_CurrentHKey = hKey;

	return TRUE;
}

BOOL CRegistryOperator::GetRegString(HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueName, LPTSTR lpData, DWORD dwDataLen, LPCTSTR pcszDefault)
{
	if (!hKey || !lpSubKey || !lpData) 
	{
		Log(_T("GetRegString: invalid parameters"));
		return FALSE;
	}
	
	INT nRet;
	HKEY hResult = NULL;
	BOOL bResult = FALSE;
	
	try 
	{
		if ( ERROR_SUCCESS != (nRet = RegOpenKeyEx(hKey, lpSubKey, 0, KEY_QUERY_VALUE, &hResult)) ) 
		{
			throw _T("GetRegString: RegOpenKeyEx failed");			
		}

		DWORD dwActLen;
		DWORD dwRegType = REG_SZ;
		if ( ERROR_SUCCESS != (nRet = RegQueryValueEx(hResult, lpValueName, NULL, &dwRegType, NULL, &dwActLen)) ) 
			throw _T("GetRegString: RegQueryValueEx failed");

		if (dwActLen > 0)
		{
			if ( dwActLen > dwDataLen ) 
			{
				nRet = 0;
				throw _T("GetRegString: the buffer size is too small");			
			}
			
			if ( ERROR_SUCCESS != (nRet = RegQueryValueEx(hResult, lpValueName, NULL, &dwRegType, (LPBYTE)lpData, &dwActLen)) ) 
				throw _T("GetRegString: RegQueryValueEx failed");
		}
		else
		{
			lpData[0] = '\0';
		}

		bResult = TRUE;
	}
	catch (LPCTSTR pMsg) 
	{
		Log(pMsg, nRet);
		bResult = FALSE;
	}
	catch (...) 
	{
		Log(_T(""), GetLastError());
		bResult = FALSE;
	}

	if (!bResult)
	{
		if (pcszDefault)
		{
			_tcsncpy_s(lpData, dwDataLen, pcszDefault, (dwDataLen - 1));
			bResult = TRUE;
		}
	}

	if (hResult)
		RegCloseKey(hResult);

	return bResult;
}

BOOL CRegistryOperator::GetRegString(LPCTSTR lpSubKey, LPCTSTR lpValueName, LPTSTR lpData, DWORD dwDataLen, LPCTSTR pcszDefault)
{
	return GetRegString(m_CurrentHKey, lpSubKey, lpValueName, lpData, dwDataLen, pcszDefault);
}

BOOL CRegistryOperator::GetRegDWord(HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueName, DWORD *pData, DWORD dwDefault)
{
	BOOL bRet = GetRegDWord(hKey, lpSubKey, lpValueName, pData);
	if (!bRet)
	{
		if (pData)
		{
			*pData = dwDefault;
			bRet = TRUE;
		}
	}

	return bRet;
}

BOOL CRegistryOperator::GetRegDWord(LPCTSTR lpSubKey, LPCTSTR lpValueName, DWORD *pData, DWORD dwDefault)
{
	return GetRegDWord(m_CurrentHKey, lpSubKey, lpValueName, pData, dwDefault);
}

BOOL CRegistryOperator::GetRegDWord(HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueName, DWORD *pData)
{
	if (!hKey || !lpSubKey || !pData)
		return FALSE;

	INT nRet;
	HKEY hResult;
	BOOL bResult = FALSE;

	try {	
		if ( ERROR_SUCCESS != (nRet = RegOpenKeyEx(hKey, lpSubKey, 0, KEY_QUERY_VALUE, &hResult)) )
			throw _T("GetRegDWord: RegOpenKeyEx failed");
		
		DWORD dwActLen;
		DWORD dwRegType = REG_DWORD;
		if ( ERROR_SUCCESS != (nRet = RegQueryValueEx(hResult, lpValueName, NULL, &dwRegType, (LPBYTE)pData, &dwActLen)) )
			throw _T("GetRegDWord: RegQueryValueEx failed");

		bResult = TRUE;
	}
	catch (LPCTSTR pMsg) 
	{
		Log(pMsg, nRet);		
		bResult = FALSE;
	}
	catch (...) 
	{
		Log(_T(""), GetLastError());
		bResult = FALSE;
	}

	if (hResult)
		RegCloseKey(hResult);

	return bResult;
}

BOOL CRegistryOperator::GetRegDWord(LPCTSTR lpSubKey, LPCTSTR lpValueName, DWORD *pData)
{
	return GetRegDWord(m_CurrentHKey, lpSubKey, lpValueName, pData);
}

BOOL CRegistryOperator::SetRegString(HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueName, LPBYTE lpData, DWORD dwDataLen)
{
	if ( !hKey || !lpSubKey || !lpData )
		return FALSE;

	INT nRet;
	HKEY hResult;
	DWORD dwDisposition;
	BOOL bResult = FALSE;
	
	try {	
		if ( ERROR_SUCCESS != (nRet = RegCreateKeyEx(hKey, lpSubKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hResult, &dwDisposition)) )
			throw _T("SetRegString: RegCreateKeyEx failed");
		
		if ( ERROR_SUCCESS != (nRet = RegSetValueEx(hResult, lpValueName, 0, REG_SZ, lpData, dwDataLen)) )
			throw _T("SetRegString: RegSetValueEx failed");
		
		bResult = TRUE;
	}
	catch (LPCTSTR pMsg) 
	{
		Log(pMsg, nRet);
		bResult = FALSE;
	}
	catch (...) 
	{
		Log(_T(""), GetLastError());
		bResult = FALSE;
	}
	
	if (hResult)
		RegCloseKey(hResult);

	return bResult;
}

BOOL CRegistryOperator::SetRegString(LPCTSTR lpSubKey, LPCTSTR lpValueName, LPBYTE lpData, DWORD dwDataLen)
{
	return SetRegString(m_CurrentHKey, lpSubKey, lpValueName, lpData, dwDataLen);
}

BOOL CRegistryOperator::SetRegDWord(HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueName, DWORD dwData)
{
	if ( !hKey || !lpSubKey )
		return FALSE;

	INT nRet;
	HKEY hResult;
	DWORD dwDisposition;
	BOOL bResult = FALSE;

	try {
		if ( ERROR_SUCCESS != (nRet = RegCreateKeyEx(hKey, lpSubKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hResult, &dwDisposition)) )
			throw _T("SetRegDWord: RegCreateKeyEx failed");
		
		if (ERROR_SUCCESS != RegSetValueEx(hResult, lpValueName, 0, REG_DWORD, (LPBYTE)&dwData, sizeof(DWORD)))
			throw _T("SetRegDWord: RegSetValueEx failed");

		bResult = TRUE;
	}
	catch (LPCTSTR pMsg) 
	{
		Log(pMsg, nRet);
		bResult = FALSE;
	}
	catch (...) 
	{
		Log(_T(""), GetLastError());
		bResult = FALSE;
	}
	
	if (hResult)
		RegCloseKey(hResult);

	return bResult;
}

BOOL CRegistryOperator::SetRegDWord(LPCTSTR lpSubKey, LPCTSTR lpValueName, DWORD dwData)
{
	return SetRegDWord(m_CurrentHKey, lpSubKey, lpValueName, dwData);
}

void CRegistryOperator::Log(LPCTSTR lpMsg, INT nMessageId)
{
#ifndef _DISABLE_LOG_
	m_Log.Log(lpMsg, nMessageId);
#else
	UNREFERENCED_PARAMETER(lpMsg);
	UNREFERENCED_PARAMETER(nMessageId);
#endif
}

INT CRegistryOperator::GetRegKeys(vector<basic_string<TCHAR>> &vKeys)
{
	return GetRegKeys(m_CurrentHKey, vKeys);
}

INT CRegistryOperator::GetRegKeys(HKEY hKey, vector<basic_string<TCHAR>> &vKeys)
{
	vKeys.clear();

	TCHAR szKey[MAX_KEY_LENGTH];
	DWORD dwSubkeys = 0;
	
	RegQueryInfoKey(hKey, NULL, NULL, NULL, &dwSubkeys, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

	if (dwSubkeys < 1)
		return 0;

	for (DWORD i = 0; i < dwSubkeys; i++)
	{
		szKey[0] = _T('\0');

		INT ret = RegEnumKey(hKey, i, szKey, MAX_KEY_LENGTH);
		if (ret != ERROR_SUCCESS)
		{
#ifndef _DISABLE_LOG_
			m_Log.VForceLog(_T("Failed to enum the Regkey #%d."), i);
#endif
			continue;
		}

		vKeys.push_back(szKey);
	}

	return vKeys.size();
}

