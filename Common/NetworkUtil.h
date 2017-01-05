// NetworkUtil.h: interface for the CNetworkUtil class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NETWORKUTIL_H__BB396A0A_C2E1_440F_9A75_63A358FC2E13__INCLUDED_)
#define AFX_NETWORKUTIL_H__BB396A0A_C2E1_440F_9A75_63A358FC2E13__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <wtypes.h>
#include <list>
#include <windows.h>
#include <TCHAR.H>
#include <string>

// !!! Need to include this header file just after the file "stdafx.h"
// !!! Need to move the following two item to top in (Options->Directories) window
	// D:\Program Files\Microsoft Visual Studio 8\VC\PlatformSDK\Include
	// D:\Program Files\Microsoft Visual Studio 8\VC\PlatformSDK\Lib

#include <httpfilt.h>
#include <Iphlpapi.h>					
#pragma comment(lib, "Iphlpapi.lib")	

using namespace std;

class CNetworkUtil  
{
public:
	PIP_ADAPTER_INFO GetAdapter(const char *pcszAdapterName);
	BYTE * GetMacAddr(const char *pcszAdapterName);
	INT GetMacAddrs(list<BYTE*> &MacAddrs);
	INT GetNICList(list<PIP_ADAPTER_INFO> &NICs);
	CNetworkUtil();
	virtual ~CNetworkUtil();

private:
	void ReleaseMem();
	PIP_ADAPTER_INFO m_pAdapterInfo;
};

#endif // !defined(AFX_NETWORKUTIL_H__BB396A0A_C2E1_440F_9A75_63A358FC2E13__INCLUDED_)
