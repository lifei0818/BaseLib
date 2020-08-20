#ifndef BaseLib_DbgPrint_H
#define BaseLib_DbgPrint_H

#include "common/sys_incl.h"
#include "common/mutex.h"
//#include "tolists.h"

/************************************************************************/
/* 
/************************************************************************/

namespace BaseLib
{
class CDbgPrint
{
typedef void (*DBGFUNCTION)(DWORD, LPCSTR, UINT, LPCSTR, LPCSTR);

public:
    CDbgPrint (void) :m_DbgFunction (DefaultDbgPrint) {}
	int GetDebugLevel (DWORD id);
	void SetDebug (LPCSTR lpString);
    inline void SetDbgFunction (DBGFUNCTION DbgFunction) { m_DbgFunction = DbgFunction; }
	void DbgPrint (DWORD nLevel, LPCSTR lpFile, UINT nLine, LPCSTR lpFunc, LPCSTR lpInfo);

protected:

protected:
    static void DefaultDbgPrint(DWORD nLevel, LPCSTR lpFile, UINT nLine, LPCSTR lpFunc, LPCSTR lpInfo);
	DBGFUNCTION				m_DbgFunction;
	std::list<DWORD>		m_list;
	std::list<std::string>	m_textlist;
	CThreadMutex			m_lock;
};

}
#endif
