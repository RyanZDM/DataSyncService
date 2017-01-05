// NetworkUtil.cpp: implementation of the CNetworkUtil class.
//
//////////////////////////////////////////////////////////////////////

#include "NetworkUtil.h"
#include <shlwapi.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNetworkUtil::CNetworkUtil()
{
	m_pAdapterInfo = NULL;
}

CNetworkUtil::~CNetworkUtil()
{
	ReleaseMem();
}

INT CNetworkUtil::GetNICList(list<PIP_ADAPTER_INFO> &NICs)
{
	PIP_ADAPTER_INFO pAdapter = NULL;
	DWORD dwRetVal = 0;

	if (m_pAdapterInfo)
		ReleaseMem();

	m_pAdapterInfo = (IP_ADAPTER_INFO *) malloc( sizeof(IP_ADAPTER_INFO) );
	ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
	
	// Make an initial call to GetAdaptersInfo to get
	// the necessary size into the ulOutBufLen variable
	if (GetAdaptersInfo( m_pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
		free(m_pAdapterInfo);
		m_pAdapterInfo = (PIP_ADAPTER_INFO) malloc (ulOutBufLen); 
	}
	
	NICs.clear();
	if ((dwRetVal = GetAdaptersInfo( m_pAdapterInfo, &ulOutBufLen)) == NO_ERROR) {
		pAdapter = m_pAdapterInfo;
		while (pAdapter) {	
			NICs.push_back(pAdapter);
			pAdapter = pAdapter->Next;
		}
	}
	else {
		//printf("Call to GetAdaptersInfo failed.\n");
	}

	return NICs.size();
}

void CNetworkUtil::ReleaseMem()
{
	if (m_pAdapterInfo)
	{
		free(m_pAdapterInfo);
		m_pAdapterInfo = NULL;
	}
}

INT CNetworkUtil::GetMacAddrs(list<BYTE*> &MacAddrs)
{
	MacAddrs.clear();
	std::list<PIP_ADAPTER_INFO> adapters;
	if (GetNICList(adapters) > 0)
	{
		std::list<PIP_ADAPTER_INFO>::iterator iter = adapters.begin();
		for (; iter != adapters.end(); iter++)
		{
			PIP_ADAPTER_INFO pAdapter = (PIP_ADAPTER_INFO)(*iter);
			if (pAdapter)
			{
				MacAddrs.push_back((BYTE*)pAdapter->Address);
			}
		}
	}

	return MacAddrs.size();
}

BYTE * CNetworkUtil::GetMacAddr(const char *pcszAdapterName)
{
	BYTE *pMac = NULL;

	if (pcszAdapterName)
	{
		list<PIP_ADAPTER_INFO> NICs;
		if (GetNICList(NICs) > 0)
		{
			list<PIP_ADAPTER_INFO>::iterator it = NICs.begin();
			for (; it != NICs.end(); it++)
			{
				PIP_ADAPTER_INFO pData = (PIP_ADAPTER_INFO)(*it);
				if (pData)
				{
					if (StrCmpI(pcszAdapterName, pData->AdapterName) == 0)
					{
						pMac = (BYTE*)pData->Address;
						break;
					}
				}
			}
		}
	}

	return pMac;
}

PIP_ADAPTER_INFO CNetworkUtil::GetAdapter(const char *pcszAdapterName)
{
	PIP_ADAPTER_INFO pAdapter = NULL;

	if (pcszAdapterName)
	{
		list<PIP_ADAPTER_INFO> NICs;
		if (GetNICList(NICs) > 0)
		{
			list<PIP_ADAPTER_INFO>::iterator it = NICs.begin();
			for (; it != NICs.end(); it++)
			{
				PIP_ADAPTER_INFO pTemp = (PIP_ADAPTER_INFO)(*it);
				if (StrCmpI(pcszAdapterName, pTemp->AdapterName) == 0)
				{
					pAdapter = pTemp;
					break;
				}
			}
		}
	}

	return pAdapter;
}
