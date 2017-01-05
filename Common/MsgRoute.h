// MsgRoute.h: interface for the CMsgRoute class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGROUTE_H__CE48FB04_258A_463F_B9A3_526A78929750__INCLUDED_)
#define AFX_MSGROUTE_H__CE48FB04_258A_463F_B9A3_526A78929750__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning( disable : 4786 )

#include <wtypes.h>
#include <map>
#include <vector>
#include <algorithm>
#include <ContainerUtil.h>
using namespace std;

typedef map<UINT, vector<HWND>*>	MsgMapping;
typedef pair <UINT, vector<HWND>*>	MsgMappingPair;
typedef map<UINT, vector<DWORD>*>	ThreadMsgMapping;
typedef pair <UINT, vector<DWORD>*>	ThreadMsgMappingPair;

class CMsgRoute  
{
public:
	INT Trigger(UINT MsgID, WPARAM wParam, LPARAM lParam);
	BOOL RemoveMsg(UINT MsgID, const HWND hWnd);
	BOOL RemoveMsg(UINT MsgID, DWORD dwThreadID);
	BOOL Detach(const HWND hWnd);
	BOOL Detach(DWORD dwThreadId);
	BOOL Subscribe(UINT MsgID, HWND hWnd);
	BOOL Subscribe(UINT MsgID, DWORD dwThreadID);
	CMsgRoute();
	virtual ~CMsgRoute();

private:
	MsgMapping m_Mappings;
	ThreadMsgMapping m_ThreadMappings;
};

#endif // !defined(AFX_MSGROUTE_H__CE48FB04_258A_463F_B9A3_526A78929750__INCLUDED_)
