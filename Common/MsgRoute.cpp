// MsgRoute.cpp: implementation of the CMsgRoute class.
//
//////////////////////////////////////////////////////////////////////

#include "MsgRoute.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMsgRoute::CMsgRoute()
{

}

CMsgRoute::~CMsgRoute()
{
	ClearMap(m_Mappings);
	ClearMap(m_ThreadMappings);
}

BOOL CMsgRoute::Subscribe(UINT MsgID, HWND hWnd)
{
	if (!hWnd)
		return FALSE;

	MsgMapping::iterator it = m_Mappings.find(MsgID);
	if (it != m_Mappings.end())	// Find the matched key
	{
		vector<HWND> *p = it->second;
		if (p)		// Append the hWnd
		{
			for (int i =0; i < p->size(); i++)
				if ( (*p)[i] == hWnd )
					return TRUE;		// It is already registered

			p->push_back(hWnd);			// No item found, append it.
		}
		else		// The pointer to mappings is NULL although the key was found, insert this key
		{
			vector<HWND> *pNew = new vector<HWND>();
			pNew->push_back(hWnd);
			it->second = pNew;
		}
	}
	else		// No key found, insert this key
	{
		vector<HWND> *pNew = new vector<HWND>;
		pNew->push_back(hWnd);
		m_Mappings.insert(MsgMappingPair(MsgID, pNew));
	}

	return TRUE;
}

BOOL CMsgRoute::Subscribe(UINT MsgID, DWORD dwThreadID)
{
	ThreadMsgMapping::iterator it = m_ThreadMappings.find(MsgID);
	if (it != m_ThreadMappings.end())	// Find the matched key
	{
		vector<DWORD> *p = it->second;
		if (p)		// Append the hWnd
		{
			for (int i =0; i < p->size(); i++)
				if ( (*p)[i] == dwThreadID )
					return TRUE;		// It is already registered

			p->push_back(dwThreadID);			// No item found, append it.
		}
		else		// The pointer to mappings is NULL although the key was found, insert this key
		{
			vector<DWORD> *pNew = new vector<DWORD>();
			pNew->push_back(dwThreadID);
			it->second = pNew;
		}
	}
	else		// No key found, insert this key
	{
		vector<DWORD> *pNew = new vector<DWORD>;
		pNew->push_back(dwThreadID);
		m_ThreadMappings.insert(ThreadMsgMappingPair(MsgID, pNew));
	}

	return TRUE;
}

BOOL CMsgRoute::Detach(const HWND hWnd)
{
	if (!hWnd)
		return FALSE;

	MsgMapping::iterator it;
	for (it = m_Mappings.begin(); it != m_Mappings.end(); it++)
	{
		try
		{
			vector<HWND> *p = it->second;
			if (p)
			{
				vector<HWND>::iterator vit;
				for (vit = p->begin(); vit != p->end(); vit++)
				{
					if (hWnd == *vit)	// Found it, delete this item
					{
						p->erase(vit);
						break;
					}
				}
			}
		}
		catch (...)
		{
			//
		}
	}
	
	return TRUE;
}

BOOL CMsgRoute::Detach(DWORD dwThreadID)
{
	ThreadMsgMapping::iterator it;
	for (it = m_ThreadMappings.begin(); it != m_ThreadMappings.end(); it++)
	{
		try
		{
			vector<DWORD> *p = it->second;
			if (p)
			{
				vector<DWORD>::iterator vit;
				for (vit = p->begin(); vit != p->end(); vit++)
				{
					if (dwThreadID == *vit)	// Found it, delete this item
					{
						p->erase(vit);
						break;
					}
				}
			}
		}
		catch (...)
		{
			//
		}
	}
	
	return TRUE;
}

BOOL CMsgRoute::RemoveMsg(UINT MsgID, const HWND hWnd)
{
	if (!hWnd)
		return FALSE;

	MsgMapping::iterator it = m_Mappings.find(MsgID);
	if (it != m_Mappings.end())	// Find the matched key
	{
		try
		{
			vector<HWND> *p = it->second;
			if (p)
			{
				vector<HWND>::iterator vit;
				for (vit = p->begin(); vit != p->end(); vit++)
				{
					if (hWnd == *vit)	// Found it, delete this item
					{
						p->erase(vit);
						if (p->size() == 0)		// No subscriber for this message, remove this message
						{
							delete p;
							m_Mappings.erase(it);
						}

						break;
					}
				}
			}
		}
		catch (...)
		{
			//
		}
	}

	return TRUE;
}

BOOL CMsgRoute::RemoveMsg(UINT MsgID, DWORD dwThreadID)
{
	ThreadMsgMapping::iterator it = m_ThreadMappings.find(dwThreadID);
	if (it != m_ThreadMappings.end())	// Find the matched key
	{
		try
		{
			vector<DWORD> *p = it->second;
			if (p)
			{
				vector<DWORD>::iterator vit;
				for (vit = p->begin(); vit != p->end(); vit++)
				{
					if (dwThreadID == *vit)	// Found it, delete this item
					{
						p->erase(vit);
						if (p->size() == 0)		// No subscriber for this message, remove this message
						{
							delete p;
							m_ThreadMappings.erase(it);
						}

						break;
					}
				}
			}
		}
		catch (...)
		{
			//
		}
	}

	return TRUE;
}

INT CMsgRoute::Trigger(UINT MsgID, WPARAM wParam, LPARAM lParam)
{
	INT nSent = 0;

	// Send message to registered threads
	ThreadMsgMapping::iterator tit = m_ThreadMappings.find(MsgID);
	if (tit != m_ThreadMappings.end())	// Find the matched key
	{
		vector<DWORD> *p = tit->second;
		if (p)
		{
			vector<DWORD>::iterator vit;
			for (vit = p->begin(); vit != p->end(); vit++)
			{
				::PostThreadMessage(*vit, MsgID, wParam, lParam);
				nSent ++;
			}
		}
	}

	// Send message to registered Wnds
	MsgMapping::iterator it = m_Mappings.find(MsgID);
	if (it != m_Mappings.end())	// Find the matched key
	{
		vector<HWND> *p = it->second;
		if (p)
		{
			vector<HWND>::iterator vit;
			for (vit = p->begin(); vit != p->end(); vit++)
			{
				::SendMessage(*vit, MsgID, wParam, lParam);
				nSent ++;
			}
		}
	}

	return nSent;
}


