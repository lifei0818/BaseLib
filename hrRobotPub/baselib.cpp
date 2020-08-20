#include "common/sys_incl.h"
#include "common/template.h"
//#include "common/socket.h"
//#include "common/ace.h"
#ifdef _MACRO_OS_WIN32
#include "common/funcpoint.h"
#endif
#include "hrRobotPub/hrRobotDebug.h"

namespace BaseLib
{

void InitBaseLib()
{
	static BOOL bInit = FALSE;

	if (!bInit)
	{
		BaseLib::TSingleton<BaseLib::CDbgPrint>::Instance();
		BaseLib::TSingleton<BaseLib::CDbgPrint>::Instance()->SetDbgFunction(BaseLib::ywSmpDbgPrint);
		//BaseLib::OS::socket_init();
#ifdef _MACRO_OS_WIN32
		BaseLib::TSingleton<CFunctionPoint>::Instance();
#endif
		bInit = TRUE;
	}
}

}
