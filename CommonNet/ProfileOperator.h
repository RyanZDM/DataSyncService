#pragma once

#include <wtypes.h>
#include <string>
#include <vector>
using namespace std;

class CProfileOperator
{
public:
	CProfileOperator();
	CProfileOperator(LPCTSTR lpIniFile);
	virtual ~CProfileOperator();

	DWORD GetString(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpDefault, LPTSTR lpReturnedString, DWORD nSize);
	DWORD GetKeys(LPCTSTR lpAppName, vector<basic_string<TCHAR>> &vKeys);
	DWORD GetSections(vector<basic_string<TCHAR>> &vKeys);

	static LPCTSTR GetExecutableFilename();
	static LPCTSTR GetExecutablePath();
	static LPCTSTR GetIniFullFilename();
	
private:
	TCHAR m_szIniFilename[MAX_PATH];

	static TCHAR s_szFullName[MAX_PATH];
	static TCHAR s_szFullPathWithoutFileName[MAX_PATH];
	static TCHAR s_szDrive[_MAX_DRIVE];
	static TCHAR s_szDir[_MAX_DIR];
	static TCHAR s_szFname[_MAX_FNAME];
	static TCHAR s_szExt[_MAX_EXT];
	static TCHAR s_szIniFullFilename[MAX_PATH];
	
};

