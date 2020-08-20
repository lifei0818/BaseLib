#ifndef YWSMPDEBUG_H
#define YWSMPDEBUG_H

#include "common/sys_incl.h"
#include "common/function.h"

#ifndef DBGFILE
    #if defined (_MACRO_OS_WIN32)
        #define DBGFILE "c:\\hrRobot.log"
    #elif defined (_MACRO_OS_LINUX)
		#define DBGFILE "/tmp/hrRobot.log"
    #endif
#endif

#ifndef HOOK_DBGFILE
#define HOOK_DBGFILE  "c:\\GZdbg.log"
#endif // HOOK_DBGFILE

#ifndef HOOK_DBGFILE_BAK
#define HOOK_DBGFILE_BAK  "c:\\GZdbg.bak"
#endif // HOOK_DBGFILE_BAK

#ifdef _MACRO_OS_WIN32
#pragma message ("DBGFILE=" DBGFILE)
#endif

namespace BaseLib
{
	typedef enum _dbgprint_type
	{
		eDbgTypeKnown = 0,   /// 默认调试信息输出格式
		eDbgTypeHook         /// Hook 信息调试信息输出格式
	}DBGPRINT_TYPE;

inline void OUTPUT_DEBUG (LPCSTR lpString, DBGPRINT_TYPE eType)
{
#ifdef DBGFILE
	CHAR chFile[MAX_PATH];
#ifdef _MACRO_OS_WIN32
	ZeroMemory(chFile, MAX_PATH);
#else
	memset(chFile, 0, sizeof(chFile));
#endif
	if (eType == eDbgTypeKnown)
	{
		memcpy(chFile, DBGFILE, sizeof(DBGFILE));	
	}
	else if (eType == eDbgTypeHook)
	{
		memcpy(chFile, HOOK_DBGFILE, sizeof(HOOK_DBGFILE));
	}

	FILE *lpFile = fopen (chFile, "a+b");
	if (lpFile)
	{
		fseek(lpFile, 0L, SEEK_END);
		int size = ftell(lpFile);
		fputs (lpString, lpFile);
		fclose (lpFile);
		if (size > (1 * 1024 * 1024))
		{
#ifdef _MACRO_OS_WIN32
			::DeleteFile(chFile);
#else
			::unlink(chFile);
#endif // _MACRO_OS_WIN32

		}
	}

#endif
    OutputDebugStringA (lpString);
}

#define INIT_DBG_CODE \
{ \
    std::string strDbgCode; \
    BaseLib::GetDbgCode (strDbgCode); \
	BaseLib::TSingleton<BaseLib::CDbgPrint>::Instance()->SetDebug (strDbgCode.c_str());\
}

//  调试输出
void ywSmpDbgPrint(DWORD nLevel, LPCSTR lpFile, UINT nLine, LPCSTR lpFunc, LPCSTR lpInfo);

/// 用于 Hook 调试信息的输出
void HookDbgPrint(DWORD nLevel, LPCSTR lpFile, UINT nLine, LPCSTR lpFunc, LPCSTR lpInfo);

BOOL GetDbgCode (std::string &strCode);

}
#endif
