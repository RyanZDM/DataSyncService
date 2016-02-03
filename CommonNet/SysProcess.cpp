// SysProcess.cpp: implementation of the CSysProcess class.
//
//////////////////////////////////////////////////////////////////////

#include "SysProcess.h"
#include <stdio.h>
#include <tchar.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSysProcess::CSysProcess()
{
	m_bOutputHeader = TRUE;
}

CSysProcess::~CSysProcess()
{

}

BOOL CSysProcess::EnumProcessesList(LPCTSTR pszFileName) {
    // Get the list of process identifiers.
    DWORD aProcesses[1024], cbNeeded, cProcesses;
    unsigned int i;

	FILE *f = NULL;
	f = _tfopen(pszFileName, _T("w"));
	if (!f)
		return false;
	
    if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) )
        return false;

	if (m_bOutputHeader) {
		_ftprintf(f, _T("%-30s\t%5s\t%10s\t%10s\n"), _T("Process Name"), _T("PID"), _T("  Mem Usage"), _T("    VM Size"));
		_ftprintf(f, _T("--------------------------------------------------------------------\n"));
	}

	// Calculate how many process identifiers were returned.
    cProcesses = cbNeeded / sizeof(DWORD);
    TCHAR szProcessName[MAX_PATH] = _T("unknown");
    for ( i = 0; i < cProcesses; i++ ) {
	    // Get a handle to the process.
		HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
									   PROCESS_VM_READ,
									   FALSE, aProcesses[i] );

		// Get the process name.
		if (NULL != hProcess )
		{
			HMODULE hMod;
			DWORD cbNeeded;
			PROCESS_MEMORY_COUNTERS pmc = {0};

			if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod), &cbNeeded) )
			{
				if ( GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName)) == 0 ) {				
					lstrcpy(szProcessName, _T("unknown"));
				}

				GetProcessMemoryInfo( hProcess, &pmc, sizeof(pmc));

				if (f)
					_ftprintf(f, _T("%-30s\t%5u\t%10uK\t%10uK\n"), szProcessName, aProcesses[i], pmc.WorkingSetSize/1024, pmc.PagefileUsage/1024);
			}
			else {				
				// do nothing
			}

			CloseHandle( hProcess );
		}
		else {
			// do nothing
		}		
	}
	if (f)
		fclose(f);
	
	return true;
}

DWORD CSysProcess::TerminateProcess(LPCTSTR pszKey, LPCTSTR pszMatchMode) {
	if (StrCmpI(pszMatchMode, _T("kp")) == 0) {
		// Kill a process by PID
		return (TerminateProcess(_ttoi(pszKey)) == 0)?1:0;
	}

	vector<DWORD> ProcessIDs;
	if (GetProcessID(ProcessIDs, pszKey, pszMatchMode) <= 0)
		return 0;

	DWORD dwCount = 0;
	vector<DWORD>::iterator vi;
	for (vi = ProcessIDs.begin(); vi != ProcessIDs.end(); vi++) {
		if (TerminateProcess(*vi) == 0)
			dwCount ++;
	}

	return dwCount;
}

DWORD CSysProcess::TerminateProcess(DWORD dwProcessID) {
	HANDLE hProcess = OpenProcess(PROCESS_TERMINATE,
								  FALSE,
								  dwProcessID);
	if (NULL == hProcess)
		return GetLastError();

	return (::TerminateProcess(hProcess, 0))?0:GetLastError();
}

BOOL CSysProcess::IsProcessMatch(LPCTSTR pszProcessName, LPCTSTR pszKey, LPCTSTR pszMatchMode) {
	if (StrCmpI(pszMatchMode, _T("kf")) == 0) {
		// full match
		return (StrCmpI(pszProcessName, pszKey) == 0);
	}
	else if (StrCmpI(pszMatchMode, _T("kc")) == 0) {
		// sub string is contained in dest string
		return (StrStrI(pszProcessName, pszKey) != NULL);
	}
	else if (StrCmpI(pszMatchMode, _T("ks")) == 0) {
		// dest string is start with sub string
		return (StrStrI(pszProcessName, pszKey) == pszProcessName);
	}
	else if (StrCmpI(pszMatchMode, _T("ke")) == 0) {
		// dest string is end with sub string
		DWORD dwNameLen = lstrlen(pszProcessName);
		DWORD dwKeyLen = lstrlen(pszKey);
		if (dwNameLen < dwKeyLen)
			return FALSE;

		LPCTSTR pszTemp = pszProcessName + (dwNameLen - dwKeyLen);
		return (StrCmpI(pszTemp, pszKey) == 0);
	}
	else {
		return FALSE;
	}

	return TRUE;
}

DWORD CSysProcess::GetProcessID(vector<DWORD> &ppProcessIDs, LPCTSTR pszKey, LPCTSTR pszMatchMode) {
	ppProcessIDs.clear();
	
	if ( (NULL == pszKey) || (NULL == pszMatchMode) )
		return 0;

	if ( (lstrlen(pszKey) <= 0) || (lstrlen(pszMatchMode) <= 0) )
		return 0;

    DWORD aProcesses[1024], cbNeeded, cProcesses;
	if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) )
        return 0;

	TCHAR szProcessName[MAX_PATH];
	cProcesses = cbNeeded / sizeof(DWORD);
	for (DWORD i = 0; i < cProcesses; i++ ) {
		HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
									   PROCESS_VM_READ,
									   FALSE, aProcesses[i] );

		// Get the process name.
		if (NULL != hProcess )
		{
			HMODULE hMod;
			DWORD cbNeeded;

			if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod), &cbNeeded) )
				if ( GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName)/sizeof(TCHAR)) > 0 )
					if (IsProcessMatch(szProcessName, pszKey, pszMatchMode))
						ppProcessIDs.push_back(aProcesses[i]);

			CloseHandle( hProcess );
		}
	}

	return ppProcessIDs.size();
}


