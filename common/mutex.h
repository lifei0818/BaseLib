#ifndef BaseLib_MUTEX_H
#define BaseLib_MUTEX_H

#include "common/sys_incl.h"

namespace BaseLib
{

/************************************************************************/
/*	
/************************************************************************/

template <class LOCK>
class TGuard
{
public:
	TGuard (LOCK *i)
		: _lpLock (i)
	{ if (_lpLock) _lpLock->Lock(); }

	~TGuard (void)
	{ if (_lpLock) _lpLock->UnLock(); }

protected:
	LOCK* _lpLock;
};


/************************************************************************/
/*	
/************************************************************************/

class CNullSynch
{
public:
	CNullSynch ()
	{}

	~CNullSynch (void)
	{}

	void Lock()
	{}

	void UnLock()
	{}

#ifdef _MACRO_OS_WIN32
	LPCRITICAL_SECTION	Handle()
	{ return NULL; }
#else
#endif
};

/************************************************************************/
/*  一个进程内使用
/************************************************************************/

class CThreadMutex
{
public:
#ifdef _MACRO_OS_WIN32
	CThreadMutex ()
		{ ::InitializeCriticalSection (&_handle); }

	~CThreadMutex (void)
		{ ::DeleteCriticalSection (&_handle); }

	void Lock()
		{ ::EnterCriticalSection (&_handle); }

	void UnLock()
		{ ::LeaveCriticalSection (&_handle); }

	LPCRITICAL_SECTION	Handle()
		{ return &_handle; }
#else
    CThreadMutex() 
        {
            ::pthread_mutexattr_t attr;
            ::pthread_mutexattr_init(&attr);
            ::pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
            ::pthread_mutex_init(&m_Mutex, &attr); 
            ::pthread_mutexattr_destroy(&attr);
        }
    ~CThreadMutex()
        { ::pthread_mutex_destroy(&m_Mutex); }
    void Lock()
        { ::pthread_mutex_lock(&m_Mutex); }
    void UnLock() 
        { ::pthread_mutex_unlock(&m_Mutex); }
	pthread_mutex_t	*Handle()
		{ return &m_Mutex; }
#endif

protected:
#ifdef _MACRO_OS_WIN32
	CRITICAL_SECTION _handle;
#else
    pthread_mutex_t m_Mutex;
#endif
};


/*
class ThreadSemaphore
{
public:
	ThreadSemaphore (long lMax = 0)
		{ _handle = ::CreateSemaphore (NULL, lMax, lMax, NULL); }

	~ThreadSemaphore (void)
		{ ::CloseHandle (_handle); }

	void Lock()
		{ ::WaitForSingleObject (_handle, INFINITE); }

	void UnLock()
		{ ::ReleaseSemaphore (_handle, 1, NULL); }

	HANDLE Handle()
		{ return _handle; }

protected:
	HANDLE _handle;
};
*/

/************************************************************************/
/* 	进程间使用
/************************************************************************/

class CProcessMutex
{
public:
#ifdef _MACRO_OS_WIN32
	CProcessMutex (LPCTSTR i)
		{ _handle = ::CreateMutex (NULL, FALSE, i); }

	~CProcessMutex (void)
		{ ::CloseHandle (_handle); }
	
	void Lock()
		{ ::WaitForSingleObject (_handle, INFINITE); }

	BOOL TryLock()
	{
        if (::WaitForSingleObject (_handle, 0) == WAIT_OBJECT_0)
        {
            return TRUE;
        }
        return FALSE;
    }

	void UnLock()
		{ ::ReleaseMutex (_handle); }

	HANDLE Handle()
		{ return &_handle; }

#else
	CProcessMutex (LPCTSTR i)
	{
        _handle = ::open(i, O_CREAT|O_RDWR, 0644);
	}

	~CProcessMutex (void)
	{
	    ::close (_handle);
	}
	
	void Lock()
	{
        ::lockf(_handle, F_LOCK, 0);
    }

	BOOL TryLock()
	{
        return ::lockf(_handle, F_TLOCK, 0) == 0 ? TRUE : FALSE;
    }

	void UnLock()
	{
	    ::lockf(_handle, F_ULOCK, 0);
	}

	HANDLE Handle()
	{ 
	    return _handle; 
	}
#endif

protected:
    HANDLE _handle;
};

typedef TGuard<CThreadMutex> CThreadLock;
typedef TGuard<CProcessMutex> CProcessLock;

#define BASELIB_LOCK(X) CThreadLock guard(&X)

}

#endif
