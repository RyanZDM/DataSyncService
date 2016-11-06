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
