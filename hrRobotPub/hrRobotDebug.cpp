#include "common/sys_incl.h"
#include "common/macro.h"
#include "common/function.h"
#include "common/inifile.h"
//#include "hrRobotPub/ExeDefine.h"
//#include "hrRobotPub/CfgDefine.h"
#include "hrRobotPub/KernelQuery.h"
#include "hrRobotPub/hrRobotDebug.h"

///日志信息文件
#define SYS_FUN_CONFIGINI_NAME		"fun_config.ini"	
#define SECTION_MAIN		("CONFIG")
#define	IDENT_RT			("rt")

namespace BaseLib
{
//  调试输出
	void ywSmpDbgPrint( DWORD nLevel, LPCSTR lpFile, UINT nLine, LPCSTR lpFunc, LPCSTR lpInfo)
{
    char __szBuf__[128]; 
    struct tm __tm__; 
    time_t __t__ = time (NULL); 

#if (defined _MACRO_OS_WIN32) && (_MSC_VER < 1400)
	struct tm *ptm = localtime(&__t__); 
	__tm__ = *ptm;
#else
	localtime_s(&__tm__, &__t__);			
#endif

#ifdef _MACRO_OS_WIN32
    sprintf (__szBuf__, 
        LEFT_MIDDLE_BRACKET_STRING _T("Level=") "0x%X" RIGHT_MIDDLE_BRACKET_STRING TAB_STRING 
        LEFT_MIDDLE_BRACKET_STRING _T("ThreadId=") "%lu" RIGHT_MIDDLE_BRACKET_STRING TAB_STRING 
        LEFT_MIDDLE_BRACKET_STRING _T("Tickcout=") "%lu" RIGHT_MIDDLE_BRACKET_STRING TAB_STRING 
        LEFT_MIDDLE_BRACKET_STRING "%.4d-%.2d-%.2d %.2d:%.2d:%.2d" RIGHT_MIDDLE_BRACKET_STRING, 
        nLevel, BaseLib::GetCurrentThreadID(), GetTickCount() / 1000, 
        __tm__.tm_year + 1900, __tm__.tm_mon + 1, __tm__.tm_mday, 
        __tm__.tm_hour, __tm__.tm_min, __tm__.tm_sec); 
#else
    snprintf (__szBuf__, sizeof (__szBuf__), 
        LEFT_MIDDLE_BRACKET_STRING _T("Level=") "0x%X" RIGHT_MIDDLE_BRACKET_STRING TAB_STRING 
        LEFT_MIDDLE_BRACKET_STRING _T("ThreadId=") "%lu" RIGHT_MIDDLE_BRACKET_STRING TAB_STRING 
        LEFT_MIDDLE_BRACKET_STRING _T("Tickcout=") "%lu" RIGHT_MIDDLE_BRACKET_STRING TAB_STRING 
        LEFT_MIDDLE_BRACKET_STRING "%.4d-%.2d-%.2d %.2d:%.2d:%.2d" RIGHT_MIDDLE_BRACKET_STRING, 
        nLevel, BaseLib::GetCurrentThreadID(), times(NULL) / sysconf(_SC_CLK_TCK) / 1000, 
        __tm__.tm_year + 1900, __tm__.tm_mon + 1, __tm__.tm_mday, 
        __tm__.tm_hour, __tm__.tm_min, __tm__.tm_sec); 
#endif

    LPCSTR __lpFile__   = strrchr(lpFile, PATH_SEPARATOR);
    __lpFile__ = (__lpFile__ == NULL ? lpFile : __lpFile__ + 1);
    std::ostringstream __oDbgString__;
    __oDbgString__ << __szBuf__ << TAB_STRING << 
        LEFT_MIDDLE_BRACKET_STRING << __lpFile__ << COLON_STRING << nLine << RIGHT_MIDDLE_BRACKET_STRING << TAB_STRING << 
        LEFT_MIDDLE_BRACKET_STRING << lpFunc << RIGHT_MIDDLE_BRACKET_STRING << NEW_LINE << TAB_STRING << lpInfo << NEW_LINE; 
    BaseLib::OUTPUT_DEBUG (__oDbgString__.str().c_str(), eDbgTypeKnown);
}

BOOL GetDbgCode (std::string &strCode)
{
	DWORD dwType;
	TCHAR szBuffer[PAGE_SIZE] = {0};
	DWORD dwSize = sizeof (szBuffer) - 1;

//#ifdef _MACRO_OS_WIN32
#if 0
	// 增加64位程序调试输出
	BOOL bIs64BitWindwosOs  = BaseLib::Is64BitWindows0s();
/*
	if (CxxRegGetValue(HKEY_LOCAL_MACHINE, 
		bIs64BitWindwosOs ? YW_SMP_SOFTWALL_X64_KEY : YW_SMP_SOFTWALL_KEY,
		IDENT_RT,
		&dwType, (LPBYTE)szBuffer, &dwSize))
	{
		strCode = szBuffer;
		return TRUE;
	}
*/
#else
	// Linux版本从fun_config.ini中获取
	std::string strConfig = BaseLib::GetModuleFullPath(TRUE) + PATH_SEPARATOR + SYS_FUN_CONFIGINI_NAME;
	BaseLib::CIniFile config(strConfig.c_str());
	strCode = config.ReadString (SECTION_MAIN, IDENT_RT, "");
	return TRUE;
#endif

	return FALSE;
}

#ifdef _MACRO_OS_WIN32
void HookDbgPrint( DWORD nLevel, LPCSTR lpFile, UINT nLine, LPCSTR lpFunc, LPCSTR lpInfo )
{
	char __szBuf__[128]; 
	struct tm __tm__; 
	time_t __t__ = time (NULL); 

#if (defined _MACRO_OS_WIN32) && (_MSC_VER < 1400)
	struct tm *ptm = localtime(&__t__); 
	__tm__ = *ptm;
#else
	localtime_s(&__tm__, &__t__);			
#endif

	sprintf (__szBuf__, 
		LEFT_MIDDLE_BRACKET_STRING _T("Level=") "0x%X" RIGHT_MIDDLE_BRACKET_STRING TAB_STRING 
		LEFT_MIDDLE_BRACKET_STRING _T("ProcessId=") "%lu" RIGHT_MIDDLE_BRACKET_STRING TAB_STRING 
		LEFT_MIDDLE_BRACKET_STRING _T("ThreadId=") "%lu" RIGHT_MIDDLE_BRACKET_STRING TAB_STRING 
		LEFT_MIDDLE_BRACKET_STRING _T("Tickcout=") "%lu" RIGHT_MIDDLE_BRACKET_STRING TAB_STRING 
		LEFT_MIDDLE_BRACKET_STRING "%.4d-%.2d-%.2d %.2d:%.2d:%.2d" RIGHT_MIDDLE_BRACKET_STRING, 
		nLevel, ::GetCurrentProcessId(), BaseLib::GetCurrentThreadID(), GetTickCount() / 1000, 
		__tm__.tm_year + 1900, __tm__.tm_mon + 1, __tm__.tm_mday, 
		__tm__.tm_hour, __tm__.tm_min, __tm__.tm_sec); 

	LPCSTR __lpFile__   = strrchr(lpFile, PATH_SEPARATOR);
	__lpFile__ = (__lpFile__ == NULL ? lpFile : __lpFile__ + 1);
	std::ostringstream __oDbgString__;
	__oDbgString__ << __szBuf__ << TAB_STRING << 
		LEFT_MIDDLE_BRACKET_STRING << __lpFile__ << COLON_STRING << nLine << RIGHT_MIDDLE_BRACKET_STRING << TAB_STRING << 
		LEFT_MIDDLE_BRACKET_STRING << lpFunc << RIGHT_MIDDLE_BRACKET_STRING << NEW_LINE << TAB_STRING << lpInfo << NEW_LINE; 
	BaseLib::OUTPUT_DEBUG (__oDbgString__.str().c_str(), eDbgTypeHook);
}
#endif

}
