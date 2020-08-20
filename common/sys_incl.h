#ifndef SYS_INCL_H
#define SYS_INCL_H

#include "os_macro.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <math.h>

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <stack>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <memory>
#include <iterator>
#include <stdexcept>
#include <iomanip>

#if defined (_MACRO_OS_WIN32)
    #include <ntstatus.h>
#define WIN32_NO_STATUS
	#include <winsock2.h>
	#include <ws2tcpip.h>
    #define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
	#include <windows.h>
    #include <tchar.h>
	#include <iphlpapi.h>
	#include <Nb30.h>
	#include <io.h>
    #include <process.h>
    #include <direct.h>
#elif defined (_MACRO_OS_LINUX)

// 支持大文件
#ifndef _FILE_OFFSET_BITS
#define _FILE_OFFSET_BITS   64
#endif

#ifndef _USE_LARGEFILE64
#define _USE_LARGEFILE64
#endif

#ifndef _LARGEFILE64_SOURCE
#define _LARGEFILE64_SOURCE
#endif


    #include <strings.h>
    #include <pthread.h>
    #include <unistd.h>
    #include <semaphore.h>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <sys/un.h>
    #include <sys/stat.h>
    #include <sys/ioctl.h>
    #include <sys/time.h>
    #include <sys/times.h>
    #include <sys/utsname.h>
    #include <netinet/in.h>
    #include <netinet/ip_icmp.h>
    #include <arpa/inet.h>
    #include <net/if.h>
    #include <net/if_arp.h>
    #include <net/ethernet.h>
    #include <netpacket/packet.h>
    #include <netdb.h>
    #include <dirent.h>
    #include <fnmatch.h>
    #include <libgen.h>
    #include <signal.h>
    #include <stdarg.h>
    #include <elf.h>
    #include <sys/wait.h>
    #include <sys/mman.h>
    #include <sys/stat.h>
    #include <sys/statfs.h>
    #include <sys/sysinfo.h>
    #include <iconv.h>
    #include <wchar.h>
    #include <dlfcn.h>
    #include "wintype.h"


#define STATUS_SUCCESS                          ((NTSTATUS)0x00000000L) // ntsubauth

#endif

typedef LONG NTSTATUS;

#endif
