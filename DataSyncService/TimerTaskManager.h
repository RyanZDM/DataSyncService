#pragma once
#include <wtypes.h>
#include <vector>
#include <string>
#include "TimerTask.h"

using namespace std;


class CTimerTaskManager
{
public:
	CTimerTaskManager();
	~CTimerTaskManager();

	INT static GetTimerTasks(vector<CTimerTask*> &vTasks);
	BOOL static ParseTimeString(LPCTSTR pcszTimeStr, tm &Time);
	DWORD static GetWaitSeconds(tm &tmFixedTime, INT nFixedDay = 0);
	
	// Get the last day of specified month of this year
	// Note: the month start from 0
	INT static GetLastDayOfMonth(INT month);

	// Get the last day of specified month and year
	// Note: the month start from 0
	INT static GetLastDayOfMonth(INT year, INT month);

private:
	// Get the last day of specified month and year
	// Note: the month start from 0, year = actual year - 1900
	INT static InernalGetLastDayOfMonth(INT year, INT month);
};

