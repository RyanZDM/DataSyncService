// ContainerUtil.h: interface for the CContainerUtil class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONTAINERUTIL_H__15F960AB_3180_46EA_BC1C_02CF7C72F733__INCLUDED_)
#define AFX_CONTAINERUTIL_H__15F960AB_3180_46EA_BC1C_02CF7C72F733__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning( disable : 4786 )

#include <wtypes.h>
#include <vector>
#include <list>
#include <map>
using namespace std;

template <typename T1, typename T2> void ClearContainer(T1& container);
template <typename T> void ClearVector(vector<T> &dest);
template <typename T> void ClearList(list<T> &dest);
template <typename K, typename T> void ClearMap(map<K, T> &dest);		// We think the pointer is saved in element, ignore the KEY

template <typename T1, typename T2>
void ClearContainer(T1& container)
{
	T1::iterator it;
	for (it = container.begin(); it != container.end(); it++)
	{
		try
		{
			T2 p = (T2)*it;
			if (p)
				delete p;
		}
		catch (...) {
			int a = 0;
		}
	}

	container.clear();
}

template <typename T1, typename T2>
void ClearContainer(T1* pContainer)
{
	if (!pContainer)
		return;

	T1::iterator it;
	for (it = pContainer->begin(); it != pContainer->end(); it++)
	{
		try
		{
			T2 p = (T2)*it;
			if (p)
				delete p;
		}
		catch (...) {
		}
	}

	pContainer->clear();
}

template <typename T>
void ClearVector(vector<T> &dest)
{
	ClearContainer<vector<T>, T>(dest);
}

template <typename T>
void ClearVector(vector<T> *pDest)
{
	ClearContainer<vector<T>, T>(pDest);
}


template <typename T>
void ShrinkVector(vector<T> &dest)
{
  if (dest.size() == 0)
	  return;

  vector<T>	vNew;  
  vNew.reserve(dest.size()); 
  vNew.assign(dest.begin(), dest.end());   
  dest.swap(vNew);
}

template <typename T>
void ClearList(list<T> &dest)
{
	ClearContainer<list<T>, T>(dest);
}

template <typename K, typename T, typename T1>
void ClearMap(map<K, T, T1> &container, BOOL bAlsoReleaseKey = FALSE)
{
	for (map<K, T, T1>::iterator mi = container.begin(); mi != container.end(); mi++)
	{
		try
		{
			T pVal = (T)mi->second;
			if (pVal)
				delete pVal;
		}
		catch (...) {
		}

		if (bAlsoReleaseKey)
		{
			try
			{
				K pKey = (K)mi->first;
				if (pKey)
					delete pKey;
			}
			catch (...) {
			}
		}
	}

	container.clear();
}

template <typename K, typename T>
void ClearMap(map<K, T> &container, BOOL bAlsoReleaseKey = FALSE)
{
	for (map<K, T>::iterator mi = container.begin(); mi != container.end(); mi++)
	{
		try
		{
			T pVal = (T)mi->second;
			if (pVal)
				delete pVal;
		}
		catch (...) {
		}

		if (bAlsoReleaseKey)
		{
			try
			{
				K pKey = (K)mi->first;
				if (pKey)
					delete pKey;
			}
			catch (...) {
			}
		}
	}

	container.clear();
}

template <typename K, typename T>
void ClearMapVector(map<K, vector<T>*> &container, BOOL bAlsoReleaseKey = FALSE)
{
	for (map<K, vector<T>*>::iterator mi = container.begin(); mi != container.end(); mi++)
	{
		try
		{
			vector<T>* p = (vector<T>*)mi->second;
			if (p)
			{
				ClearVector(p);
			}
		}
		catch (...) {
		}

		if (bAlsoReleaseKey)
		{
			try
			{
				K pKey = (K)mi->first;
				if (pKey)
					delete pKey;
			}
			catch (...) {
			}
		}
	}


	container.clear();
}

#endif // !defined(AFX_CONTAINERUTIL_H__15F960AB_3180_46EA_BC1C_02CF7C72F733__INCLUDED_)
