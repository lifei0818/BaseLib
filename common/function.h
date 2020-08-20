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

/// ��ȡ����ʱ����ǰʱ��ĺ����� (32λ)
DWORD
GetTickCount();

DWORDLONG
GetTickCount64();

#ifdef _MACRO_OS_WIN32

/************************************************************************/
/*	ע������ 
/************************************************************************/

HKEY
CxxParseRootKey (IN LPCTSTR lpName, 
				 OUT LPCTSTR *lpNext = NULL);

/// �鿴 �Ӽ��Ƿ����
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

/// �鿴 ��ֵ�Ƿ����
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

/// ��ȡ��ֵ����޸�ʱ��
BOOL
CxxRegLastWriteTime(HKEY hkey,
					LPCSTR lpSubKey,
					PFILETIME lpftLastWriteTime);
					
///	�Ƿ�XP����(����)�汾
BOOL IsUpWinXp();

/// �Ƿ���64λϵͳ
BOOL
IsWow64();

/// �Ƿ���64λ����IsWow64 ()��64λ�������ж��Ǵ���ģ����������������
BOOL Is64BitWindows0s ();

#endif

/*
   ����: �õ�ģ�����ڵ�ַ
   ����: ���Ϊ FALSE, �õ�ȫ·��  �� c:\windows\aa.dll
		 ���Ϊ TRUE, �õ�ȫ·����һ��(ģ������Ŀ¼) �� c:\windows
   ����ֵ: ģ��·��
*/
std::string GetModuleFullPath ( BOOL bLastPath = FALSE );
	
/// ɾ���ļ�
BOOL
CxxDeleteFile (LPCSTR lpszFileName);

/// ��ؐ�ļ�
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
/* 	Ŀ¼����
/*	@Param dwMode
/*		MODE_GENERAL ��ͨ����ģʽ
/*		MODE_CUT ���п���ģʽ
/*		MODE_MUSTRIGHT ���뿽���ɹ���������Ϊ����
/*
/*	@Return 0	�ɹ���
/*			1	������ɣ����Ǵ��ڴ���
/*			-1	����ʧ��
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