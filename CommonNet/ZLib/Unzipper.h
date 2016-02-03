// Unzipper.h: interface for the CUnzipper class (c) daniel godson 2002
//
// CUnzipper is a simple c++ wrapper for the 'unzip' file extraction
// api written by Gilles Vollant (c) 2002, which in turn makes use of 
// 'zlib' written by Jean-loup Gailly and Mark Adler (c) 1995-2002.
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely without restriction.
//
// Notwithstanding this, you are still bound by the conditions imposed
// by the original authors of 'unzip' and 'zlib'
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UNZIPPER_H__EBC42716_31C7_4659_8EF3_9BF8D4409709__INCLUDED_)
#define AFX_UNZIPPER_H__EBC42716_31C7_4659_8EF3_9BF8D4409709__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <wtypes.h>
#include <windows.h>

const UINT MAX_COMMENT = 255;

// create our own fileinfo struct to hide the underlying implementation
struct UZ_FileInfo
{
	char szFileName[MAX_PATH + 1];
	char szComment[MAX_COMMENT + 1];
	
	DWORD dwVersion;  
	DWORD dwVersionNeeded;
	DWORD dwFlags;	 
	DWORD dwCompressionMethod; 
	DWORD dwDosDate;	
	DWORD dwCRC;   
	DWORD dwCompressedSize; 
	DWORD dwUncompressedSize;
	DWORD dwInternalAttrib; 
	DWORD dwExternalAttrib; 
	bool bFolder;
};

class CUnzipper  
{
public:
	CUnzipper(LPCSTR szFilePath = NULL);
	virtual ~CUnzipper();
	
	// simple interface
	static bool Unzip(LPCSTR szFileName, LPCSTR szFolder = NULL, bool bIgnoreFilePath = FALSE);
	
	// works with prior opened zip
	bool Unzip(bool bIgnoreFilePath = FALSE); // unzips to output folder or sub folder with zip name 
	bool UnzipTo(LPCSTR szFolder, bool bIgnoreFilePath = FALSE); // unzips to specified folder

	// extended interface
	bool OpenZip(LPCSTR szFilePath);
	bool CloseZip(); // for multiple reuse
	bool SetOutputFolder(LPCSTR szFolder); // will try to create
	
	// unzip by file index
	int GetFileCount();
	bool GetFileInfo(int nFile, UZ_FileInfo& info);
	bool UnzipFile(int nFile, LPCSTR szFolder = NULL, bool bIgnoreFilePath = FALSE);
	
	// unzip current file
	bool GotoFirstFile(LPCSTR szExt = NULL);
	bool GotoNextFile(LPCSTR szExt = NULL);
	bool GetFileInfo(UZ_FileInfo& info);
	bool UnzipFile(LPCSTR szFolder = NULL, bool bIgnoreFilePath = FALSE);

	// helpers
	bool GotoFile(LPCSTR szFileName, bool bIgnoreFilePath = TRUE);
	bool GotoFile(int nFile);
	
protected:
	void* m_uzFile;
	char m_szOutputFolder[MAX_PATH + 1];

protected:
	static bool CreateFolder(LPCSTR szFolder);
	static bool CreateFilePath(LPCSTR szFilePath); // truncates from the last '\'
	static bool SetFileModTime(LPCSTR szFilePath, DWORD dwDosDate);

};

#endif // !defined(AFX_UNZIPPER_H__EBC42716_31C7_4659_8EF3_9BF8D4409709__INCLUDED_)
