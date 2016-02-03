// ImgUtil.h: interface for the CImgUtil class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMGUTIL_H__BC603313_5634_415F_9F9A_55B4247783C4__INCLUDED_)
#define AFX_IMGUTIL_H__BC603313_5634_415F_9F9A_55B4247783C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef _DISABLE_LOG_
#include <LogUtil.h>
#endif

class CImgUtil  
{
public:
	BOOL Bmp2JPG(LPCSTR pBmpFilename, LPCSTR pJpegFilename, INT nQuality = 100);
	BOOL Screen2BmpFile(LPCSTR pBmpFilename);
	BOOL Screen2JpegFile(LPCSTR pFilename, INT nQuality = 100);
	HANDLE Screen2HDIB();
	BOOL DIB2JpegFile(HANDLE hDIB, LPCSTR pFilename, INT nQuality = 100);
	BOOL DIB2BmpFile(HANDLE hDIB, LPCSTR pFilename);
	HANDLE DDB2DIB(HBITMAP hBitmap);
	
#ifndef _DISABLE_LOG_
	CImgUtil(CLogUtil *pLogger = NULL) {m_pLogger = pLogger;};
#else
	CImgUtil() {};
#endif
	
	virtual ~CImgUtil();

private:
#ifndef _DISABLE_LOG_
	CLogUtil* m_pLogger;
#endif
	void Log(LPCTSTR pMsg, INT nMessageID = 0);
};

#endif // !defined(AFX_IMGUTIL_H__BC603313_5634_415F_9F9A_55B4247783C4__INCLUDED_)
