// USBCommonUtil.h: interface for the CUSBCommonUtil class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USBUTIL_H__0A1EF6A8_FE21_4251_AF55_82D2FDBAD384__INCLUDED_)
#define AFX_USBUTIL_H__0A1EF6A8_FE21_4251_AF55_82D2FDBAD384__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning (disable : 4200)

#include <basetyps.h>
#include <windows.h>
#include <devguid.h>
#include <regstr.h>
#include <windowsx.h>
#include <initguid.h>
#include <devioctl.h>
#include <usbioctl.h>
#include <dbt.h>
#include <stdio.h>
#include <cfgmgr32.h>
#include <setupapi.h>
#include <vector>
using namespace std;

#include "usbdesc.h"

//*****************************************************************************
// D E F I N E S
//*****************************************************************************

#ifdef  DEBUG
#undef  DBG
#define DBG 1
#endif

#if DBG
#define OOPS() Oops(__FILE__, __LINE__)
#else
#define OOPS()
#endif


#if DBG

#define ALLOC(dwBytes) MyAlloc(__FILE__, __LINE__, (dwBytes))
#define REALLOC(hMem, dwBytes) MyReAlloc((hMem), (dwBytes))
#define FREE(hMem)  MyFree((hMem))
#define CHECKFORLEAKS() MyCheckForLeaks()

#else

#define ALLOC(dwBytes) GlobalAlloc(GPTR,(dwBytes))
#define REALLOC(hMem, dwBytes) GlobalReAlloc((hMem), (dwBytes), (GMEM_MOVEABLE|GMEM_ZEROINIT))
#define FREE(hMem)  GlobalFree((hMem))
#define CHECKFORLEAKS()

#endif

// add to SDK comed from DDK
#define SPDRP_BUSTYPEGUID                 (0x00000013)  // BusTypeGUID (R)
#define SPDRP_LEGACYBUSTYPE               (0x00000014)  // LegacyBusType (R)
#define SPDRP_BUSNUMBER                   (0x00000015)  // BusNumber (R)
#define SPDRP_ENUMERATOR_NAME             (0x00000016)  // Enumerator Name (R)
#define SPDRP_SECURITY                    (0x00000017)  // Security (R/W, binary form)
#define SPDRP_SECURITY_SDS                (0x00000018)  // Security (W, SDS form)
#define SPDRP_DEVTYPE                     (0x00000019)  // Device Type (R/W)
#define SPDRP_EXCLUSIVE                   (0x0000001A)  // Device is exclusive-access (R/W)
#define SPDRP_CHARACTERISTICS             (0x0000001B)  // Device Characteristics (R/W)
#define SPDRP_ADDRESS                     (0x0000001C)  // Device Address (R)
#define SPDRP_UI_NUMBER_DESC_FORMAT       (0X0000001E)  // UiNumberDescFormat (R/W)

//
// Structure used to build a linked list of String Descriptors
// retrieved from a device.
//

typedef struct _STRING_DESCRIPTOR_NODE
{
    struct _STRING_DESCRIPTOR_NODE *Next;
    UCHAR                           DescriptorIndex;
    USHORT                          LanguageID;
    USB_STRING_DESCRIPTOR           StringDescriptor[0];
} STRING_DESCRIPTOR_NODE, *PSTRING_DESCRIPTOR_NODE;


//
// Structures assocated with TreeView items through the lParam.  When an item
// is selected, the lParam is retrieved and the structure it which it points
// is used to display information in the edit control.
//

typedef struct
{
    PUSB_NODE_INFORMATION               HubInfo;        // NULL if not a HUB

    PCHAR                               HubName;        // NULL if not a HUB

    PUSB_NODE_CONNECTION_INFORMATION    ConnectionInfo; // NULL if root HUB

    PUSB_DESCRIPTOR_REQUEST             ConfigDesc;     // NULL if root HUB

    PSTRING_DESCRIPTOR_NODE             StringDescs;

} USBDEVICEINFO, *PUSBDEVICEINFO;

#define DESC_MAX_LEN	200
typedef struct tagUSBDevInfo {
	DEVINST	hDevInst;
	DWORD	dwIndex;
	DWORD	dwAddress;
	GUID	ClassGuid;	
	string	szDevDesc;
	string	szHardwareId;
	string	szLocationInfo;
	string szDriver;
} USB_DEV_INFO, *LPUSB_DEV_INFO;

typedef struct tagProductInfo {
	string szVendor;
	string szProductId;
	string szRev;
} PRODUCT_INFO, *LPPRODUCT_INFO;

typedef struct tagFilterCondition {
	bool	bEqual;
	string	szName;
	string	szValue;
} FILTER_CONDITION, *LPFILTER_CONDITION;

class CUSBCommonUtil  
{
public:
	static const GUID GUID_DEVCLASS_USB;

public:
	INT GetEnabledUSBListCount();
	INT EjectAllUsb();
	BOOL EjectBySN(LPCSTR pProductID, LPCSTR pVendorID, LPCSTR pSN);
	BOOL GetProductInfo(LPCSTR pszHardwareId, LPPRODUCT_INFO pProductInfo);
	BOOL EjectUsingLocationInfo(LPCSTR pszLocationInfo);
	BOOL EjectUsingHardwareId(LPCSTR pszHardwareId);
	BOOL Eject(DEVINST hDevice);
	LPUSB_DEV_INFO GetUSBDevAt(DWORD dwIndex);
	LPUSB_DEV_INFO GetUSBDevByDevIndex(DWORD dwDevIndex);
	LPUSB_DEV_INFO GetUSBDevByHardwareId(LPCSTR pszHardwareId);
	LPUSB_DEV_INFO GetUSBDevByLocationInfo(LPCSTR pszLocationInfo);
	DWORD GetUSBListCount();
	INT GetUSBList1();
	INT GetUSBList(LPCGUID pGuid = &GUID_DEVCLASS_USB, BOOL bUsingFilter = TRUE, LPCSTR pFilterFile = "C:\\Services\\USBFilter.ini");
	BOOL IsDisabled (DEVINST DevInst);
	BOOL IsDisableable (DEVINST DevInst);	
	CUSBCommonUtil();
	virtual ~CUSBCommonUtil();

private:	
	BOOL CheckFilter(const USB_DEV_INFO &usb, const vector<FILTER_CONDITION> &conditions);
	HDEVINFO m_hDevInfo;
	vector<USB_DEV_INFO> m_USBList;		// get by calling SetupDiGetClassDevs
	vector<USBDEVICEINFO> m_USBList1;		// get by calling DeviceIoControl	
	ULONG m_ulTotalDevicesConnected;
	ULONG m_ulTotalHubs;

	void Clear();

	INT CreateFilter(vector<FILTER_CONDITION> & Conditions, LPCSTR pszFileName);

	INT EnumerateHostControllers();
	VOID EnumerateHub (PCHAR                               HubName,
					   PUSB_NODE_CONNECTION_INFORMATION    ConnectionInfo,
					   PUSB_DESCRIPTOR_REQUEST             ConfigDesc,
					   PSTRING_DESCRIPTOR_NODE             StringDescs,
					   PCHAR                               DeviceDesc
					   );
	VOID CUSBCommonUtil::EnumerateHubPorts (HANDLE      hHubDevice,
											ULONG       NumPorts);
	PCHAR GetDriverKeyName (HANDLE Hub, ULONG ConnectionIndex);
	PUSB_DESCRIPTOR_REQUEST GetConfigDescriptor (HANDLE  hHubDevice,
												 ULONG   ConnectionIndex,
												 UCHAR   DescriptorIndex);
	PCHAR GetHCDDriverKeyName (HANDLE  HCD);
	PCHAR DriverNameToDeviceDesc (PCHAR DriverName);
	PCHAR GetRootHubName (HANDLE HostController);
	BOOL AreThereStringDescriptors (PUSB_DEVICE_DESCRIPTOR			DeviceDesc,
									PUSB_CONFIGURATION_DESCRIPTOR   ConfigDesc);
	PSTRING_DESCRIPTOR_NODE GetAllStringDescriptors (HANDLE                          hHubDevice,
													 ULONG                           ConnectionIndex,
													 PUSB_DEVICE_DESCRIPTOR          DeviceDesc,
													 PUSB_CONFIGURATION_DESCRIPTOR   ConfigDesc);
	PSTRING_DESCRIPTOR_NODE GetStringDescriptor (HANDLE  hHubDevice,
												 ULONG   ConnectionIndex,
												 UCHAR   DescriptorIndex,
												 USHORT  LanguageID);
	PSTRING_DESCRIPTOR_NODE GetStringDescriptors (HANDLE  hHubDevice,
												  ULONG   ConnectionIndex,
												  UCHAR   DescriptorIndex,
												  ULONG   NumLanguageIDs,
												  USHORT  *LanguageIDs,
												  PSTRING_DESCRIPTOR_NODE StringDescNodeTail);
	PCHAR GetExternalHubName (HANDLE Hub, ULONG ConnectionIndex);
	PCHAR GetExternalHubName1 (HANDLE Hub, ULONG ConnectionIndex);
	PCHAR WideStrToMultiStr (PWCHAR WideStr);

};

#endif // !defined(AFX_USBUTIL_H__0A1EF6A8_FE21_4251_AF55_82D2FDBAD384__INCLUDED_)
