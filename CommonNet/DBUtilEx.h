// DBUtilEx.h: interface for the CDBUtilEx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBUTILEX_H__277A710B_59BA_4032_8995_554AAE0D98A6__INCLUDED_)
#define AFX_DBUTILEX_H__277A710B_59BA_4032_8995_554AAE0D98A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DBUtil.h"

#define	CONNECTION_POOL_COUNT	10

class CDBUtilEx : public CDBUtil  
{
public:
	CDBUtilEx();
	virtual ~CDBUtilEx();

private:
	WORD	m_dwConnectionPoolCount;
};

#endif // !defined(AFX_DBUTILEX_H__277A710B_59BA_4032_8995_554AAE0D98A6__INCLUDED_)
