#include "common/sys_incl.h"
#include "common/os.h"

namespace BaseLib
{
    int 
    OS::lasterror(void)
    {
#ifdef _MACRO_OS_WIN32
		return ::WSAGetLastError ();
#else
		return errno;
#endif
    }

	HANDLE
	OS::accept (HANDLE handle,
		struct sockaddr *addr,
		int *addrlen)
	{
		SOCKCALL_RETURN (::accept ((SOCKET) handle,
			addr,
			(ACE_SOCKET_LEN *)addrlen),
			HANDLE,
			INVALID_HANDLE_VALUE);
	}

	int
	OS::bind (HANDLE handle, struct sockaddr *addr, int addrlen)
	{
		SOCKCALL_RETURN (::bind ((SOCKET) handle,
			addr,
			addrlen), int, -1);
	}

	int
	OS::closesocket (HANDLE handle)
	{
#ifdef _MACRO_OS_WIN32
		SOCKCALL_RETURN (::closesocket ((SOCKET) handle), int, -1);
#else
        SOCKCALL_RETURN (::close (handle), int, -1);
#endif
	}

	int
	OS::connect (HANDLE handle,
		struct sockaddr *addr,
		int addrlen)
	{
		SOCKCALL_RETURN (::connect ((SOCKET) handle,
			addr,
			addrlen), int, -1);
	}

	int 
	OS::getpeername (HANDLE handle,
		struct sockaddr *addr,
		int *addrlen)
	{
		SOCKCALL_RETURN (::getpeername ((SOCKET) handle,
			addr,
			(ACE_SOCKET_LEN *)addrlen),
			int,
			-1);
	}

	int
	OS::getsockname (HANDLE handle,
		struct sockaddr *addr,
		int *addrlen)
	{
		SOCKCALL_RETURN (::getsockname ((SOCKET) handle,
			addr,
			(ACE_SOCKET_LEN *)addrlen),
			int, -1);
	}

	int
	OS::getsockopt (HANDLE handle,
		int level,
		int optname,
		char *optval,
		int *optlen)
	{
		SOCKCALL_RETURN (::getsockopt ((SOCKET) handle,
			level,
			optname,
			optval,
			(ACE_SOCKET_LEN *)optlen),
			int,
			-1);
	}

	int
	OS::listen (HANDLE handle, int backlog)
	{
		SOCKCALL_RETURN (::listen ((SOCKET) handle, backlog), int, -1);
	}

	size_t
	OS::recv (HANDLE handle, char *buf, size_t len, int flags)
	{
		SOCKCALL_RETURN (::recv ((SOCKET) handle, buf,
			static_cast<int> (len), flags), size_t, -1);
	}

	size_t
	OS::recvfrom (HANDLE handle,
		char *buf,
		size_t len,
		int flags,
		struct sockaddr *addr,
		int *addrlen)
	{
#ifdef _MACRO_OS_WIN32
		int const shortened_len = static_cast<int> (len);
		int const result = ::recvfrom ((SOCKET) handle,
			buf,
			shortened_len,
			flags,
			addr,
			addrlen);
		if (result == SOCKET_ERROR)
		{
			OS::set_errno_to_wsa_last_error ();
			if (errno == WSAEMSGSIZE &&
				ACE_BIT_ENABLED (flags, MSG_PEEK))
				return shortened_len;
			else
				return -1;
		}
		else
			return result;
#else
        SOCKCALL_RETURN (::recvfrom ((SOCKET) handle,
                                     buf,
                                     len,
                                     flags,
                                     addr,
                                     (ACE_SOCKET_LEN *)addrlen),
                         size_t, -1);
#endif
	}


	size_t
	OS::send (HANDLE handle, const char *buf, size_t len, int flags)
	{
#ifdef _MACRO_OS_WIN32
		SOCKCALL_RETURN (::send ((SOCKET) handle,
			buf,
			static_cast<int> (len),
			flags), size_t, -1);
#else
		SOCKCALL_RETURN (::send ((SOCKET) handle,
			buf,
			static_cast<int> (len),
			flags|MSG_NOSIGNAL), size_t, -1);
#endif
	}

	size_t
	OS::sendto (HANDLE handle,
		const char *buf,
		size_t len,
		int flags,
		const struct sockaddr *addr,
		int addrlen)
	{
#ifdef _MACRO_OS_WIN32
		SOCKCALL_RETURN (::sendto ((SOCKET) handle,
			buf,
			static_cast<int> (len),
			flags,
			const_cast<struct sockaddr *> (addr),
			addrlen),
			size_t, -1);
#else
		SOCKCALL_RETURN (::sendto ((SOCKET) handle,
			buf,
			len,
			flags,
			const_cast<struct sockaddr *> (addr),
			addrlen),
			size_t, -1);
#endif
	}

	int
	OS::setsockopt (HANDLE handle,
		int level,
		int optname,
		const char *optval,
		int optlen)
	{
		SOCKCALL_RETURN (::setsockopt ((SOCKET) handle,
			level,
			optname,
			optval,
			optlen),
			int,
			-1);
	}

	int
	OS::shutdown (HANDLE handle, int how)
	{
		SOCKCALL_RETURN (::shutdown ((SOCKET) handle, how), int, -1);
	}

	HANDLE
	OS::socket (int domain,
		int type,
		int proto)
	{
		SOCKCALL_RETURN (::socket (domain,
			type,
			proto),
			HANDLE,
			INVALID_HANDLE_VALUE);
	}

#ifdef _MACRO_OS_WIN32
	int
	OS::ioctl (HANDLE handle,
		int cmd,
		void *val)
	{
		SOCKET sock = (SOCKET) handle;
		SOCKCALL_RETURN (::ioctlsocket (sock, cmd, reinterpret_cast<unsigned long *> (val)), int, -1);
	}
#endif

	int
	OS::msleep (u_int milliseconds)
	{
#ifdef _MACRO_OS_WIN32
		::Sleep (milliseconds);
#else
        ::usleep(milliseconds * 1000);
#endif
		return 0;
	}

	int
	OS::sleep (u_int seconds)
	{
#ifdef _MACRO_OS_WIN32
		::Sleep (seconds * 1000L);
#else
        ::sleep(seconds);
#endif
		return 0;
	}

	int
	OS::set_errno_to_wsa_last_error (void)
	{
#ifdef _MACRO_OS_WIN32
		return errno = ::WSAGetLastError ();
#else
		return errno;
#endif
	}

	int
	OS::close (HANDLE handle)
	{
#ifdef _MACRO_OS_WIN32
		return ::CloseHandle (handle);
#else
		return ::close (handle);
#endif
	}

	size_t
	OS::write (HANDLE handle, const void *buf, size_t nbyte)
	{
#ifdef _MACRO_OS_WIN32
		DWORD bytes_written;
		DWORD short_nbyte = static_cast<DWORD> (nbyte);
		if (::WriteFile (handle, buf, short_nbyte, &bytes_written, 0))
			return (size_t) bytes_written;
		return -1;
#else
		return ::write (handle, buf, nbyte);
#endif
	}

	size_t
	OS::read (HANDLE handle, void *buf, size_t len)
	{
#ifdef _MACRO_OS_WIN32
		DWORD ok_len;
		if (::ReadFile (handle, buf, static_cast<DWORD> (len), &ok_len, 0))
			return (size_t) ok_len;
		return -1;
#else
		return ::read (handle, buf, len);
#endif
	}

	size_t
	OS::write_n (HANDLE handle,
		const void *buf,
		size_t len,
		size_t *bt)
	{
		size_t temp;
		size_t &bytes_transferred = bt == 0 ? temp : *bt;
		size_t n;

		for (bytes_transferred = 0;
			bytes_transferred < len;
			bytes_transferred += n)
		{
			n = OS::write (handle,
				(char *) buf + bytes_transferred,
				len - bytes_transferred);

			if (n == -1 || n == 0)
				return n;
		}

		return bytes_transferred;
	}

	size_t
	OS::read_n (HANDLE handle,
		void *buf,
		size_t len,
		size_t *bt)
	{
		size_t temp;
		size_t &bytes_transferred = bt == 0 ? temp : *bt;
		size_t n = 0;

		for (bytes_transferred = 0;
			bytes_transferred < len;
			bytes_transferred += n)
		{
			n = OS::read (handle,
				(char *) buf + bytes_transferred,
				len - bytes_transferred);

			if (n == -1 || n == 0)
				return n;
		}

		return bytes_transferred;
	}

	BOOL
	OS::ipvn_check (int pf)
	{
    	HANDLE handle = OS::socket (pf, SOCK_DGRAM, 0);
    	if (handle == INVALID_HANDLE_VALUE)
    	{
    	    return FALSE;
    	}
   	    OS::closesocket(handle);
    	return TRUE;
	}

#ifdef _MACRO_OS_WIN32
	LPSECURITY_ATTRIBUTES
	OS::default_win32_security_attributes_r (LPSECURITY_ATTRIBUTES sa,
		LPSECURITY_ATTRIBUTES sa_buffer,
		SECURITY_DESCRIPTOR* sd_buffer)
	{
#if defined (ACE_DEFINES_DEFAULT_WIN32_SECURITY_ATTRIBUTES)
		if (sa == 0)
		{
			if (sa_buffer != 0 && sd_buffer != 0)
			{
				InitializeSecurityDescriptor
					(sd_buffer, SECURITY_DESCRIPTOR_REVISION);
				SetSecurityDescriptorDacl (sd_buffer, TRUE, NULL, FALSE);
				sa_buffer->nLength = sizeof(SECURITY_ATTRIBUTES);
				sa_buffer->lpSecurityDescriptor = sd_buffer;
				sa_buffer->bInheritHandle       = TRUE;
				sa = sa_buffer;
			}
		}
		return sa;
#else /* !ACE_DEFINES_DEFAULT_WIN32_SECURITY_ATTRIBUTES */
		return sa;
#endif /* ACE_DEFINES_DEFAULT_WIN32_SECURITY_ATTRIBUTES */
	}

	HANDLE
	OS::open (const char *filename,
				int mode,
				int perms,
				LPSECURITY_ATTRIBUTES sa)
	{
		DWORD access = GENERIC_READ;
		if (ACE_BIT_ENABLED (mode, O_WRONLY))
			access = GENERIC_WRITE;
		else if (ACE_BIT_ENABLED (mode, O_RDWR))
			access = GENERIC_READ | GENERIC_WRITE;

		DWORD creation = OPEN_EXISTING;

		if ((mode & (_O_CREAT | _O_EXCL)) == (_O_CREAT | _O_EXCL))
			creation = CREATE_NEW;
		else if ((mode & (_O_CREAT | _O_TRUNC)) == (_O_CREAT | _O_TRUNC))
			creation = CREATE_ALWAYS;
		else if (ACE_BIT_ENABLED (mode, _O_CREAT))
			creation = OPEN_ALWAYS;
		else if (ACE_BIT_ENABLED (mode, _O_TRUNC))
			creation = TRUNCATE_EXISTING;

		DWORD flags = 0;

		if (ACE_BIT_ENABLED (mode, _O_TEMPORARY))
			flags |= FILE_FLAG_DELETE_ON_CLOSE | FILE_ATTRIBUTE_TEMPORARY;

		if (ACE_BIT_ENABLED (mode, FILE_FLAG_WRITE_THROUGH))
			flags |= FILE_FLAG_WRITE_THROUGH;
		if (ACE_BIT_ENABLED (mode, FILE_FLAG_OVERLAPPED))
			flags |= FILE_FLAG_OVERLAPPED;
		if (ACE_BIT_ENABLED (mode, FILE_FLAG_NO_BUFFERING))
			flags |= FILE_FLAG_NO_BUFFERING;
		if (ACE_BIT_ENABLED (mode, FILE_FLAG_RANDOM_ACCESS))
			flags |= FILE_FLAG_RANDOM_ACCESS;
		if (ACE_BIT_ENABLED (mode, FILE_FLAG_SEQUENTIAL_SCAN))
			flags |= FILE_FLAG_SEQUENTIAL_SCAN;
		if (ACE_BIT_ENABLED (mode, FILE_FLAG_DELETE_ON_CLOSE))
			flags |= FILE_FLAG_DELETE_ON_CLOSE;
		if (ACE_BIT_ENABLED (mode, FILE_FLAG_BACKUP_SEMANTICS))
			flags |= FILE_FLAG_BACKUP_SEMANTICS;
		if (ACE_BIT_ENABLED (mode, FILE_FLAG_POSIX_SEMANTICS))
			flags |= FILE_FLAG_POSIX_SEMANTICS;


		DWORD shared_mode = perms;
		SECURITY_ATTRIBUTES sa_buffer;
		SECURITY_DESCRIPTOR sd_buffer;

		HANDLE h = ::CreateFile (filename,
										access,
										shared_mode,
										OS::default_win32_security_attributes_r
										(sa, &sa_buffer, &sd_buffer),
										creation,
										flags,
										0);

		return h;
	}
#endif

}
