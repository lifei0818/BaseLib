#include "common/sys_incl.h"
#include "common/ace.h"

namespace BaseLib
{

/************************************************************************/
/* 
/************************************************************************/

	void
	CTimeValue::Set (long sec, long usec)
	{
		this->_tv.tv_sec = sec;
		this->_tv.tv_usec = usec;
	}

	
	CTimeValue::CTimeValue (long sec, long usec)
	{
		this->Set (sec, usec);
	}

	
	CTimeValue::operator const timeval * () const
	{
		return (const timeval *) &this->_tv;
	}

	long
	CTimeValue::Sec (void) const
	{
		return this->_tv.tv_sec;
	}

	void
	CTimeValue::Sec (long sec)
	{
		this->_tv.tv_sec = sec;
	}

	long
	CTimeValue::Usec (void) const
	{
		return this->_tv.tv_usec;
	}

	void
	CTimeValue::Usec (long usec)
	{
		this->_tv.tv_usec = usec;
	}

	unsigned long
	CTimeValue::Msec (void) const
	{
		unsigned long secs = this->_tv.tv_sec * 1000 + this->_tv.tv_usec / 1000;
		return secs;
//		return ACE_Utils::truncate_cast<unsigned long> (secs);
	}

	
	CTimeValue::operator timeval () const
	{
		return this->_tv;
	}

/************************************************************************/
/* 
/************************************************************************/
	CHandleSet::CHandleSet (void)
	{
		this->Reset ();
	}

	int
	CHandleSet::IsSet (HANDLE handle) const
	{
		return FD_ISSET (handle,
			&this->_mask);
	}

	void
	CHandleSet::SetBit (HANDLE handle)
	{
		if ((handle != INVALID_HANDLE_VALUE)
			&& (!this->IsSet (handle)))
		{
			FD_SET ((SOCKET) handle,
				&this->_mask);
			++this->_size;
		}
	}

	void
	CHandleSet::ClrBit (HANDLE handle)
	{
		if ((handle != INVALID_HANDLE_VALUE) &&
			(this->IsSet (handle)))
		{
			FD_CLR ((SOCKET) handle,
				&this->_mask);
			--this->_size;
		}
	}

	void
	CHandleSet::Reset (void)
	{
		this->_size = 0;
		FD_ZERO (&this->_mask);
	}

	fd_set *
	CHandleSet::FdSet (void)
	{
		if (this->_size > 0)
			return (fd_set *) &this->_mask;
		else
			return (fd_set *) 0;
	}

	CHandleSet::operator fd_set *()
	{
		return FdSet();
	}

/************************************************************************/
/* 
/************************************************************************/

	int
	OS::select (int width,
		fd_set *rfds, fd_set *wfds, fd_set *efds,
		const CTimeValue *timeout)
	{
#ifdef _MACRO_OS_WIN32
		const timeval *timep = (timeout == 0 ? (const timeval *)0 : *timeout);
#else
        timeval copy;
        timeval *timep = NULL;

        if (timeout != NULL)
        {
            copy = *timeout;
            timep = &copy;
        }
#endif

		SOCKCALL_RETURN (::select (width,
			rfds,
			wfds,
			efds,
			timep),
			int, -1);
	}

	int
	OS::socket_init (int version_high, int version_low)
	{
#ifdef _MACRO_OS_WIN32
		WORD version_requested = MAKEWORD (version_high, version_low);
		WSADATA wsa_data;
		return ::WSAStartup (version_requested, &wsa_data);
#else
        return 0;
#endif
	}

	int
	OS::socket_fini (void)
	{
#ifdef _MACRO_OS_WIN32
		::WSACleanup ();
#endif
		return 0;
	}

	int
	OS::sleep (const CTimeValue &tv)
	{
#ifdef _MACRO_OS_WIN32
		::Sleep (tv.Msec ());
#else
        struct timeval t;
        t.tv_sec = tv.Sec();
		t.tv_usec = tv.Usec();
        ::select(0, NULL, NULL, NULL, &t);
#endif
		return 0;
	}

/************************************************************************/
/* 
/************************************************************************/
	int
	ACE::HandleReady (HANDLE handle,
		const CTimeValue *timeout,
		int read_ready,
		int write_ready,
		int exception_ready)
	{
		CHandleSet handle_set;
		handle_set.SetBit (handle);

		// Wait for data or for the timeout to elapse.
		int select_width;
		// This arg is ignored on Windows and causes pointer truncation
		// warnings on 64-bit compiles.
#ifdef _MACRO_OS_WIN32
		select_width = 0;
#else
        select_width = (int)handle + 1;
#endif
        int result = OS::select (select_width,
			read_ready ? handle_set.FdSet() : 0, // read_fds.
			write_ready ? handle_set.FdSet() : 0, // write_fds.
			exception_ready ? handle_set.FdSet() : 0, // exception_fds.
			timeout);

		switch (result)
		{
		case 0:  // Timer expired.
//			errno = ETIME;
			/* FALLTHRU */
		case -1: // we got here directly - select() returned -1.
			return -1;
		case 1: // Handle has data.
			/* FALLTHRU */
		default: // default is case result > 0; return a
			// ACE_ASSERT (result == 1);
			return result;
		}
	}


	int
	ACE::HandleReadReady (HANDLE handle,
		const CTimeValue *timeout)
	{
		return ACE::HandleReady (handle,
			timeout,
			1,
			0,
			0);
	}

	int
	ACE::HandleWriteReady (HANDLE handle,
		const CTimeValue *timeout)
	{
		return ACE::HandleReady (handle,
			timeout,
			0,
			1,
			0);
	}

	int
	ACE::HandleExceptionReady (HANDLE handle,
		const CTimeValue *timeout)
	{
		return ACE::HandleReady (handle,
			timeout,
			0,
			0,
			1);
	}

	HANDLE
	ACE::HandleTimedComplete (HANDLE h,
		const CTimeValue *timeout,
		int is_tli)
	{
		CHandleSet rd_handles;
		CHandleSet wr_handles;

		rd_handles.SetBit (h);
		wr_handles.SetBit (h);

		CHandleSet ex_handles;
		ex_handles.SetBit (h);

		bool need_to_check = false;
		bool known_failure = false;

        int select_width;
#ifdef _MACRO_OS_WIN32
        select_width = 0;
#else
        select_width = (int)h + 1;
#endif
        int n = OS::select (select_width,
			0,
			wr_handles,
			ex_handles,
			timeout);

		if (n <= 0)
		{
			if (n == 0 && timeout != 0)
				errno = ETIMEDOUT;
			return INVALID_HANDLE_VALUE;
		}

		if (ex_handles.IsSet (h))
		{
			return INVALID_HANDLE_VALUE;
		}

		return h;
	}

	int
	ACE::HandleTimedAccept (HANDLE listener,
		CTimeValue *timeout,
		int restart)
	{
		if (listener == INVALID_HANDLE_VALUE)
			return -1;

		CHandleSet rd_handle;
		rd_handle.SetBit (listener);

		for (;;)
		{
			int select_width;
#ifdef _MACRO_OS_WIN32
            select_width = 0;
#else
            select_width = (int)listener + 1;
#endif
			int n = OS::select (select_width,
				rd_handle, 0, 0,
				timeout);

			switch (n)
			{
			case -1:
				if (errno == EINTR && restart)
					continue;
				else
					return -1;
				/* NOTREACHED */
			case 0:
				if (timeout != 0
					&& timeout->Sec () == 0
					&& timeout->Usec () == 0)
					errno = EWOULDBLOCK;
				else
					errno = ETIMEDOUT;
				return -1;
				/* NOTREACHED */
			case 1:
				return 0;
				/* NOTREACHED */
			default:
				errno = EINVAL;
				return -1;
				/* NOTREACHED */
			}
		}
	}

	int
	ACE::EnterRecvTimedWait (HANDLE handle,
		const CTimeValue *timeout,
		int &val)
	{
		int result = ACE::HandleReadReady (handle,
			timeout);

		if (result == -1)
			return -1;

		ACE::SetFlags (handle, ACE_NONBLOCKING);

		return result;
	}

	int
	ACE::EnterSendTimedWait (HANDLE handle,
		const CTimeValue *timeout,
		int &val)
	{
		int result = ACE::HandleWriteReady (handle,
			timeout);

		if (result == -1)
			return -1;

		ACE::SetFlags (handle, ACE_NONBLOCKING);

		return result;
	}


/*	int
	ACE::clr_flags (HANDLE handle, int flags)
	{
		switch (flags)
		{
		case ACE_NONBLOCK:
			// nonblocking argument (1)
			// blocking:            (0)
			{
				int nonblock = 0;
				return OS::ioctl (handle, FIONBIO, &nonblock);
			}
		default:
			ACE_NOTSUP_RETURN (-1);
		}
	}

	void
	ACE::RestoreNonBlockingMode (HANDLE handle,
		int val)
	{
		if (ACE_BIT_DISABLED (val,
			ACE_NONBLOCK))
		{
			// Save/restore errno.
			ACE_Errno_Guard error (errno);
			// Only disable ACE_NONBLOCK if we weren't in non-blocking mode
			// originally.
			ACE::clr_flags (handle, ACE_NONBLOCK);
		}
	}
*/

	size_t
	ACE::Recv (HANDLE handle,
		void *buf,
		size_t len,
		int flags,
		const CTimeValue *timeout)
	{
		if (timeout == 0)
			return OS::recv (handle, (char *) buf, len, flags);
		else
		{
			int val = 0;
			if (ACE::EnterRecvTimedWait (handle, timeout, val) == -1)
				return -1;
			else
			{
				size_t bytes_transferred =	OS::recv (handle, (char *) buf, len, flags);
				ACE::SetFlags (handle, ACE_BLOCKING);
				return bytes_transferred;
			}
		}
	}
	
	size_t
	ACE::Recv_xx (HANDLE handle,
		void *buf,
		size_t len,
		int flags,
		const CTimeValue *timeout)
	{
		if (timeout == 0)
			return OS::recv (handle, (char *) buf, len, flags);
		else
		{
			int val = 0;
			if (ACE::EnterRecvTimedWait (handle, timeout, val) == -1)
				return -2;
			else
			{
				size_t bytes_transferred =	OS::recv (handle, (char *) buf, len, flags);
				ACE::SetFlags (handle, ACE_BLOCKING);
				return bytes_transferred;
			}
		}
	}

	size_t
	ACE::Send (HANDLE handle,
		const void *buf,
		size_t n,
		int flags,
		const CTimeValue *timeout)
	{
		if (timeout == 0)
			return OS::send (handle, (const char *) buf, n, flags);
		else
		{
			int val = 0;
			if (ACE::EnterSendTimedWait (handle, timeout, val) == -1)
				return -1;
			else
			{
				size_t bytes_transferred = OS::send (handle, (const char *) buf, n, flags);
				ACE::SetFlags (handle, ACE_BLOCKING);
				return bytes_transferred;
			}
		}
	}

	size_t
	ACE::Recv_n (HANDLE handle,
				void *buf,
				size_t len,
				int flags,
				const CTimeValue *timeout,
				size_t *bt)
	{
		size_t temp;
		size_t &bytes_transferred = bt == 0 ? temp : *bt;
		size_t n;
		size_t result = 0;
		int error = 0;



		for (bytes_transferred = 0;
			bytes_transferred < len;
			bytes_transferred += n)
		{
			// Try to transfer as much of the remaining data as possible.
			// Since the socket is in non-blocking mode, this call will not
			// block.
			n = Recv (handle,
				static_cast <char *> (buf) + bytes_transferred,
				len - bytes_transferred,
				flags,
				0);
			// Check for errors.
			if (n == 0 ||
				n == -1)
			{
				// Check for possible blocking.
				if (n == -1 &&
					errno == EWOULDBLOCK)
				{
					// Wait upto <timeout> for the blocking to subside.
					int rtn = ACE::HandleReadReady (handle,
						timeout);
					// Did select() succeed?
					if (rtn != -1)
					{
						// Blocking subsided in <timeout> period.  Continue
						// data transfer.
						n = 0;
						continue;
					}
				}

				// Wait in select() timed out or other data transfer or
				// select() failures.
                //如果没有指定超时，对端关闭不认为是错误，适用于不知道接收长度的情况下
                if (timeout != 0 || n != 0)
                {
				    error = 1;
				    result = n;
			    }
				break;
			}
		}

		if (error)
			return result;
		else
			return static_cast<size_t> (bytes_transferred);
	}

	size_t
	ACE::Send_n (HANDLE handle,
				const void *buf,
				size_t len,
				int flags,
				const CTimeValue *timeout,
				size_t *bt)
	{
		size_t temp;
		size_t &bytes_transferred = bt == 0 ? temp : *bt;
		size_t n;
		size_t result = 0;
		int error = 0;

		for (bytes_transferred = 0;
			bytes_transferred < len;
			bytes_transferred += n)
		{
			// Try to transfer as much of the remaining data as possible.
			// Since the socket is in non-blocking mode, this call will not
			// block.
			n = Send (handle,
								(char *) buf + bytes_transferred,
								len - bytes_transferred,
								flags,
								0);

			// Check for errors.
			if (n == 0 ||
				n == -1)
			{
				// Check for possible blocking.
				if (n == -1 &&
					errno == EWOULDBLOCK || errno == ENOBUFS)
				{
					// Wait upto <timeout> for the blocking to subside.
					int rtn = ACE::HandleWriteReady (handle,
														timeout);

					// Did select() succeed?
					if (rtn != -1)
					{
						// Blocking subsided in <timeout> period.  Continue
						// data transfer.
						n = 0;
						continue;
					}
				}

				// Wait in select() timed out or other data transfer or
				// select() failures.
				error = 1;
				result = n;
				break;
			}
		}

		if (error)
			return result;
		else
			return bytes_transferred;
	}

	int
	ACE::BindPort (HANDLE handle, UINT ip_addr)
	{
//		CINetAddr Addr ((USHORT)0, ip_addr);
        sockaddr_in Addr;
        memset (&Addr, 0, sizeof (Addr));
        Addr.sin_family = AF_INET;
        Addr.sin_port   = 0;
        Addr.sin_addr.s_addr = htonl(ip_addr);

		return OS::bind (handle, (sockaddr*)&Addr, sizeof(Addr));
	}

	int
	ACE::SetFlags (HANDLE handle, int flags)
	{
		// nonblocking argument (1)
		// blocking:            (0)
#ifdef _MACRO_OS_WIN32
		return OS::ioctl (handle, FIONBIO, &flags);
#else
        int val;
        
        if ((val = ::fcntl(handle, F_GETFL, 0)) < 0)
        {
            return -1;
        }
        
        if (flags == ACE_NONBLOCKING)
        {
            val |= O_NONBLOCK;
            return ::fcntl(handle, F_SETFL, flags);
        }
        else
        {
            val &= ~O_NONBLOCK;
            return ::fcntl(handle, F_SETFL, flags);
        }
#endif
	}
}
