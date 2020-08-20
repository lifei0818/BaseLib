#ifndef BASELIB_FUNCPOINT_H
#define BASELIB_FUNCPOINT_H

#include "common/sys_incl.h"
#include "common/function.h"
#include <snmp.h>
#include <Tlhelp32.h>
#include <psapi.h>
#include <wininet.h>
#include <winsvc.h>
#include <wtsapi32.h>
#include <Cfgmgr32.h>
#include <Msi.h>
#include <Aclapi.h>

#if (_MSC_VER >= 1400)
#include <Ntsecapi.h>
#endif


#ifndef __in
	#define __in
	#define __out
	#define __inout
	#define __in_bcount(X)
	#define __out_ecount(X)
	#define __deref_out_ecount(X)
	#define __in_opt
#endif

typedef UINT (WINAPI* PFNMsiDeterminePatchSequence)(
	LPCTSTR szProductCode,
	LPCTSTR szUserSid,
	LPVOID dwContext,
	DWORD cPatchInfo,
	LPVOID pPatchInfo
	);

typedef UINT (WINAPI* PFNMsiEnumPatches)(
	IN LPCTSTR szProduct,
	IN DWORD iPatchIndex,
	OUT LPTSTR lpPatchBuf,
	OUT LPTSTR lpTransformsBuf,
	IN OUT DWORD *pcchTransformsBuf
	);


typedef UINT (WINAPI* PFNMsiEnumProducts)(
	 DWORD iProductIndex, // 0-based index into registered products
	 LPTSTR lpProductBuf  // buffer to receive GUID
	 );

typedef UINT (WINAPI* PFNMsiEnumFeatures)(
	 LPCTSTR szProduct,     // product code, string GUID
	 DWORD iFeatureIndex,   // 0-based index into published features
	 LPTSTR lpFeatureBuf,   // buffer for feature
	 LPTSTR lpParentBuf     // buffer for parent of feature
	);


typedef INSTALLSTATE (WINAPI* PFNMsiGetProductInfo)(
    IN LPCTSTR szProduct,   // product code
	IN LPCTSTR szProperty,  // product property
	IN LPTSTR lpValueBuf,   // buffer to return property value
	IN OUT DWORD *pcchValueBuf  // in/out buffer character count, 
       	       				   // including NULL
   );

typedef INSTALLSTATE (WINAPI* PFNMsiInstallProduct)(
	IN LPCTSTR szPackagePath,
	IN LPCTSTR szCommandLine
	);

typedef INSTALLSTATE (WINAPI* PFNMsiConfigureProduct)(
	IN LPCSTR		szProduct,			// product code
	IN int          iInstallLevel,		// how much of the product to install
	IN INSTALLSTATE eInstallState		// local/source/default/absent/lock/uncache
	);

typedef INSTALLSTATE (WINAPI* PFNMsiQueryProductState)(
	IN LPCTSTR szProduct  // product code
	);

typedef INSTALLSTATE (WINAPI* PFNMsiQueryFeatureState)(
	IN	LPCTSTR szProduct,    // product code
	IN  LPCTSTR szFeature     // feature ID
	);

typedef UINT (WINAPI* PFNMsiQueryComponentState)(
	IN LPTSTR szProductCode,
	IN LPTSTR szUserSid,
	IN DWORD dwContent,
	IN LPCTSTR szComponent,
	OUT INSTALLSTATE *pdwState
	);

typedef CMAPI CONFIGRET (WINAPI *PFNCM_Get_DevNode_Status_Ex)(
	OUT PULONG  pulStatus,
	OUT PULONG  pulProblemNumber,
	IN DEVINST  dnDevInst,
	IN ULONG  ulFlags,
	IN HMACHINE  hMachine
    );

typedef HRESULT (WINAPI *PFNSHGetFolderPath)(
		HWND hwndOwner,
		int nFolder,
		HANDLE hToken,
		DWORD dwFlags,
		LPTSTR pszPath
		);

typedef BOOL (WINAPI *PFNGetSystemTimes)(
	   __out         LPFILETIME lpIdleTime,
       __out         LPFILETIME lpKernelTime,
	   __out         LPFILETIME lpUserTime
	   );

typedef BOOL (WINAPI *PFNQueryFullProcessImageName)(
	__in         HANDLE hProcess,
	__in		 DWORD dwFlags,
	__out        LPTSTR lpExeName,
	__inout      PDWORD lpdwSize
	);

typedef BOOL (WINAPI *PFNPathAppend)( 
		LPTSTR pszPath,
		LPCTSTR pszMore
		);

typedef BOOL (WINAPI *PFNPathFileExists)(    
		LPCTSTR pszPath
		);

typedef BOOL (WINAPI *PFNPathFindOnPath)(  
		LPTSTR pszFile,
		LPCTSTR *ppszOtherDirs
		);


typedef BOOL (WINAPI *PFNOpenProcessToken)(
	HANDLE ProcessHandle,
	DWORD DesiredAccess,
	PHANDLE TokenHandle
	);

typedef BOOL (WINAPI *PFNSetTokenInformation)(
	__in HANDLE TokenHandle,
	__in int TokenInformationClass,
	LPVOID TokenInformation,
	__in DWORD TokenInformationLength);

typedef BOOL (WINAPI *PFNImpersonateLoggedOnUser)(
	HANDLE hToken
	);

typedef BOOL (WINAPI *PFNRevertToSelf)(void);

typedef DWORD (WINAPI *PFNWTSGetActiveConsoleSessionId)();

typedef BOOL (WINAPI *PFNWTSQueryUserToken)(
	ULONG SessionId, 
	PHANDLE phToken 
	);

typedef BOOL (WINAPI *PFNDuplicateTokenEx)(
	HANDLE hExistingToken,
	DWORD dwDesiredAccess,
	LPSECURITY_ATTRIBUTES lpTokenAttributes,
	SECURITY_IMPERSONATION_LEVEL ImpersonationLevel,
	TOKEN_TYPE TokenType,
	PHANDLE phNewToken
	);

typedef BOOL (WINAPI *PFNProcessIdToSessionId)(
	DWORD dwProcessId,
	DWORD* pSessionId
	);

typedef BOOL (WINAPI *PFNGetSecurityDescriptorDacl)(
	PSECURITY_DESCRIPTOR pSecurityDescriptor,
	LPBOOL lpbDaclPresent,
	PACL* pDacl,
	LPBOOL lpbDaclDefaulted
	);

typedef BOOL (WINAPI *PFNGetAclInformation)(
	PACL pAcl,
	LPVOID pAclInformation,
	DWORD nAclInformationLength,
	ACL_INFORMATION_CLASS dwAclInformationClass
	);

typedef BOOL (WINAPI *PFNGetAce)(
	PACL pAcl,
	DWORD dwAceIndex,
	LPVOID* pAce
	);

typedef BOOL (WINAPI *PFNLookupAccountSid)(
	LPCTSTR lpSystemName,
	PSID lpSid,
	LPTSTR lpName,
	LPDWORD cchName,
	LPTSTR lpReferencedDomainName,
	LPDWORD cchReferencedDomainName,
	PSID_NAME_USE peUse
	);

typedef DWORD (WINAPI *PFNGetAdaptersInfo)(
	PIP_ADAPTER_INFO pAdapterInfo,
	PULONG pOutBufLen
	);

typedef DWORD (WINAPI *PFNGetAdaptersAddresses)(
	IN		ULONG Family,
	IN		DWORD Flags,
	IN		PVOID Reserved,
	OUT		PIP_ADAPTER_ADDRESSES pAdapterAddresses,
	IN OUT	PULONG pOutBufLen
	);

typedef DWORD (WINAPI *PFNGetPerAdapterInfo)(
	__in   ULONG IfIndex,
	__out  PIP_PER_ADAPTER_INFO pPerAdapterInfo,
	__in   PULONG pOutBufLen
	);

typedef DWORD (WINAPI *PFNGetIfEntry)(
	__inout  PMIB_IFROW pIfRow
);

typedef DWORD (WINAPI *PFNGetIfTable)(
				 __out    PMIB_IFTABLE pIfTable,
				 __inout  PULONG pdwSize,
				 __in     BOOL bOrder
				 );

typedef DWORD (WINAPI *PFNGetNetworkParams)(
	PFIXED_INFO pFixedInfo,
	PULONG pOutBufLen
	);


typedef HANDLE (WINAPI *PFNIcmpCreateFile)(void);

typedef DWORD (WINAPI *PFNIcmpSendEcho)(
	HANDLE IcmpHandle,
	IPAddr DestinationAddress,
	LPVOID RequestData,
	WORD RequestSize,
	PIP_OPTION_INFORMATION RequestOptions,
	LPVOID ReplyBuffer,
	DWORD ReplySize,
	DWORD Timeout
);

typedef BOOL (WINAPI *PFNIcmpCloseHandle)(
	HANDLE IcmpHandle
);

typedef BOOL (WINAPI *PFNSnmpExtensionInit)(
	IN  DWORD               dwTimeZeroReference,
	OUT HANDLE              *hPollForTrapEvent,
	OUT AsnObjectIdentifier *supportedView
	);

typedef BOOL (WINAPI *PFNSnmpExtensionQuery)(
	IN BYTE                   requestType,
	IN OUT RFC1157VarBindList *variableBindings,
	OUT AsnInteger            *errorStatus,
	OUT AsnInteger            *errorIndex
	);

typedef LPVOID (WINAPI *PFNSnmpUtilMemReAlloc)(
	LPVOID	pMem,
	UINT	nBytes
	);

typedef VOID (WINAPI *PFNSnmpUtilVarBindListFree)(
	SnmpVarBindList * pVbl
	);

typedef SNMPAPI (WINAPI *PFNSnmpUtilOidCpy)(
	AsnObjectIdentifier * pOidDst,
	AsnObjectIdentifier * pOidSrc
	);

typedef DWORD (WINAPI *PFNGetIpNetTable)(
	PMIB_IPNETTABLE pIpNetTable,
	PULONG pdwSize, 
	BOOL bOrder
	);

typedef DWORD (WINAPI *PFNDeleteIpNetEntry)(
	PMIB_IPNETROW pArpEntry);

typedef DWORD (WINAPI *PFNSendARP)(
	IPAddr DestIP,
	IPAddr SrcIP,
	PULONG pMacAddr,
	PULONG PhyAddrLen
	);

typedef HANDLE (WINAPI *PFNCreateToolhelpSnapshot)(
	DWORD dwFlags,
	DWORD th32ProcessID
	);

typedef BOOL (WINAPI *PFNProcess32First)(
	HANDLE hSnapshot,
	LPPROCESSENTRY32 lppe
	);

typedef BOOL (WINAPI *PFNProcess32Next)(
	HANDLE hSnapshot,
	LPPROCESSENTRY32 lppe
	);

typedef DWORD (WINAPI *PFNGetModuleFileNameEx)(
	HANDLE hProcess,
	HMODULE hModule,
	LPTSTR lpFilename,
	DWORD nSize
	);

typedef DWORD (WINAPI *PFNGetProcessImageFileName)(
	__in          HANDLE hProcess,
	__out         LPTSTR lpImageFileName,
	__in          DWORD nSize
	);

typedef BOOL (WINAPI *PFNEnumProcessModules)(
				   HANDLE hProcess,
				   HMODULE *lphModule,
				   DWORD cb,
				   LPDWORD lpcbNeeded
				   );

typedef struct {
	DWORD   dwState;        // state of the connection
	DWORD   dwLocalAddr;    // address on local computer
	DWORD   dwLocalPort;    // port number on local computer
	DWORD   dwRemoteAddr;   // address on remote computer
	DWORD   dwRemotePort;   // port number on remote computer
	DWORD	dwProcessId;
	} MIB_TCPEXROW, *PMIB_TCPEXROW;


typedef struct {
	DWORD			dwNumEntries;
	MIB_TCPEXROW	table[ANY_SIZE];
	} MIB_TCPEXTABLE, *PMIB_TCPEXTABLE;

typedef struct {
	DWORD   dwLocalAddr;    // address on local computer
	DWORD   dwLocalPort;    // port number on local computer
	DWORD	dwProcessId;
	} MIB_UDPEXROW, *PMIB_UDPEXROW;

typedef struct {
	DWORD			dwNumEntries;
	MIB_UDPEXROW	table[ANY_SIZE];
	} MIB_UDPEXTABLE, *PMIB_UDPEXTABLE;

/// vista, 2008, win7
typedef struct{
	DWORD dwState;      //连接状态
	DWORD dwLocalAddr;  //本地地址
	DWORD dwLocalPort;  //本地端口
	DWORD dwRemoteAddr; //远程地址
	DWORD dwRemotePort; //远程端口
	DWORD dwProcessId;  //进程标识
	DWORD Unknown;      //待定标识
}MIB_TCPEXROW_VISTA,*PMIB_TCPEXROW_VISTA;

typedef struct{
	DWORD dwNumEntries;
	MIB_TCPEXROW_VISTA table[ANY_SIZE];
}MIB_TCPEXTABLE_VISTA,*PMIB_TCPEXTABLE_VISTA;

typedef DWORD (WINAPI *_InternalGetTcpTable2)(
	PMIB_TCPEXTABLE_VISTA *pTcpTable_Vista,
	HANDLE heap,
	DWORD flags
	);

typedef DWORD (WINAPI *_InternalGetUdpTableWithOwnerPid)(
	PMIB_UDPEXTABLE *pUdpTable,
	HANDLE heap,
	DWORD flags
	);

typedef DWORD (WINAPI *PFNAllocateAndGetTcpExTableFromStack)(
	PMIB_TCPEXTABLE *pTcpTable,  // buffer for the connection table
	BOOL bOrder,               // sort the table?
	HANDLE heap,
	DWORD zero,
	DWORD flags
	);

typedef DWORD (WINAPI *PFNAllocateAndGetUdpExTableFromStack)(
	PMIB_UDPEXTABLE *pTcpTable,  // buffer for the connection table
	BOOL bOrder,               // sort the table?
	HANDLE heap,
	DWORD zero,
	DWORD flags
	);

typedef DWORD (WINAPI *PFNGetUdpTable)(
	PMIB_UDPTABLE pUdpTable,
	PDWORD pdwSize,
	BOOL bOrder
	);

typedef DWORD (WINAPI *PFNGetTcpTable)(
	PMIB_TCPTABLE pTcpTable,
	PDWORD pdwSize,
	BOOL bOrder
	);

typedef DWORD (WINAPI *PFNNotifyAddrChange)(
	PHANDLE Handle,
	LPOVERLAPPED overlapped
	);

typedef DWORD (WINAPI *PFNNotifyRouteChange)(
	PHANDLE Handle,
	LPOVERLAPPED overlapped
	);

typedef NET_API_STATUS (WINAPI * PFNNetUserEnum)(
	LPCWSTR servername,
	DWORD level,
	DWORD filter,
	LPBYTE *bufptr,
	DWORD prefmaxlen,
	LPDWORD entriesread,
	LPDWORD totalentries,
	LPDWORD resume_handle
	);

typedef NET_API_STATUS (WINAPI *PFNNetShareEnum)(
	__in          LPWSTR servername,
	__in          DWORD level,
	__out         LPBYTE* bufptr,
	__in          DWORD prefmaxlen,
	__out         LPDWORD entriesread,
	__out         LPDWORD totalentries,
    __in __out    LPDWORD resume_handle
	);

typedef NET_API_STATUS (WINAPI *PFNNetShareDel)(
	__in          LMSTR servername,
	__in          LMSTR netname,
	__in          DWORD reserved
	);

typedef NET_API_STATUS (WINAPI *PFNNetShareGetInfo)(
	__in          LMSTR servername,
	__in          LMSTR netname,
	__in          DWORD level,
	__out         LPBYTE *bufptr
	);

typedef NET_API_STATUS (WINAPI * PFNNetUserGetInfo)(
	LPCWSTR servername,
	LPCWSTR username,
	DWORD level,
	LPBYTE *bufptr
	);

typedef NET_API_STATUS (WINAPI * PFNNetApiBufferFree)(
	LPVOID Buffer
	);

typedef NET_API_STATUS (WINAPI * PFNNetGroupEnum)(
	LPCWSTR servername,
	DWORD level,
	LPBYTE *bufptr,
	DWORD prefmaxlen,
	LPDWORD entriesread,
	LPDWORD totalentries,
	PDWORD_PTR resume_handle
	);

typedef NET_API_STATUS (WINAPI * PFNNetGroupGetUsers)(
	LPCWSTR servername,
	LPCWSTR groupname,
	DWORD level,
	LPBYTE *bufptr,
	DWORD prefmaxlen,
	LPDWORD entriesread,
	LPDWORD totalentries,
	PDWORD_PTR ResumeHandle
	);

typedef NET_API_STATUS (WINAPI * PFNNetUserGetGroups)(
	LPCWSTR servername,
	LPCWSTR username,
	DWORD level,
	LPBYTE *bufptr,
	DWORD prefmaxlen,
	LPDWORD entriesread,
	LPDWORD totalentries
	);

typedef NET_API_STATUS (WINAPI * PFNNetLocalGroupEnum)(
	LPCWSTR servername,
	DWORD level,
	LPBYTE *bufptr,
	DWORD prefmaxlen,
	LPDWORD entriesread,
	LPDWORD totalentries,
	PDWORD_PTR resumehandle
	);

typedef NET_API_STATUS (WINAPI * PFNNetLocalGroupGetMembers)(
	LPCWSTR servername,
	LPCWSTR localgroupname,
	DWORD level,
	LPBYTE *bufptr,
	DWORD prefmaxlen,
	LPDWORD entriesread,
	LPDWORD totalentries,
	PDWORD_PTR resumehandle
	);

typedef NET_API_STATUS (WINAPI * PFNNetUserGetLocalGroups)(
	LPCWSTR servername,
	LPCWSTR username,
	DWORD level,
	DWORD flags,
	LPBYTE *bufptr,
	DWORD prefmaxlen,
	LPDWORD entriesread,
	LPDWORD totalentries
	);

typedef NET_API_STATUS (WINAPI * PFNNetUserAdd)(
	LPCWSTR servername,
	DWORD level,
	LPBYTE buf,
	LPDWORD parm_err
	);

typedef NET_API_STATUS (WINAPI * PFNNetUserDel)(
	LPCWSTR servername,
	LPCWSTR username
	);


typedef NET_API_STATUS (WINAPI * PFNNetUserSetInfo)(
	LPCWSTR servername,
	LPCWSTR username,
	DWORD level,
	LPBYTE buf,
	LPDWORD parm_err
	);

typedef NET_API_STATUS (WINAPI *PFNNetApiBufferAllocate)(
	DWORD ByteCount,  
	LPVOID *Buffer    
	);

typedef NET_API_STATUS (WINAPI *PFNNetUserModalsGet)(
	LPCWSTR servername,  
	DWORD level,        
	LPBYTE *bufptr      
	);

typedef NET_API_STATUS (WINAPI *PFNNetUserModalsSet)(
	LPCWSTR servername, 
	DWORD level,       
	LPBYTE buf,        
	LPDWORD parm_err   
	);

typedef BOOL (WINAPI *PFNGetTokenInformation)(
	HANDLE TokenHandle,
	TOKEN_INFORMATION_CLASS TokenInformationClass,
	LPVOID TokenInformation,
	DWORD TokenInformationLength,
	PDWORD ReturnLength
	);
/*
typedef VOID (WINAPI *PFNRtlFreeUnicodeString)(
	IN PUNICODE_STRING  UnicodeString
	);
*/

typedef BOOL (WINAPI *PFNWTSRegisterSessionNotification)(
	IN HWND hWnd, 
	IN DWORD dwFlags
	);

typedef BOOL (WINAPI *PFNWTSUnRegisterSessionNotification)(
	IN HWND hWnd
	);

typedef BOOL (WINAPI *PFNWTSEnumerateSessions)(
	IN HANDLE hServer,
	IN DWORD Reserved,
	IN DWORD Version,
	PWTS_SESSION_INFOA * ppSessionInfo,
	__out DWORD * pCount
	);


typedef BOOL (WINAPI *PFNWTSQuerySessionInformation)(
	IN HANDLE hServer,
	IN DWORD SessionId,
	IN WTS_INFO_CLASS WTSInfoClass,
	OUT LPSTR * ppBuffer,
	OUT DWORD * pBytesReturned
	);

typedef BOOL (WINAPI *PFNWTSFreeMemory)(
    IN PVOID pMemory
	);

typedef BOOL (WINAPI *PFNGetProcessHandleCount)(
	 __in          HANDLE hProcess,
	 __in __out    PDWORD pdwHandleCount
	 );


typedef BOOL (WINAPI *PFNGetProcessMemoryInfo)(
	HANDLE Process, 
	PPROCESS_MEMORY_COUNTERS ppsmemCounters, 
	DWORD  cb
	);

typedef BOOL (WINAPI *PFNGetProcessTimes)(
	 __in          HANDLE hProcess,
	 __out         LPFILETIME lpCreationTime,
	 __out         LPFILETIME lpExitTime,
	 __out         LPFILETIME lpKernelTime,
	 __out         LPFILETIME lpUserTime
	 ); 

typedef BOOL (WINAPI *PFNGlobalMemoryStatusEx)(
					 __out LPMEMORYSTATUSEX lpBuffer
					 );

typedef BOOL (WINAPI *PFNReadProcessMemory)(
	__in          HANDLE hProcess,
	__in          LPCVOID lpBaseAddress,
	__out         LPVOID lpBuffer,
	__in          SIZE_T nSize,
	__out         SIZE_T* lpNumberOfBytesRead
	);

#ifndef _WINTERNL_

typedef enum process_info_class{
	ProcessBasicInformation = 0,
	ProcessDebugPort = 7,
	ProcessWow64Information = 26,
	ProcessImageFileName = 27
}PROCESSINFOCLASS,*LPPROCESSINFOCLASS;

typedef enum _SYSTEM_INFORMATION_CLASS {
	SystemBasicInformation,
	SystemProcessorInformation,
	SystemPerformanceInformation,
	SystemTimeOfDayInformation,
	SystemPathInformation,
 	SystemProcessInformation,
 	SystemCallCountInformation,
 	SystemDeviceInformation,
 	SystemProcessorPerformanceInformation,
// 	SystemFlagsInformation,
// 	SystemCallTimeInformation,
 	SystemModuleInformation = 11
// 	SystemLocksInformation,
// 	SystemStackTraceInformation,
// 	SystemPagedPoolInformation,
// 	SystemNonPagedPoolInformation,
// 	SystemHandleInformation,
// 	SystemObjectInformation,
// 	SystemPageFileInformation,
// 	SystemVdmInstemulInformation,
// 	SystemVdmBopInformation,
// 	SystemFileCacheInformation,
// 	SystemPoolTagInformation,
// 	SystemInterruptInformation,
// 	SystemDpcBehaviorInformation,
// 	SystemFullMemoryInformation,
// 	SystemLoadGdiDriverInformation,
// 	SystemUnloadGdiDriverInformation,
// 	SystemTimeAdjustmentInformation,
// 	SystemSummaryMemoryInformation,
// 	SystemNextEventIdInformation,
// 	SystemEventIdsInformation,
// 	SystemCrashDumpInformation,
// 	SystemExceptionInformation,
// 	SystemCrashDumpStateInformation,
// 	SystemKernelDebuggerInformation,
// 	SystemContextSwitchInformation,
// 	SystemRegistryQuotaInformation,
// 	SystemExtendServiceTableInformation,
// 	SystemPrioritySeperation,
// 	SystemPlugPlayBusInformation,
// 	SystemDockInformation,
// 	SystemPowerInformation,
// 	SystemProcessorSpeedInformation,
// 	SystemCurrentTimeZoneInformation,
// 	SystemLookasideInformation
} SYSTEM_INFORMATION_CLASS, *PSYSTEM_INFORMATION_CLASS;

// typedef struct _IO_COUNTERS
// {
// 	LARGE_INTEGER ReadOperationCount;
// 	LARGE_INTEGER WriteOperationCount;
// 	LARGE_INTEGER OtherOperationCount;
// 	LARGE_INTEGER ReadTransferCount;
// 	LARGE_INTEGER WriteTransferCount;
// 	LARGE_INTEGER OtherTransferCount;
// }IO_COUNTERS,*PIO_COUNTERS;

typedef struct _VM_COUNTERS
{
	ULONG PeakVirtualSize;
	ULONG VirtualSize;
	ULONG PageFaultCount;
	ULONG PeakWorkingSetSize;
	ULONG WorkingSetSize;
	ULONG QuotaPeakPagedPoolUsage;
	ULONG QuotaPagedPoolUsage;
	ULONG QuotaPeakNonPagedPoolUsage;
	ULONG QuotaNonPagedPoolUsage;
	ULONG PagefileUsage;
	ULONG PeakPagefileUsage;
}VM_COUNTERS,*PVM_COUNTERS;

typedef struct _CLIENT_ID
{
	HANDLE UniqueProcess;
	HANDLE UniqueThread;
}CLIENT_ID;

#if (_MSC_VER < 1400)

typedef struct _LSA_UNICODE_STRING
{
	USHORT Length;
	USHORT MaximumLength;
	PWSTR Buffer;
}LSA_UNICODE_STRING,*PLSA_UNICODE_STRING;
typedef LSA_UNICODE_STRING UNICODE_STRING, *PUNICODE_STRING;

#endif


typedef enum _KWAIT_REASON
{
	Executive,
	FreePage,
	PageIn,
	PoolAllocation,
	DelayExecution,
	Suspended,
	UserRequest,
	WrExecutive,
	WrFreePage,
	WrPageIn,
	WrPoolAllocation,
	WrDelayExecution,
	WrSuspended,
	WrUserRequest,
	WrEventPair,
	WrQueue,
	WrLpcReceive,
	WrLpcReply,
	WrVertualMemory,
	WrPageOut,
	WrRendezvous,
	Spare2,
	Spare3,
	Spare4,
	Spare5,
	Spare6,
	WrKernel
}KWAIT_REASON;

typedef enum _THREAD_STATE
{
	StateInitialized,
	StateReady,
	StateRunning,
	StateStandby,
	StateTerminated,
	StateWait,
	StateTransition,
	StateUnknown
}THREAD_STATE;

typedef LONG KPRIORITY;

typedef struct _SYSTEM_THREADS
{
	LARGE_INTEGER KernelTime;
	LARGE_INTEGER UserTime;
	LARGE_INTEGER CreateTime;
	ULONG         WaitTime;
	PVOID         StartAddress;
	CLIENT_ID     ClientId;
	KPRIORITY     Priority;
	KPRIORITY     BasePriority;
	ULONG         ContextSwitchCount;
	THREAD_STATE State;
	KWAIT_REASON WaitReason;
}SYSTEM_THREADS,*PSYSTEM_THREADS;

typedef struct _SYSTEM_PROCESS_INFORMATION {
	ULONG    NextEntryOffset;
	ULONG    ThreadCount;
	ULONG    Reserved1[6];
	LARGE_INTEGER CreateTime;
	LARGE_INTEGER UserTime;
	LARGE_INTEGER KernelTime;
	UNICODE_STRING ProcessName;
	KPRIORITY     BasePriority;
	ULONG         ProcessId;
	ULONG         InheritedFromProcessId;
	ULONG         HandleCount;
	ULONG         Reserved2[2];
	VM_COUNTERS   VmCounters;
	IO_COUNTERS   IoCounters;
	SYSTEM_THREADS Threads[1];
} SYSTEM_PROCESS_INFORMATION, *LP_SYSTEM_PROCESS_INFORMATION;

typedef struct
{
	ULONG          AllocationSize;
	ULONG          ActualSize;
	ULONG          Flags;
	ULONG          Unknown1;
	UNICODE_STRING Unknown2;
	HANDLE         InputHandle;
	HANDLE         OutputHandle;
	HANDLE         ErrorHandle;
	UNICODE_STRING CurrentDirectory;
	HANDLE         CurrentDirectoryHandle;
	UNICODE_STRING SearchPaths;
	UNICODE_STRING ApplicationName;
	UNICODE_STRING CommandLine;
	PVOID          EnvironmentBlock;
	ULONG          Unknown[9];
	UNICODE_STRING Unknown3;
	UNICODE_STRING Unknown4;
	UNICODE_STRING Unknown5;
	UNICODE_STRING Unknown6;
} PROCESS_PARAMETERS, *PPROCESS_PARAMETERS;

typedef struct
{
	ULONG               AllocationSize;
	HANDLE              Unknown1;
	HINSTANCE           ProcessHinstance;
	PVOID               ListDlls;
	PPROCESS_PARAMETERS ProcessParameters;
	ULONG               Unknown2;
	HANDLE              Heap;
} PEB, *PPEB;

typedef struct _PROCESS_BASIC_INFORMATION {
	NTSTATUS ExitStatus;
	PPEB PebBaseAddress;
	ULONG_PTR AffinityMask;
	KPRIORITY BasePriority;
	ULONG_PTR UniqueProcessId;
	ULONG_PTR InheritedFromUniqueProcessId;
} PROCESS_BASIC_INFORMATION;
typedef PROCESS_BASIC_INFORMATION *PPROCESS_BASIC_INFORMATION;

typedef struct _OBJECT_ATTRIBUTES {
	ULONG Length;
	HANDLE RootDirectory;
	PUNICODE_STRING ObjectName;
	ULONG Attributes;
	PVOID SecurityDescriptor;        // Points to type SECURITY_DESCRIPTOR
	PVOID SecurityQualityOfService;  // Points to type SECURITY_QUALITY_OF_SERVICE
} OBJECT_ATTRIBUTES;
typedef OBJECT_ATTRIBUTES *POBJECT_ATTRIBUTES;


typedef struct _IO_STATUS_BLOCK {
	LONG Status; // 原先为 NTSTATUS
	ULONG Information;
} IO_STATUS_BLOCK, *PIO_STATUS_BLOCK;


#endif  //_WINTERNL_
///////////////////////////////////////////////////////

typedef LONG (WINAPI *PFNNtQueryInformationProcess)(
	__in          HANDLE ProcessHandle,
	__in          PROCESSINFOCLASS ProcessInformationClass,
	__out         PVOID ProcessInformation,
	__in          ULONG ProcessInformationLength,
	     PULONG ReturnLength
	);

typedef LONG (WINAPI *PFNNtQuerySystemInformation)(
	__in          SYSTEM_INFORMATION_CLASS SystemInformationClass,
    __out         PVOID SystemInformation,
	__in          ULONG SystemInformationLength,
	     PULONG ReturnLength
	);

typedef BOOL (WINAPI *PFNWow64DisableWow64FsRedirection)( __out PVOID *OldValue );
typedef BOOL (WINAPI *PFNWow64RevertWow64FsRedirection)( __in PVOID OlValue );
typedef UINT (WINAPI *PFNGetSystemWow64Directory)(LPSTR lpBuffer,__in UINT uSize);

typedef HINTERNET (WINAPI *PFNInternetOpen)(
    __in          LPCTSTR lpszAgent,
    __in          DWORD dwAccessType,
	__in          LPCTSTR lpszProxyName,
	__in          LPCTSTR lpszProxyBypass,
	__in          DWORD dwFlags
	);

typedef HINTERNET (WINAPI *PFNInternetOpenUrl)(
	__in          HINTERNET hInternet,
	__in          LPCTSTR lpszUrl,
	__in          LPCTSTR lpszHeaders,
	__in          DWORD dwHeadersLength,
	__in          DWORD dwFlags,
	__in          DWORD_PTR dwContext
	);

typedef BOOL (WINAPI *PFNHttpQueryInfo)(
    __in          HINTERNET hRequest,
	__in          DWORD dwInfoLevel,
	__in __out    LPVOID lpvBuffer,
	__in __out    LPDWORD lpdwBufferLength,
	__in __out    LPDWORD lpdwIndex
	);

typedef BOOL (WINAPI *PFNInternetSetOption)(
    __in          HINTERNET hInternet,
	__in          DWORD dwOption,
	__in          LPVOID lpBuffer,
    __in          DWORD dwBufferLength
	);

typedef BOOL (WINAPI *PFNInternetReadFile)(
   __in          HINTERNET hFile,
   __out         LPVOID lpBuffer,
   __in          DWORD dwNumberOfBytesToRead,
   __out         LPDWORD lpdwNumberOfBytesRead
   );

typedef BOOL (WINAPI *PFNInternetCloseHandle)(
   __in          HINTERNET hInternet
   );


typedef HANDLE (WINAPI *PFNCreateFile)(
     __in          LPCTSTR lpFileName,
     __in          DWORD dwDesiredAccess,
     __in          DWORD dwShareMode,
     __in          LPSECURITY_ATTRIBUTES lpSecurityAttributes,
	 __in          DWORD dwCreationDisposition,
	 __in          DWORD dwFlagsAndAttributes,
	 __in          HANDLE hTemplateFile
	 );

typedef BOOL (WINAPI *PFNChangeServiceConfig2)( 
	__in SC_HANDLE hService, 
	__in DWORD dwInfoLevel, 
	LPVOID lpInfo
	);

typedef
VOID
(NTAPI *PIO_APC_ROUTINE) (
						  IN PVOID ApcContext,
						  IN PIO_STATUS_BLOCK IoStatusBlock,
						  IN ULONG Reserved
						  );

typedef LONG (WINAPI * PFNNtCreateFile)(
	OUT PHANDLE FileHandle,
	IN ACCESS_MASK DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes,
	OUT PIO_STATUS_BLOCK IoStatusBlock,
	IN PLARGE_INTEGER AllocationSize OPTIONAL,
	IN ULONG FileAttributes,
	IN ULONG ShareAccess,
	IN ULONG CreateDisposition,
	IN ULONG CreateOptions,
	IN PVOID EaBuffer OPTIONAL,
	IN ULONG EaLength
	);

typedef LONG (WINAPI *PFNNtDeviceIoControlFile) (
	IN HANDLE FileHandle,
	IN HANDLE Event OPTIONAL,
	IN PIO_APC_ROUTINE ApcRoutine OPTIONAL,
	IN PVOID ApcContext OPTIONAL,
	PVOID	IoStatusBlock,
	IN ULONG IoControlCode,
	IN PVOID InputBuffer OPTIONAL,
	IN ULONG InputBufferLength,
	OUT PVOID OutputBuffer OPTIONAL,
	IN ULONG OutputBufferLength
	);

typedef LONG (WINAPI* PFNNtClose) (
									   IN HANDLE Handle
									   );


typedef BOOL (WINAPI* PFNSHGetSpecialFolderPathA) (         
			  HWND hwndOwner,
			  LPSTR lpszPath,
			  int nFolder,
			  BOOL fCreate
			);

typedef DWORD (WINAPI* PFNGetNamedSecurityInfo) (
								  __in          LPTSTR pObjectName,
								  __in          SE_OBJECT_TYPE ObjectType,
								  __in          SECURITY_INFORMATION SecurityInfo,
								       PSID* ppsidOwner,
								       PSID* ppsidGroup,
								       PACL* ppDacl,
								       PACL* ppSacl,
								       PSECURITY_DESCRIPTOR* ppSecurityDescriptor
								  );


typedef DWORD (WINAPI* PFNSetEntriesInAcl) (
							 __in          ULONG cCountOfExplicitEntries,
							      PEXPLICIT_ACCESS pListOfExplicitEntries,
							      PACL OldAcl,
							 __out         PACL* NewAcl
							 );


typedef DWORD (WINAPI* PFNSetNamedSecurityInfo) (
								  __in          LPTSTR pObjectName,
								  __in          SE_OBJECT_TYPE ObjectType,
								  __in          SECURITY_INFORMATION SecurityInfo,
								       PSID psidOwner,
								       PSID psidGroup,
								       PACL pDacl,
								       PACL pSacl
								  );


typedef BOOL (WINAPI* PFNGetFileVersionInfo) ( LPCTSTR lptstrFilename,
						DWORD dwHandle,
						DWORD dwLen,
						LPVOID lpData
						);

typedef DWORD (WINAPI* PFNGetFileVersionInfoSize) ( LPCTSTR lptstrFilename,
							 LPDWORD lpdwHandle
							 );

typedef BOOL (WINAPI* PFNVerQueryValue) ( LPCVOID pBlock,
				   LPCTSTR lpSubBlock,
				   LPVOID *lplpBuffer,
				   PUINT puLen
				   );

typedef CONFIGRET (WINAPI* PFNCM_Get_Device_ID_List_Ex) (
						IN PCWSTR   pszFilter, OPTIONAL
						OUT PWCHAR  Buffer,
						IN ULONG    BufferLen,
						IN ULONG    ulFlags,
						IN HMACHINE hMachine
						);

typedef CONFIGRET (WINAPI* PFNCM_Get_HW_Prof_Flags_Ex) (
						 IN  DEVINSTID_W szDevInstName,
						 IN  ULONG       ulHardwareProfile,
						 OUT PULONG      pulValue,
						 IN  ULONG       ulFlags,
						 IN  HMACHINE    hMachine
						 );

typedef CONFIGRET (WINAPI* PFNCM_Set_HW_Prof_Flags_Ex) (
						 IN DEVINSTID_W szDevInstName,
						 IN ULONG       ulConfig,
						 IN ULONG       ulValue,
						 IN ULONG       ulFlags,
						 IN HMACHINE    hMachine
						 );

typedef BOOL (WINAPI* PFNSetupDiGetHwProfileList) (
	PDWORD HwProfileList,
	__in DWORD HwProfileListSize,
	__out PDWORD RequiredSize,
	 PDWORD CurrentlyActiveIndex
	);

typedef BOOL (WINAPI* PFNConvertStringSidToSid)(
	__in LPCTSTR StringSid,
	__out PSID* Sid
	);

typedef int (WINAPI* PFNgetaddrinfo) (
	IN const char				*nodename,
	IN const char               *servname,
	IN const struct addrinfo    *hints,
	OUT struct addrinfo         **res);


typedef int (WINAPI* PFNgetnameinfo) (
	IN  const struct sockaddr	*sa,
	IN  socklen_t               salen,
	OUT char                    *host,
	IN  size_t                  hostlen,
	OUT char                    *serv,
	IN  size_t                  servlen,
	IN  int                     flags);

typedef int (WINAPI* PFNgetnameinfo) (
									  IN  const struct sockaddr	*sa,
									  IN  socklen_t               salen,
									  OUT char                    *host,
									  IN  size_t                  hostlen,
									  OUT char                    *serv,
									  IN  size_t                  servlen,
									  IN  int                     flags);


typedef void (WINAPI* PFNfreeaddrinfo) (IN  struct addrinfo *ai);

class CFunctionPoint
{
public:

	friend class BaseLib::TSingleton<CFunctionPoint>;
	PFNMsiDeterminePatchSequence    lpMsiDeterminePatchSequence;
	PFNMsiEnumPatches               lpMsiEnumPatches;
	PFNMsiEnumProducts              lpMsiEnumProducts;
	PFNMsiEnumFeatures              lpMsiEnumFeatures;
	PFNMsiGetProductInfo            lpMsiGetProductInfo;
	PFNMsiQueryProductState         lpMsiQueryProductState; 
	PFNMsiQueryFeatureState         lpMsiQueryFeatureState;
	PFNMsiQueryComponentState       lpMsiQueryComponentState;

	PFNMsiInstallProduct            lpMsiInstallProduct;
	PFNMsiConfigureProduct			lpMsiConfigureProduct;
	

	PFNCM_Get_DevNode_Status_Ex     lpCM_Get_DevNode_Status_Ex;

	PFNSHGetFolderPath              lpSHGetFolderPath;
	PFNSHGetSpecialFolderPathA      lpSHGetSpecialFolderPathA;
	PFNPathAppend                   lpPathAppend;
	PFNPathFileExists               lpPathFileExists;
	PFNPathFindOnPath               lpPathFindOnPath;
	PFNGetSystemTimes               lpGetSystemTimes;
	PFNQueryFullProcessImageName    lpQueryFullProcessImageName;

	PFNOpenProcessToken				lpOpenProcessToken;
	PFNSetTokenInformation			lpSetTokenInformation;
	PFNImpersonateLoggedOnUser		lpImpersonateLoggedOnUser;
	PFNRevertToSelf					lpRevertToSelf;
	PFNWTSGetActiveConsoleSessionId	lpWTSGetActiveConsoleSessionId;
	PFNWTSQueryUserToken			lpWTSQueryUserToken;
	PFNDuplicateTokenEx				lpDuplicateTokenEx;
	PFNProcessIdToSessionId			lpProcessIdToSessionId;
	PFNGetSecurityDescriptorDacl	lpGetSecurityDescriptorDacl;
	PFNGetAclInformation			lpGetAclInformation;
	PFNGetAce						lpGetAce;
	PFNLookupAccountSid				lpLookupAccountSid;
	PFNChangeServiceConfig2			lpChangeServiceConfig2;

	PFNGetAdaptersInfo				lpGetAdaptersInfo;
	PFNGetAdaptersAddresses			lpGetAdaptersAddresses;
	PFNGetPerAdapterInfo			lpGetPerAdapterInfo;
	PFNGetNetworkParams				lpGetNetworkParams;
	PFNGetIfEntry					lpGetIfEntry;
	PFNGetIfTable					lpGetIfTable;

	PFNSnmpExtensionInit			lpSnmpExtensionInit; 
	PFNSnmpExtensionQuery			lpSnmpExtensionQuery;
	PFNSnmpUtilMemReAlloc			lpSnmpUtilMemReAlloc;
	PFNSnmpUtilVarBindListFree		lpSnmpUtilVarBindListFree;
	PFNSnmpUtilOidCpy				lpSnmpUtilOidCpy;

	PFNGetIpNetTable				lpGetIpNetTable;
	PFNDeleteIpNetEntry				lpDeleteIpNetEntry;
	PFNSendARP						lpSendARP;
	PFNAllocateAndGetTcpExTableFromStack lpAllocateAndGetTcpExTableFromStack;
	PFNAllocateAndGetUdpExTableFromStack lpAllocateAndGetUdpExTableFromStack;
	PFNGetUdpTable					lpGetUdpTable;
	PFNGetTcpTable					lpGetTcpTable;
	PFNNotifyAddrChange				lpNotifyAddrChange;
	PFNNotifyRouteChange			lpNotifyRouteChange;
	PFNIcmpCreateFile				lpIcmpCreateFile;
	PFNIcmpSendEcho					lpIcmpSendEcho;
	PFNIcmpCloseHandle				lpIcmpCloseHandle;

	PFNCreateToolhelpSnapshot		lpCreateToolhelpSnapshot;
	PFNProcess32First				lpProcess32First;
	PFNProcess32Next				lpProcess32Next;
	PFNGetModuleFileNameEx			lpGetModuleFileNameEx;
	PFNGetProcessImageFileName      lpGetProcessImageFileName;
	PFNEnumProcessModules			lpEnumProcessModules;

	PFNNetUserEnum					lpNetUserEnum;
	PFNNetShareEnum                 lpNetShareEnum;
	PFNNetShareDel                  lpNetShareDel;
	PFNNetShareGetInfo              lpNetShareGetInfo;
	PFNNetUserGetInfo				lpNetUserGetInfo;
	PFNNetApiBufferFree				lpNetApiBufferFree;
	PFNNetGroupEnum					lpNetGroupEnum;
	PFNNetGroupGetUsers				lpNetGroupGetUsers;
	PFNNetUserGetGroups				lpNetUserGetGroups;
	PFNNetLocalGroupEnum			lpNetLocalGroupEnum;
	PFNNetLocalGroupGetMembers		lpNetLocalGroupGetMembers;
	PFNNetUserGetLocalGroups		lpNetUserGetLocalGroups;
	PFNNetUserAdd					lpNetUserAdd;
	PFNNetUserDel                   lpNetUserDel;
	PFNNetUserSetInfo				lpNetUserSetInfo;
	PFNNetApiBufferAllocate			lpNetApiBufferAllocate;
	PFNNetUserModalsGet				lpNetUserModalsGet;
	PFNNetUserModalsSet				lpNetUserModalsSet;

	PFNGetProcessHandleCount        lpGetProcessHandleCount;
	PFNGetTokenInformation			lpGetTokenInformation;
	PFNGetProcessMemoryInfo			lpGetProcessMemoryInfo;
	PFNGetProcessTimes				lpGetProcessTimes;
	PFNReadProcessMemory            lpReadProcessMemory;
	PFNCreateFile                   lpCreateFile;
	PFNGlobalMemoryStatusEx         lpGlobalMemoryStatusEx;

	PFNNtQueryInformationProcess    lpNtQueryInformationProcess;
	PFNNtQuerySystemInformation     lpNtQuerySystemInformation;
//	PFNRtlConvertSidToUnicodeString	lpRtlConvertSidToUnicodeString;
//	PFNRtlFreeUnicodeString			lpRtlFreeUnicodeString;

	PFNWTSRegisterSessionNotification	lpWTSRegisterSessionNotification;
 	PFNWTSUnRegisterSessionNotification	lpWTSUnRegisterSessionNotification;
	PFNWTSEnumerateSessions				lpWTSEnumerateSessions;
	PFNConvertStringSidToSid            lpConvertStringSidToSid;

	PFNWTSQuerySessionInformation		lpWTSQuerySessionInformation;
	PFNWTSFreeMemory					lpWTSFreeMemory;


	PFNWow64DisableWow64FsRedirection	lpWow64DisableWow64FsRedirection;
	PFNWow64RevertWow64FsRedirection	lpWow64RevertWow64FsRedirection;
	PFNGetSystemWow64Directory			lpGetSystemWow64Directory;

	PFNInternetOpen                 lpInternetOpen;
	PFNInternetOpenUrl              lpInternetOpenUrl;
	PFNHttpQueryInfo                lpHttpQueryInfo; 
	PFNInternetSetOption            lpInternetSetOption;
	PFNInternetReadFile             lpInternetReadFile; 
	PFNInternetCloseHandle          lpInternetCloseHandle;

	PFNNtCreateFile				    lpNtCreateFile;
	PFNNtDeviceIoControlFile	    lpNtDeviceIoControlFile;
	PFNNtClose					    lpNtClose;

	PFNGetNamedSecurityInfo         lpGetNamedSecurityInfo;
	PFNSetEntriesInAcl              lpSetEntriesInAcl;
	PFNSetNamedSecurityInfo         lpSetNamedSecurityInfo;

	PFNGetFileVersionInfo           lpGetFileVersionInfo;
	PFNGetFileVersionInfoSize       lpGetFileVersionInfoSize;
	PFNVerQueryValue                lpVerQueryValue;

	PFNCM_Get_Device_ID_List_Ex		lpCM_Get_Device_ID_List_Ex;
	PFNCM_Get_HW_Prof_Flags_Ex		lpCM_Get_HW_Prof_Flags_Ex;
	PFNCM_Set_HW_Prof_Flags_Ex		lpCM_Set_HW_Prof_Flags_Ex;

	PFNSetupDiGetHwProfileList		lpSetupDiGetHwProfileList;

	PFNgetaddrinfo					lpgetaddrinfo;
	PFNgetnameinfo					lpgetnameinfo;
	PFNfreeaddrinfo					lpfreeaddrinfo;

private:
	CFunctionPoint ();
};


#endif
