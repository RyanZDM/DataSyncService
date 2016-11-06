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

	INT GetTimerTasks(vector<CTimerTask*> &vTasks);
	BOOL ParseTimeString(LPCTSTR pcszTimeStr, tm &Time);
	DWORD GetWaitSeconds(tm &tmFixedTime, INT nFixedDay = 0);
};

