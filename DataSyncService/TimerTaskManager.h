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
};

