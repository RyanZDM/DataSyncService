// DiskUtil.h: interface for the CDiskUtil class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DISKUTIL_H__1EE897F4_F871_4467_9A0A_E7C48B1D1AA1__INCLUDED_)
#define AFX_DISKUTIL_H__1EE897F4_F871_4467_9A0A_E7C48B1D1AA1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <wtypes.h>
#include <tchar.h>

//static const LPCTSTR LOG_FILE_NAME = _T("DiskUtil.log");

class CDiskUtil  
{
public:
	static INT IsDiskProtected(LPCTSTR pcszDiskLetter);

	CDiskUtil();
	virtual ~CDiskUtil();

};

#endif // !defined(AFX_DISKUTIL_H__1EE897F4_F871_4467_9A0A_E7C48B1D1AA1__INCLUDED_)
