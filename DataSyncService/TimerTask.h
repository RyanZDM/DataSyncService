#pragma once
#include <wtypes.h>
#include <string>
using namespace std;

class CTimerTask
{
public:
	basic_string<TCHAR>	m_szName;
	basic_string<TCHAR>	m_szRun;
	BOOL				m_bEnabled;
	
	INT GetFixedDay() { return m_nRunAtFixedDay; }
	void SetFixedDay(INT nDelay) { m_nRunAtFixedDay = (nDelay > 0 && nDelay < 32) ? nDelay : 0; }
	INT GetDelay() { return m_nDelay; }
	void SetDelay(INT nDelay) { m_nDelay = (nDelay > 0) ? nDelay : 0; }
	INT GetInterval() { return m_nInterval; }
	void SetInterval(INT nInterval) { m_nInterval = (nInterval > 0) ? nInterval : 0; }
	tm* GetRunAtFixedTimePtr() { return m_ptmRunAtFixedTime; }
	void SetRunAtFixedTime(const tm &time);

	CTimerTask();
	~CTimerTask();

private:
	INT					m_nInterval;			// in seconds, 0 means run one time only, will be ignord if ptmRunAtFixedTime is not NULL	
	INT					m_nDelay;				// Delay how many seconds to run timer when service started
	INT					m_nRunAtFixedDay;		// The day of month
	tm *				m_ptmRunAtFixedTime;	// Only care about the time part

	void CopyTime(tm *pTime, const tm &time);
};

