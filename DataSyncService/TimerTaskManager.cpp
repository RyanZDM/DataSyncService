#include "TimerTaskManager.h"
#include <time.h>
#include "StrUtil.h"
#include "LogUtil.h"
#include "SysParams.h"
#include "Constants.h"
#include "ProfileOperator.h"
#include "ContainerUtil.h"

CTimerTaskManager::CTimerTaskManager()
{
}


CTimerTaskManager::~CTimerTaskManager()
{
}

/**************************************************************************
* Get the task definition of timer task from ini file TimerTasksFileName.
* Note: The elements in vector need to be released outside
**************************************************************************/
INT CTimerTaskManager::GetTimerTasks(vector<CTimerTask*> &vTasks)
{
	ClearVector(vTasks);

	LPCTSTR pPath = CProfileOperator::GetExecutablePath();
	TCHAR szTaskIniFile[MAX_PATH];
	_stprintf_s(szTaskIniFile, MAX_PATH, _T("%s%s"), CProfileOperator::GetExecutablePath(), TimerTasksFileName);
	CProfileOperator po(szTaskIniFile);
	vector<basic_string<TCHAR>> vSections;
	po.GetSections(vSections);

	TCHAR szBuf[1000];
	DWORD dwLen = sizeof(szBuf) / sizeof(szBuf[0]);
	for (vector<basic_string<TCHAR>>::const_iterator vi = vSections.begin(); vi != vSections.end(); vi++)
	{
		CTimerTask *pTask = new CTimerTask();

		basic_string<TCHAR> szSection = *vi;
		pTask->m_szName = szSection;

		po.GetString(szSection.c_str(), _T("Run"), NULL, szBuf, dwLen);
		pTask->m_szRun = szBuf;
		
		tm time;
		po.GetString(szSection.c_str(), _T("RunAtFixedTime"), NULL, szBuf, dwLen);
		if (ParseTimeString(szBuf, time))
		{
			pTask->SetRunAtFixedTime(time);
		}

		pTask->SetFixedDay(po.GetInt(szSection.c_str(), _T("RunAtFixedDay"), 0));
		pTask->SetDelay(po.GetInt(szSection.c_str(), _T("RunAtFixedDay"), 0));
		pTask->SetInterval(po.GetInt(szSection.c_str(), _T("Interval"), 0));
		pTask->m_bEnabled = po.GetBool(szSection.c_str(), _T("Enable"), FALSE);

		vTasks.push_back(pTask);
	}

	return vTasks.size();
}

BOOL CTimerTaskManager::ParseTimeString(LPCTSTR pcszTimeStr, tm &Time)
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

	Time.tm_year = 1900;
	Time.tm_mon = 1;
	Time.tm_mday = 1;
	Time.tm_hour = nHour;
	Time.tm_min = nMinute;
	Time.tm_sec = nSecond;

	return TRUE;
}

DWORD CTimerTaskManager::GetWaitSeconds(tm &tmFixedTime, INT nFixedDay)
{
	time_t now = time(nullptr);
	tm localTime;
	localtime_s(&localTime, &now);

	// Check if the fixed time has passed today first
	tmFixedTime.tm_year = localTime.tm_year;
	tmFixedTime.tm_mon = localTime.tm_mon;
	tmFixedTime.tm_mday = localTime.tm_mday;
	time_t target = mktime(&tmFixedTime);
	DOUBLE seconds = difftime(target, now);

	BOOL timePassedToday = (seconds < 0);
	BOOL dayPassedToday = nFixedDay < localTime.tm_mday;

	if (nFixedDay <= 0)	// Run at fixed time today or tomorrow if the specified time has passed
	{
		if (timePassedToday)
		{
			// Delay one day		
			target += 24 * 60 * 60;
		}
	}
	else				// Run at fixed time at fixed sday of a month
	{
		if (dayPassedToday || (timePassedToday && (nFixedDay == localTime.tm_mday)))
		{
			// Delay to next month if time or day passed
			INT month = tmFixedTime.tm_mon + 1;
			tmFixedTime.tm_mon = (month < 12) ? month : 0;		// start from 0
			tmFixedTime.tm_year = (month < 12) ? localTime.tm_year : localTime.tm_year + 1;
		}
		else
		{
			// The last day of each month may be different. eg. 2.28, 3.31, 4.30
			tmFixedTime.tm_mday = min(nFixedDay, localTime.tm_mday);	// TODO get last day of month
		}

		target = mktime(&tmFixedTime);
	}

	return (DWORD)difftime(target, now);
}

INT GetLastDayOfMonth(INT month)
{
	return 0;
}