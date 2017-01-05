// ImgUtil.cpp: implementation of the CImgUtil class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ImgUtil.h"
#include <fstream>
#include "jpegfile.h"

using namespace std;

#pragma comment(lib, "Gdi32.lib")

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CImgUtil::~CImgUtil()
{

}

void CImgUtil::Log(LPCTSTR pMsg, INT nMessageID)
{
#ifndef _DISABLE_LOG_
	if (m_pLogger) m_pLogger->Log(pMsg, nMessageID);
#else
	UNREFERENCED_PARAMETER(pMsg);
	UNREFERENCED_PARAMETER(nMessageID);
#endif	
}

BOOL CImgUtil::Screen2BmpFile(LPCSTR pBmpFilename)
{
	if (!pBmpFilename)
		return FALSE;

	HANDLE hDIB = Screen2HDIB();
	if (!hDIB)
		return FALSE;

	BOOL bRet = DIB2BmpFile(hDIB, pBmpFilename);

	GlobalFree(hDIB);

	return bRet;;
}


HANDLE CImgUtil::DDB2DIB( HBITMAP hBitmap)
{
	BITMAP	TmpBmp;
	BITMAPINFOHEADER bi;
	
	HDC hDC = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
	DWORD dwBitcount = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);
	DeleteDC(hDC);
	if (dwBitcount <= 1)
		dwBitcount = 1;
	else if (dwBitcount <= 4)
		dwBitcount = 4;
	else if (dwBitcount <= 8)
		dwBitcount = 8;
	else if (dwBitcount <= 24)
		dwBitcount = 24;

	DWORD dwPaletteSize = 0;
	if (dwBitcount <= 8)
	{
		dwPaletteSize = (1 << dwBitcount) *sizeof(RGBQUAD);
		//if (dwPaletteSize > 256)
		//	dwPaletteSize = 0;
	}


	//Get bitmap info
	GetObject(hBitmap, sizeof(TmpBmp), &TmpBmp);

	//Initialize the bitmapinfoheader
	bi.biSize			= sizeof(BITMAPINFOHEADER);
	bi.biWidth			= TmpBmp.bmWidth;
	bi.biHeight			= TmpBmp.bmHeight;
	bi.biPlanes			= 1;
	bi.biBitCount		= (WORD)dwBitcount;
	bi.biCompression	= BI_RGB;
	bi.biSizeImage		= 0;
	bi.biXPelsPerMeter	= 0;
	bi.biYPelsPerMeter	= 0;
	bi.biClrUsed		= 0;
	bi.biClrImportant	= 0;

	//compute the bytes of the BITMAPINFOHEADER and the color table
	DWORD	dwBytes = bi.biSizeImage + sizeof(BITMAPINFOHEADER) + dwPaletteSize;

	hDC = ::GetDC(NULL);
	//Get the palette
	HPALETTE hPal = (HPALETTE)GetStockObject(DEFAULT_PALETTE);
	//Get DC
	hPal = SelectPalette(hDC, hPal, FALSE);
	RealizePalette(hDC);

	//Allocate enough memory to hold bitmapinfoheader and color table
	HANDLE	hDIB = GlobalAlloc(GMEM_FIXED, dwBytes);
	if (hDIB == NULL)
	{
		Log(_T("DDB2DIB:GlobalAlloc failed. "), GetLastError());
		SelectPalette(hDC, hPal, FALSE);
		::ReleaseDC(NULL, hDC);
		return NULL;
	}
	
	//Get the dimensions and format of the bitmap
	LPBITMAPINFOHEADER lpbi = (LPBITMAPINFOHEADER)hDIB;
	//memcpy(lpbi, pbi, sizeof(BITMAPINFOHEADER));
	*lpbi = bi;

	GetDIBits(hDC, hBitmap, 0, bi.biHeight, NULL, (LPBITMAPINFO)lpbi, DIB_RGB_COLORS);
	//memcpy(pbi, lpbi, sizeof(BITMAPINFOHEADER));
	bi = *lpbi;
	
	//If the driver did not fill in the bisizeimage field, then compute it
	// each scan line of the image is aligned on a dword (32bit) boundary
	if (bi.biSizeImage == 0)
		bi.biSizeImage = (((bi.biWidth * bi.biBitCount + 31) & ~31) / 8) *bi.biHeight;

	//Realloc the buffer so that it can hold all the bits
	HANDLE hHandle;

	dwBytes += bi.biSizeImage;
	if (NULL == (hHandle = GlobalReAlloc(hDIB, dwBytes, GMEM_MOVEABLE)))
	{
		Log(_T("DDB2DIB:GlobalReAlloc failed. "), GetLastError());
		GlobalFree(hDIB);
		SelectPalette(hDC, hPal, FALSE);
		::ReleaseDC(NULL, hDC);
		return NULL;
	}

	hDIB = hHandle;
	lpbi = (LPBITMAPINFOHEADER)hDIB;

	//Finally get the dib
	if (!GetDIBits(hDC, 
				   hBitmap, 
				   0, 
				   bi.biHeight,
				   (LPBYTE)lpbi + (bi.biSize + dwPaletteSize),
				   (LPBITMAPINFO)lpbi, 
				   DIB_RGB_COLORS) )
	{
		Log(_T("DDB2DIB:GetDIBits failed. "), GetLastError());
		GlobalFree(hDIB);
		SelectPalette(hDC, hPal, FALSE);
		::ReleaseDC(NULL, hDC);
		return NULL;
	}

	SelectPalette(hDC, hPal, FALSE);
	::ReleaseDC(NULL, hDC);

	return hDIB;
}

BOOL CImgUtil::DIB2BmpFile(HANDLE hDIB, LPCSTR pFilename)
{
	if ( (NULL == hDIB) || (NULL == pFilename) )
	{
		Log(_T("DIB2BmpFile:Parameter is invalid. "));
		return FALSE;
	}
	
	HANDLE fh = CreateFile(pFilename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);	
	if (fh == INVALID_HANDLE_VALUE)
	{
		Log(_T("DIB2BmpFile:CreateFile failed. "), GetLastError());
		return FALSE;
	}

	DWORD dwPaletteSize = 0;
	PBITMAPINFOHEADER pbi = (PBITMAPINFOHEADER)hDIB;
	if (pbi->biBitCount <= 8)
		dwPaletteSize = (1 << pbi->biBitCount) *sizeof(RGBQUAD);	

	BITMAPFILEHEADER bmfHdr;	
	bmfHdr.bfType		= 0x4D42;  // "BM"
	bmfHdr.bfReserved1	= 0;
	bmfHdr.bfReserved2	= 0;
	bmfHdr.bfOffBits	= (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;
	bmfHdr.bfSize		= bmfHdr.bfOffBits + pbi->biSizeImage;
	
	DWORD dwWritten;
	::WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
	::WriteFile(fh, (LPSTR)hDIB, (pbi->biSizeImage + sizeof(BITMAPINFOHEADER)), &dwWritten, NULL);

	CloseHandle(fh);

	return TRUE;;
}

BOOL CImgUtil::DIB2JpegFile(HANDLE hDIB, LPCSTR pFilename, INT nQuality)
{
	if ( (NULL == hDIB) || (NULL == pFilename) )
	{
		Log(_T("DIB2JpegFile:Parameter is invalid. "));
		return FALSE;
	}
	
	if (nQuality <= 0)
		nQuality = 100;

	BOOL bRet = FALSE;

	try
	{
		PBITMAPINFOHEADER pbi = (PBITMAPINFOHEADER)hDIB;
		
		DWORD dwPaletteSize = 0;
		if (pbi->biBitCount <= 8)
			dwPaletteSize = (1 << pbi->biBitCount) * sizeof(RGBQUAD);
		
		BYTE *pData = (BYTE*)(hDIB)+sizeof(BITMAPINFOHEADER) + dwPaletteSize;
		
		JpegFile::VertFlipBuf(pData, pbi->biWidth * 3, pbi->biHeight);
		JpegFile::BGRFromRGB(pData, pbi->biWidth, pbi->biHeight);
		bRet = JpegFile::RGBToJpegFile(pFilename, pData, pbi->biWidth, pbi->biHeight, TRUE, nQuality);
	}
	catch (...) {
		Log(_T("DIB2JpegFile:One exception was thrown. "));
		bRet = FALSE;
	}
	
	return bRet;
}

HANDLE CImgUtil::Screen2HDIB()
{
	INT nRet = 0;
	HDC hdcScreen = CreateDC(_T("DISPLAY"), NULL, NULL, NULL); 
	HDC hdcCompatible = CreateCompatibleDC(hdcScreen); 
	HBITMAP hbmScreen = CreateCompatibleBitmap(hdcScreen, GetDeviceCaps(hdcScreen, HORZRES), GetDeviceCaps(hdcScreen, VERTRES));
	if (hbmScreen == NULL) 
	{
		Log(_T("Screen2HDIB:CreateCompatibleBitmap failed. "), GetLastError());		
		return NULL;
	}

	HGDIOBJ hOld = SelectObject(hdcCompatible, hbmScreen);
	if (!hOld) 
	{
		Log(_T("Screen2HDIB:SelectObject failed. "), GetLastError());
		return NULL;
	}

	int nWidth=GetSystemMetrics(SM_CXSCREEN);
	int nHeight=GetSystemMetrics(SM_CYSCREEN);
	if (!BitBlt(hdcCompatible, 0, 0, nWidth, nHeight, hdcScreen, 0, 0, SRCCOPY)) 
	{
		Log(_T("Screen2HDIB:BitBlt failed. "), GetLastError());
		return NULL;
	}

	SelectObject(hdcCompatible, hOld);
	ReleaseDC(NULL, hdcCompatible);
	ReleaseDC(NULL, hdcScreen);

	HANDLE hDIB = DDB2DIB(hbmScreen);
	DeleteObject(hbmScreen);

	return hDIB;
}


BOOL CImgUtil::Screen2JpegFile(LPCSTR pFilename, INT nQuality)
{
	HANDLE hDIB = Screen2HDIB();
	if (!hDIB)
		return FALSE;

	BOOL bRet = DIB2JpegFile(hDIB, pFilename, 100);

	GlobalFree(hDIB);

	return bRet;
}

BOOL CImgUtil::Bmp2JPG(LPCSTR pBmpFilename, LPCSTR pJpegFilename, INT nQuality)
{
	FILE *f = _tfopen(pBmpFilename, _T("rb"));
	if (!f)
	{
		Log(_T("Bmp2JPG:_tfOpen failed. "), GetLastError());
		return FALSE;
	}

	BYTE *pszBuf = NULL;
	try
	{
		
		size_t nSize = sizeof(BITMAPFILEHEADER);
		pszBuf = new BYTE[nSize];
		if (fread(pszBuf, 1, nSize, f) != nSize)
			throw -1;
		
		if ( !((pszBuf[0] == 'B') && (pszBuf[1] == 'M')) )
			throw -2;		
		
		PBITMAPFILEHEADER pFH = (PBITMAPFILEHEADER)pszBuf;
		nSize = pFH->bfSize - sizeof(BITMAPFILEHEADER);
		DWORD dwOffset = pFH->bfOffBits;

		delete[] pszBuf;
		pszBuf = new BYTE[nSize];		
		if (fread(pszBuf, 1, nSize, f) != nSize)
			throw -3;
		
		fclose(f);
		PBITMAPINFOHEADER pIH = (PBITMAPINFOHEADER)pszBuf;
		BYTE *pData = (BYTE*)(pszBuf + dwOffset - sizeof(BITMAPFILEHEADER));
		
		BOOL bRet = FALSE;
		if (JpegFile::VertFlipBuf(pData, pIH->biWidth * 3, pIH->biHeight))
			if (JpegFile::BGRFromRGB(pData, pIH->biWidth, pIH->biHeight))
				bRet = JpegFile::RGBToJpegFile(pJpegFilename, pData, pIH->biWidth, pIH->biHeight, TRUE, nQuality);
		
		delete[] pszBuf;
		return bRet;
	}
	catch (INT nNum) {
#ifndef _DISABLE_LOG_
		if (m_pLogger) m_pLogger->VLog(_T("Bmp2JPG: One exception was thrown, the result is:%d."), nNum);
#endif
		delete[] pszBuf;
		fclose(f);
		return FALSE;
	}
	catch (...) {
		delete[] pszBuf;
		Log(_T("Bmp2JPG: One exception was thrown. "));
		return FALSE;
	}
}
