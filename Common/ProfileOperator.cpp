#include <tchar.h>
#include "ProfileOperator.h"

TCHAR CProfileOperator::s_szFullName[MAX_PATH] = { _T('\0') };
TCHAR CProfileOperator::s_szFullPathWithoutFileName[MAX_PATH] = { _T('\0') };
TCHAR CProfileOperator::s_szDrive[_MAX_DRIVE] = { _T('\0') };
TCHAR CProfileOperator::s_szDir[_MAX_DIR] = { _T('\0') };
TCHAR CProfileOperator::s_szFname[_MAX_FNAME] = { _T('\0') };
TCHAR CProfileOperator::s_szExt[_MAX_EXT] = { _T('\0') };
TCHAR CProfileOperator::s_szIniFullFilename[MAX_PATH] = { _T('\0') };

CProfileOperator::CProfileOperator()
{
	GetExecutableFilename();
	_tcscpy_s(m_szIniFilename, sizeof(m_szIniFilename) / sizeof(m_szIniFilename[0]), s_szIniFullFilename);
}

CProfileOperator::CProfileOperator(LPCTSTR lpIniFile)
{
	_tcscpy_s(m_szIniFilename, sizeof(m_szIniFilename) / sizeof(m_szIniFilename[0]), lpIniFile);
}

CProfileOperator::~CProfileOperator()
{
}

BOOL CProfileOperator::GetBool(LPCTSTR lpAppName, LPCTSTR lpKeyName, BOOL bDefault)
{	
	TCHAR szBuf[10];
	GetPrivateProfileString(lpAppName, lpKeyName, NULL, szBuf, sizeof(szBuf) / sizeof(szBuf[0]), m_szIniFilename);

	if (_tcslen(szBuf) == 0)
		return bDefault;

	return (_tcsicmp(szBuf, _T("true")) == 0);
}

INT CProfileOperator::GetInt(LPCTSTR lpAppName, LPCTSTR lpKeyName, INT nDefault)
{
	TCHAR szBuf[40];
	GetPrivateProfileString(lpAppName, lpKeyName, NULL, szBuf, sizeof(szBuf) / sizeof(szBuf[0]), m_szIniFilename);

	if (_tcslen(szBuf) == 0)
		return nDefault;

	return _ttol(szBuf);
}

DWORD CProfileOperator::GetString(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpDefault, LPTSTR lpReturnedString, DWORD nSize)
{
	return GetPrivateProfileString(lpAppName, lpKeyName, lpDefault, lpReturnedString, nSize, m_szIniFilename);
}

DWORD CProfileOperator::GetKeys(LPCTSTR lpAppName, vector<basic_string<TCHAR>> &vKeys)
{
	vKeys.clear();

	DWORD dwBufLen = 1000;
	LPTSTR pBuf = new TCHAR[dwBufLen];
	GetPrivateProfileString(lpAppName, NULL, NULL, pBuf, dwBufLen, m_szIniFilename);
	while (GetLastError() == ERROR_MORE_DATA)
	{
		delete[] pBuf;
		dwBufLen *= 2;
		pBuf = new TCHAR[dwBufLen];

		GetPrivateProfileString(lpAppName, NULL, NULL, pBuf, dwBufLen, m_szIniFilename);
	}

	DWORD dwKeyLen;
	LPTSTR pKey = pBuf;
	while ((dwKeyLen = _tcslen(pKey)) > 0)
	{
		vKeys.push_back(pKey);				
		pKey += (dwKeyLen + 1);
	}
	
	delete[] pBuf;

	return vKeys.size();
}

DWORD CProfileOperator::GetSections(vector<basic_string<TCHAR>> &vSections)
{
	vSections.clear();

	DWORD dwBufLen = 1000;
	LPTSTR pBuf = new TCHAR[dwBufLen];
	GetPrivateProfileSectionNames(pBuf, dwBufLen, m_szIniFilename);
	while (GetLastError() == ERROR_MORE_DATA)
	{
		delete[] pBuf;
		dwBufLen *= 2;
		pBuf = new TCHAR[dwBufLen];

		GetPrivateProfileSectionNames(pBuf, dwBufLen, m_szIniFilename);
	}

	DWORD dwSectionLen;
	LPTSTR pSection = pBuf;
	while ((dwSectionLen = _tcslen(pSection)) > 0)
	{
		vSections.push_back(pSection);
		pSection += (dwSectionLen + 1);
	}

	delete[] pBuf;

	return vSections.size();
}

LPCTSTR CProfileOperator::GetExecutableFilename()
{
	if (_tcslen(s_szFullName) > 0)
		return s_szFullName;

	if (!GetModuleFileName(NULL, s_szFullName, MAX_PATH))
	{
		// TODO: log error
		return NULL;
	}

	_tsplitpath_s(s_szFullName, s_szDrive, _MAX_DRIVE, s_szDir, _MAX_DIR, s_szFname, _MAX_FNAME, s_szExt, _MAX_EXT);
	_tmakepath_s(s_szFullPathWithoutFileName, s_szDrive, s_szDir, NULL, NULL);
	_tmakepath_s(s_szIniFullFilename, s_szDrive, s_szDir, s_szFname, _T("ini"));
	
	return s_szFullName;
}

LPCTSTR CProfileOperator::GetExecutablePath()
{
	if (_tcslen(s_szFullPathWithoutFileName) < 1)
	{
		GetExecutableFilename();
	}
	
	return s_szFullPathWithoutFileName;
}

LPCTSTR CProfileOperator::GetIniFullFilename()
{
	if (_tcslen(s_szIniFullFilename) < 1)
	{
		GetExecutableFilename();
	}
	
	return s_szIniFullFilename;
}
