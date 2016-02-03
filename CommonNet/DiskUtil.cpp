// DiskUtil.cpp: implementation of the CDiskUtil class.
//
//////////////////////////////////////////////////////////////////////
#include "DiskUtil.h"

// for DeviceIoControl
#include <Winioctl.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDiskUtil::CDiskUtil()
{

}

CDiskUtil::~CDiskUtil()
{

}

/**************************************************************************
* Desc: Check whether the destination disk is write protected
* Args: 
*	bstrDiskLetter	disk letter to check, such as c, d or h...
* Return:			check result
*					1:	the disk is write protected
*					0:	the disk isn't write protected	
*				   -1:	error occur, the error message is logged
			
* Remark:
* Author:	Ryan Zhang
*************************************************************************/
INT CDiskUtil::IsDiskProtected(LPCTSTR pcszDiskLetter)
{
	// TODO: Add your implementation code here
	//CLogUtil log(LOG_FILE_NAME);
	if (!pcszDiskLetter) {
		//log.Log(_T("IsDiskProtected: Disk letter is null"));
		return -1;
	}

	if (_tcslen(pcszDiskLetter) == 0) {
		//log.Log(_T("IsDiskProtected: Disk letter is null"));
		return -1;
	}

	TCHAR chDiskName[7];
	_tcscpy(chDiskName, _T("\\\\.\\c:"));
	chDiskName[4] = *pcszDiskLetter;

	HANDLE hDevice;
	hDevice = CreateFile(chDiskName,
						 STANDARD_RIGHTS_READ,
						 FILE_SHARE_READ | FILE_SHARE_WRITE,
						 NULL,
						 OPEN_EXISTING,
						 NULL,
						 NULL);
	if (INVALID_HANDLE_VALUE == hDevice) {
		//log.Log(_T("IsDiskProtected::CreateFile failed"), GetLastError());
		return -1;
	}

	int nWriteProtected = 1;
	DWORD dwBytesReturnd = 0;
	BOOL bProtect = DeviceIoControl(hDevice,
									IOCTL_DISK_IS_WRITABLE,
									NULL,
									0,
									NULL,
									0,
									&dwBytesReturnd,
									NULL);

	if (bProtect) {
		// the disk is not write protected
		nWriteProtected = 0;
	}
	else {
		DWORD dwErrorCode = GetLastError();
		_tcscpy(chDiskName, _T("c:\\"));
		chDiskName[0] = *pcszDiskLetter;
		if (dwErrorCode == 19) {
			// the disk is write protected
			nWriteProtected = 1;
		}
		else if ( (dwErrorCode == S_FALSE) && (GetDriveType(chDiskName) == DRIVE_CDROM) ) {
			// if the drive type is CDROM, then S_FALSE will be returned
			nWriteProtected = 1;
		}
		else {
			// error
			//log.Log(_T("IsDiskProtected::DeviceIoControl failed"), dwErrorCode);
			nWriteProtected = -1;
		}
	}

	CloseHandle(hDevice);

	return nWriteProtected;
}
