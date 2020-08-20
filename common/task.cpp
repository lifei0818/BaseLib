#include "common/sys_incl.h"
#include "common/task.h"
#include "common/function.h"
#include "common/debug.h"
#include "common/os.h"

namespace BaseLib
{

/************************************************************************/
/* 
/************************************************************************/

CTask::CTask ()
	: _threadCount (0)
	, _lpHandle (NULL)
	, _lpThreadId (NULL)
	, _lpThreadStatus (NULL)
	, _state (TASK_STOP_STATE)
	, _dwdbgThreadTick(0)
	, _stackSize (0)
	, _bTryAgain (0)
{
}

CTask::~CTask (void)
{
	Close (TRUE);
	if (_lpThreadStatus)
	{
		delete []_lpThreadStatus;
	}
}

std::string
CTask::PrintThreadId()
{
	std::ostringstream o;
	
	{
		BASELIB_LOCK (_mutex);
        if (_lpThreadId)
        {
		    for (int i = 0; i < _threadCount; i++) 
		    {
			    o << _lpThreadId[i] << ";";
		    }
        }
	}

	return o.str().c_str();
}

UINT32
CTask::ThreadIndex ()
{
    ULONG dwCurrentId = BaseLib::GetCurrentThreadID();
    BASELIB_LOCK (_mutex);
    for (int i = 0; i < _threadCount; i++)
    {
        if (_lpThreadId[i] == dwCurrentId)
        {
            return i + 1;
        }
    }

    return 0;
}

INT32
CTask::Open (INT iThreadCount)
{
	if (iThreadCount <= 0)
	{
		return -1;
	}

	BASELIB_LOCK (_mutex);
	if (_threadCount > 0) 
	{
		return 1;
	}

	State (TASK_RUNNING_STATE);
	_lpHandle = new HANDLE [iThreadCount];
#ifdef _MACRO_OS_WIN32
	_lpThreadId = new UINT32 [iThreadCount];
#else
	_lpThreadId = new pthread_t [iThreadCount];
#endif
	if (_lpThreadStatus)
	{
		delete []_lpThreadStatus;
		_lpThreadStatus = NULL;
	}
	_lpThreadStatus = new INT32 [iThreadCount];
	for (int i = 0; i < iThreadCount; i++)
	{
		_lpThreadStatus[i] = 0;
	}

	_threadCount = iThreadCount;
	for (int i = 0; i < iThreadCount; i++)
	{
#ifdef _MACRO_OS_WIN32
		_lpHandle[i] = (HANDLE)::_beginthreadex (NULL, _stackSize, &SvcFun, (CTask*)this, NULL, &_lpThreadId[i]);
		if (NULL == _lpHandle[i])
		{
			DBGPRINT(DBG_ERROR, "_beginthreadex, error=" << GetLastError());
		}
		else
		{
			DBGPRINT (DBG_THREAD, _T("创建新线程:") << _lpThreadId[i]);
		}
#else
		pthread_attr_t attr;
		pthread_attr_init (&attr);
		pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_DETACHED);
        if (::pthread_create(&_lpThreadId[i], &attr, (void *(*)(void *))&SvcFun, (CTask*)this) == 0)
        {
            _lpHandle[i] = 1;     //Linux下没有线程句柄一说
			DBGPRINT (DBG_THREAD, _T("创建新线程:") << _lpThreadId[i]);
		}
		else
		{
		    _lpHandle[i] = 0;
            DBGPRINT(DBG_ERROR, "pthread_create: " << BaseLib::OS::lasterror());
		}
		::pthread_attr_destroy (&attr);
#endif
	}

	return 0;
}

INT32
CTask::Close ( BOOL bWait /*= TRUE*/, DWORD dwMilliseconds /*= INFINITE*/ )
{
	int iThreadCount = 0;
	HANDLE* lpHandle = _lpHandle;
#ifdef _MACRO_OS_WIN32
	UINT32* lpThreadId = _lpThreadId;
#else
    pthread_t *lpThreadId = _lpThreadId;
#endif
	{
		BASELIB_LOCK (_mutex);
		
		// 清理掉,之后可以创建新的线程
		if (_threadCount > 0)
		{
			iThreadCount = _threadCount;
			State (TASK_STOP_STATE);
			_threadCount = 0;
			_lpHandle = NULL;
			_lpThreadId = NULL;
		}
	}

	if (iThreadCount > 0)
	{
		if (bWait)
		{
			DWORD dwCurrentId = BaseLib::GetCurrentThreadID();
			int i = 0;
			for (; i < iThreadCount; i++)
			{
				if (lpThreadId[i] == dwCurrentId)
				{
					// 线程内调用,不等待
					break;
				}
			}

			if (i >= iThreadCount)
			{
				for (i = 0; i < iThreadCount; i++)
				{
					if (lpHandle[i])
					{
						INT32 iStatus = -1;
#ifdef _MACRO_OS_WIN32
						if (::WaitForSingleObject (lpHandle[i], dwMilliseconds) == WAIT_TIMEOUT)
						{
							DBGPRINT (DBG_ERROR, _T("线程超时dwMilliseconds:") << dwMilliseconds);
							::TerminateThread(lpHandle[i], 0);
						}
						::GetExitCodeThread(lpHandle[i], (LPDWORD)&iStatus);
#else
						void *lpret = NULL;
                        ::pthread_join(lpThreadId[i], &lpret);
						iStatus = (LONG)lpret;
#endif
						_lpThreadStatus[i] = iStatus;
					}
				}
			}
		}

		for (int i = 0; i < iThreadCount; i++)
		{
			if (lpHandle[i])
			{
#ifdef _MACRO_OS_WIN32
				CloseHandle (lpHandle[i]);
#else
                lpHandle[i] = 0;
#endif
			}
		}

		delete []lpHandle;
		delete []lpThreadId;
	}

	return 0;
}

void
CTask::Suspend ()
{
#ifdef _MACRO_OS_WIN32
	for (int i = 0; i < _threadCount; i++)
	{
		if (_lpHandle[i])
		{
			::SuspendThread (_lpHandle[i]);
		}
	}
#endif
}

void
CTask::Resume ()
{
#ifdef _MACRO_OS_WIN32
	for (int i = 0; i < _threadCount; i++)
	{
		if (_lpHandle[i])
		{
			::ResumeThread (_lpHandle[i]);
		}
	}
#endif
}

void
CTask::Terminate ()
{
	int iThreadCount = 0;
	HANDLE* lpHandle = _lpHandle;
#ifdef _MACRO_OS_WIN32
	UINT32* lpThreadId = _lpThreadId;
#else
	pthread_t *lpThreadId = _lpThreadId;
#endif

	DWORD dwCurrentId = BaseLib::GetCurrentThreadID();
	for (int i = 0; i < _threadCount; i++)
	{
		if (_lpThreadId[i] == dwCurrentId)
		{
			// 不能在线程内调用
			return;
		}
	}

	{
		BASELIB_LOCK (_mutex);

		if (_threadCount > 0)
		{
			iThreadCount = _threadCount;
			State (TASK_STOP_STATE);
			_threadCount = 0;
			_lpHandle = NULL;
			_lpThreadId = NULL;
		}
	}

	if (iThreadCount > 0)
	{
		for (int i = 0; i < iThreadCount; i++)
		{
			if (lpHandle[i])
			{
#ifdef _MACRO_OS_WIN32
				::TerminateThread(lpHandle[i], 0);
				::CloseHandle (lpHandle[i]);
#else
				::pthread_cancel(lpThreadId[i]);
				lpHandle[i] = 0;
#endif
				_lpThreadStatus[i] = -1;
			}
		}
		delete []_lpHandle;
		delete []_lpThreadId;
	}
}

void
CTask::State (LONG state)
{
	_state = state;
}

LONG
CTask::State (void)
{
	return _state;
}

UINT32
CTask::ThreadCount (void)
{
	BASELIB_LOCK (_mutex);
	return _threadCount;
}

INT32 CTask::SvcRun (void)
{
	return Svc ();
}

UINT32
WINAPI
CTask::SvcFun (LPVOID lpParam)
{
	INT32 svc_status = -1;
	CTask *t = (CTask *) lpParam;
	if (t)
	{
		do
		{
			svc_status = t->SvcRun ();
			if (svc_status < 0)
			{
				break;
			}
		}
		while (t->State() != TASK_STOP_STATE);
	}

	DBGPRINT (DBG_THREAD, _T("线程退出 Id:") << BaseLib::GetCurrentThreadID());
	
//	_endthreadex (svc_status);
	return (DWORD) svc_status;
}

void CTask::TryAgain()
{
	if (!ThreadCount ())
	{
		Open ();
	}
	else
	{
		DBGPRINT (DBG_THREAD, PrintThreadId ());
		_bTryAgain = TRUE;
	}
}

BOOL CTask::IsAgain()
{
	BOOL bTryAgain = _bTryAgain;
	_bTryAgain = FALSE;

	return bTryAgain;
}


}
