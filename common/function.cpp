#include "common/sys_incl.h"
#include "common/debug.h"
#include "common/function.h"
//#include "common/MacroDefine.h"
#include "common/tolists.h"
#include "common/stdfile.h"
#include "common/os.h"
#include "common/ace.h"

using namespace std;
namespace BaseLib
{
/************************************************************************/
/*
/************************************************************************/
ULONG
GetCurrentThreadID()
{
#ifdef _MACRO_OS_WIN32
	return ::GetCurrentThreadId();
#else
    return ::pthread_self();
#endif
}

DWORD
GetTickCount()
{
#ifdef _MACRO_OS_WIN32
	return ::GetTickCount();
#else
/*
    struct timeval tv;
    struct tm _tm;

    //Linux不能获取到从开机时到当前的时间，改为取当天0点以来的时间
    ::gettimeofday(&tv, NULL);
    ::gmtime_r(&tv.tv_sec, &_tm);
    _tm.tm_hour  = 0;
    _tm.tm_min   = 0;
    _tm.tm_sec   = 0;
    tv.tv_sec -= ::mktime(&_tm);

    return tv.tv_sec * 1000L + tv.tv_usec / 1000;
*/
    struct sysinfo info;
    if (sysinfo(&info))
    {
        return 0;
    }
    return  info.uptime * 1000;
#endif
}


static DWORDLONG dlTick = 0;
static CThreadMutex TickCountLock;

DWORDLONG
GetTickCount64()
{
#ifdef _MACRO_OS_WIN32
	DWORD dwTick = GetTickCount();
	BASELIB_LOCK(TickCountLock);
	if (*((DWORD*)&dlTick) > dwTick)
	{
		dlTick += 0x0100000000;
	}
	*((DWORD*)&dlTick) = dwTick;
#else
	struct timeval tv;
	gettimeofday(&tv, NULL);
	BASELIB_LOCK(TickCountLock);
	dlTick = tv.tv_sec * 1000LL + tv.tv_usec / 1000;
#endif

	return dlTick;
}


#ifdef _MACRO_OS_WIN32

HKEY
CxxParseRootKey (LPCTSTR lpName,
				 LPCTSTR *lpNext)
{
	if (!lpName || NULL == *lpName) return NULL;

	size_t	nSize;
	LPCTSTR	lpNextEx = _tcschr (lpName, '\\');

	if (lpNextEx)
	{
		nSize = lpNextEx - lpName;
		lpNextEx++;
		if (lpNext) *lpNext = lpNextEx;
	}
	else
	{
		nSize = _tcslen (lpName);
	}

	if (_strnicmp ("HKEY_CLASSES_ROOT", lpName, nSize) == 0)
	{
		return HKEY_CLASSES_ROOT;
	}
	else if (_strnicmp ("HKEY_CURRENT_USER", lpName, nSize) == 0)
	{
		return HKEY_CURRENT_USER;
	}
	else if (_strnicmp ("HKEY_LOCAL_MACHINE", lpName, nSize) == 0)
	{
		return HKEY_LOCAL_MACHINE;
	}
	else if (_strnicmp ("HKEY_USERS", lpName, nSize) == 0)
	{
		return HKEY_USERS;
	}
	else if (_strnicmp ("HKEY_CURRENT_CONFIG", lpName, nSize) == 0)
	{
		return HKEY_CURRENT_CONFIG;
	}

	return NULL;
}

/// 查看 子键是否存在
BOOL
CxxSubKeyIsExist (IN HKEY hKey,
				  IN LPCSTR lpSubKey,
				  IN BOOL bEnableRedirection)
{
	if ( NULL == hKey ||
		!lpSubKey )
	{
		return FALSE;
	}

	long		rc;
	HKEY		hSubKey;
	REGSAM		samDesired = KEY_READ;

	if (bEnableRedirection == FALSE && IsWow64())
	{
		samDesired |= KEY_WOW64_64KEY;
	}
	rc = RegOpenKeyEx (hKey,
		lpSubKey,
		0,
		samDesired,
		&hSubKey);

	if (ERROR_SUCCESS != rc) return FALSE;

	RegCloseKey (hSubKey);

	return TRUE;
}

BOOL
CxxRegGetValue (HKEY hKey,
				LPCSTR lpSubKey,
				LPCSTR lpValueName,
				LPDWORD lpType,
				LPBYTE lpData,
				LPDWORD lpcbData,
				BOOL bEnableRedirection)
{
	if (NULL == hKey ||
		!lpSubKey || NULL == *lpSubKey ||
		!lpData ||
		!lpcbData)
	{
		return FALSE;
	}

	long		rc;
	HKEY		hSubKey;
	REGSAM		samDesired = KEY_READ;

	if (bEnableRedirection == FALSE && IsWow64())
	{
		samDesired |= KEY_WOW64_64KEY;
	}
	rc = RegOpenKeyEx (hKey,
		lpSubKey,
		0,
		samDesired,
		&hSubKey);

	if (ERROR_SUCCESS != rc) return FALSE;

	DWORD dwSize = *lpcbData;
	rc = RegQueryValueEx (hSubKey,
		lpValueName,
		0,
		lpType,
		lpData,
		lpcbData);

	RegCloseKey (hSubKey);

	if (ERROR_SUCCESS != rc) return FALSE;


	if (lpType && REG_MULTI_SZ == *lpType && (*lpcbData))
	{
		for (DWORD i = 0;
			i < ((*lpcbData) - 1) && i < dwSize;
			i++)
		{
			if (lpData [i] == 0)
			{
				lpData [i] = ' ';
			}
		}
	}

	return TRUE;
}

/// 查看 键值是否存在
BOOL
CxxRegValueIsExist (IN HKEY hKey,
				  IN LPCSTR lpSubKey,
				  LPCSTR lpValueName,
				  IN BOOL bEnableRedirection)
{
	BOOL  rc;
	DWORD dwType;
	TCHAR szBuffer[PAGE_SIZE] = {0};
	DWORD dwSize = sizeof(szBuffer);

	rc = CxxRegGetValue(hKey,
						lpSubKey,
						lpValueName,
						&dwType,
						(LPBYTE)szBuffer,
						&dwSize,
						bEnableRedirection);
	return rc;
}

BOOL
CxxRegDeleteValue (HKEY hKey,
				LPCSTR lpSubKey,
				LPCSTR lpValueName,
				BOOL bEnableRedirection)
{
	if (NULL == hKey ||
		!lpSubKey || NULL == *lpSubKey)
	{
		return FALSE;
	}

	long		rc;
	HKEY		hSubKey;
	REGSAM		samDesired = KEY_ALL_ACCESS;

	if (bEnableRedirection == FALSE && IsWow64())
	{
		samDesired |= KEY_WOW64_64KEY;
	}
	rc = RegOpenKeyEx (hKey,
		lpSubKey,
		0,
		samDesired,
		&hSubKey);

	if (ERROR_SUCCESS != rc)
	{
		return FALSE;
	}

	rc = RegDeleteValue (hSubKey, lpValueName);

	RegCloseKey (hSubKey);

	if (ERROR_SUCCESS != rc)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL
CxxRegCreate (HKEY	hKey,
			  LPCSTR lpSubKey,
			  BOOL bEnableRedirection)
{
	if (hKey == NULL) return FALSE;

	std::string szSubKey = lpSubKey;
	size_t nPos = szSubKey.rfind('\\');
	if (std::string::npos != nPos &&
		!CxxRegCreate(hKey, szSubKey.substr(0, nPos).c_str(), bEnableRedirection))
	{
		return FALSE;
	}

	HKEY hSubKey;
	REGSAM samDesired = KEY_ALL_ACCESS;

	if (bEnableRedirection == FALSE && IsWow64())
	{
		samDesired |= KEY_WOW64_64KEY;
	}
	long rc = RegCreateKeyEx (hKey,
		lpSubKey,
		0,
		REG_NONE,
		REG_OPTION_NON_VOLATILE,
		samDesired,
		NULL,
		&hSubKey,
		NULL);

	if (ERROR_SUCCESS != rc) return FALSE;

	RegCloseKey (hSubKey);
	return TRUE;
}

BOOL
CxxRegSetValue (HKEY hKey,
				LPCSTR lpSubKey,
				LPCSTR lpValueName,
				DWORD dwType,
				LPBYTE lpData,
				DWORD dwSize,
				BOOL bEnableRedirection)
{
	long		rc;
	HKEY		hSubKey;
	REGSAM		samDesired = KEY_SET_VALUE;

	if (bEnableRedirection == FALSE && IsWow64())
	{
		samDesired |= KEY_WOW64_64KEY;
	}
	rc = RegOpenKeyEx (hKey,
		lpSubKey,
		0,
		samDesired,
		&hSubKey);

	if (ERROR_SUCCESS != rc)
	{
		if (!CxxRegCreate (hKey, lpSubKey, bEnableRedirection)) return FALSE;

		rc = RegOpenKeyEx (hKey,
			lpSubKey,
			0,
			samDesired,
			&hSubKey);

		if (ERROR_SUCCESS != rc) return FALSE;
	}

	rc = RegSetValueEx (hSubKey,
		lpValueName,
		0,
		dwType,
		lpData,
		dwSize);

	RegCloseKey (hSubKey);
	return ERROR_SUCCESS == rc ? TRUE : FALSE;
}

BOOL
CxxRegDeleteKey (HKEY hKey,
				 LPCSTR	lpSubKey,
				 BOOL bEnableRedirection)
{
	HKEY hSubKey;
	long	rc;
	REGSAM	samDesired = KEY_ALL_ACCESS;

	if (bEnableRedirection == FALSE && IsWow64())
	{
		samDesired |= KEY_WOW64_64KEY;
	}
	rc = RegOpenKeyEx (hKey,
		lpSubKey,
		0,
		samDesired,
		&hSubKey);

	if (ERROR_SUCCESS == rc)
	{
//		boost::array<char,STANDARD_SIZE> a = {0};
		char a[STANDARD_SIZE] = {0};
		int	 i = 0;
//		while (RegEnumKey (hKey, i++, a.c_array(), (DWORD)a.size() - 1) == ERROR_SUCCESS)
		while (RegEnumKey (hSubKey, i++, a, sizeof(a) - 1) == ERROR_SUCCESS)
		{
			if (!CxxRegDeleteKey (hSubKey, a, bEnableRedirection))
			{
				continue;
			}
			i--;
		}
		RegCloseKey (hSubKey);
	}
	RegDeleteKey (hKey, lpSubKey);
	return TRUE;
}

BOOL CxxRegLastWriteTime( HKEY hkey, LPCSTR lpSubKey, PFILETIME lpftLastWriteTime )
{
	HKEY hSubKey;
	BOOL bResult = FALSE;
	if (ERROR_SUCCESS == ::RegOpenKey(hkey, lpSubKey, &hSubKey))
	{
		bResult = ::RegQueryInfoKey(hSubKey, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, lpftLastWriteTime)
			== ERROR_SUCCESS ? TRUE : FALSE;
		
		RegCloseKey(hSubKey);
	}

	return bResult;
}

BOOL
IsWinNT()
{
	OSVERSIONINFO OSVersionInfo;
	OSVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&OSVersionInfo);
	return OSVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT;
}

BOOL
GetWinOSVersion(DWORD &dwMajorVersion, DWORD &dwMinorVersion)
{
	OSVERSIONINFOEX OSVersionInfo;
	ZeroMemory(&OSVersionInfo, sizeof(OSVERSIONINFOEX));
	OSVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	if (!GetVersionEx ((OSVERSIONINFO *)&OSVersionInfo))
	{
		// Win2K以前的GetVersionEx版本采用OSVERSIONINFO结构
		OSVersionInfo.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
		if (!GetVersionEx ((OSVERSIONINFO *)&OSVersionInfo))
		{
			return FALSE;
		}
	}
	dwMajorVersion = OSVersionInfo.dwMajorVersion;
	dwMinorVersion = OSVersionInfo.dwMinorVersion;

	if (OSVersionInfo.dwMajorVersion == 6 && OSVersionInfo.dwMinorVersion == 2)
	{
		// 从Windows8开始，用GetVersionEx()获取版本号都返回6.2，要调用内部API来获取准确的版本
		HMODULE hModNtdll= NULL;
		DWORD dwMajorVer = 0;
		DWORD dwMinorVer = 0;
		DWORD dwBuildNumber = 0;
		if (hModNtdll= ::LoadLibrary("ntdll.dll"))
		{
			typedef void (WINAPI *pfRTLGETNTVERSIONNUMBERS)(DWORD*,DWORD*, DWORD*);
			pfRTLGETNTVERSIONNUMBERS pfRtlGetNtVersionNumbers;
			pfRtlGetNtVersionNumbers = (pfRTLGETNTVERSIONNUMBERS)::GetProcAddress(hModNtdll, "RtlGetNtVersionNumbers");
			if (pfRtlGetNtVersionNumbers)
			{
				pfRtlGetNtVersionNumbers(&dwMajorVer, &dwMinorVer, &dwBuildNumber);
				dwBuildNumber &= 0x0ffff;
			}
		}
		else
		{
			return FALSE;
		}
		::FreeLibrary(hModNtdll);
		if (dwMajorVer == 6)
		{
			dwMinorVersion = dwMinorVer;
		}
		else if (dwMajorVer == 10)
		{
			// Windows10的版本号为6.4
			dwMajorVersion = 6;
			dwMinorVersion = 4;
		}
		else
		{
			return FALSE;
		}
	}

	return TRUE;
}

BOOL IsWinVerEqualTo(DWORD dwMajorVersion, DWORD dwMinorVersion)
{
/*
	OSVERSIONINFOEX osvi = {0};
	DWORDLONG dwlConditionMask = 0;

	ZeroMemory(&osvi, sizeof(osvi));
	osvi.dwOSVersionInfoSize= sizeof(osvi);
	osvi.dwMajorVersion= dwMajorVersion;
	osvi.dwMinorVersion= dwMinorVersion;

	VER_SET_CONDITION(dwlConditionMask, VER_MAJORVERSION, VER_EQUAL);
	VER_SET_CONDITION(dwlConditionMask, VER_MINORVERSION, VER_EQUAL);

	return ::VerifyVersionInfo(&osvi, VER_MAJORVERSION | VER_MINORVERSION, dwlConditionMask);
*/
	DWORD dwMajorVer;
	DWORD dwMinorVer;
	if (!GetWinOSVersion(dwMajorVer, dwMinorVer))
	{
		return FALSE;
	}

	return (dwMajorVer == dwMajorVersion && dwMinorVer == dwMinorVersion);
}

BOOL IsWinVerGreaterThan(DWORD dwMajorVersion, DWORD dwMinorVersion)
{
/*
	OSVERSIONINFOEX osvi = {0};
	DWORDLONG dwlConditionMask = 0;

	ZeroMemory(&osvi, sizeof(osvi));
	osvi.dwOSVersionInfoSize= sizeof(osvi);
	osvi.dwMajorVersion= dwMajorVersion;
	osvi.dwMinorVersion= dwMinorVersion;

	// 主版本号判断
	VER_SET_CONDITION(dwlConditionMask, VER_MAJORVERSION, VER_GREATER);
	if (::VerifyVersionInfo(&osvi, VER_MAJORVERSION, dwlConditionMask))
	{
		return TRUE;
	}

	// 次版本号判断
	ZeroMemory(&osvi, sizeof(osvi));
	dwlConditionMask = 0;
	VER_SET_CONDITION(dwlConditionMask, VER_MAJORVERSION, VER_EQUAL);
	VER_SET_CONDITION(dwlConditionMask, VER_MINORVERSION, VER_GREATER);

	return ::VerifyVersionInfo(&osvi, VER_MAJORVERSION | VER_MINORVERSION, dwlConditionMask);
*/
	DWORD dwMajorVer;
	DWORD dwMinorVer;
	if (!GetWinOSVersion(dwMajorVer, dwMinorVer))
	{
		return FALSE;
	}

	return (dwMajorVer > dwMajorVersion || (dwMajorVer == dwMajorVersion && dwMinorVer > dwMinorVersion));
}

BOOL IsWinVerEqualOrGreater(DWORD dwMajorVersion, DWORD dwMinorVersion)
{
	DWORD dwMajorVer;
	DWORD dwMinorVer;
	if (!GetWinOSVersion(dwMajorVer, dwMinorVer))
	{
		return FALSE;
	}

	return (dwMajorVer > dwMajorVersion || (dwMajorVer == dwMajorVersion && dwMinorVer >= dwMinorVersion));
}

BOOL IsWin2K()
{
	return IsWinVerEqualTo(5, 0);
}

BOOL IsUpWin2000()
{
	return IsWinVerEqualOrGreater(5, 0);
}

BOOL IsUpWinXp()
{
	return IsWinVerEqualOrGreater(5, 1);
}

BOOL IsUpWin2003()
{
	return IsWinVerEqualOrGreater(5, 2);
}

BOOL IsUpWinVista()
{
	return IsWinVerEqualOrGreater(6, 0);
}

BOOL IsUpWin7()
{
	return IsWinVerEqualOrGreater(6, 1);
}

BOOL IsUpWin8()
{
	return IsWinVerEqualOrGreater(6, 2);
}

BOOL IsWin10()
{
	return IsWinVerEqualTo(6, 4);
}

BOOL
IsWow64()
{
	typedef BOOL (WINAPI *PFNISWOW64PROCESS)(HANDLE, PBOOL);

	PFNISWOW64PROCESS lpIsWow64Process = (PFNISWOW64PROCESS) GetProcAddress (GetModuleHandle ("kernel32"), "IsWow64Process");
	BOOL bWow64 = FALSE;

	if (lpIsWow64Process)
	{
		if (!lpIsWow64Process (GetCurrentProcess(), &bWow64))
		{
			return FALSE;
		}
	}

	return bWow64;
}

BOOL
Is64BitWindows0s ()
{
#if defined (_WIN64)
	return TRUE;
#else
	return BaseLib::IsWow64();
#endif
}

#endif

std::string GetModuleFullPath ( BOOL bLastPath )
{
	CHAR chPath [MAX_PATH] = {0};

#ifdef _MACRO_OS_WIN32
	::GetModuleFileName ( NULL, chPath, sizeof (chPath) );
#else
    memset(chPath, 0, sizeof(chPath));
    ::readlink("/proc/self/exe", chPath, sizeof (chPath));
#endif
	std::string strModuleFileName = chPath;

	if ( bLastPath == FALSE )
	{
		return strModuleFileName.c_str();
	}

    //如果文件名称有生僻汉字并且第二字节是'\\'时，用find_last_of的方法就会有问题
    //比如："c:\\ABC癨.txt"就会返回"c:\\ABC\\"，而用_splitpath和_makepath正确返回"c:\\"
//	size_t nPos = strModuleFileName.find_last_of(PATH_SEP_STRING);
//	std::string strLastPath = strModuleFileName.substr(0, nPos);

#ifdef _MACRO_OS_WIN32
	char szDrive[_MAX_DRIVE];
	char szDir[_MAX_DIR];
	::_splitpath (chPath, szDrive, szDir, NULL, NULL);
	::_makepath (chPath, szDrive, szDir, NULL, NULL);
	//去除最后的'\\'
	if (chPath[strlen(chPath) - 1] == PATH_SEPARATOR)
	{
		chPath[strlen(chPath) - 1] = '\0';
	}

#else
    ::dirname(chPath);
#endif

    std::string strLastPath = chPath;
	return strLastPath.c_str();
}

BOOL
CxxDeleteFile (LPCSTR lpszFileName)
{
#ifdef _MACRO_OS_WIN32

	DWORD dwAttribute = ::GetFileAttributes(lpszFileName);
	if ((dwAttribute != INVALID_FILE_ATTRIBUTES) && (dwAttribute & FILE_ATTRIBUTE_READONLY))
	{
		dwAttribute &= ~FILE_ATTRIBUTE_READONLY;
		if (!SetFileAttributes(lpszFileName, dwAttribute))
		{
			DBGPRINT(DBG_ERROR, "setAttr false , err=" << GetLastError())
			return FALSE;
		}
	}
	if (::DeleteFile(lpszFileName)) //::DeleteFile成功返回非0
	{
		return TRUE;
	}
	else if (GetLastError() != ERROR_FILE_NOT_FOUND) {
		DBGPRINT(DBG_ERROR, "deletefile false , err=" << GetLastError())	
	}
    return FALSE;
#else
    if (::unlink (lpszFileName))     //::unlink成功返回0
    {
        return FALSE;
    }
    return TRUE;
#endif
}


/************************************************************************/
/*	文件拷贝
/************************************************************************/

BOOL
CxxSDCopyFile(LPCSTR lpExistingFileName,
	LPCSTR lpNewFileName,
	BOOL bFailIfExists,
	BOOL* lpbExit)
{
	if (!lpExistingFileName || !lpNewFileName)
	{
		return FALSE;
	}

	std::string szExistingFileName = lpExistingFileName;
	std::string szNewFileName = lpNewFileName;

	std::replace(szExistingFileName.begin(), szExistingFileName.end(), '\\', '/');
	std::replace(szNewFileName.begin(), szNewFileName.end(), '\\', '/');

	if (_stricmp(szExistingFileName.c_str(), szNewFileName.c_str()) == 0)
	{
		return TRUE;
	}

	if (bFailIfExists &&
		BaseLib::FileAccess(szNewFileName.c_str(), 0) != -1)
	{
		return TRUE;
	}

	CAutoFile ifile(fopen(szExistingFileName.c_str(), "rb"));
	if (NULL == ifile.Get())
	{
		return FALSE;
	}

	CAutoFile ofile(fopen(szNewFileName.c_str(), "wb"));
	if (NULL == ofile.Get())
	{
		return FALSE;
	}

	char szData[PAGE_SIZE];
	while (!feof(ifile.Get()))
	{
		if (lpbExit && *lpbExit)
		{
			break;
		}

		int iRead = fread(szData, 1, sizeof(szData), ifile.Get());
		if (iRead > 0)
		{
			if (fwrite(szData, 1, iRead, ofile.Get()) != iRead)
			{
				return FALSE;
			}
		}
	}

	return TRUE;
}


BOOL
CxxCopyFile(LPCSTR lpExistingFileName,
	LPCSTR lpNewFileName,
	BOOL bFailIfExists)
{
	return CxxSDCopyFile(lpExistingFileName, lpNewFileName, bFailIfExists, NULL);
}

/************************************************************************/
/* 	目录拷贝
/************************************************************************/

BOOL
CxxCreateDirectory(LPCTSTR lpDirectory)
{
	std::string szBuffer = SAFE_STRING(lpDirectory);
#ifdef _MACRO_OS_WIN32
	//	boost::replace_all (szBuffer, "/", "\\");
	BaseLib::StringReplaceAll(szBuffer, "/", "\\");
#endif

	BaseLib::VECTOR_STRING vetor = BaseLib::ToVectors(szBuffer, PATH_SEP_STRING);

	std::ostringstream o;
	if (lpDirectory[0] == PATH_SEPARATOR)
	{
		o << PATH_SEP_STRING;
	}

	for (int i = 0; i < vetor.size(); i++)
	{
		if (vetor[i].empty())
			continue;

		o << vetor[i] << PATH_SEP_STRING;
		if (o.str().size() <= 3)
		{
			continue;
		}

#ifdef _MACRO_OS_WIN32
		if (!::CreateDirectory(o.str().c_str(), NULL))
		{
			DWORD dwResult = BaseLib::OS::lasterror();
			if (ERROR_ALREADY_EXISTS != dwResult)
			{
				//				DBGPRINT (DBG_ERROR, _T("CreateDirectory:") << o.str().c_str() << _T(" 错误:") << dwResult);
				return FALSE;
			}
		}
		else
		{
			//			DBGPRINT (DBG_ERROR, _T("CreateDirectory:") << o.str().c_str() << _T(" 成功:"));
		}
#else
		if (!::mkdir(o.str().c_str(), 0755))
		{
			DWORD dwResult = BaseLib::OS::lasterror();
			if (EEXIST != dwResult)
			{
				DBGPRINT(DBG_ERROR, _T("mkdir:") << o.str() << _T(" 错误:") << dwResult);
				return FALSE;
			}
		}
#endif
	}

	return TRUE;
}

CDirectoryCopy::CDirectoryCopy(DWORD dwMode)
	: _dwMode(dwMode)
{
}

int
CDirectoryCopy::Copy(LPCTSTR lpSourceDirectory,
	LPCTSTR lpDestDirectory)
{
	if (!lpSourceDirectory || '\0' == *lpSourceDirectory ||
		!lpDestDirectory || '\0' == *lpDestDirectory)
	{
		return -1;
	}

	int	nResult = NO_ERROR;
	std::string szSourceDirectory = lpSourceDirectory;
	std::string szDestDirectory = lpDestDirectory;

	//	需要创建目录
	if (PATH_SEPARATOR != *szSourceDirectory.rbegin())
		szSourceDirectory += PATH_SEPARATOR;

	if (PATH_SEPARATOR != *szDestDirectory.rbegin())
	{
		szDestDirectory += PATH_SEPARATOR;
	}

#ifdef _MACRO_OS_WIN32
	if (!CxxCreateDirectory(szDestDirectory.c_str()))
	{
		DBGPRINT(DBG_ERROR, "CreateDirectory:" << szDestDirectory <<
			"Error:" << GetLastError());
	}

	std::ostringstream o;
	o << szSourceDirectory << _T("*.*");

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = FindFirstFile(o.str().c_str(), &FindFileData);
	if (INVALID_HANDLE_VALUE == hFind)
	{
		return 1;
	}

	do
	{
		::Sleep(1);
		std::ostringstream oSource, oDest;
		oSource << szSourceDirectory << FindFileData.cFileName;
		oDest << szDestDirectory << FindFileData.cFileName;

		if (FILE_ATTRIBUTE_DIRECTORY == (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			if (_T('.') == FindFileData.cFileName[0])
			{
				continue;
			}

			if (CDirectoryCopy::Copy(oSource.str().c_str(), oDest.str().c_str()) < 0)
			{
				nResult = CopyDirectoryFail(oSource.str().c_str(), oDest.str().c_str());
				if (nResult < 0)
				{
					return nResult;
				}
			}
		}
		else
		{
			if (!::CopyFile(oSource.str().c_str(), oDest.str().c_str(), FALSE))
			{
				nResult = CopyFileFail(oSource.str().c_str(), oDest.str().c_str());
				if (nResult < 0)
				{
					return nResult;
				}
			}
			else if (MODE_CUT & _dwMode)
			{
				DeleteFile(oSource.str().c_str());
			}
		}
	} while (FindNextFile(hFind, &FindFileData));
	FindClose(hFind);
#else
	if (!::mkdir(lpDestDirectory, 0755))
	{
		DBGPRINT(DBG_ERROR, "mkdir:" << szDestDirectory <<
			"Error:" << BaseLib::OS::lasterror());
	}
	DIR *lpDir;
	struct dirent Entry;
	struct dirent *lpEntry;
	struct stat sb;

	if ((lpDir = ::opendir(szSourceDirectory.c_str())) == NULL)
	{
		return FALSE;
	}
	while (1)
	{
		std::ostringstream oSource, oDest;

		::readdir_r(lpDir, &Entry, &lpEntry);
		if (lpEntry == NULL)
		{
			break;
		}
		//过滤当前目录(.)和上一级目录(..)
		if (strcmp(lpEntry->d_name, ".") == 0 || strcmp(lpEntry->d_name, "..") == 0)
		{
			continue;
		}
		oSource << szSourceDirectory << lpEntry->d_name;
		oDest << szDestDirectory << lpEntry->d_name;

		//判断该条目是否是目录
		if (::stat(lpEntry->d_name, &sb) >= 0 && S_ISDIR(sb.st_mode))
		{
			if (CDirectoryCopy::Copy(oSource.str().c_str(), oDest.str().c_str()) < 0)
			{
				nResult = CopyDirectoryFail(oSource.str().c_str(), oDest.str().c_str());
				if (nResult < 0)
				{
					return nResult;
				}
			}
		}
		else
		{
			if (!CxxCopyFile(oSource.str().c_str(), oDest.str().c_str(), FALSE))
			{
				nResult = CopyFileFail(oSource.str().c_str(), oDest.str().c_str());
				if (nResult < 0)
				{
					return nResult;
				}
			}
			else if (MODE_CUT & _dwMode)
			{
				::unlink(oSource.str().c_str());
			}
		}
	}
	::closedir(lpDir);
#endif

	return nResult;
}

int
CDirectoryCopy::CopyFileFail(LPCTSTR lpSourceFile,
	LPCTSTR lpDestFile)
{
	DBGPRINT(DBG_ERROR, "源文件:" << lpSourceFile << NEW_LINE <<
		"目标文件:" << lpDestFile << NEW_LINE <<
		"错误:" << BaseLib::OS::lasterror());

	if (MODE_MUSTRIGHT & _dwMode)
	{
		return -1;
	}
	return 1;
}

int
CDirectoryCopy::CopyDirectoryFail(LPCTSTR lpSourceDirectory,
	LPCTSTR lpDestDirectory)
{
	DBGPRINT(DBG_ERROR, "源文件:" << lpSourceDirectory << NEW_LINE <<
		"目标文件:" << lpDestDirectory << NEW_LINE <<
		"错误:" << BaseLib::OS::lasterror());

	if (MODE_MUSTRIGHT & _dwMode)
	{
		return -1;
	}
	return 1;
}

#ifdef _MACRO_OS_WIN32
BOOL CxxDeleteFileInDeep(LPCSTR lpszFileName, UINT iWipeTime)
{
	if (INVALID_PARAM_LPSTR(lpszFileName))
	{
		DBGPRINT(DBG_ERROR, "文件深度删除，传入路径为空");
		return FALSE;
	}
	string strFilePath = lpszFileName;
	BaseLib::StringReplaceAll(strFilePath, "/", "\\");
	if (!BaseLib::IsFileExist(strFilePath.c_str()))
	{
		DBGPRINT(DBG_ERROR, "文件深度删除，传入文件路径不存在！" << strFilePath);
		return TRUE;
	}
	FILE *fp = NULL;
	int icounter = iWipeTime;
	while (0 != icounter)
	{
		ULONG64 lFileLength = 0;
		fp = fopen(strFilePath.c_str(), "rb+");
		if (!fp)
		{
			DBGPRINT(DBG_ERROR, "文件深度擦除，打开文件失败：" << strFilePath << " ,错误: " << GetLastError());
			return FALSE;
		}
		fseek(fp, 0L, SEEK_END);
		lFileLength = ftell(fp);
		fseek(fp, 0L, SEEK_SET);
		while (lFileLength > 0)
		{
			char *p;
			unsigned int iWritePage = 0;
			if (lFileLength >= 4096)
			{
				iWritePage = 4096;
			}
			else
			{
				iWritePage = lFileLength;
			}
			p = new char[iWritePage + 1];
			memset(p, 0, (iWritePage + 1));
			ULONG64 lWritedSize = fwrite(p, 1, iWritePage, fp);
			delete[]p;
			if (0 == lWritedSize)
			{
				fclose(fp);
				return FALSE;
			}
			lFileLength -= lWritedSize;
		}
		fclose(fp);
		HANDLE h = CreateFile(strFilePath.c_str(), GENERIC_ALL, 0, NULL, OPEN_EXISTING, 0, NULL);
		BOOL bRet = FlushFileBuffers(h);
		CloseHandle(h);
		icounter--;
	}
	char szFileExt[MAX_PATH] = { 0 };
	char szFileDir[MAX_PATH] = { 0 };
	char szDrive[MAX_PATH] = { 0 };
	_splitpath(strFilePath.c_str(), szDrive, szFileDir, NULL, szFileExt);
	std::ostringstream o;
	if (NULL != szFileDir && NULL != szFileExt)
	{
		o << szDrive << szFileDir << BaseLib::GetTickCount() << szFileExt;
		if (rename(strFilePath.c_str(), o.str().c_str()) == 0)
		{
			if (BaseLib::CxxDeleteFile(o.str().c_str()))
			{
				return TRUE;
			}
			else
			{
				DBGPRINT(DBG_ERROR, "文件深度擦除，删除文件失败：" << strFilePath);
				return FALSE;
			}
		}
	}
	return FALSE;
}
#endif

}