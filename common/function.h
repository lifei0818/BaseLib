#ifndef BASELIB_FUNCTION_H
#define BASELIB_FUNCTION_H

#include "common/template.h"

#define SHORT_MONTH_JAN     "Jan"
#define SHORT_MONTH_FEB     "Feb"
#define SHORT_MONTH_MAR     "Mar"
#define SHORT_MONTH_APR     "Apr"
#define SHORT_MONTH_MAY     "May"
#define SHORT_MONTH_JUN     "Jun"
#define SHORT_MONTH_JUL     "Jul"
#define SHORT_MONTH_AUG     "Aug"
#define SHORT_MONTH_SEP     "Sep"
#define SHORT_MONTH_OCT     "Oct"
#define SHORT_MONTH_NOV     "Nov"
#define SHORT_MONTH_DEC     "Dec"

namespace BaseLib
{

ULONG
GetCurrentThreadID();

/// 获取开机时到当前时间的毫秒数 (32位)
DWORD
GetTickCount();

DWORDLONG
GetTickCount64();

#ifdef _MACRO_OS_WIN32

/************************************************************************/
/*	注册表相关 
/************************************************************************/

HKEY
CxxParseRootKey (IN LPCTSTR lpName, 
				 OUT LPCTSTR *lpNext = NULL);

/// 查看 子键是否存在
BOOL
CxxSubKeyIsExist (IN HKEY hKey, 
				 IN LPCSTR lpSubKey,
				 IN BOOL bEnableRedirection = TRUE);

BOOL
CxxRegGetValue (IN HKEY hKey, 
				IN LPCSTR lpSubKey, 
				IN LPCSTR lpValueName, 
				OUT LPDWORD lpType,
				IN OUT LPBYTE lpData,
				IN OUT LPDWORD lpcbData,
				IN BOOL bEnableRedirection = TRUE);

/// 查看 键值是否存在
BOOL
CxxRegValueIsExist (IN HKEY hKey, 
					IN LPCSTR lpSubKey,
					IN LPCSTR lpValueName,
					IN BOOL bEnableRedirection = TRUE);

BOOL
CxxRegCreate (HKEY hKey, 
			  LPCSTR lpSubKey,
			  BOOL bEnableRedirection = TRUE);

BOOL
CxxRegDeleteValue (HKEY hKey, 
				LPCSTR lpSubKey, 
				LPCSTR lpValueName,
				BOOL bEnableRedirection = TRUE);

BOOL
CxxRegSetValue (HKEY hKey, 
				LPCSTR lpSubKey, 
				LPCSTR lpValueName, 
				DWORD dwType,
				LPBYTE lpData,
				DWORD dwSize,
				BOOL bEnableRedirection = TRUE);
BOOL
CxxRegDeleteKey (HKEY hKey, 
				 LPCSTR lpSubKey,
				 BOOL bEnableRedirection = TRUE);

/// 获取键值最后修改时间
BOOL
CxxRegLastWriteTime(HKEY hkey,
					LPCSTR lpSubKey,
					PFILETIME lpftLastWriteTime);
					
///	是否XP以上(包括)版本
BOOL IsUpWinXp();

/// 是否是64位系统
BOOL
IsWow64();

/// 是否是64位程序，IsWow64 ()在64位程序中判断是错误的，故用这个函数补充
BOOL Is64BitWindows0s ();

#endif

/*
   作用: 得到模块所在地址
   参数: 如果为 FALSE, 得到全路径  如 c:\windows\aa.dll
		 如果为 TRUE, 得到全路径上一层(模块所在目录) 如 c:\windows
   返回值: 模块路径
*/
std::string GetModuleFullPath ( BOOL bLastPath = FALSE );
	
/// 删除文件
BOOL
CxxDeleteFile (LPCSTR lpszFileName);

/// 拷貝文件
//BOOL
//CxxSDCopyFile(LPCSTR lpExistingFileName,
//	LPCSTR lpNewFileName,
//	BOOL bFailIfExists,
//	BOOL* lpbExit);

BOOL
CxxCopyFile(LPCSTR lpExistingFileName,
	LPCSTR lpNewFileName,
	BOOL bFailIfExists);

BOOL
CxxCreateDirectory(LPCTSTR lpDirectory);
/************************************************************************/
/* 	目录拷贝
/*	@Param dwMode
/*		MODE_GENERAL 普通拷贝模式
/*		MODE_CUT 剪切拷贝模式
/*		MODE_MUSTRIGHT 必须拷贝成功，否则认为错误
/*
/*	@Return 0	成功。
/*			1	拷贝完成，但是存在错误。
/*			-1	拷贝失败
/*
/************************************************************************/

#define		MODE_GENERAL	0
#define		MODE_CUT		1
#define		MODE_MUSTRIGHT	0x10000000

class CDirectoryCopy
{
public:
	CDirectoryCopy(DWORD dwMode = MODE_GENERAL);

	virtual int Copy(LPCTSTR lpSourceDirectory,
		LPCTSTR lpDestDirectory);

protected:
	virtual int CopyFileFail(LPCTSTR lpSourceFile,
		LPCTSTR lpDestFile);

	virtual int CopyDirectoryFail(LPCTSTR lpSourceDirectory,
		LPCTSTR lpDestDirectory);

protected:
	DWORD _dwMode;
};



}
#endif