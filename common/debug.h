#ifndef BaseLib_DEBUG_H
#define BaseLib_DEBUG_H

#include "common/DbgPrint.h"
#include "common/template.h"

/************************************************************************/
/*  调试输出等级
/************************************************************************/

const DWORD DBG_ERROR = 0xF0000000;

/// 线程
const DWORD DBG_THREAD = 0x0001;
/************************************************************************/
/* 
/************************************************************************/

namespace BaseLib
{
#define DBGPRINT(LEVEL,INFO) \
    {\
        std::ostringstream StrInfo;\
        StrInfo<<INFO;\
        BaseLib::TSingleton<BaseLib::CDbgPrint>::Instance()->DbgPrint(LEVEL, __FILE__, __LINE__, __FUNCTION__, StrInfo.str().c_str());\
    }

#define DBGPRINTTIMES(LEVEL,TIMES,INFO) \
	{\
	static int _itimes_=0; \
	if (_itimes_ < TIMES) \
	{ \
		_itimes_++; \
		DBGPRINT(LEVEL,INFO); \
	} \
}

#define DBGPRINT2(LEVEL1,LEVEL2,INFO) \
    { DBGPRINT(LEVEL1, INFO); DBGPRINT(LEVEL2, INFO); }


/// 申请内存通用宏
# define NEW(POINTER,CONSTRUCTOR) \
    do \
    { \
        try \
        { \
            POINTER = new CONSTRUCTOR; \
        } \
        catch (...) \
        { \
            POINTER = NULL; \
            DBGPRINT (DBG_ERROR, "new"); \
        } \
    } while (0)
}
#endif
