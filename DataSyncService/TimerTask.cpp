#include <tchar.h>
#include "TimerTask.h"

CTimerTask::CTimerTask() : m_nInterval(0), m_nDelay(0), m_bEnabled(FALSE), m_ptmRunAtFixedTime(NULL), m_szName(_T("")), m_szRun(_T(""))
{
}

CTimerTask::~CTimerTask()
{	
	if (m_ptmRunAtFixedTime)
	{
		delete m_ptmRunAtFixedTime;
	}
}

void CTimerTask::SetRunAtFixedTime(const tm &time)
{
	if (!m_ptmRunAtFixedTime)
	{
		m_ptmRunAtFixedTime = new tm();
	}

	CopyTime(m_ptmRunAtFixedTime, time);
}

void CTimerTask::CopyTime(tm *pTime, const tm &time)
{
	if (!pTime)
		return;

	m_ptmRunAtFixedTime->tm_year = time.tm_year;
	m_ptmRunAtFixedTime->tm_mon = time.tm_mon;
	m_ptmRunAtFixedTime->tm_mday = time.tm_mday;
	m_ptmRunAtFixedTime->tm_hour = time.tm_hour;
	m_ptmRunAtFixedTime->tm_min = time.tm_min;
	m_ptmRunAtFixedTime->tm_sec = time.tm_sec;
}

LPCTSTR CTimerTask::ToString()
{
	m_szDescription.clear();
		
	TCHAR szInterval[50];
	TCHAR szDelay[50];
	TCHAR szRunAtFixedDay[10];

	_itot_s(m_nInterval, szInterval, sizeof(szInterval) / sizeof(szInterval[0]), 10);
	_itot_s(m_nDelay, szDelay, sizeof(szDelay) / sizeof(szDelay[0]), 10);
	_itot_s(m_nRunAtFixedDay, szRunAtFixedDay, sizeof(szRunAtFixedDay) / sizeof(szRunAtFixedDay[0]), 10);	

	m_szDescription.append(_T("Task: {Name:")).append(m_szName)
		.append(_T("}{Enabled:")).append(m_bEnabled ? _T("True") : _T("False"))
		.append(_T("}{Delay:")).append(szDelay)
		.append(_T("}{Interval:")).append(szInterval)
		.append(_T("}{RunAtFixedDay:")).append(szRunAtFixedDay)		
		.append(_T("}{Run:")).append(m_szRun).append(_T("}"));

	if (m_ptmRunAtFixedTime)
	{	
		TCHAR szTime[50];
		_stprintf_s(szTime, _T("%d:%d:%d"), m_ptmRunAtFixedTime->tm_hour, m_ptmRunAtFixedTime->tm_min, m_ptmRunAtFixedTime->tm_sec);
		m_szDescription.append(_T("{RunAtFixedTime:")).append(szTime).append(_T("}"));
	}
	return m_szDescription.c_str();
}

