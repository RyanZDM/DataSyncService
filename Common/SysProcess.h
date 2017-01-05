// SysProcess.h: interface for the CSysProcess class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYSPROCESS_H__3D180C2E_218B_45BA_A87D_D0B3E63E4B6E__INCLUDED_)
#define AFX_SYSPROCESS_H__3D180C2E_218B_45BA_A87D_D0B3E63E4B6E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdlib.h>
#include <string>
#include <shlwapi.h>
#include <vector>
#include <string>
using namespace std;

#include "Psapi.h"
#pragma comment( lib, "psapi.lib" )

class CSysProcess  
{
public:
	CSysProcess();
	virtual ~CSysProcess();
	
	BOOL EnumProcessesList(LPCTSTR pszFileName);
	DWORD TerminateProcess(LPCTSTR pszKey, LPCTSTR pszMatchMode);
	DWORD TerminateProcess(DWORD dwProcessID);
	BOOL IsProcessMatch(LPCTSTR pszProcessName, LPCTSTR pszKey, LPCTSTR pszMatchMode);
	DWORD GetProcessID(vector<DWORD> &ppProcessIDs, LPCTSTR pszKey, LPCTSTR pszMatchMode);

private:
	BOOL m_bOutputHeader;
};

#endif // !defined(AFX_SYSPROCESS_H__3D180C2E_218B_45BA_A87D_D0B3E63E4B6E__INCLUDED_)
