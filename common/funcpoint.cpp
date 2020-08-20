#include "common/sys_incl.h"
#include "common/funcpoint.h"
#include "common/debug.h"

#define GETMODULEFILENAMEEX			_T("GetModuleFileNameExA")
#define MSIQUERYPRODUCTSTATE		_T("MsiQueryProductStateA")
#define MSIQUERYFEATURESTATE		_T("MsiQueryFeatureStateA")
#define MSIQUERYCOMPONENTSTATE		_T("MsiQueryComponentStateA")
#define MSIGETPRODUCTINFO			_T("MsiGetProductInfoA")
#define MSIENUMFEATURES				_T("MsiEnumFeaturesA")
#define MSIENUMPRODUCTS				_T("MsiEnumProductsA")
#define MSIENUMPATCHES				_T("MsiEnumPatchesA")
#define MSIDETERMINEPATCHSEQUENCE	_T("MsiDeterminePatchSequenceA")
#define MSIENUMPATCHESEX			_T("MsiEnumPatchesExA")

#define MSIINSTALLPRODUCT			_T("MsiInstallProductA")
#define MSICONFIGUREPRODUCT			_T("MsiConfigureProductA")

CFunctionPoint::CFunctionPoint ()
	: lpMsiEnumPatches (NULL)
	, lpMsiEnumProducts (NULL)
	, lpMsiEnumFeatures (NULL)
	, lpMsiGetProductInfo (NULL)
	, lpMsiQueryFeatureState (NULL)
	, lpMsiQueryComponentState (NULL) 
	, lpMsiQueryProductState (NULL)
	, lpMsiInstallProduct (NULL)
	, lpMsiConfigureProduct (NULL)
	
	, lpCM_Get_DevNode_Status_Ex (NULL)
    , lpSHGetFolderPath (NULL)
	, lpSHGetSpecialFolderPathA (NULL)
	, lpPathAppend (NULL)
	, lpPathFileExists (NULL)
	, lpPathFindOnPath (NULL)
	, lpGetSystemTimes (NULL)

	, lpOpenProcessToken (NULL)
	, lpSetTokenInformation (NULL)
	, lpImpersonateLoggedOnUser (NULL)
	, lpRevertToSelf (NULL)
	, lpWTSGetActiveConsoleSessionId (NULL)
	, lpWTSQueryUserToken (NULL)
	, lpDuplicateTokenEx (NULL)
	, lpProcessIdToSessionId (NULL)
	, lpGetSecurityDescriptorDacl (NULL)
	, lpGetAclInformation (NULL)
	, lpGetAce (NULL)
	, lpLookupAccountSid (NULL)
	, lpChangeServiceConfig2 (NULL)

	, lpGetAdaptersInfo (NULL)
	, lpGetAdaptersAddresses (NULL)
	, lpGetPerAdapterInfo (NULL)
	, lpGetNetworkParams (NULL)
	, lpGetIfEntry (NULL)
	, lpGetIfTable (NULL)

	, lpSnmpExtensionInit (NULL) 
	, lpSnmpExtensionQuery (NULL)
	, lpSnmpUtilMemReAlloc (NULL)
	, lpSnmpUtilVarBindListFree (NULL)
	, lpSnmpUtilOidCpy (NULL)

	, lpGetIpNetTable (NULL)
	, lpDeleteIpNetEntry (NULL)
	, lpSendARP (NULL)
	, lpAllocateAndGetTcpExTableFromStack (NULL)
	, lpAllocateAndGetUdpExTableFromStack (NULL)
	, lpGetUdpTable (NULL)
	, lpGetTcpTable (NULL)
	, lpNotifyAddrChange (NULL)
	, lpNotifyRouteChange (NULL)
	, lpIcmpCreateFile (NULL)
	, lpIcmpSendEcho (NULL)
	, lpIcmpCloseHandle (NULL)

	, lpCreateToolhelpSnapshot (NULL)
	, lpProcess32First (NULL)
	, lpProcess32Next (NULL)
	, lpGetModuleFileNameEx (NULL)
	, lpGetProcessImageFileName (NULL)
	, lpEnumProcessModules (NULL)

	, lpNetUserEnum (NULL)
	, lpNetShareEnum (NULL)
	, lpNetShareDel (NULL)
	, lpNetShareGetInfo (NULL)
	, lpNetUserGetInfo (NULL)
	, lpNetApiBufferFree (NULL)
	, lpNetGroupEnum (NULL)
	, lpNetGroupGetUsers (NULL)
	, lpNetUserGetGroups (NULL)
	, lpNetLocalGroupEnum (NULL)
	, lpNetLocalGroupGetMembers (NULL)
	, lpNetUserGetLocalGroups (NULL)
	, lpNetUserAdd (NULL)
	, lpNetUserDel (NULL)
	, lpNetUserSetInfo (NULL)
	, lpNetApiBufferAllocate (NULL)
	, lpNetUserModalsGet (NULL)
	, lpNetUserModalsSet (NULL)
	, lpConvertStringSidToSid (NULL)
	
	, lpGetProcessHandleCount (NULL)
	, lpGetTokenInformation (NULL)
	, lpGetProcessMemoryInfo (NULL)
	, lpGetProcessTimes (NULL)
	, lpReadProcessMemory (NULL)
	, lpCreateFile (NULL)
	, lpGlobalMemoryStatusEx (NULL)

	, lpNtQueryInformationProcess (NULL)
	, lpNtQuerySystemInformation (NULL)

	, lpWTSRegisterSessionNotification (NULL)
	, lpWTSUnRegisterSessionNotification (NULL)
	, lpWTSEnumerateSessions (NULL)
	, lpWTSQuerySessionInformation (NULL)
	, lpWTSFreeMemory (NULL)

	, lpWow64DisableWow64FsRedirection (NULL)
	, lpWow64RevertWow64FsRedirection (NULL)

	, lpInternetOpen (NULL)
	, lpInternetOpenUrl (NULL)
	, lpHttpQueryInfo (NULL) 
	, lpInternetSetOption (NULL)
	, lpInternetReadFile (NULL) 
	, lpInternetCloseHandle (NULL)

	, lpNtCreateFile (NULL)
	, lpNtDeviceIoControlFile (NULL)
	, lpNtClose (NULL)
	
	, lpGetNamedSecurityInfo (NULL)
	, lpSetEntriesInAcl (NULL)
	, lpSetNamedSecurityInfo (NULL)

	, lpGetFileVersionInfo (NULL)
	, lpGetFileVersionInfoSize (NULL)
	, lpVerQueryValue (NULL)

	, lpMsiDeterminePatchSequence(NULL)

	, lpQueryFullProcessImageName (NULL)

	,lpCM_Get_Device_ID_List_Ex (NULL)
	,lpCM_Get_HW_Prof_Flags_Ex  (NULL)
	,lpCM_Set_HW_Prof_Flags_Ex  (NULL)
	,lpSetupDiGetHwProfileList (NULL)
	,lpgetaddrinfo (NULL)
	,lpgetnameinfo (NULL)
	,lpfreeaddrinfo (NULL)	
{
	HMODULE hMsi = ::LoadLibrary ("msi.dll");
	if( hMsi)
	{
		lpMsiInstallProduct = (PFNMsiInstallProduct)
			GetProcAddress(hMsi, MSIINSTALLPRODUCT);

		lpMsiConfigureProduct = (PFNMsiConfigureProduct)
			GetProcAddress(hMsi, MSICONFIGUREPRODUCT);

		lpMsiQueryComponentState = (PFNMsiQueryComponentState)
			GetProcAddress( hMsi, MSIQUERYCOMPONENTSTATE);
		lpMsiQueryProductState = (PFNMsiQueryProductState)
			GetProcAddress( hMsi, MSIQUERYPRODUCTSTATE);
		lpMsiQueryFeatureState = (PFNMsiQueryFeatureState)
			GetProcAddress(hMsi, MSIQUERYFEATURESTATE);
		lpMsiGetProductInfo = (PFNMsiGetProductInfo)
			GetProcAddress(hMsi, MSIGETPRODUCTINFO);
		lpMsiEnumFeatures = (PFNMsiEnumFeatures)
			GetProcAddress(hMsi, MSIENUMFEATURES);
		lpMsiEnumProducts = (PFNMsiEnumProducts)
			GetProcAddress(hMsi, MSIENUMPRODUCTS);
		lpMsiEnumPatches = (PFNMsiEnumPatches)
			GetProcAddress(hMsi, MSIENUMPATCHES);
		lpMsiDeterminePatchSequence = (PFNMsiDeterminePatchSequence)
			GetProcAddress(hMsi, MSIDETERMINEPATCHSEQUENCE);

	}
	else
	{
		DBGPRINT(DBG_ERROR, "Msi.dll");
	}

	HMODULE hCfgmgr32 = LoadLibrary ("Cfgmgr32.dll");
	if ( hCfgmgr32)
	{
		lpCM_Get_DevNode_Status_Ex = (PFNCM_Get_DevNode_Status_Ex)
			GetProcAddress (hCfgmgr32, "CM_Get_DevNode_Status_Ex");
	}
	else
	{
		DBGPRINT (DBG_ERROR, "Cfgmgr32.dll");
	}

	HMODULE hAdvapi32 = LoadLibrary ("Advapi32.dll");
	if (hAdvapi32)
	{
		lpOpenProcessToken = (PFNOpenProcessToken)
			GetProcAddress(hAdvapi32, "OpenProcessToken"); 
		lpSetTokenInformation = (PFNSetTokenInformation)
			GetProcAddress(hAdvapi32, "SetTokenInformation"); 
		lpImpersonateLoggedOnUser = (PFNImpersonateLoggedOnUser) 
			GetProcAddress(hAdvapi32, "ImpersonateLoggedOnUser"); 
		lpRevertToSelf = (PFNRevertToSelf) 
			GetProcAddress(hAdvapi32, "RevertToSelf"); 
		lpDuplicateTokenEx = (PFNDuplicateTokenEx) 
			GetProcAddress(hAdvapi32, "DuplicateTokenEx"); 
		lpGetSecurityDescriptorDacl = (PFNGetSecurityDescriptorDacl) 
			GetProcAddress(hAdvapi32, "GetSecurityDescriptorDacl");
		lpGetAclInformation = (PFNGetAclInformation) 
			GetProcAddress (hAdvapi32, "GetAclInformation");
		lpGetAce = (PFNGetAce) 
			GetProcAddress (hAdvapi32, "GetAce");
		lpLookupAccountSid = (PFNLookupAccountSid) 
			GetProcAddress (hAdvapi32, "LookupAccountSidA");
		lpGetTokenInformation = (PFNGetTokenInformation)
			GetProcAddress(hAdvapi32, "GetTokenInformation");
		lpChangeServiceConfig2 = (PFNChangeServiceConfig2)
			GetProcAddress(hAdvapi32, "ChangeServiceConfig2A");
		lpGetNamedSecurityInfo = (PFNGetNamedSecurityInfo)
			GetProcAddress(hAdvapi32, "GetNamedSecurityInfoA");
		lpSetEntriesInAcl = (PFNSetEntriesInAcl)
			GetProcAddress(hAdvapi32, "SetEntriesInAclA");
		lpSetNamedSecurityInfo = (PFNSetNamedSecurityInfo)
			GetProcAddress(hAdvapi32, "SetNamedSecurityInfoA");
		lpConvertStringSidToSid = (PFNConvertStringSidToSid)
			GetProcAddress(hAdvapi32, "ConvertStringSidToSidA");
	}
	else
	{
		DBGPRINT (DBG_ERROR, "Advapi32.dll");
	}

	HMODULE hShell32 = LoadLibrary("shell32.dll");
	if (hShell32)
	{
		lpSHGetSpecialFolderPathA = (PFNSHGetSpecialFolderPathA)
			GetProcAddress(hShell32, "SHGetSpecialFolderPathA");

		lpSHGetFolderPath = (PFNSHGetFolderPath)
			GetProcAddress(hShell32, "SHGetFolderPathA");
	}
	else
	{
		DBGPRINT (DBG_ERROR, "shell32.dll");
	}

	HMODULE hShFolder = LoadLibrary("ShFolder.dll");
	if (hShFolder)
	{
		lpSHGetFolderPath = (PFNSHGetFolderPath)
			GetProcAddress(hShFolder, "SHGetFolderPathA");
	}
	else
	{
		DBGPRINT (DBG_ERROR, "shfolder.dll");
	}

	HMODULE hShlwapi = LoadLibrary ("shlwapi.dll");
	if(hShlwapi)
	{
		lpPathAppend = (PFNPathAppend)
			GetProcAddress(hShlwapi, "PathAppendA");
		lpPathFileExists = (PFNPathFileExists)
			GetProcAddress(hShlwapi, "PathFileExistsA");
		lpPathFindOnPath = (PFNPathFindOnPath)
			GetProcAddress(hShlwapi, "PathFindOnPathA");
	}
	else
	{
		DBGPRINT (DBG_ERROR, "shlwapi.dll");
	}

	HMODULE hWtsapi32 = LoadLibrary ("Wtsapi32.dll");
	if (hWtsapi32)
	{
		lpWTSQueryUserToken = (PFNWTSQueryUserToken)
			GetProcAddress (hWtsapi32, "WTSQueryUserToken"); 

		lpWTSRegisterSessionNotification = (PFNWTSRegisterSessionNotification)
			GetProcAddress (hWtsapi32, "WTSRegisterSessionNotification"); 

		lpWTSEnumerateSessions = (PFNWTSEnumerateSessions)
			GetProcAddress(hWtsapi32, "WTSEnumerateSessionsA"); 

		lpWTSQuerySessionInformation = (PFNWTSQuerySessionInformation)
			GetProcAddress(hWtsapi32, "WTSQuerySessionInformationA"); 

		lpWTSFreeMemory = (PFNWTSFreeMemory)
			GetProcAddress(hWtsapi32, "WTSFreeMemory"); 

		lpWTSUnRegisterSessionNotification = (PFNWTSUnRegisterSessionNotification)
			GetProcAddress (hWtsapi32, "WTSUnRegisterSessionNotification");
	}
	else
	{
		DBGPRINT (DBG_ERROR, "Wtsapi32.dll");
	}

	HMODULE hIPhlpapi = LoadLibrary ("Iphlpapi.dll");
	if (hIPhlpapi) 
	{
		lpGetAdaptersInfo = (PFNGetAdaptersInfo) 
			GetProcAddress (hIPhlpapi, "GetAdaptersInfo"); 
		lpGetAdaptersAddresses = (PFNGetAdaptersAddresses) 
			GetProcAddress (hIPhlpapi, "GetAdaptersAddresses"); 
		lpGetNetworkParams = (PFNGetNetworkParams) 
			GetProcAddress (hIPhlpapi, "GetNetworkParams"); 
		lpGetPerAdapterInfo = (PFNGetPerAdapterInfo) 
			GetProcAddress (hIPhlpapi, "GetPerAdapterInfo"); 
		lpGetIfEntry = (PFNGetIfEntry) 
			GetProcAddress (hIPhlpapi, "GetIfEntry"); 
		lpGetIfTable = (PFNGetIfTable) 
			GetProcAddress (hIPhlpapi, "GetIfTable"); 
		lpSendARP = (PFNSendARP) 
			GetProcAddress (hIPhlpapi, "SendARP");
		lpGetIpNetTable = (PFNGetIpNetTable) 
			GetProcAddress (hIPhlpapi, "GetIpNetTable");
		lpDeleteIpNetEntry = (PFNDeleteIpNetEntry) 
			GetProcAddress (hIPhlpapi, "DeleteIpNetEntry");
		lpAllocateAndGetTcpExTableFromStack = (PFNAllocateAndGetTcpExTableFromStack) 
			GetProcAddress (hIPhlpapi, _T ("AllocateAndGetTcpExTableFromStack"));
		lpAllocateAndGetUdpExTableFromStack = (PFNAllocateAndGetUdpExTableFromStack) 
			GetProcAddress (hIPhlpapi, _T ("AllocateAndGetUdpExTableFromStack"));
		lpGetUdpTable = (PFNGetUdpTable) 
			GetProcAddress (hIPhlpapi, _T ("GetUdpTable"));
		lpGetTcpTable = (PFNGetTcpTable) 
			GetProcAddress (hIPhlpapi, _T ("GetTcpTable"));
		lpNotifyAddrChange = (PFNNotifyAddrChange) 
			GetProcAddress (hIPhlpapi, _T ("NotifyAddrChange"));
		lpNotifyRouteChange = (PFNNotifyRouteChange) 
			GetProcAddress (hIPhlpapi, _T ("NotifyRouteChange"));
		// 在win2000上,是从Icmp.dll中导出的
		if ( BaseLib::IsUpWinXp() )
		{
			lpIcmpCreateFile = (PFNIcmpCreateFile) 
				GetProcAddress (hIPhlpapi, _T ("IcmpCreateFile"));
			lpIcmpSendEcho = (PFNIcmpSendEcho) 
				GetProcAddress (hIPhlpapi, _T ("IcmpSendEcho"));
			lpIcmpCloseHandle = (PFNIcmpCloseHandle) 
				GetProcAddress (hIPhlpapi, _T ("IcmpCloseHandle"));
		}
		else
		{
			// 2000 或 2000 以下
			HMODULE hIcmp1 = LoadLibrary ("Icmp.dll");
			if (hIcmp1) 
			{
				lpIcmpCreateFile = (PFNIcmpCreateFile) 
					GetProcAddress (hIcmp1, _T ("IcmpCreateFile"));
				lpIcmpSendEcho = (PFNIcmpSendEcho) 
					GetProcAddress (hIcmp1, _T ("IcmpSendEcho"));
				lpIcmpCloseHandle = (PFNIcmpCloseHandle) 
					GetProcAddress (hIcmp1, _T ("IcmpCloseHandle"));

				//FreeLibrary ( hIcmp1 );
			}
		}
	}
	else
	{
		DBGPRINT (DBG_ERROR, "Iphlpapi.dll");
	}

	if (NULL == lpIcmpCreateFile)
	{
		HMODULE hIcmp = LoadLibrary ("Icmp.dll");
		if (hIcmp) 
		{
			lpIcmpCreateFile = (PFNIcmpCreateFile) 
				GetProcAddress (hIcmp, _T ("IcmpCreateFile"));
			lpIcmpSendEcho = (PFNIcmpSendEcho) 
				GetProcAddress (hIcmp, _T ("IcmpSendEcho"));
			lpIcmpCloseHandle = (PFNIcmpCloseHandle) 
				GetProcAddress (hIcmp, _T ("IcmpCloseHandle"));
		}
	}

	HMODULE hInetmib1 = LoadLibrary ("inetmib1.dll");
	if (hInetmib1) 
	{
		lpSnmpExtensionInit	 = (PFNSnmpExtensionInit)
			GetProcAddress (hInetmib1, "SnmpExtensionInit");
		lpSnmpExtensionQuery = (PFNSnmpExtensionQuery)
			GetProcAddress (hInetmib1, "SnmpExtensionQuery");

		HMODULE hSnmpapi = LoadLibrary ("Snmpapi.dll");
		if (hSnmpapi)
		{
			lpSnmpUtilMemReAlloc = (PFNSnmpUtilMemReAlloc)
				GetProcAddress (hSnmpapi, "SnmpUtilMemReAlloc");
			lpSnmpUtilVarBindListFree = (PFNSnmpUtilVarBindListFree)
				GetProcAddress (hSnmpapi, "SnmpUtilVarBindListFree");
			lpSnmpUtilOidCpy = (PFNSnmpUtilOidCpy)
				GetProcAddress (hSnmpapi, "SnmpUtilOidCpy");
		}
		else
		{
			DBGPRINT (DBG_ERROR, "Snmpapi.dll");
		}

// 		HANDLE hTrapEvent;
// 		AsnObjectIdentifier hIdentifier;
// 
// 		if (!(lpSnmpExtensionInit &&
// 			lpSnmpExtensionInit (GetCurrentTime(), &hTrapEvent, &hIdentifier))) 
// 		{
// 			DBGPRINT (DBG_ERROR, "SnmpExtensionInit");
// 		}
	}
	else
	{
		DBGPRINT (DBG_ERROR, "inetmib1");
	}

	HMODULE hKernel32 = GetModuleHandle ("Kernel32.DLL");
	lpGetSystemTimes = (PFNGetSystemTimes)
		GetProcAddress(hKernel32, "GetSystemTimes");
	lpCreateToolhelpSnapshot = (PFNCreateToolhelpSnapshot) 
		GetProcAddress (hKernel32, "CreateToolhelp32Snapshot");
	lpProcess32First = (PFNProcess32First)
		GetProcAddress (hKernel32, "Process32First");
	lpProcess32Next = (PFNProcess32Next)
		GetProcAddress (hKernel32, "Process32Next");
	lpProcessIdToSessionId = (PFNProcessIdToSessionId)
		GetProcAddress (hKernel32, "ProcessIdToSessionId");
	lpWTSGetActiveConsoleSessionId = (PFNWTSGetActiveConsoleSessionId)
		GetProcAddress (hKernel32, _T("WTSGetActiveConsoleSessionId"));
	lpGetProcessTimes = (PFNGetProcessTimes) 
		GetProcAddress (hKernel32, "GetProcessTimes");
	lpReadProcessMemory = (PFNReadProcessMemory)
		GetProcAddress (hKernel32, "ReadProcessMemory");
	lpGetProcessHandleCount = (PFNGetProcessHandleCount)
		GetProcAddress (hKernel32, "GetProcessHandleCount");
	lpCreateFile = (PFNCreateFile)
		GetProcAddress (hKernel32, "CreateFileA");
	lpGlobalMemoryStatusEx = (PFNGlobalMemoryStatusEx)
		GetProcAddress (hKernel32, "GlobalMemoryStatusEx");
	lpQueryFullProcessImageName = (PFNQueryFullProcessImageName)
		GetProcAddress (hKernel32, "QueryFullProcessImageNameA");

	HMODULE hPsapi = LoadLibrary ("Psapi.dll");
	if (hPsapi)
	{
		lpGetModuleFileNameEx = (PFNGetModuleFileNameEx)
			GetProcAddress (hPsapi, GETMODULEFILENAMEEX);
		lpGetProcessMemoryInfo = (PFNGetProcessMemoryInfo)
			GetProcAddress(hPsapi, "GetProcessMemoryInfo");
		lpGetProcessImageFileName  = (PFNGetProcessImageFileName)
			GetProcAddress(hPsapi, "GetProcessImageFileNameA");
		lpEnumProcessModules = (PFNEnumProcessModules)
			GetProcAddress(hPsapi, "EnumProcessModules");
	}

	HMODULE hNetapi32 = LoadLibrary ("Netapi32.dll");
	if(hNetapi32)
	{
		lpNetUserEnum = (PFNNetUserEnum)
			GetProcAddress (hNetapi32, "NetUserEnum");
		lpNetShareEnum = (PFNNetShareEnum)
			GetProcAddress(hNetapi32, "NetShareEnum");
		lpNetShareDel =	(PFNNetShareDel)
			GetProcAddress(hNetapi32, "NetShareDel");
		lpNetShareGetInfo = (PFNNetShareGetInfo)
			GetProcAddress(hNetapi32, "NetShareGetInfo");
		lpNetUserGetInfo = (PFNNetUserGetInfo)
			GetProcAddress (hNetapi32, "NetUserGetInfo");
		lpNetApiBufferFree = (PFNNetApiBufferFree)
			GetProcAddress(hNetapi32, "NetApiBufferFree");
		lpNetGroupEnum = (PFNNetGroupEnum)
			GetProcAddress (hNetapi32, "NetGroupEnum");
		lpNetGroupGetUsers = (PFNNetGroupGetUsers)
			GetProcAddress (hNetapi32, "NetGroupGetUsers");
		lpNetUserGetGroups = (PFNNetUserGetGroups)
			GetProcAddress (hNetapi32, "NetUserGetGroups");
		lpNetLocalGroupEnum = (PFNNetLocalGroupEnum)
			GetProcAddress (hNetapi32, "NetLocalGroupEnum");
		lpNetLocalGroupGetMembers = (PFNNetLocalGroupGetMembers)
			GetProcAddress (hNetapi32, "NetLocalGroupGetMembers");
		lpNetUserGetLocalGroups = (PFNNetUserGetLocalGroups)
			GetProcAddress (hNetapi32, "NetUserGetLocalGroups");
		lpNetUserAdd = (PFNNetUserAdd)
			GetProcAddress (hNetapi32, "NetUserAdd");
		lpNetUserDel = (PFNNetUserDel)
			GetProcAddress (hNetapi32, "NetUserDel");
		lpNetUserSetInfo = (PFNNetUserSetInfo)
			GetProcAddress (hNetapi32, "NetUserSetInfo");
		
		lpNetApiBufferAllocate = (PFNNetApiBufferAllocate)
			GetProcAddress (hNetapi32, "NetApiBufferAllocate");
		lpNetUserModalsGet = (PFNNetUserModalsGet)
			GetProcAddress (hNetapi32, "NetUserModalsGet");
		lpNetUserModalsSet = (PFNNetUserModalsSet)
			GetProcAddress (hNetapi32, "NetUserModalsSet");
	}
	else
	{
		DBGPRINT (DBG_ERROR, "Netapi32.dll");
	}

	lpWow64DisableWow64FsRedirection = (PFNWow64DisableWow64FsRedirection)
		GetProcAddress (hKernel32, "Wow64DisableWow64FsRedirection");
	lpWow64RevertWow64FsRedirection = (PFNWow64RevertWow64FsRedirection)
		GetProcAddress (hKernel32, "Wow64RevertWow64FsRedirection");
	lpGetSystemWow64Directory = (PFNGetSystemWow64Directory)
		GetProcAddress (hKernel32, "GetSystemWow64DirectoryA");

	HMODULE hWinInet = LoadLibrary("wininet.dll");
	if(hWinInet)
	{
		lpInternetOpen = (PFNInternetOpen)
			GetProcAddress( hWinInet, "InternetOpenA");
		lpInternetOpenUrl = (PFNInternetOpenUrl)
			GetProcAddress( hWinInet, "InternetOpenUrlA");
		lpHttpQueryInfo = (PFNHttpQueryInfo)
			GetProcAddress( hWinInet, "HttpQueryInfoA");
		lpInternetReadFile = (PFNInternetReadFile)
			GetProcAddress( hWinInet, "InternetReadFile");
		lpInternetCloseHandle = (PFNInternetCloseHandle)
			GetProcAddress( hWinInet, "InternetCloseHandle");
		lpInternetSetOption = (PFNInternetSetOption)
			GetProcAddress( hWinInet, "InternetSetOptionA");
	}
	else
	{
		DBGPRINT(DBG_ERROR, "wininet.dll");
	}

	HMODULE hNtdll = LoadLibrary("Ntdll.dll");
	if (hNtdll)
	{
		lpNtQueryInformationProcess = (PFNNtQueryInformationProcess)
			GetProcAddress( hNtdll, "NtQueryInformationProcess");
		lpNtQuerySystemInformation = (PFNNtQuerySystemInformation)
			GetProcAddress( hNtdll, "NtQuerySystemInformation");

		lpNtCreateFile = (PFNNtCreateFile)
			GetProcAddress ( hNtdll,"NtCreateFile" );
		lpNtDeviceIoControlFile = (PFNNtDeviceIoControlFile )
			GetProcAddress ( hNtdll,"NtDeviceIoControlFile" );
		lpNtClose = (PFNNtClose)
			GetProcAddress ( hNtdll,"NtClose" );
	}
	else
	{
		DBGPRINT (DBG_ERROR, "ntdll.dll");
	}
	
	HMODULE hVersiondll = LoadLibrary("version.dll");
	if (hVersiondll)
	{
		lpGetFileVersionInfo = (PFNGetFileVersionInfo)
			GetProcAddress( hVersiondll, "GetFileVersionInfoA");
		lpGetFileVersionInfoSize = (PFNGetFileVersionInfoSize)
			GetProcAddress( hVersiondll, "GetFileVersionInfoSizeA");
		lpVerQueryValue = (PFNVerQueryValue)
			GetProcAddress ( hVersiondll, "VerQueryValueA");
	}
	else
	{
		DBGPRINT (DBG_ERROR, "version.dll");
	}

	HMODULE hSetupapidll = LoadLibrary("setupapi.dll");
	if (hSetupapidll)
	{
		lpCM_Get_Device_ID_List_Ex = (PFNCM_Get_Device_ID_List_Ex)
			GetProcAddress( hSetupapidll, "CM_Get_Device_ID_List_ExW");
		lpCM_Get_HW_Prof_Flags_Ex  = (PFNCM_Get_HW_Prof_Flags_Ex)
			GetProcAddress( hSetupapidll, "CM_Get_HW_Prof_Flags_ExW");
		lpCM_Set_HW_Prof_Flags_Ex  = (PFNCM_Set_HW_Prof_Flags_Ex)
			GetProcAddress( hSetupapidll, "CM_Set_HW_Prof_Flags_ExW");
		lpSetupDiGetHwProfileList = (PFNSetupDiGetHwProfileList)
			GetProcAddress( hSetupapidll, "SetupDiGetHwProfileList");
	}
	else
	{
		DBGPRINT (DBG_ERROR, "setupapi.dll");
	}

	HMODULE hWs2_32dll = LoadLibrary("ws2_32.dll");
	if (hWs2_32dll)
	{
		lpgetaddrinfo = (PFNgetaddrinfo)
			GetProcAddress( hWs2_32dll, "getaddrinfo");
		lpgetnameinfo  = (PFNgetnameinfo)
			GetProcAddress( hWs2_32dll, "getnameinfo");
		lpfreeaddrinfo  = (PFNfreeaddrinfo)
			GetProcAddress( hWs2_32dll, "freeaddrinfo");
	}
	else
	{
		DBGPRINT (DBG_ERROR, "ws2_32.dll");
	}
}
