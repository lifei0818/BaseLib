#ifndef BASELIB_TASK_H
#define BASELIB_TASK_H

#include "common/sys_incl.h"
#include "common/mutex.h"

namespace BaseLib
{

#define TASK_STOP_STATE			0	///< 任务处于停止状态
#define TASK_RUNNING_STATE		1	///< 任务处于运行状态

#define TASK_BREAK		-1	///< 线程正常中断退出
#define TASK_EXCEPTION	-2	///< 线程异常中断退出
#define TASK_CONTINUE	1	///< 线程继续

/// 任务 (封装了多个线程)
class CTask
{
public:
	CTask ();
	virtual ~CTask (void);

	/// 开始线程
	virtual INT32 Open (INT iThreadCount = 1);

	/**
	* 结束线程
	* 在svc内调用 Close () 不会阻塞
	*
	* @param BOOL bWait 是否等待
	* @param DWORD dwMilliseconds 等待时间（单位：微秒）
	* @return INT32
	* - 1  已存在
	* - 0  成功
	* - -1 失败
	*/
	virtual INT32 Close (BOOL bWait = TRUE, DWORD dwMilliseconds = INFINITE);

	/**
	 * 子类派生,实现具体的线程操作
	 * 
	 * @return 
	 *	- @see TASK_BREAK
	 *	- @see TASK_CONTINUE
	 */
	virtual INT32 Svc (void) = 0;

	/// 挂起线程,不建议使用,特定场合使用
	void Suspend ();

	/// 恢复线程,不建议使用,特定场合使用
	void Resume ();

	/// 强行终止线程,不建议使用,特定场合使用
	void Terminate ();

	UINT32 ThreadCount (void);
	std::string PrintThreadId();

	void TryAgain();
	BOOL IsAgain();

    /// 线程序号（从1开始），主线程调用返回0
    UINT32 ThreadIndex (void);

protected:

	/**
	 * 线程状态
	 * 
	 * @return 
	 *	- @see TASK_STOP_STATE
	 *	- @see TASK_RUNNING_STATE
	 */
	LONG State();
	void State(LONG);

	virtual INT32 SvcRun (void);
	static UINT32 WINAPI SvcFun (LPVOID lpParam);

protected:
	volatile LONG _state;
	/*volatile*/ HANDLE* _lpHandle;
#ifdef _MACRO_OS_WIN32
	UINT32* _lpThreadId;
#else
	pthread_t *_lpThreadId;
#endif
	INT32* _lpThreadStatus;
	UINT32 _threadCount;
	CThreadMutex _mutex;
	DWORD _dwdbgThreadTick;
	unsigned _stackSize;

	BOOL _bTryAgain;
};

// 与函数内的 TryAgain 相同,使用宏的话输出调试时方便输出具体的函数
#define TASK_TRY_AGAIN \
	if (!ThreadCount ()) \
	{ \
		Open (); \
	} \
	else \
	{ \
		DBGPRINT (DBG_THREAD, "已存在线程:" << PrintThreadId ()); \
		_bTryAgain = TRUE; \
	}

}
#endif
