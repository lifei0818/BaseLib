#ifndef BaseLib_H
#define BaseLib_H

#include "hrRobotPub/DBGDefine.h"
#include "hrRobotPub/hrRobotDebug.h"
#include "common/baselib_incl.h"
#if defined (_MACRO_OS_WIN32)
#pragma comment(lib, "ws2_32.lib")
#endif

//#include <boost/lexical_cast.hpp>
//#include <boost/algorithm/string.hpp>
#include "jsoncpp/json.h"


namespace BaseLib
{

void InitBaseLib();

}


//定义的一个全局类对象来自动执行InitBaseLib()


#endif
