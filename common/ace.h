#ifndef CXXDEV_ACE_H
#define CXXDEV_ACE_H

#include "common/os.h"

namespace BaseLib
{

class CTimeValue
{
public:
	explicit CTimeValue (long sec, long usec = 0);
	void Set (long sec, long usec);

	operator timeval () const;
	operator const timeval *() const;

	long Sec (void) const;
	void Sec (long sec);

	long Usec (void) const;
	void Usec (long usec);

	unsigned long	Msec (void) const;

private:
	timeval _tv;
};

/************************************************************************/
/* 
/************************************************************************/

class CHandleSet
{
public:
	CHandleSet();
	void SetBit (HANDLE handle);
	int IsSet (HANDLE handle) const;
	void ClrBit (HANDLE handle);
	void Reset (void);
	fd_set *FdSet (void);
	operator fd_set *();

private:
	fd_set	_mask;
	int		_size;
};

/************************************************************************/
/* 
/************************************************************************/

namespace OS
{

int
select (int width,
	fd_set *rfds, fd_set *wfds, fd_set *efds,
	const CTimeValue *timeout);

int socket_init (int version_high = 1,
	int version_low = 1);

int socket_fini (void);

int sleep (const CTimeValue &tv);

}

namespace ACE
{

int
HandleReady (HANDLE handle,
	const CTimeValue *timeout,
	int read_ready,
	int write_ready,
	int exception_ready);

int
HandleReadReady (HANDLE handle,
	const CTimeValue *timeout);

int
HandleWriteReady (HANDLE handle,
	const CTimeValue *timeout);

int
HandleExceptionReady (HANDLE handle,
	const CTimeValue *timeout);

HANDLE
HandleTimedComplete (HANDLE h,
	const CTimeValue *timeout,
	int is_tli = 0);

int
HandleTimedAccept (HANDLE listener,
	CTimeValue *timeout,
	int restart);

int
EnterRecvTimedWait (HANDLE handle,
	const CTimeValue *timeout,
	int &val);

int
EnterSendTimedWait (HANDLE handle,
	const CTimeValue *timeout,
	int &val);

size_t
Recv (HANDLE handle,
	void *buf,
	size_t len,
	int flags,
	const CTimeValue *timeout);

size_t
Recv_xx (HANDLE handle,
	  void *buf,
	  size_t len,
	  int flags,
	  const CTimeValue *timeout);

size_t
Send (HANDLE handle,
	const void *buf,
	size_t n,
	int flags,
	const CTimeValue *timeout);

size_t
Recv_n (HANDLE handle,
	void *buf,
	size_t len,
	int flags,
	const CTimeValue *timeout,
	size_t *bt);

size_t
Send_n (HANDLE handle,
	const void *buf,
	size_t len,
	int flags,
	const CTimeValue *timeout,
	size_t *bt);

int
BindPort (HANDLE handle,
	UINT ip_addr = INADDR_ANY);

#define ACE_BLOCKING		0
#define ACE_NONBLOCKING		1

int
SetFlags (HANDLE handle, int flags);

}

}

#endif
