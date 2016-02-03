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
	
	INT GetInterval() { return m_nInterval; }
	void SetInterval(INT nInterval) { m_nInterval = (nInterval > 0) ? nInterval : 0; }
	tm* GetRunAtFixedTimePtr() { return m_ptmRunAtFixedTime; }
	BOOL ParseTimeString(LPCTSTR pcszTimeStr);

	CTimerTask();
	~CTimerTask();

private:
	INT					m_nInterval;			// in seconds, 0 means run one time only, will be ignord if ptmRunAtFixedTime is not NULL	
	tm *				m_ptmRunAtFixedTime;	// Only care about the time part
};

