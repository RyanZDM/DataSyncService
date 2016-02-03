#include <tchar.h>
#include "TimerTask.h"

CTimerTask::CTimerTask() : m_nInterval(0), m_bEnabled(FALSE), m_ptmRunAtFixedTime(NULL), m_szName(_T("")), m_szRun(_T(""))
{
}

CTimerTask::~CTimerTask()
{	
	if (m_ptmRunAtFixedTime)
	{
		delete m_ptmRunAtFixedTime;
	}
}

BOOL CTimerTask::ParseTimeString(LPCTSTR pcszTimeStr)
{
	if (!pcszTimeStr)
		return FALSE;

	size_t nLen = _tcslen(pcszTimeStr);
	if (nLen < 5)	// eg. 0:0:1
		return FALSE;

	// Hour part
	LPCTSTR pStart = pcszTimeStr;
	LPCTSTR pEnd = _tcsstr(pStart, _T(":"));
	if (!pEnd || (pEnd - pStart) > 2)
		return FALSE;
	
	TCHAR szBuf[3] = { _T('\0') };
	memcpy(szBuf, pStart, (pEnd - pStart) * sizeof(TCHAR));
	INT nHour = _tstol(szBuf);
	if (nHour < 0 || nHour > 23)
		return FALSE;
	
	//Minute part
	pStart = pEnd + 1;
	pEnd = _tcsstr(pStart, _T(":"));
	if (!pEnd || (pEnd - pStart) > 2)
		return FALSE;

	ZeroMemory(szBuf, 3);
	memcpy(szBuf, pStart, (pEnd - pStart) * sizeof(TCHAR));
	INT nMinute = _tstol(szBuf);
	if (nMinute < 0 || nMinute > 59)
		return FALSE;

	// Second part
	pStart = pEnd + 1;
	if (!pStart || _tcslen(pStart) > 2)
		return FALSE;

	INT nSecond = _tstol(pStart);
	if (nSecond < 0 || nSecond > 59)
		return FALSE;

	if (!m_ptmRunAtFixedTime)
	{		
		m_ptmRunAtFixedTime = new tm();
	}

	m_ptmRunAtFixedTime->tm_year = 1900;
	m_ptmRunAtFixedTime->tm_mon = 1;
	m_ptmRunAtFixedTime->tm_mday = 1;
	m_ptmRunAtFixedTime->tm_hour = nHour;
	m_ptmRunAtFixedTime->tm_min = nMinute;
	m_ptmRunAtFixedTime->tm_sec = nSecond;

	return TRUE;
}