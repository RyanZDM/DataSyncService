// USBUtil.cpp: implementation of the CUSBUtil class.
//
//////////////////////////////////////////////////////////////////////

#include "USBCommonUtil.h"
#include "LogUtil.h"
#include "StrUtil.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
#endif

// whether using STL algorithm
//#define _USING_STL_ALGORITHM_

#ifdef _USING_STL_ALGORITHM_
#include <algorithm>
#include <functional>
using namespace std;

CUSBCommonUtil *g_pUSB = NULL;
#endif

#include <fstream>

PCHAR CUSBCommonUtil::GetExternalHubName1 (HANDLE Hub, ULONG ConnectionIndex)
{
    BOOL                        success;
    ULONG                       nBytes;
    USB_NODE_CONNECTION_NAME    extHubName;
    PUSB_NODE_CONNECTION_NAME   extHubNameW;
    PCHAR                       extHubNameA;

    extHubNameW = NULL;
    extHubNameA = NULL;

	try {
		// Get the length of the name of the external hub attached to the
		// specified port.
		//
		extHubName.ConnectionIndex = ConnectionIndex;
		
		success = DeviceIoControl(Hub,
								  IOCTL_USB_GET_NODE_CONNECTION_DRIVERKEY_NAME
								  /*IOCTL_USB_GET_NODE_CONNECTION_NAME*/,
								  &extHubName,
								  sizeof(extHubName),
								  &extHubName,
								  sizeof(extHubName),
								  &nBytes,
								  NULL);
		
		if (!success)
			throw -1;
		
		// Allocate space to hold the external hub name
		//
		nBytes = extHubName.ActualLength;
// ryan temp		
		if (nBytes < sizeof(extHubName))
			throw -1;
		
		extHubNameW = (PUSB_NODE_CONNECTION_NAME)ALLOC(nBytes);
		
		if (extHubNameW == NULL)
			throw -1;
		
		// Get the name of the external hub attached to the specified port
		//
		extHubNameW->ConnectionIndex = ConnectionIndex;
		
		success = DeviceIoControl(Hub,
			IOCTL_USB_GET_NODE_CONNECTION_NAME,
			extHubNameW,
			nBytes,
			extHubNameW,
			nBytes,
			&nBytes,
			NULL);
		
		if (!success)
			throw -1;
		
		// Convert the External Hub name
		//
		extHubNameA = WideStrToMultiStr(extHubNameW->NodeName);
		
		// All done, free the uncoverted external hub name and return the
		// converted external hub name
		//
		FREE(extHubNameW);
		
		return extHubNameA;
	}
	catch (...) {
		// There was an error, free anything that was allocated
		//
		OOPS();
		if (extHubNameW != NULL)
		{
			FREE(extHubNameW);
			extHubNameW = NULL;
		}
		
		return NULL;
	}
}

const GUID CUSBCommonUtil::GUID_DEVCLASS_USB = {0x36fc9e60L, 0xc465, 0x11cf, 0x80, 0x56, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00};
CLogUtil g_Log(_T("C:\\Log\\USBMgr.log"));
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUSBCommonUtil::CUSBCommonUtil()
{
	m_hDevInfo = NULL;	
	//g_Log.SetEnable(TRUE);

#ifdef _USING_STL_ALGORITHM_
	g_pUSB = this;
#endif
}

CUSBCommonUtil::~CUSBCommonUtil()
{
	if (m_hDevInfo) {
		SetupDiDestroyDeviceInfoList(m_hDevInfo);
		m_hDevInfo = NULL;
	}

	Clear();
}

void CUSBCommonUtil::Clear()
{
	m_USBList.clear();	
	m_USBList1.clear();
}

INT CUSBCommonUtil::GetUSBList(LPCGUID pGuid, BOOL bUsingFilter, LPCSTR pFilterFile)
{    
	INT nConditionCount = 0;
	vector<FILTER_CONDITION> Conditions;
	if (bUsingFilter) {
		nConditionCount = CreateFilter(Conditions, pFilterFile);
	}

	if (!m_hDevInfo) {
		// Create a HDEVINFO with all USB devices.
		m_hDevInfo = SetupDiGetClassDevs(pGuid,
										 0, 
										 0,
										 (pGuid)?(DIGCF_PRESENT):(DIGCF_PRESENT|DIGCF_ALLCLASSES));

		if (m_hDevInfo == INVALID_HANDLE_VALUE) {
			m_hDevInfo = NULL;
			return -1;
		}
	} 

	Clear();
	
    // Enumerate through all devices in Set. 
	BYTE szBuf[DESC_MAX_LEN];
    DWORD dwCount = 0;
    SP_DEVINFO_DATA DeviceInfoData;
	DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);	
    for (DWORD i = 0; SetupDiEnumDeviceInfo(m_hDevInfo, i, &DeviceInfoData); i++)
    {
        DWORD DataT;
		DWORD dwSize = 0;

        // 
        // Call function with null to begin with, 
        // then use the returned buffer size 
        // to Alloc the buffer. Keep calling until
        // success or an unknown failure.
        // 
	
		memset(szBuf, 0, DESC_MAX_LEN);
        if (SetupDiGetDeviceRegistryProperty(m_hDevInfo,
											 &DeviceInfoData,
											 SPDRP_ENUMERATOR_NAME,
											 &DataT,
											 szBuf,
											 DESC_MAX_LEN,
											 &dwSize)) {
			// Skip if it is a USB Host Controller
			if ( strcmpi((const char*)szBuf, "PCI") == 0 )
				continue;		
		}
		else {        
			// error
            break;
        }

		memset(szBuf, 0, DESC_MAX_LEN);
        if (SetupDiGetDeviceRegistryProperty(m_hDevInfo,
											 &DeviceInfoData,
											 SPDRP_ADDRESS,
											 &DataT,
											 szBuf,
											 DESC_MAX_LEN,
											 &dwSize)) {
			// Skip if it is a USB Root Hub
			if ( ((PDWORD)szBuf)[0] == 0)			
				continue;
		}
		else {
			// error
            //break;
        }      
		
		dwCount ++;
		USB_DEV_INFO usb;
		usb.dwIndex = i;
		usb.hDevInst = DeviceInfoData.DevInst;
		usb.dwAddress = ((PDWORD)szBuf)[0];
		usb.ClassGuid = DeviceInfoData.ClassGuid;

		memset(szBuf, 0, DESC_MAX_LEN);
        if (SetupDiGetDeviceRegistryProperty(m_hDevInfo,
											 &DeviceInfoData,
											 SPDRP_DEVICEDESC,
											 &DataT,
											 szBuf,
											 DESC_MAX_LEN,
											 &dwSize)) {
			
			usb.szDevDesc = (LPSTR)szBuf;			
		}

		memset(szBuf, 0, DESC_MAX_LEN);
        if (SetupDiGetDeviceRegistryProperty(m_hDevInfo,
											 &DeviceInfoData,
											 SPDRP_HARDWAREID,
											 &DataT,
											 szBuf,
											 DESC_MAX_LEN,
											 &dwSize)) {

			usb.szHardwareId = (LPSTR)szBuf;			
		}

		memset(szBuf, 0, DESC_MAX_LEN);
        if (SetupDiGetDeviceRegistryProperty(m_hDevInfo,
											 &DeviceInfoData,
											 SPDRP_LOCATION_INFORMATION,
											 &DataT,
											 szBuf,
											 DESC_MAX_LEN,
											 &dwSize)) {

			usb.szLocationInfo = (LPSTR)szBuf;			
		}

		memset(szBuf, 0, DESC_MAX_LEN);
        if (SetupDiGetDeviceRegistryProperty(m_hDevInfo,
											 &DeviceInfoData,
											 SPDRP_DRIVER,
											 &DataT,
											 szBuf,
											 DESC_MAX_LEN,
											 &dwSize)) {

			usb.szDriver = (LPSTR)szBuf;			
		}

		if ( (nConditionCount > 0) && CheckFilter(usb, Conditions) ) {
			m_USBList.push_back(usb);
		}

    }
	
    return dwCount;
}

DWORD CUSBCommonUtil::GetUSBListCount()
{
	return m_USBList.size();
}

LPUSB_DEV_INFO CUSBCommonUtil::GetUSBDevAt(DWORD dwIndex)
{
	if (dwIndex >= m_USBList.size())
		return NULL;

	return &m_USBList[dwIndex];
}

LPUSB_DEV_INFO CUSBCommonUtil::GetUSBDevByDevIndex(DWORD dwDevIndex)
{
	vector<USB_DEV_INFO>::iterator vi;
	for (vi = m_USBList.begin(); vi != m_USBList.end(); vi++) {
		LPUSB_DEV_INFO p = (LPUSB_DEV_INFO)vi;
		if (p) {
			if (p->dwIndex == dwDevIndex)
				return p;
		}
	}

	return NULL;
}

LPUSB_DEV_INFO CUSBCommonUtil::GetUSBDevByLocationInfo(LPCSTR pszLocationInfo)
{
	if (!pszLocationInfo)
		return NULL;
	
	vector<USB_DEV_INFO>::iterator vi;
	for (vi = m_USBList.begin(); vi != m_USBList.end(); vi++) {
		LPUSB_DEV_INFO p = (LPUSB_DEV_INFO)vi;
		if (p) {
			if (strcmpi(p->szLocationInfo.c_str(), pszLocationInfo) == 0)
				return p;
		}
	}

	return NULL;
}

LPUSB_DEV_INFO CUSBCommonUtil::GetUSBDevByHardwareId(LPCSTR pszHardwareId)
{
	if (!pszHardwareId)
		return NULL;
	
	vector<USB_DEV_INFO>::iterator vi;
	for (vi = m_USBList.begin(); vi != m_USBList.end(); vi++) {
		LPUSB_DEV_INFO p = (LPUSB_DEV_INFO)vi;
		if (p) {
			if (strcmpi(p->szHardwareId.c_str(), pszHardwareId) == 0)
				return p;
		}
	}

	return NULL;
}

BOOL CUSBCommonUtil::IsDisabled ( DEVINST DevInst )
{
	if (!DevInst)
		return FALSE;

	DWORD dwStatus, dwProblem;

    if (CR_SUCCESS != CM_Get_DevNode_Status(&dwStatus, &dwProblem, DevInst, 0))
        return FALSE;

    return ((dwStatus & DN_HAS_PROBLEM) && 
			( (CM_PROB_DISABLED == dwProblem) || (CM_PROB_DEVICE_NOT_THERE == dwProblem) ) ) ;
}

BOOL CUSBCommonUtil::IsDisableable ( DEVINST DevInst )
{
	if (!DevInst)
		return FALSE;

	DWORD dwStatus, dwProblem;
	
    if (CR_SUCCESS != CM_Get_DevNode_Status(&dwStatus, &dwProblem, DevInst, 0))
        return FALSE;
	
    return ((dwStatus & DN_DISABLEABLE) && (CM_PROB_HARDWARE_DISABLED != dwProblem));
}
BOOL CUSBCommonUtil::Eject(DEVINST hDevice)
{
	if (!m_hDevInfo)
		return FALSE;

	PNP_VETO_TYPE Pnp_Veto_type;
	TCHAR szVetoName[MAX_PATH] = {0};

	return (CR_SUCCESS == CM_Request_Device_Eject(hDevice,
												  &Pnp_Veto_type,
												  szVetoName,
												  sizeof(szVetoName)/sizeof(szVetoName[0]),
												  0));

}

BOOL CUSBCommonUtil::EjectUsingHardwareId(LPCSTR pszHardwareId)
{
	//LPUSB_DEV_INFO p = GetUSBDevByHardwareId(pszHardwareId);
	BOOL bRet = TRUE;
	vector<USB_DEV_INFO>::iterator vi;
	for (vi = m_USBList.begin(); vi != m_USBList.end(); vi++) {
		LPUSB_DEV_INFO p = (LPUSB_DEV_INFO)vi;
		if (p && p->hDevInst) {
			if (strcmpi(p->szHardwareId.c_str(), pszHardwareId) == 0)
				if (Eject(p->hDevInst))
					p->hDevInst = NULL;
				else
					bRet = FALSE;
		}
	}

	return bRet;
}

BOOL CUSBCommonUtil::EjectUsingLocationInfo(LPCSTR pszLocationInfo)
{
	LPUSB_DEV_INFO p = GetUSBDevByLocationInfo(pszLocationInfo);

	return (p)?Eject(p->hDevInst):FALSE;
}

BOOL CUSBCommonUtil::GetProductInfo(LPCSTR pszHardwareId, LPPRODUCT_INFO pProductInfo)
{
	if (!pszHardwareId || (strlen(pszHardwareId) < 1) || !pProductInfo)
		return FALSE;
	
	pProductInfo->szProductId = "";
	pProductInfo->szVendor = "";
	pProductInfo->szRev = "";

	CStrUtil StrUtil;
	vector<string> vRet;
	if (StrUtil.StrToVector(vRet, pszHardwareId, "&") > 0) {
		for (vector<string>::iterator vi = vRet.begin(); vi != vRet.end(); vi++) {
			LPCSTR pVal = ((string)(*vi)).c_str();
			LPCSTR pFound = strstr(pVal, "Pid_");
			if (pFound) 
				pProductInfo->szProductId = (pFound + 4);
			
			pFound = strstr(pVal, "Vid_");
			if (pFound)
				pProductInfo->szVendor = (pFound + 4);

			pFound = strstr(pVal, "Rev_");
			if (pFound)
				pProductInfo->szRev = (pFound + 4);
		}
	}

	return TRUE;

}

INT CUSBCommonUtil::EnumerateHostControllers()
{
	char        HCName[16];
    INT         nHCNum;
    HANDLE      hHCDev;
    PCHAR       rootHubName;

    m_ulTotalDevicesConnected = 0;
    m_ulTotalHubs = 0;
	m_USBList1.clear();

	const INT NUM_HCS_TO_CHECK = 10;
    // Iterate over some Host Controller names and try to open them.
    //
    for (nHCNum = 0; nHCNum < NUM_HCS_TO_CHECK; nHCNum++)
    {
        wsprintf(HCName, "\\\\.\\HCD%d", nHCNum);

        hHCDev = CreateFile(HCName,
                            GENERIC_WRITE,
                            FILE_SHARE_WRITE,
                            NULL,
                            OPEN_EXISTING,
                            0,
                            NULL);

        // If the handle is valid, then we've successfully opened a Host
        // Controller.  Display some info about the Host Controller itself,
        // then enumerate the Root Hub attached to the Host Controller.
        //
        if (hHCDev != INVALID_HANDLE_VALUE)
        {
            PCHAR driverKeyName, deviceDesc;

            driverKeyName = GetHCDDriverKeyName(hHCDev);

            if (driverKeyName)
            {
                deviceDesc = DriverNameToDeviceDesc(driverKeyName);
				FREE(driverKeyName);
            }

			rootHubName = GetRootHubName(hHCDev);
			if (rootHubName != NULL)
            {
				EnumerateHub(rootHubName,
							 NULL,      // ConnectionInfo
							 NULL,      // ConfigDesc
							 NULL,      // StringDescs
							 "RootHub"  // DeviceDesc
							 );
                
            }

            CloseHandle(hHCDev);
        }
    }

    return m_ulTotalDevicesConnected;
}

//*****************************************************************************
//
// EnumerateHub()
//
// hTreeParent - Handle of the TreeView item under which this hub should be
// added.
//
// HubName - Name of this hub.  This pointer is kept so the caller can neither
// free nor reuse this memory.
//
// ConnectionInfo - NULL if this is a root hub, else this is the connection
// info for an external hub.  This pointer is kept so the caller can neither
// free nor reuse this memory.
//
// ConfigDesc - NULL if this is a root hub, else this is the Configuration
// Descriptor for an external hub.  This pointer is kept so the caller can
// neither free nor reuse this memory.
//
//*****************************************************************************

VOID CUSBCommonUtil::EnumerateHub (PCHAR                               HubName,
								   PUSB_NODE_CONNECTION_INFORMATION    ConnectionInfo,
								   PUSB_DESCRIPTOR_REQUEST             ConfigDesc,
								   PSTRING_DESCRIPTOR_NODE             StringDescs,
								   PCHAR                               DeviceDesc)
{
    HANDLE          hHubDevice;
//    HTREEITEM       hItem;
    PCHAR           deviceName;
    BOOL            success;
    ULONG           nBytes;
    PUSBDEVICEINFO  info;
//    CHAR            leafName[512]; // XXXXX how big does this have to be?

    // Initialize locals to not allocated state so the error cleanup routine
    // only tries to cleanup things that were successfully allocated.
    //
    info        = NULL;
    hHubDevice  = INVALID_HANDLE_VALUE;

	try {
		// Allocate some space for a USBDEVICEINFO structure to hold the
		// hub info, hub name, and connection info pointers.  GPTR zero
		// initializes the structure for us.
		//
		info = (PUSBDEVICEINFO) ALLOC(sizeof(USBDEVICEINFO));
		if (info == NULL)
			throw -1;
		
		// Keep copies of the Hub Name, Connection Info, and Configuration
		// Descriptor pointers
		//
		info->HubName = HubName;
		
		info->ConnectionInfo = ConnectionInfo;
		
		info->ConfigDesc = ConfigDesc;
		
		info->StringDescs = StringDescs;
		
		
		// Allocate some space for a USB_NODE_INFORMATION structure for this Hub,
		//
		info->HubInfo = (PUSB_NODE_INFORMATION)ALLOC(sizeof(USB_NODE_INFORMATION));
		
		if (info->HubInfo == NULL)
			throw -1;
		
		// Allocate a temp buffer for the full hub device name.
		//
		deviceName = (PCHAR)ALLOC(strlen(HubName) + sizeof("\\\\.\\"));
		
		if (deviceName == NULL)
			throw -1;
		
		// Create the full hub device name
		//
		strcpy(deviceName, "\\\\.\\");
		strcpy(deviceName + sizeof("\\\\.\\") - 1, info->HubName);
		
		// Try to hub the open device
		//
		hHubDevice = CreateFile(deviceName,
			GENERIC_WRITE,
			FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			0,
			NULL);
		
		// Done with temp buffer for full hub device name
		//
		FREE(deviceName);
		
		if (hHubDevice == INVALID_HANDLE_VALUE)
			throw -1;
		
		//
		// Now query USBHUB for the USB_NODE_INFORMATION structure for this hub.
		// This will tell us the number of downstream ports to enumerate, among
		// other things.
		//
		success = DeviceIoControl(hHubDevice,
			IOCTL_USB_GET_NODE_INFORMATION,
			info->HubInfo,
			sizeof(USB_NODE_INFORMATION),
			info->HubInfo,
			sizeof(USB_NODE_INFORMATION),
			&nBytes,
			NULL);
		
		if (!success)
			throw -1;		
		  
		// Now recursively enumrate the ports of this hub.
		//
		EnumerateHubPorts(hHubDevice, info->HubInfo->u.HubInformation.HubDescriptor.bNumberOfPorts);		
		
		CloseHandle(hHubDevice);
		return;
	}
	catch (...) {
		OOPS();
		//
		// Clean up any stuff that got allocated
		//
		
		if (hHubDevice != INVALID_HANDLE_VALUE)
		{
			CloseHandle(hHubDevice);
			hHubDevice = INVALID_HANDLE_VALUE;
		}
		
		if (info != NULL)
		{
			if (info->HubName != NULL)
			{
				FREE(info->HubName);
				info->HubName = NULL;
			}
			
			if (info->HubInfo != NULL)
			{
				FREE(info->HubInfo);
				info->HubInfo;
			}
			
			FREE(info);
			info = NULL;
		}
		
		if (ConnectionInfo)
		{
			FREE(ConnectionInfo);
		}
		
		if (ConfigDesc)
		{
			FREE(ConfigDesc);
		}
		
		if (StringDescs != NULL)
		{
			PSTRING_DESCRIPTOR_NODE Next;
			
			do {
				
				Next = StringDescs->Next;
				FREE(StringDescs);
				StringDescs = Next;
				
			} while (StringDescs != NULL);
		}
	}

}

//*****************************************************************************
//
// EnumerateHubPorts()
//
// hHubDevice - Handle of the hub device to enumerate.
//
// NumPorts - Number of ports on the hub.
//
//*****************************************************************************

VOID CUSBCommonUtil::EnumerateHubPorts (HANDLE      hHubDevice,
										ULONG       NumPorts)
{
    ULONG       index;
    BOOL        success;

    PUSB_NODE_CONNECTION_INFORMATION    connectionInfo;
    PUSB_DESCRIPTOR_REQUEST             configDesc;
    PSTRING_DESCRIPTOR_NODE             stringDescs;
    PUSBDEVICEINFO                      info;

    PCHAR driverKeyName;
    PCHAR deviceDesc;

    // Loop over all ports of the hub.
    //
    // Port indices are 1 based, not 0 based.
    //
    for (index=1; index <= NumPorts; index++)
    {
        ULONG nBytes;

        // Allocate space to hold the connection info for this port.
        // For now, allocate it big enough to hold info for 30 pipes.
        //
        // Endpoint numbers are 0-15.  Endpoint number 0 is the standard
        // control endpoint which is not explicitly listed in the Configuration
        // Descriptor.  There can be an IN endpoint and an OUT endpoint at
        // endpoint numbers 1-15 so there can be a maximum of 30 endpoints
        // per device configuration.
        //
        // Should probably size this dynamically at some point.
        //
        nBytes = sizeof(USB_NODE_CONNECTION_INFORMATION) +
                 sizeof(USB_PIPE_INFO) * 30;

        connectionInfo = (PUSB_NODE_CONNECTION_INFORMATION)ALLOC(nBytes);

        if (connectionInfo == NULL)
        {
            OOPS();
            break;
        }

        //
        // Now query USBHUB for the USB_NODE_CONNECTION_INFORMATION structure
        // for this port.  This will tell us if a device is attached to this
        // port, among other things.
        //
        connectionInfo->ConnectionIndex = index;

        success = DeviceIoControl(hHubDevice,
                                  IOCTL_USB_GET_NODE_CONNECTION_INFORMATION,
                                  connectionInfo,
                                  nBytes,
                                  connectionInfo,
                                  nBytes,
                                  &nBytes,
                                  NULL);

        if (!success)
        {
            FREE(connectionInfo);
            continue;
        }

        // Update the count of connected devices
        //
        if (connectionInfo->ConnectionStatus == DeviceConnected)
            m_ulTotalDevicesConnected++;

        if (connectionInfo->DeviceIsHub)
            m_ulTotalHubs++;

        // If there is a device connected, get the Device Description
        //
        deviceDesc = NULL;
        if (connectionInfo->ConnectionStatus != NoDeviceConnected)
        {
            driverKeyName = GetDriverKeyName(hHubDevice, index);
            if (driverKeyName)
            {
                deviceDesc = DriverNameToDeviceDesc(driverKeyName);
                FREE(driverKeyName);
            }
        }

        // If there is a device connected to the port, try to retrieve the
        // Configuration Descriptor from the device.
        //
        if (connectionInfo->ConnectionStatus == DeviceConnected)
        {
            configDesc = GetConfigDescriptor(hHubDevice,
                                             index,
                                             0);
		}     

		try {
			if (configDesc != NULL &&
				AreThereStringDescriptors(&connectionInfo->DeviceDescriptor, (PUSB_CONFIGURATION_DESCRIPTOR)(configDesc+1)))
			{
				stringDescs = GetAllStringDescriptors(
					hHubDevice,
					index,
					&connectionInfo->DeviceDescriptor,
					(PUSB_CONFIGURATION_DESCRIPTOR)(configDesc+1));
			}
			else
			{
				stringDescs = NULL;
			}
		}
		catch (...) {
			stringDescs = NULL;
		}

        // If the device connected to the port is an external hub, get the
        // name of the external hub and recursively enumerate it.
        //
        if (connectionInfo->DeviceIsHub)
        {
            PCHAR extHubName;

            extHubName = GetExternalHubName(hHubDevice, index);

            if (extHubName != NULL)
            {
                EnumerateHub(extHubName,
                             connectionInfo,
                             configDesc,
                             stringDescs,
                             deviceDesc);

                // On to the next port
                //
                continue;
            }
        }

        // Allocate some space for a USBDEVICEINFO structure to hold the
        // hub info, hub name, and connection info pointers.  GPTR zero
        // initializes the structure for us.
        //
        info = (PUSBDEVICEINFO) ALLOC(sizeof(USBDEVICEINFO));

        if (info == NULL)
        {
            OOPS();
            if (configDesc != NULL)
            {
                FREE(configDesc);
            }
            FREE(connectionInfo);
            break;
        }

        info->ConnectionInfo = connectionInfo;

        info->ConfigDesc = configDesc;

        info->StringDescs = stringDescs;

		if ( (DeviceConnected == connectionInfo->ConnectionStatus) && !connectionInfo->DeviceIsHub)
		{
			m_USBList1.push_back(*info);
		}

		if (!info)
			FREE(info);

    }
}

//*****************************************************************************
//
// GetHCDDriverKeyName()
//
//*****************************************************************************

PCHAR CUSBCommonUtil::GetHCDDriverKeyName (HANDLE HCD)
{
    BOOL                    success;
    ULONG                   nBytes;
    USB_HCD_DRIVERKEY_NAME  driverKeyName;
    PUSB_HCD_DRIVERKEY_NAME driverKeyNameW;
    PCHAR                   driverKeyNameA;

    driverKeyNameW = NULL;
    driverKeyNameA = NULL;

	try {
		// Get the length of the name of the driver key of the HCD
		//
		success = DeviceIoControl(HCD,
								  IOCTL_GET_HCD_DRIVERKEY_NAME,
								  &driverKeyName,
								  sizeof(driverKeyName),
								  &driverKeyName,
								  sizeof(driverKeyName),
								  &nBytes,
								  NULL);
		
		if (!success) 
			throw -1;
					
		// Allocate space to hold the driver key name
		//
		nBytes = driverKeyName.ActualLength;		
		if (nBytes <= sizeof(driverKeyName))
			throw -1;
		
		driverKeyNameW = (PUSB_HCD_DRIVERKEY_NAME)ALLOC(nBytes);
		
		if (driverKeyNameW == NULL)
			throw -1;
		
		// Get the name of the driver key of the device attached to
		// the specified port.
		//
		success = DeviceIoControl(HCD,
								  IOCTL_GET_HCD_DRIVERKEY_NAME,
								  driverKeyNameW,
								  nBytes,
								  driverKeyNameW,
								  nBytes,
								  &nBytes,
								  NULL);
		
		if (!success) {
			throw -1;
		}
		
		// Convert the driver key name
		//
		driverKeyNameA = WideStrToMultiStr(driverKeyNameW->DriverKeyName);
		
	}
	catch (...) { 
		// All done, free the uncoverted driver key name and return the
		// converted driver key name
		//
		if (driverKeyNameW != NULL)
		{
			FREE(driverKeyNameW);
			driverKeyNameW = NULL;
		}
		return NULL;
	}

    return driverKeyNameA;
}

//*****************************************************************************
//
// GetDriverKeyName()
//
//*****************************************************************************

PCHAR CUSBCommonUtil::GetDriverKeyName (HANDLE Hub, ULONG ConnectionIndex)
{
    BOOL                                success;
    ULONG                               nBytes;
    USB_NODE_CONNECTION_DRIVERKEY_NAME  driverKeyName;
    PUSB_NODE_CONNECTION_DRIVERKEY_NAME driverKeyNameW;
    PCHAR                               driverKeyNameA;

    driverKeyNameW = NULL;
    driverKeyNameA = NULL;
	
	try {
		// Get the length of the name of the driver key of the device attached to
		// the specified port.
		//
		driverKeyName.ConnectionIndex = ConnectionIndex;
		
		success = DeviceIoControl(Hub,
								  IOCTL_USB_GET_NODE_CONNECTION_DRIVERKEY_NAME,
								  &driverKeyName,
								  sizeof(driverKeyName),
								  &driverKeyName,
								  sizeof(driverKeyName),
								  &nBytes,
								  NULL);
		
		if (!success)
			throw -1;
		
		// Allocate space to hold the driver key name
		//
		nBytes = driverKeyName.ActualLength;
		
		if (nBytes <= sizeof(driverKeyName))
			throw -1;
		
		driverKeyNameW = (PUSB_NODE_CONNECTION_DRIVERKEY_NAME)ALLOC(nBytes);
		
		if (driverKeyNameW == NULL)
			throw -1;
		
		// Get the name of the driver key of the device attached to
		// the specified port.
		//
		driverKeyNameW->ConnectionIndex = ConnectionIndex;
		
		success = DeviceIoControl(Hub,
								  IOCTL_USB_GET_NODE_CONNECTION_DRIVERKEY_NAME,
								  driverKeyNameW,
								  nBytes,
								  driverKeyNameW,
								  nBytes,
								  &nBytes,
								  NULL);
		
		if (!success)
			throw -1;
		
		// Convert the driver key name
		//
		driverKeyNameA = WideStrToMultiStr(driverKeyNameW->DriverKeyName);
		
		// All done, free the uncoverted driver key name and return the
		// converted driver key name
		//
		FREE(driverKeyNameW);
		
		return driverKeyNameA;
	}
	catch (...) {
		// There was an error, free anything that was allocated
		//
		if (driverKeyNameW != NULL)
		{
			FREE(driverKeyNameW);
			driverKeyNameW = NULL;
		}
		
		return NULL;
	}
}

CHAR buf[512];  // XXXXX How big does this have to be? Dynamically size it?

//*****************************************************************************
//
// DriverNameToDeviceDesc()
//
// Returns the Device Description of the DevNode with the matching DriverName.
// Returns NULL if the matching DevNode is not found.
//
// The caller should copy the returned string buffer instead of just saving
// the pointer value.  XXXXX Dynamically allocate return buffer?
//
//*****************************************************************************

PCHAR CUSBCommonUtil::DriverNameToDeviceDesc (PCHAR DriverName)
{
    DEVINST     devInst;
    DEVINST     devInstNext;
    CONFIGRET   cr;
    ULONG       walkDone = 0;
    ULONG       len;

    // Get Root DevNode
    //
    cr = CM_Locate_DevNode(&devInst,
                           NULL,
                           0);

    if (cr != CR_SUCCESS)
    {
        return NULL;
    }

    // Do a depth first search for the DevNode with a matching
    // DriverName value
    //
    while (!walkDone)
    {
        // Get the DriverName value
        //
        len = sizeof(buf);
        cr = CM_Get_DevNode_Registry_Property(devInst,
                                              CM_DRP_DRIVER,
                                              NULL,
                                              buf,
                                              &len,
                                              0);

        // If the DriverName value matches, return the DeviceDescription
        //
        if (cr == CR_SUCCESS && strcmp(DriverName, buf) == 0)
        {
            len = sizeof(buf);
            cr = CM_Get_DevNode_Registry_Property(devInst,
                                                  CM_DRP_DEVICEDESC,
                                                  NULL,
                                                  buf,
                                                  &len,
                                                  0);

            if (cr == CR_SUCCESS)
            {
                return buf;
            }
            else
            {
                return NULL;
            }
        }

        // This DevNode didn't match, go down a level to the first child.
        //
        cr = CM_Get_Child(&devInstNext,
                          devInst,
                          0);

        if (cr == CR_SUCCESS)
        {
            devInst = devInstNext;
            continue;
        }

        // Can't go down any further, go across to the next sibling.  If
        // there are no more siblings, go back up until there is a sibling.
        // If we can't go up any further, we're back at the root and we're
        // done.
        //
        for (;;)
        {
            cr = CM_Get_Sibling(&devInstNext,
                                devInst,
                                0);

            if (cr == CR_SUCCESS)
            {
                devInst = devInstNext;
                break;
            }

            cr = CM_Get_Parent(&devInstNext,
                               devInst,
                               0);


            if (cr == CR_SUCCESS)
            {
                devInst = devInstNext;
            }
            else
            {
                walkDone = 1;
                break;
            }
        }
    }

    return NULL;
}

//*****************************************************************************
//
// GetRootHubName()
//
//*****************************************************************************

PCHAR CUSBCommonUtil::GetRootHubName (HANDLE HostController)
{
    BOOL                success;
    ULONG               nBytes;
    USB_ROOT_HUB_NAME   rootHubName;
    PUSB_ROOT_HUB_NAME  rootHubNameW;
    PCHAR               rootHubNameA;

    rootHubNameW = NULL;
    rootHubNameA = NULL;
	
	try {
		// Get the length of the name of the Root Hub attached to the
		// Host Controller
		//
		success = DeviceIoControl(HostController,
								  IOCTL_USB_GET_ROOT_HUB_NAME,
								  0,
								  0,
								  &rootHubName,
								  sizeof(rootHubName),
								  &nBytes,
								  NULL);
		
		if (!success)
			throw -1;
		
		// Allocate space to hold the Root Hub name
		//
		nBytes = rootHubName.ActualLength;		
		rootHubNameW = (PUSB_ROOT_HUB_NAME)ALLOC(nBytes);
		
		if (rootHubNameW == NULL)
			throw -1;
		
		// Get the name of the Root Hub attached to the Host Controller
		//
		success = DeviceIoControl(HostController,
								  IOCTL_USB_GET_ROOT_HUB_NAME,
								  NULL,
								  0,
								  rootHubNameW,
								  nBytes,
								  &nBytes,
								  NULL);
		
		if (!success)
			throw -1;
		
		// Convert the Root Hub name
		//
		rootHubNameA = WideStrToMultiStr(rootHubNameW->RootHubName);
		
		// All done, free the uncoverted Root Hub name and return the
		// converted Root Hub name
		//
		FREE(rootHubNameW);
		
		return rootHubNameA;
	}
	catch(...) {
		// There was an error, free anything that was allocated
		//
		OOPS();
		if (rootHubNameW != NULL)
		{
			FREE(rootHubNameW);
			rootHubNameW = NULL;
		}
		
		return NULL;
	}
}


//*****************************************************************************
//
// GetConfigDescriptor()
//
// hHubDevice - Handle of the hub device containing the port from which the
// Configuration Descriptor will be requested.
//
// ConnectionIndex - Identifies the port on the hub to which a device is
// attached from which the Configuration Descriptor will be requested.
//
// DescriptorIndex - Configuration Descriptor index, zero based.
//
//*****************************************************************************

PUSB_DESCRIPTOR_REQUEST CUSBCommonUtil::GetConfigDescriptor (HANDLE  hHubDevice,
															 ULONG   ConnectionIndex,
															 UCHAR   DescriptorIndex)
{
    BOOL    success;
    ULONG   nBytes;
    ULONG   nBytesReturned;

    UCHAR   configDescReqBuf[sizeof(USB_DESCRIPTOR_REQUEST) +
                             sizeof(USB_CONFIGURATION_DESCRIPTOR)];

    PUSB_DESCRIPTOR_REQUEST         configDescReq;
    PUSB_CONFIGURATION_DESCRIPTOR   configDesc;


    // Request the Configuration Descriptor the first time using our
    // local buffer, which is just big enough for the Cofiguration
    // Descriptor itself.
    //
    nBytes = sizeof(configDescReqBuf);

    configDescReq = (PUSB_DESCRIPTOR_REQUEST)configDescReqBuf;
    configDesc = (PUSB_CONFIGURATION_DESCRIPTOR)(configDescReq+1);

    // Zero fill the entire request structure
    //
    memset(configDescReq, 0, nBytes);

    // Indicate the port from which the descriptor will be requested
    //
    configDescReq->ConnectionIndex = ConnectionIndex;

    //
    // USBHUB uses URB_FUNCTION_GET_DESCRIPTOR_FROM_DEVICE to process this
    // IOCTL_USB_GET_DESCRIPTOR_FROM_NODE_CONNECTION request.
    //
    // USBD will automatically initialize these fields:
    //     bmRequest = 0x80
    //     bRequest  = 0x06
    //
    // We must inititialize these fields:
    //     wValue    = Descriptor Type (high) and Descriptor Index (low byte)
    //     wIndex    = Zero (or Language ID for String Descriptors)
    //     wLength   = Length of descriptor buffer
    //
    configDescReq->SetupPacket.wValue = (USB_CONFIGURATION_DESCRIPTOR_TYPE << 8)
                                        | DescriptorIndex;

    configDescReq->SetupPacket.wLength = (USHORT)(nBytes - sizeof(USB_DESCRIPTOR_REQUEST));

	try {
		// Now issue the get descriptor request.
		//
		success = DeviceIoControl(hHubDevice,
								  IOCTL_USB_GET_DESCRIPTOR_FROM_NODE_CONNECTION,
								  configDescReq,
								  nBytes,
								  configDescReq,
								  nBytes,
								  &nBytesReturned,
								  NULL);
		
		if (!success)
			throw -1;
		
		if (nBytes != nBytesReturned)
			throw -1;
		
		if (configDesc->wTotalLength < sizeof(USB_CONFIGURATION_DESCRIPTOR))
			throw -1;
		
		// Now request the entire Configuration Descriptor using a dynamically
		// allocated buffer which is sized big enough to hold the entire descriptor
		//
		nBytes = sizeof(USB_DESCRIPTOR_REQUEST) + configDesc->wTotalLength;
		
		configDescReq = (PUSB_DESCRIPTOR_REQUEST)ALLOC(nBytes);
		
		if (configDescReq == NULL)
			throw -1;
		
		configDesc = (PUSB_CONFIGURATION_DESCRIPTOR)(configDescReq+1);
		
		// Indicate the port from which the descriptor will be requested
		//
		configDescReq->ConnectionIndex = ConnectionIndex;
		
		//
		// USBHUB uses URB_FUNCTION_GET_DESCRIPTOR_FROM_DEVICE to process this
		// IOCTL_USB_GET_DESCRIPTOR_FROM_NODE_CONNECTION request.
		//
		// USBD will automatically initialize these fields:
		//     bmRequest = 0x80
		//     bRequest  = 0x06
		//
		// We must inititialize these fields:
		//     wValue    = Descriptor Type (high) and Descriptor Index (low byte)
		//     wIndex    = Zero (or Language ID for String Descriptors)
		//     wLength   = Length of descriptor buffer
		//
		configDescReq->SetupPacket.wValue = (USB_CONFIGURATION_DESCRIPTOR_TYPE << 8) | DescriptorIndex;
		
		configDescReq->SetupPacket.wLength = (USHORT)(nBytes - sizeof(USB_DESCRIPTOR_REQUEST));
		
		// Now issue the get descriptor request.
		//
		success = DeviceIoControl(hHubDevice,
								  IOCTL_USB_GET_DESCRIPTOR_FROM_NODE_CONNECTION,
								  configDescReq,
								  nBytes,
								  configDescReq,
								  nBytes,
								  &nBytesReturned,
								  NULL);
		
		if (!success)
			throw -1;
		
		if (nBytes != nBytesReturned)
			throw -1;
		
		if (configDesc->wTotalLength != (nBytes - sizeof(USB_DESCRIPTOR_REQUEST)))
			throw -1;
		
		return configDescReq;
	}
	catch (...) {
		OOPS();
		FREE(configDescReq);
        return NULL;
	}
}


//*****************************************************************************
//
// AreThereStringDescriptors()
//
// DeviceDesc - Device Descriptor for which String Descriptors should be
// checked.
//
// ConfigDesc - Configuration Descriptor (also containing Interface Descriptor)
// for which String Descriptors should be checked.
//
//*****************************************************************************

BOOL CUSBCommonUtil::AreThereStringDescriptors (PUSB_DEVICE_DESCRIPTOR          DeviceDesc,
												PUSB_CONFIGURATION_DESCRIPTOR   ConfigDesc)
{
    PUCHAR                  descEnd;
    PUSB_COMMON_DESCRIPTOR  commonDesc;

    //
    // Check Device Descriptor strings
    //

    if (DeviceDesc->iManufacturer ||
        DeviceDesc->iProduct      ||
        DeviceDesc->iSerialNumber
       )
    {
        return TRUE;
    }


    //
    // Check the Configuration and Interface Descriptor strings
    //

    descEnd = (PUCHAR)ConfigDesc + ConfigDesc->wTotalLength;

    commonDesc = (PUSB_COMMON_DESCRIPTOR)ConfigDesc;

    while ((PUCHAR)commonDesc + sizeof(USB_COMMON_DESCRIPTOR) < descEnd &&
           (PUCHAR)commonDesc + commonDesc->bLength <= descEnd)
    {
        switch (commonDesc->bDescriptorType)
        {
            case USB_CONFIGURATION_DESCRIPTOR_TYPE:
                if (commonDesc->bLength != sizeof(USB_CONFIGURATION_DESCRIPTOR))
                {
                    OOPS();
                    break;
                }
                if (((PUSB_CONFIGURATION_DESCRIPTOR)commonDesc)->iConfiguration)
                {
                    return TRUE;
                }
				commonDesc = (PUSB_COMMON_DESCRIPTOR)((PUCHAR)commonDesc + commonDesc->bLength);
                continue;

            case USB_INTERFACE_DESCRIPTOR_TYPE:
                if (commonDesc->bLength != sizeof(USB_INTERFACE_DESCRIPTOR) &&
                    commonDesc->bLength != sizeof(USB_INTERFACE_DESCRIPTOR2))
                {
                    OOPS();
                    break;
                }
                if (((PUSB_INTERFACE_DESCRIPTOR)commonDesc)->iInterface)
                {
                    return TRUE;
                }
				commonDesc = (PUSB_COMMON_DESCRIPTOR)((PUCHAR)commonDesc + commonDesc->bLength);
                continue;

            default:
				commonDesc = (PUSB_COMMON_DESCRIPTOR)((PUCHAR)commonDesc + commonDesc->bLength);
                continue;
        }
        break;
    }

    return FALSE;
}


//*****************************************************************************
//
// GetAllStringDescriptors()
//
// hHubDevice - Handle of the hub device containing the port from which the
// String Descriptors will be requested.
//
// ConnectionIndex - Identifies the port on the hub to which a device is
// attached from which the String Descriptors will be requested.
//
// DeviceDesc - Device Descriptor for which String Descriptors should be
// requested.
//
// ConfigDesc - Configuration Descriptor (also containing Interface Descriptor)
// for which String Descriptors should be requested.
//
//*****************************************************************************

PSTRING_DESCRIPTOR_NODE CUSBCommonUtil::GetAllStringDescriptors (
							HANDLE                          hHubDevice,
							ULONG                           ConnectionIndex,
							PUSB_DEVICE_DESCRIPTOR          DeviceDesc,
							PUSB_CONFIGURATION_DESCRIPTOR   ConfigDesc)
{
    PSTRING_DESCRIPTOR_NODE supportedLanguagesString;
    PSTRING_DESCRIPTOR_NODE stringDescNodeTail;
    ULONG                   numLanguageIDs;
    USHORT                  *languageIDs;

    PUCHAR                  descEnd;
    PUSB_COMMON_DESCRIPTOR  commonDesc;

    //
    // Get the array of supported Language IDs, which is returned
    // in String Descriptor 0
    //
    supportedLanguagesString = GetStringDescriptor(hHubDevice,
                                                   ConnectionIndex,
                                                   0,
                                                   0);

    if (supportedLanguagesString == NULL)
    {
        return NULL;
    }

    numLanguageIDs = (supportedLanguagesString->StringDescriptor->bLength - 2) / 2;

    languageIDs = &supportedLanguagesString->StringDescriptor->bString[0];

    stringDescNodeTail = supportedLanguagesString;

    //
    // Get the Device Descriptor strings
    //

    if (DeviceDesc->iManufacturer)
    {
        stringDescNodeTail = GetStringDescriptors(hHubDevice,
                                                  ConnectionIndex,
                                                  DeviceDesc->iManufacturer,
                                                  numLanguageIDs,
                                                  languageIDs,
                                                  stringDescNodeTail);
    }

    if (DeviceDesc->iProduct)
    {
        stringDescNodeTail = GetStringDescriptors(hHubDevice,
                                                  ConnectionIndex,
                                                  DeviceDesc->iProduct,
                                                  numLanguageIDs,
                                                  languageIDs,
                                                  stringDescNodeTail);
    }

    if (DeviceDesc->iSerialNumber)
    {
        stringDescNodeTail = GetStringDescriptors(hHubDevice,
                                                  ConnectionIndex,
                                                  DeviceDesc->iSerialNumber,
                                                  numLanguageIDs,
                                                  languageIDs,
                                                  stringDescNodeTail);
    }


    //
    // Get the Configuration and Interface Descriptor strings
    //

    descEnd = (PUCHAR)ConfigDesc + ConfigDesc->wTotalLength;

    commonDesc = (PUSB_COMMON_DESCRIPTOR)ConfigDesc;

    while ((PUCHAR)commonDesc + sizeof(USB_COMMON_DESCRIPTOR) < descEnd &&
           (PUCHAR)commonDesc + commonDesc->bLength <= descEnd)
    {
        switch (commonDesc->bDescriptorType)
        {
            case USB_CONFIGURATION_DESCRIPTOR_TYPE:
                if (commonDesc->bLength != sizeof(USB_CONFIGURATION_DESCRIPTOR))
                {
                    OOPS();
                    break;
                }
                if (((PUSB_CONFIGURATION_DESCRIPTOR)commonDesc)->iConfiguration)
                {
                    stringDescNodeTail = GetStringDescriptors(
                                             hHubDevice,
                                             ConnectionIndex,
                                             ((PUSB_CONFIGURATION_DESCRIPTOR)commonDesc)->iConfiguration,
                                             numLanguageIDs,
                                             languageIDs,
                                             stringDescNodeTail);
                }
				commonDesc = (PUSB_COMMON_DESCRIPTOR)((PUCHAR)commonDesc + commonDesc->bLength);
                continue;

            case USB_INTERFACE_DESCRIPTOR_TYPE:
                if (commonDesc->bLength != sizeof(USB_INTERFACE_DESCRIPTOR) &&
                    commonDesc->bLength != sizeof(USB_INTERFACE_DESCRIPTOR2))
                {
                    OOPS();
                    break;
                }
                if (((PUSB_INTERFACE_DESCRIPTOR)commonDesc)->iInterface)
                {
                    stringDescNodeTail = GetStringDescriptors(
                                             hHubDevice,
                                             ConnectionIndex,
                                             ((PUSB_INTERFACE_DESCRIPTOR)commonDesc)->iInterface,
                                             numLanguageIDs,
                                             languageIDs,
                                             stringDescNodeTail);
                }
                commonDesc = (PUSB_COMMON_DESCRIPTOR)((PUCHAR)commonDesc + commonDesc->bLength);
                continue;

            default:
                commonDesc = (PUSB_COMMON_DESCRIPTOR)((PUCHAR)commonDesc + commonDesc->bLength);
                continue;
        }
        break;
    }

    return supportedLanguagesString;
}


//*****************************************************************************
//
// GetStringDescriptor()
//
// hHubDevice - Handle of the hub device containing the port from which the
// String Descriptor will be requested.
//
// ConnectionIndex - Identifies the port on the hub to which a device is
// attached from which the String Descriptor will be requested.
//
// DescriptorIndex - String Descriptor index.
//
// LanguageID - Language in which the string should be requested.
//
//*****************************************************************************

PSTRING_DESCRIPTOR_NODE CUSBCommonUtil::GetStringDescriptor (
							HANDLE  hHubDevice,
							ULONG   ConnectionIndex,
							UCHAR   DescriptorIndex,
							USHORT  LanguageID)
{
    BOOL    success;
    ULONG   nBytes;
    ULONG   nBytesReturned;

    UCHAR   stringDescReqBuf[sizeof(USB_DESCRIPTOR_REQUEST) +
                             MAXIMUM_USB_STRING_LENGTH];

    PUSB_DESCRIPTOR_REQUEST stringDescReq;
    PUSB_STRING_DESCRIPTOR  stringDesc;
    PSTRING_DESCRIPTOR_NODE stringDescNode;

    nBytes = sizeof(stringDescReqBuf);

    stringDescReq = (PUSB_DESCRIPTOR_REQUEST)stringDescReqBuf;
    stringDesc = (PUSB_STRING_DESCRIPTOR)(stringDescReq+1);

    // Zero fill the entire request structure
    //
    memset(stringDescReq, 0, nBytes);

    // Indicate the port from which the descriptor will be requested
    //
    stringDescReq->ConnectionIndex = ConnectionIndex;

    //
    // USBHUB uses URB_FUNCTION_GET_DESCRIPTOR_FROM_DEVICE to process this
    // IOCTL_USB_GET_DESCRIPTOR_FROM_NODE_CONNECTION request.
    //
    // USBD will automatically initialize these fields:
    //     bmRequest = 0x80
    //     bRequest  = 0x06
    //
    // We must inititialize these fields:
    //     wValue    = Descriptor Type (high) and Descriptor Index (low byte)
    //     wIndex    = Zero (or Language ID for String Descriptors)
    //     wLength   = Length of descriptor buffer
    //
    stringDescReq->SetupPacket.wValue = (USB_STRING_DESCRIPTOR_TYPE << 8)
                                        | DescriptorIndex;

    stringDescReq->SetupPacket.wIndex = LanguageID;

    stringDescReq->SetupPacket.wLength = (USHORT)(nBytes - sizeof(USB_DESCRIPTOR_REQUEST));

	try {
		// Now issue the get descriptor request.
		//
		success = DeviceIoControl(hHubDevice,
								  IOCTL_USB_GET_DESCRIPTOR_FROM_NODE_CONNECTION,
								  stringDescReq,
								  nBytes,
								  stringDescReq,
								  nBytes,
								  &nBytesReturned,
								  NULL);
		
		//
		// Do some sanity checks on the return from the get descriptor request.
		//
		
		if (!success)
			throw -1;
		
		if (nBytesReturned < 2)
			throw -1;
		
		if (stringDesc->bDescriptorType != USB_STRING_DESCRIPTOR_TYPE)
			throw -1;
		
		if (stringDesc->bLength != nBytesReturned - sizeof(USB_DESCRIPTOR_REQUEST))
			throw -1;
		
		if (stringDesc->bLength % 2 != 0)
			throw -1;
		
		//
		// Looks good, allocate some (zero filled) space for the string descriptor
		// node and copy the string descriptor to it.
		//
		
		stringDescNode = (PSTRING_DESCRIPTOR_NODE)ALLOC(sizeof(STRING_DESCRIPTOR_NODE) +
			stringDesc->bLength);
		
		if (stringDescNode == NULL)
			throw -1;
		
		stringDescNode->DescriptorIndex = DescriptorIndex;
		stringDescNode->LanguageID = LanguageID;
		
		memcpy(stringDescNode->StringDescriptor, stringDesc, stringDesc->bLength);
		
		return stringDescNode;
	}
	catch (...) {
        OOPS();
        return NULL;
	}
}


//*****************************************************************************
//
// GetStringDescriptors()
//
// hHubDevice - Handle of the hub device containing the port from which the
// String Descriptor will be requested.
//
// ConnectionIndex - Identifies the port on the hub to which a device is
// attached from which the String Descriptor will be requested.
//
// DescriptorIndex - String Descriptor index.
//
// NumLanguageIDs -  Number of languages in which the string should be
// requested.
//
// LanguageIDs - Languages in which the string should be requested.
//
//*****************************************************************************

PSTRING_DESCRIPTOR_NODE CUSBCommonUtil::GetStringDescriptors (
						HANDLE  hHubDevice,
						ULONG   ConnectionIndex,
						UCHAR   DescriptorIndex,
						ULONG   NumLanguageIDs,
						USHORT  *LanguageIDs,
						PSTRING_DESCRIPTOR_NODE StringDescNodeTail)
{
    ULONG i;

    for (i=0; i<NumLanguageIDs; i++)
    {
        StringDescNodeTail->Next = GetStringDescriptor(hHubDevice,
                                                       ConnectionIndex,
                                                       DescriptorIndex,
                                                       *LanguageIDs);

        if (StringDescNodeTail->Next)
        {
            StringDescNodeTail = StringDescNodeTail->Next;
        }

        LanguageIDs++;
    }

    return StringDescNodeTail;
}

//*****************************************************************************
//
// GetExternalHubName()
//
//*****************************************************************************

PCHAR CUSBCommonUtil::GetExternalHubName (HANDLE Hub, ULONG ConnectionIndex)
{
    BOOL                        success;
    ULONG                       nBytes;
    USB_NODE_CONNECTION_NAME    extHubName;
    PUSB_NODE_CONNECTION_NAME   extHubNameW;
    PCHAR                       extHubNameA;

    extHubNameW = NULL;
    extHubNameA = NULL;

	try {
		// Get the length of the name of the external hub attached to the
		// specified port.
		//
		extHubName.ConnectionIndex = ConnectionIndex;
		
		success = DeviceIoControl(Hub,
								  IOCTL_USB_GET_NODE_CONNECTION_NAME,
								  &extHubName,
								  sizeof(extHubName),
								  &extHubName,
								  sizeof(extHubName),
								  &nBytes,
								  NULL);
		
		if (!success)
			throw -1;
		
		// Allocate space to hold the external hub name
		//
		nBytes = extHubName.ActualLength;
// ryan temp		
		if (nBytes < sizeof(extHubName))
			throw -1;
		
		extHubNameW = (PUSB_NODE_CONNECTION_NAME)ALLOC(nBytes);
		
		if (extHubNameW == NULL)
			throw -1;
		
		// Get the name of the external hub attached to the specified port
		//
		extHubNameW->ConnectionIndex = ConnectionIndex;
		
		success = DeviceIoControl(Hub,
			IOCTL_USB_GET_NODE_CONNECTION_NAME,
			extHubNameW,
			nBytes,
			extHubNameW,
			nBytes,
			&nBytes,
			NULL);
		
		if (!success)
			throw -1;
		
		// Convert the External Hub name
		//
		extHubNameA = WideStrToMultiStr(extHubNameW->NodeName);
		
		// All done, free the uncoverted external hub name and return the
		// converted external hub name
		//
		FREE(extHubNameW);
		
		return extHubNameA;
	}
	catch (...) {
		// There was an error, free anything that was allocated
		//
		OOPS();
		if (extHubNameW != NULL)
		{
			FREE(extHubNameW);
			extHubNameW = NULL;
		}
		
		return NULL;
	}
}

INT CUSBCommonUtil::GetUSBList1()
{
	EnumerateHostControllers();
// temp
	
PCHAR driverKeyName = "{36FC9E60-C465-11CF-8056-444553540000}\\0015";
		// Create the full hub device name
		//
PCHAR pTest = new CHAR[strlen(driverKeyName) + 4];
memset(pTest, 0, strlen(driverKeyName) + 4);
		strcpy(pTest, "\\\\.\\");
		strcpy(pTest + sizeof("\\\\.\\") - 1, driverKeyName);
		
		// Try to hub the open device
		//
		HANDLE hHandel = CreateFile(pTest,
			GENERIC_WRITE,
			FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			0,
			NULL);
delete[] pTest;


	vector<USBDEVICEINFO>::iterator vi;
	for (vi = m_USBList1.begin(); vi != m_USBList1.end(); vi++) {
		PUSBDEVICEINFO p = (PUSBDEVICEINFO)vi;
		if (p) {
			if (p->ConnectionInfo->DeviceDescriptor.iSerialNumber) {
				PSTRING_DESCRIPTOR_NODE pDesc = p->StringDescs;
				while(pDesc) {
					if (p->ConnectionInfo->DeviceDescriptor.iSerialNumber == pDesc->DescriptorIndex) {
						PCHAR pSN = WideStrToMultiStr(pDesc->StringDescriptor->bString);
						INT nLen = strlen(pSN);
						// cleare the 0x3f at the end of pTemp
						for (INT i = (nLen - 1); i > 0; i--) 
							if (pSN[i] == 0x3F) {
								pSN[i] = 0x00;
							}
							else
								break;
						FREE(pSN);
					}
					pDesc = pDesc->Next;
				}
			}
		}
	}

	return m_ulTotalDevicesConnected;
}

PCHAR CUSBCommonUtil::WideStrToMultiStr (PWCHAR WideStr)
{
    ULONG nBytes;
    PCHAR MultiStr;

    // Get the length of the converted string
    //
    nBytes = WideCharToMultiByte(CP_ACP,
								 0,
								 WideStr,
								 -1,
								 NULL,
								 0,
								 NULL,
								 NULL);

    if (nBytes == 0)
        return NULL;

    // Allocate space to hold the converted string
    //
    MultiStr = (PCHAR)ALLOC(nBytes);

    if (MultiStr == NULL)
        return NULL;

    // Convert the string
    //
    nBytes = WideCharToMultiByte(CP_ACP,
								 0,
								 WideStr,
								 -1,
								 MultiStr,
								 nBytes,
								 NULL,
								 NULL);

    if (nBytes == 0)
    {
        delete MultiStr;
        return NULL;
    }

    return MultiStr;
}

BOOL CUSBCommonUtil::EjectBySN(LPCSTR pProductID, LPCSTR pVendorID, LPCSTR pSN)
{
	BOOL bResult = FALSE;
	EnumerateHostControllers();
	if (m_ulTotalDevicesConnected < 1)
		return TRUE;

	vector<USBDEVICEINFO>::iterator vi;
	for (vi = m_USBList1.begin(); vi != m_USBList1.end(); vi++) {
		PUSBDEVICEINFO p = (PUSBDEVICEINFO)vi;
		if (!p)
			continue;

		if (p->ConnectionInfo->DeviceDescriptor.iProduct)
		{			
			if (atoi(pProductID) != p->ConnectionInfo->DeviceDescriptor.iProduct)
				return FALSE;
		}
		else {
			return FALSE;
		}

		if (p->ConnectionInfo->DeviceDescriptor.idVendor)
		{
			if (atoi(pVendorID) != p->ConnectionInfo->DeviceDescriptor.idVendor)
				return FALSE;
		}
		else {
			return FALSE;
		}

		if (p->ConnectionInfo->DeviceDescriptor.iSerialNumber)
		{
			PSTRING_DESCRIPTOR_NODE pDesc = p->StringDescs;
			while(pDesc) {
				if (p->ConnectionInfo->DeviceDescriptor.iSerialNumber == pDesc->DescriptorIndex) {
					PCHAR pTemp = WideStrToMultiStr(pDesc->StringDescriptor->bString);
					INT nLen = strlen(pTemp);
					// cleare the 0x3f at the end of pTemp
					for (INT i = (nLen - 1); i > 0; i--) 
						if (pTemp[i] == 0x3F)
							pTemp[i] = 0x00;
						else
							break;

					if (strcmpi(pSN, pTemp) == 0) {
						//bResult = Eject(p->ConnectionInfo->);
					}					

					FREE(pTemp);
					break;
				}
				pDesc = pDesc->Next;
			}
		}
		else {
			return FALSE;
		}
	}

	return bResult;
}

/************************************************************************/
/* eject the usb device                                                 */
/* using the STL algorithm
/* 
/************************************************************************/

INT CUSBCommonUtil::EjectAllUsb()
{
	INT nUsbCnt = GetUSBList();
	if (nUsbCnt < 1)
		return nUsbCnt;

	nUsbCnt = 0;
	vector<USB_DEV_INFO>::iterator vi;
	for (vi = m_USBList.begin(); vi != m_USBList.end(); vi++) {
		LPUSB_DEV_INFO p = (LPUSB_DEV_INFO)vi;
		if (p && p->hDevInst) {
			if (Eject(p->hDevInst)) {
				p->hDevInst = NULL;
				nUsbCnt ++;
			}
		}
	}

	return nUsbCnt;
}

/************************************************************************/
/* whether the usb is enabled                                           */
/* using the STL algorithm
/* 
/************************************************************************/
#ifdef _USING_STL_ALGORITHM_

bool STLIsUSBEnabled(const USB_DEV_INFO& usbInfo) {
	return (g_pUSB && !g_pUSB->IsDisableable(usbInfo.hDevInst));
}

#endif

INT CUSBCommonUtil::GetEnabledUSBListCount()
{
	INT nUsbCnt = 0;

#ifdef _USING_STL_ALGORITHM_
	// using STL algorithm
	nUsbCnt = count_if(m_USBList.begin(), m_USBList.end(), STLIsUSBEnabled);
#else	
	// not using STL algorithm
	vector<USB_DEV_INFO>::const_iterator vi;
	for (vi = m_USBList.begin(); vi != m_USBList.end(); vi++) {
		LPUSB_DEV_INFO p = (LPUSB_DEV_INFO)vi;
		if ( p && !(IsDisabled(p->hDevInst)) ) {
			nUsbCnt++;
		}
	}	
#endif

	return nUsbCnt;
}

/************************************************************************/
/* Generate filter conditions according the file content                */
/* using the STL algorithm
/* 
/************************************************************************/
INT CUSBCommonUtil::CreateFilter(vector<FILTER_CONDITION> &Conditions, LPCSTR pszFileName)
{
	if (!pszFileName) {
		g_Log.Log("CreateFilter:File name is null!, DO NOT use the filter!");
		return -1;
	}
/*
	char szFullName[_MAX_PATH];
	if (!_tfullpath(szFullName, pszFileName, MAX_PATH)) {
		g_Log.Log("_tfullpath error");
	}
	g_Log.Log(szFullName);

	GetCurrentDirectory(_MAX_PATH, szFullName);
	g_Log.Log(szFullName);
	ifstream inFile("e:\\USBFilter.ini");
*/
	ifstream inFile(pszFileName);
	if (!inFile) {
		g_Log.Log("CreateFilter:Can not open filter file, DO NOT use the filter!");
		g_Log.Log(pszFileName);
		return -2;
	}

	vector<string> E_Desc;
	vector<string> E_HardwareID;
	vector<string> E_Location;
	vector<string> I_Desc;
	vector<string> I_HardwareID;
	vector<string> I_Location;

	const int MAX_LEN = 200;
	string szBuf;
	char chBuf[MAX_LEN];
	// 1. Read conditions from the section EQUAL
	BOOL bFoundEqual = FALSE;	
	while (!inFile.eof()) {
		inFile.getline(chBuf, MAX_LEN);
		szBuf = chBuf;
		// TODO: Maybe need do some trim operation

		if (strcmpi(chBuf, "[EQUAL]") == 0) {
			bFoundEqual = TRUE;
			break;
		}
	}

	if (bFoundEqual) {
		// Exit when condition is such as [xxxx]
		while (!inFile.eof()) {
			inFile.getline(chBuf, MAX_LEN);
			szBuf = chBuf;

			if ( (szBuf.length() > 0) && (szBuf[0] != '#') ) {		// Start with # means that it is remark line
				// Exit when condition is such as [xxxx]
				if ( (szBuf[0] == '[') && (szBuf[szBuf.length() - 1] = ']') )
					break;

				vector<string> *pCurrent = NULL;
				string szLeft;
				string szRight;
				if ( ((szLeft = szBuf.substr(0, 5)).length() > 0) && (strcmpi(szLeft.c_str(), "Desc=") == 0) ) {
					pCurrent = &E_Desc;
					szRight = szBuf.substr(5);
				}
				else if ( ((szLeft = szBuf.substr(0, 11)).length() > 0) && (strcmpi(szLeft.c_str(), "HardwareId=") == 0) ) {
					pCurrent = &E_HardwareID;
					szRight = szBuf.substr(11);
				}
				else if ( ((szLeft = szBuf.substr(0, 9)).length() > 0) && (strcmpi(szLeft.c_str(), "Location=") == 0) ) {
					pCurrent = &E_Location;
					szRight = szBuf.substr(9);
				}

				// Find duplicated record(s)
				if ( pCurrent && (szRight.length() > 0) )  {
					LPCSTR pValue = szRight.c_str();
					vector<string>::iterator iter;
					BOOL bDuplicated = FALSE;
					for (iter = pCurrent->begin(); iter != pCurrent->end(); iter++) {
						string temp = *iter;
						// Ship if the condition has already exist
						if (strcmpi(temp.c_str(), pValue) == 0) {
							bDuplicated = TRUE;
							break;
						}						
					}		// for iter

					if (!bDuplicated) {
						pCurrent->push_back(string(pValue));
					}
				}		// if pCurrent
			}		// if buff.length() > 0
		}		// while inFile >> buf
	}		// if bFoundEqual

	// 2. Read conditions from the section INCLUE
	inFile.seekg(ios_base::beg );
	inFile.clear();

	BOOL bFoundInclude = FALSE;
	while (!inFile.eof()) {
		inFile.getline(chBuf, MAX_LEN);
		szBuf = chBuf;
		// TODO: Maybe need do some trim operation

		if (strcmpi(chBuf, "[INCLUDE]") == 0) {
			bFoundInclude = TRUE;
			break;
		}
	}

	if (bFoundInclude) {
		// Exit when condition is such as [xxxx]
		while (!inFile.eof()) {
			inFile.getline(chBuf, MAX_LEN);
			szBuf = chBuf;
			if ( (szBuf.length() > 0) && (szBuf[0] != '#') ) {		// Start with # means that it is remark line
				// Exit when condition is such as [xxxx]
				if ( (szBuf[0] == '[') && (szBuf[szBuf.length() - 1] = ']') )
					break;

				vector<string> *pCurrent = NULL;
				string szLeft;
				string szRight;
				if ( ((szLeft = szBuf.substr(0, 5)).length() > 0) && (strcmpi(szLeft.c_str(), "Desc=") == 0) ) {
					pCurrent = &I_Desc;
					szRight = szBuf.substr(5);				
				}
				else if ( ((szLeft = szBuf.substr(0, 11)).length() > 0) && (strcmpi(szLeft.c_str(), "HardwareId=") == 0) ) {
					pCurrent = &I_HardwareID;
					szRight = szBuf.substr(11);
				}
				else if ( ((szLeft = szBuf.substr(0, 9)).length() > 0) && (strcmpi(szLeft.c_str(), "Location=") == 0) ) {
					pCurrent = &I_Location;
					szRight = szBuf.substr(9);
				}

				// Find duplicated record(s)
				if ( pCurrent && (szRight.length() > 0) )  {
					LPCSTR pNewValue = szRight.c_str();
					BOOL bHasProcessed = FALSE;
					vector<string>::iterator iter;
					for (iter = pCurrent->begin(); iter != pCurrent->end(); iter++) {							
						LPCSTR pTemp = ((string)(*iter)).c_str();
						// Skip if the condition is already included by existed conditions
						// or replace the old item with this one if it include the old one.
 						if (strstr(pNewValue, pTemp)) {
							// Skip this one
							bHasProcessed = TRUE;
							break;
						}
						else if (strstr(pTemp, pNewValue)) {
							// Replace with this one
							*iter = string(pNewValue);							
							bHasProcessed = TRUE;
							break;
						}
					}		// for iter
					
					if (!bHasProcessed) {
						// Add it
						pCurrent->push_back(string(pNewValue));
					}		// if !bHasProcessed
				}		// if pCurrent
			}		// if buff.length() > 0
		}		// while inFile >> buf
	}		// if bFoundInclude

	// 3. Fill all conditions to vector
	Conditions.clear();
	vector<string>::iterator iter;
	for (iter = E_Desc.begin(); iter != E_Desc.end(); iter++) {
		FILTER_CONDITION condition;		
		condition.bEqual = TRUE;
		condition.szName = "Desc";
		condition.szValue = *iter;
		Conditions.push_back(condition);
	}
	for (iter = E_HardwareID.begin(); iter != E_HardwareID.end(); iter++) {
		FILTER_CONDITION condition;		
		condition.bEqual = TRUE;
		condition.szName = "HardwareId";
		condition.szValue = *iter;
		Conditions.push_back(condition);
	}
	for (iter = E_Location.begin(); iter != E_Location.end(); iter++) {
		FILTER_CONDITION condition;		
		condition.bEqual = TRUE;
		condition.szName = "Location";
		condition.szValue = *iter;
		Conditions.push_back(condition);
	}
	for (iter = I_Desc.begin(); iter != I_Desc.end(); iter++) {
		FILTER_CONDITION condition;		
		condition.bEqual = FALSE;
		condition.szName = "Desc";
		condition.szValue = *iter;
		Conditions.push_back(condition);
	}
	for (iter = I_HardwareID.begin(); iter != I_HardwareID.end(); iter++) {
		FILTER_CONDITION condition;		
		condition.bEqual = FALSE;
		condition.szName = "HardwareId";
		condition.szValue = *iter;
		Conditions.push_back(condition);
	}
	for (iter = I_Location.begin(); iter != I_Location.end(); iter++) {
		FILTER_CONDITION condition;		
		condition.bEqual = FALSE;
		condition.szName = "Location";
		condition.szValue = *iter;
		Conditions.push_back(condition);
	}

	return Conditions.size();
}

BOOL CUSBCommonUtil::CheckFilter(const USB_DEV_INFO &usb, const vector<FILTER_CONDITION> &conditions)
{
//char msg[100];
//string msg1;
//msg1 = "USB:Desc=";
//msg1 += usb.szDevDesc;
//msg1 += ";HardwareID=";
//msg1 += usb.szHardwareId;
//msg1 += ";Location=";
//msg1 += usb.szLocationInfo;
//g_Log.Log(msg1.c_str());

	INT nPos = -1;
	CStrUtil StrUtil;
	vector<FILTER_CONDITION>::const_iterator iter;	
	for (iter = conditions.begin(); iter != conditions.end(); iter++) {
		FILTER_CONDITION condition = (FILTER_CONDITION)*iter;
		if (condition.bEqual) {		// full match	
			if (strcmpi(condition.szName.c_str(), "Desc") == 0) {				
				if (strcmpi(condition.szValue.c_str(), usb.szDevDesc.c_str()) == 0)
					return FALSE;
			}
			else if (strcmpi(condition.szName.c_str(), "HardwareId") == 0) {
				if (strcmpi(condition.szValue.c_str(), usb.szHardwareId.c_str()) == 0)
					return FALSE;
			}
			else if (strcmpi(condition.szName.c_str(), "Location") == 0) {
				if (strcmpi(condition.szValue.c_str(), usb.szLocationInfo.c_str()) == 0)
					return FALSE;
			}
		}
		else {						// include substring			
			string val(condition.szValue);
			StrUtil.ToLower(val);
			if (strcmpi(condition.szName.c_str(), "Desc") == 0) {
				string original(usb.szDevDesc);
				StrUtil.ToLower(original);				
				if ((nPos = original.find(val)) >= 0)
					return FALSE;
			}
			else if (strcmpi(condition.szName.c_str(), "HardwareId") == 0) {
				string original(usb.szHardwareId);
				StrUtil.ToLower(original);				
				if ((nPos = original.find(val)) >= 0)
					return FALSE;
			}
			else if (strcmpi(condition.szName.c_str(), "Location") == 0) {
				string original(usb.szLocationInfo);
				StrUtil.ToLower(original);
				if ((nPos = original.find(val)) >= 0)
					return FALSE;
			}
		}
	}

	return TRUE;
}
