#ifndef BASELIB_TASK_H
#define BASELIB_TASK_H

#include "common/sys_incl.h"
#include "common/mutex.h"

namespace BaseLib
{

#define TASK_STOP_STATE			0	///< ������ֹͣ״̬
#define TASK_RUNNING_STATE		1	///< ����������״̬

#define TASK_BREAK		-1	///< �߳������ж��˳�
#define TASK_EXCEPTION	-2	///< �߳��쳣�ж��˳�
#define TASK_CONTINUE	1	///< �̼߳���

/// ���� (��װ�˶���߳�)
class CTask
{
public:
	CTask ();
	virtual ~CTask (void);

	/// ��ʼ�߳�
	virtual INT32 Open (INT iThreadCount = 1);

	/**
	* �����߳�
	* ��svc�ڵ��� Close () ��������
	*
	* @param BOOL bWait �Ƿ�ȴ�
	* @param DWORD dwMilliseconds �ȴ�ʱ�䣨��λ��΢�룩
	* @return INT32
	* - 1  �Ѵ���
	* - 0  �ɹ�
	* - -1 ʧ��
	*/
	virtual INT32 Close (BOOL bWait = TRUE, DWORD dwMilliseconds = INFINITE);

	/**
	 * ��������,ʵ�־�����̲߳���
	 * 
	 * @return 
	 *	- @see TASK_BREAK
	 *	- @see TASK_CONTINUE
	 */
	virtual INT32 Svc (void) = 0;

	/// �����߳�,������ʹ��,�ض�����ʹ��
	void Suspend ();

	/// �ָ��߳�,������ʹ��,�ض�����ʹ��
	void Resume ();

	/// ǿ����ֹ�߳�,������ʹ��,�ض�����ʹ��
	void Terminate ();

	UINT32 ThreadCount (void);
	std::string PrintThreadId();

	void TryAgain();
	BOOL IsAgain();

    /// �߳���ţ���1��ʼ�������̵߳��÷���0
    UINT32 ThreadIndex (void);

protected:

	/**
	 * �߳�״̬
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

// �뺯���ڵ� TryAgain ��ͬ,ʹ�ú�Ļ��������ʱ�����������ĺ���
#define TASK_TRY_AGAIN \
	if (!ThreadCount ()) \
	{ \
		Open (); \
	} \
	else \
	{ \
		DBGPRINT (DBG_THREAD, "�Ѵ����߳�:" << PrintThreadId ()); \
		_bTryAgain = TRUE; \
	}

}
#endif
