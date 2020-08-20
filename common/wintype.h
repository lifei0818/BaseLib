#ifndef BaseLib_WINTYPE_H
#define BaseLib_WINTYPE_H

typedef int  BOOL;

typedef char CHAR, TCHAR, *LPSTR, *PSTR, *LPTSTR;
typedef char *PCHAR;
typedef const char *LPCTSTR, *LPCSTR, *PCSTR;
typedef unsigned char UCHAR;
typedef unsigned char BYTE, *LPBYTE;

typedef wchar_t WCHAR, *LPWSTR;
typedef const wchar_t * LPCWSTR;


typedef short SHORT;
typedef unsigned short USHORT, WORD;
typedef USHORT *PUSHORT;

typedef int INT, INT32;
typedef int *LPINT;
typedef int *PINT;
typedef long HANDLE;
typedef int  SOCKET;
typedef unsigned int UINT, UINT32, *LPUINT, *LPUINT32;
typedef unsigned short UINT16, *PUINT16;

typedef unsigned int DWORD;

typedef long LONG;
typedef unsigned long ULONG;
typedef unsigned long ULONG_PTR, *PULONG_PTR;
typedef ULONG *PULONG;
typedef DWORD *PDWORD, *LPDWORD, *PDWORD;

typedef long long    LONGLONG, __int64;
typedef unsigned long long    UINT64, ULONG64, DWORDLONG, ULONGLONG;

typedef float FLOAT;
typedef double DOUBLE;

typedef void *LPVOID;
typedef void *HMODULE;

typedef BYTE  BOOLEAN; 
typedef ULONG_PTR KSPIN_LOCK;
typedef KSPIN_LOCK *PKSPIN_LOCK;

typedef HANDLE SC_HANDLE;

#define FORCEINLINE __inline


#ifndef FALSE
    #define FALSE  0
#endif

#ifndef TRUE
    #define TRUE   1
#endif

#ifndef VOID
    #define VOID void
#endif
    
#ifndef _T
    #define _T(x)   x
#endif

#ifndef IN
    #define IN
#endif

#ifndef OUT
    #define OUT
#endif

#ifndef __stdcall
    #define __stdcall
#endif

#ifndef WINAPI
    #define WINAPI
#endif

#ifndef APIENTRY
    #define APIENTRY
#endif

#ifndef MAX_PATH
    #define MAX_PATH        260
#endif    

#ifndef INVALID_HANDLE_VALUE
    #define INVALID_HANDLE_VALUE (-1)
#endif

#ifndef NO_ERROR
    #define NO_ERROR    0
#endif


typedef struct _LIST_ENTRY {
   struct _LIST_ENTRY *Flink;
   struct _LIST_ENTRY *Blink;
} LIST_ENTRY;

typedef struct _LIST_ENTRY  *PLIST_ENTRY;

typedef struct _SYSTEMTIME 
{
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
} SYSTEMTIME, *PSYSTEMTIME, *LPSYSTEMTIME;

typedef struct _FILETIME 
{
    DWORD dwLowDateTime;
    DWORD dwHighDateTime;
} FILETIME, *PFILETIME, *LPFILETIME;

#define Int32x32To64(a, b)  ((__int64)(((__int64)((long)(a))) * ((long)(b))))

typedef union _LARGE_INTEGER 
{
    struct 
    {
        DWORD LowPart;
        LONG HighPart;
    } DUMMYSTRUCTNAME;
    struct 
    {
        DWORD LowPart;
        LONG HighPart;
    } u;
    LONGLONG QuadPart;
} LARGE_INTEGER;

#define CONTAINING_RECORD(address, type, field) ((type *)( \
                                                  (PCHAR)(address) - \
                                                  (ULONG_PTR)(&((type *)0)->field)))


//winerror:
#define ERROR_SUCCESS                    0L
#define ERROR_INVALID_HANDLE             6L
#define ERROR_INVALID_PARAMETER          87L
#define ERROR_BUFFER_OVERFLOW            111L
#define ERROR_INSUFFICIENT_BUFFER        122L
#define ERROR_MORE_DATA                  234L
#define ERROR_NOT_FOUND                  1168L
#define ERROR_CONNECTION_REFUSED         1225L
#define ERROR_UNKNOWN_REVISION           1305L
/*
 * ShowWindow() Commands
 */
#define SW_HIDE             0
#define SW_SHOWNORMAL       1
#define SW_NORMAL           1
#define SW_SHOWMINIMIZED    2
#define SW_SHOWMAXIMIZED    3
#define SW_MAXIMIZE         3
#define SW_SHOWNOACTIVATE   4
#define SW_SHOW             5
#define SW_MINIMIZE         6
#define SW_SHOWMINNOACTIVE  7
#define SW_SHOWNA           8
#define SW_RESTORE          9
#define SW_SHOWDEFAULT      10
#define SW_FORCEMINIMIZE    11
#define SW_MAX              11


//
// dwCreationFlag values
//

#define DEBUG_PROCESS                     0x00000001
#define DEBUG_ONLY_THIS_PROCESS           0x00000002

#define CREATE_SUSPENDED                  0x00000004

#define DETACHED_PROCESS                  0x00000008

#define CREATE_NEW_CONSOLE                0x00000010

#define NORMAL_PRIORITY_CLASS             0x00000020
#define IDLE_PRIORITY_CLASS               0x00000040
#define HIGH_PRIORITY_CLASS               0x00000080
#define REALTIME_PRIORITY_CLASS           0x00000100

#define CREATE_NEW_PROCESS_GROUP          0x00000200
#define CREATE_UNICODE_ENVIRONMENT        0x00000400

#define CREATE_SEPARATE_WOW_VDM           0x00000800
#define CREATE_SHARED_WOW_VDM             0x00001000
#define CREATE_FORCEDOS                   0x00002000

#define BELOW_NORMAL_PRIORITY_CLASS       0x00004000
#define ABOVE_NORMAL_PRIORITY_CLASS       0x00008000

#define STACK_SIZE_PARAM_IS_A_RESERVATION 0x00010000    // Threads only

#define INHERIT_CALLER_PRIORITY           0x00020000
#define CREATE_PROTECTED_PROCESS          0x00040000
#define EXTENDED_STARTUPINFO_PRESENT      0x00080000

#define PROCESS_MODE_BACKGROUND_BEGIN     0x00100000
#define PROCESS_MODE_BACKGROUND_END       0x00200000

#define CREATE_BREAKAWAY_FROM_JOB         0x01000000
#define CREATE_PRESERVE_CODE_AUTHZ_LEVEL  0x02000000

#define CREATE_DEFAULT_ERROR_MODE         0x04000000

#define CREATE_NO_WINDOW                  0x08000000

#define PROFILE_USER                      0x10000000
#define PROFILE_KERNEL                    0x20000000
#define PROFILE_SERVER                    0x40000000

#define CREATE_IGNORE_SYSTEM_DEFAULT      0x80000000


#define INFINITE            0xFFFFFFFF  // Infinite timeout

//
// Service State -- for CurrentState
//
#define SERVICE_STOPPED                        0x00000001
#define SERVICE_START_PENDING                  0x00000002
#define SERVICE_STOP_PENDING                   0x00000003
#define SERVICE_RUNNING                        0x00000004
#define SERVICE_CONTINUE_PENDING               0x00000005
#define SERVICE_PAUSE_PENDING                  0x00000006
#define SERVICE_PAUSED                         0x00000007


#define EWX_SHUTDOWN        0x00000001

#define _tcscpy                 strcpy
#define _tcscat                 strcat
#define _stscanf                sscanf
#define _stricmp                strcmp
#define _tcsicmp                strcasecmp
#define _tcsnicmp               strncasecmp
#define stricmp                 strcasecmp
#define strnicmp                strncasecmp
#define _swab                   swab
#define _tcsstr                 strstr
#define _tcsrchr                strrchr
#define _taccess                access
#define localtime_s(x, y)       localtime_r((y), (x))
#define OutputDebugString(x)    //fprintf(stderr, x)
#define OutputDebugStringA      OutputDebugString
#define GetProcAddress          dlsym

#define DLL_PROCESS_ATTACH   1
#define DLL_THREAD_ATTACH    2
#define DLL_THREAD_DETACH    3
#define DLL_PROCESS_DETACH   0
#define DLL_PROCESS_VERIFIER 4

BOOL CloseServiceHandle(SC_HANDLE   hSCObject);

inline 
char *strcpy_s(char *dest, size_t size, const char *src)
{
    return strncpy(dest, src, size);
}

template <size_t _size>
inline
char *strcpy_s(char (&dest)[_size], const char *src)
{
    return strcpy_s(dest, _size, src);
}

inline 
int sprintf_s(char *str, size_t size, const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    int ret = vsnprintf(str, size, format, ap);
    va_end(ap);
    return ret;
}

template <size_t _size>
inline
int sprintf_s(char (&str)[_size], const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    int ret = vsnprintf(str, _size, format, ap);
    va_end(ap);
    return ret;
}

inline
void *LoadLibrary(LPCSTR lpDllName)
{
    if (!lpDllName || !(*lpDllName))
    {
        return NULL;
    }
    std::string strDllName = lpDllName;
	size_t nPos = strDllName.find_last_of('.');
	if (nPos != std::string::npos)
	{
		std::string strExtName = strDllName.substr(nPos + 1);
		std::transform(strExtName.begin(), strExtName.end(), strExtName.begin(), ::tolower);
		// 扩展名是.dll的替换成.so
		if (strExtName.compare("dll") == 0)
		{
    		strDllName.replace(nPos + 1, strExtName.size(), "so");
		}
	}

	std::string strFullDllName;
	// 不带路径的话，取模块所在目录
	if (strDllName.find_last_of('/') == std::string::npos)
	{
	    CHAR chPath [MAX_PATH] = {0};
	    memset(chPath, 0, sizeof(chPath));
        ::readlink("/proc/self/exe", chPath, sizeof (chPath));
        ::dirname(chPath);
    	strFullDllName = chPath;
    	strFullDllName += "/" + strDllName;
    }
    else
    {
	    strFullDllName = strDllName;
    }
    void *handle = NULL;
    handle = dlopen(strFullDllName.c_str(), RTLD_NOW|RTLD_LOCAL|RTLD_NOLOAD);
    if (handle)
    {
        return handle;
    }
    else
    {
        handle = dlopen(strFullDllName.c_str(), RTLD_NOW|RTLD_LOCAL);
        if (handle)
        {
            BOOL (*func)(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved);
            func = (BOOL (*)(HMODULE , DWORD , LPVOID))dlsym(handle, "DllMain");
            if (func)
            {
                func(NULL, DLL_PROCESS_ATTACH, NULL);
            }
        }
    }
    return handle;
}

inline
int FreeLibrary(void *handle)
{
    if (!handle)
    {
        return 0;
    }
    return dlclose(handle);
}

inline
void GetLocalTime(LPSYSTEMTIME lpSystemTime)
{
    struct timeval tv;
    gettimeofday (&tv , NULL);

    tm tm_now;
    localtime_r(&tv.tv_sec,&tm_now); 

    lpSystemTime->wYear = tm_now.tm_year + 1900;          //年份
    lpSystemTime->wMonth = tm_now.tm_mon + 1;             //月 tm[0-11] sys[1-12]
    lpSystemTime->wDay = tm_now.tm_mday;                  //日
    lpSystemTime->wDayOfWeek = (tm_now.tm_wday + 1) % 7 ; //tm一星期的日数，从星期一算起，范围为0-6 sys从星期日算起
    lpSystemTime->wHour =tm_now.tm_hour;                  //小时
    lpSystemTime->wMinute = tm_now.tm_min;                //分钟
    lpSystemTime->wSecond = tm_now.tm_sec;                //秒
    lpSystemTime->wMilliseconds = tv.tv_usec/1000;        //毫秒
} 

#endif
