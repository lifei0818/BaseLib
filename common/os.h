#ifndef CXXDEV_OS_H
#define CXXDEV_OS_H

#include "common/sys_incl.h"

namespace BaseLib
{
#ifdef _MACRO_OS_WIN32

#define EWOULDBLOCK             WSAEWOULDBLOCK
#define EINPROGRESS             WSAEINPROGRESS
#define EALREADY                WSAEALREADY
#define ENOTSOCK                WSAENOTSOCK
#define EDESTADDRREQ            WSAEDESTADDRREQ
#define EMSGSIZE                WSAEMSGSIZE
#define EPROTOTYPE              WSAEPROTOTYPE
#define ENOPROTOOPT             WSAENOPROTOOPT
#define EPROTONOSUPPORT         WSAEPROTONOSUPPORT
#define ESOCKTNOSUPPORT         WSAESOCKTNOSUPPORT
#define EOPNOTSUPP              WSAEOPNOTSUPP
#define EPFNOSUPPORT            WSAEPFNOSUPPORT
#define EAFNOSUPPORT            WSAEAFNOSUPPORT
#define EADDRINUSE              WSAEADDRINUSE
#define EADDRNOTAVAIL           WSAEADDRNOTAVAIL
#define ENETDOWN                WSAENETDOWN
#define ENETUNREACH             WSAENETUNREACH
#define ENETRESET               WSAENETRESET
#define ECONNABORTED            WSAECONNABORTED
#define ECONNRESET              WSAECONNRESET
#define ENOBUFS                 WSAENOBUFS
#define EISCONN                 WSAEISCONN
#define ENOTCONN                WSAENOTCONN
#define ESHUTDOWN               WSAESHUTDOWN
#define ETOOMANYREFS            WSAETOOMANYREFS
#define ETIMEDOUT               WSAETIMEDOUT
#define ECONNREFUSED            WSAECONNREFUSED
#define ELOOP                   WSAELOOP
//	#define ENAMETOOLONG            WSAENAMETOOLONG
#define EHOSTDOWN               WSAEHOSTDOWN
#define EHOSTUNREACH            WSAEHOSTUNREACH
//	#define ENOTEMPTY               WSAENOTEMPTY
#define EPROCLIM                WSAEPROCLIM
#define EUSERS                  WSAEUSERS
#define EDQUOT                  WSAEDQUOT
#define ESTALE                  WSAESTALE
#define EREMOTE                 WSAEREMOTE

#endif  //_MACRO_OS_WIN32

# define ACE_BIT_ENABLED(WORD, BIT) (((WORD) & (BIT)) != 0)
# define ACE_BIT_DISABLED(WORD, BIT) (((WORD) & (BIT)) == 0)
# define ACE_BIT_CMP_MASK(WORD, BIT, MASK) (((WORD) & (BIT)) == MASK)
# define ACE_SET_BITS(WORD, BITS) (WORD |= (BITS))
# define ACE_CLR_BITS(WORD, BITS) (WORD &= ~(BITS))

#ifdef _MACRO_OS_WIN32

#define SOCKCALL_RETURN(OP,TYPE,FAILVALUE) \
	do\
	{\
		TYPE result = (TYPE) OP; \
		if (result == FAILVALUE) \
		{ \
			int ___ = ::WSAGetLastError (); \
			errno = ___; \
			return (TYPE) FAILVALUE; \
		} else return result; \
	} while (0)

#define SOCKCALL(OP,TYPE,FAILVALUE,RESULT) \
	do \
	{ \
		RESULT = (TYPE) OP; \
		if (RESULT == FAILVALUE) \
		{ \
			int ___ = ::WSAGetLastError ();\
			errno = ___; RESULT = FAILVALUE; \
		} \
	} while (0)

#else

#define SOCKCALL_RETURN(OP,TYPE,FAILVALUE) \
	do\
	{\
		TYPE result = (TYPE) OP; \
		return result; \
	} while (0)

#define SOCKCALL(OP,TYPE,FAILVALUE,RESULT) \
	do \
	{ \
		RESULT = (TYPE) OP; \
	} while (0)

#endif //_MACRO_OS_WIN32


#ifdef _MACRO_OS_WIN32
    typedef int       ACE_SOCKET_LEN;
#else
    typedef socklen_t ACE_SOCKET_LEN;
#endif

namespace OS
{
int lasterror(void);

HANDLE accept (HANDLE handle,
	struct sockaddr *addr,
	int *addrlen);

int bind (HANDLE s,
	struct sockaddr *name,
	int namelen);

int closesocket (HANDLE s);

int connect (HANDLE handle,
	struct sockaddr *addr,
	int addrlen);

int getpeername (HANDLE handle,
	struct sockaddr *addr,
	int *addrlen);

int getsockname (HANDLE handle,
	struct sockaddr *addr,
	int *addrlen);

int getsockopt (HANDLE handle,
	int level,
	int optname,
	char *optval,
	int *optlen);

int listen (HANDLE handle,
	int backlog);

size_t recv (HANDLE handle,
	char *buf,
	size_t len,
	int flags = 0);

size_t recvfrom (HANDLE handle,
	char *buf,
	size_t len,
	int flags,
	struct sockaddr *addr,
	int *addrlen);

size_t send (HANDLE handle,
	const char *buf,
	size_t len,
	int flags = 0);

size_t sendto (HANDLE handle,
	const char *buf,
	size_t len,
	int flags,
	const struct sockaddr *addr,
	int addrlen);

int setsockopt (HANDLE handle,
	int level,
	int optname,
	const char *optval,
	int optlen);

int shutdown (HANDLE handle,
	int how);

HANDLE socket (int protocol_family,
	int type,
	int proto);

#ifdef _MACRO_OS_WIN32
int ioctl (HANDLE handle,
	int cmd,
	void *val);
#endif

int
msleep (u_int milliseconds);
	
int
sleep (u_int seconds);

int
set_errno_to_wsa_last_error (void);

int
close (HANDLE handle);

size_t
write (HANDLE handle, const void *buf, size_t nbyte);

size_t
read (HANDLE handle, void *buf, size_t len);

size_t
write_n (HANDLE handle,
	const void *buf,
	size_t len,
	size_t *bt = 0);

size_t
read_n (HANDLE handle,
	void *buf,
	size_t len,
	size_t *bt = 0);

BOOL
ipvn_check (int pf);

#define ipv6_enabled        ifvn_check(AF_INET6)
#define ipv4_enabled        ifvn_check(AF_INET4)

#ifdef _MACRO_OS_WIN32

LPSECURITY_ATTRIBUTES
default_win32_security_attributes_r (LPSECURITY_ATTRIBUTES sa,
	LPSECURITY_ATTRIBUTES sa_buffer,
	SECURITY_DESCRIPTOR* sd_buffer);

HANDLE
open (const char *filename,
	int mode,
	int perms,
	LPSECURITY_ATTRIBUTES sa);

#endif

}

}

#endif
