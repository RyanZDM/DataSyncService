// MyDB.h: interface for the CMyDB class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYDB_H__414CBBB6_64B5_4AD2_83F9_52021CD9012B__INCLUDED_)
#define AFX_MYDB_H__414CBBB6_64B5_4AD2_83F9_52021CD9012B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DBUtil.h"

class CMyDB : public CDBUtil  
{
public:
	INT RemoveAllItems();
	BOOL Connect();
	CMyDB();
	virtual ~CMyDB();

};

#endif // !defined(AFX_MYDB_H__414CBBB6_64B5_4AD2_83F9_52021CD9012B__INCLUDED_)
