#ifndef __OSL_H__
#define __OSL_H__

#include "platform.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>

/**
 * @brief common feature
 * unused : http://stackoverflow.com/questions/3599160/unused-parameter-warnings-in-c-code
 */
#define SUPPRESS_UNUSED(x) (void)(x)

/*
 * Semaphore
 */
#if defined(USE_APPLE_SEMAPHORE)

#	include <dispatch/dispatch.h>
typedef dispatch_semaphore_t SEM_HANDLE;

#elif defined(USE_POSIX_SEMAPHORE)

#	include <semaphore.h>
typedef sem_t SEM_HANDLE;

#elif defined(USE_WIN_SEMAPHORE)

#	define SEM_HANDLE HANDLE

#endif


/*
 * Thread
 */
#if defined(USE_PTHREAD)

#	include <pthread.h>

#if defined(USE_PRCTL)
#	include <sys/prctl.h>
#endif

typedef pthread_t THREAD_HANDLE;

#elif defined(USE_WIN_THREAD)

#	define WIN32_LEAN_AND_MEAN
#	define _WINSOCKAPI_
#	include <Windows.h>
#	include <process.h>
typedef HANDLE THREAD_HANDLE;

#endif


/*
 * @brief Socket
 */
#if defined(USE_BSD_SOCKET) /* BSD */


#	include <arpa/inet.h>
#	include <sys/types.h>
#	include <sys/stat.h>
#	include <sys/socket.h>
#	include <ifaddrs.h>
#	include <netdb.h>
#	include <netinet/in.h>

#	if defined(USE_APPLE_STD)
#		include <net/if_dl.h>
#	else
#		include <linux/if_packet.h>
#		include <net/ethernet.h>
#	endif

typedef int osl_socket;

#define INVALID_SOCKET -1

#elif defined(USE_WINSOCK2) /* winsock2 */

// reference: https://msdn.microsoft.com/ko-kr/library/windows/desktop/ms737629%28v=vs.85%29.aspx
// refenrece: https://msdn.microsoft.com/en-us/library/windows/desktop/aa365947%28v=vs.85%29.aspx

#	include <WinSock2.h>
#	include <WS2tcpip.h>
#	include <ws2ipdef.h>
#	include <iphlpapi.h>

#	pragma comment(lib, "Ws2_32.lib")
#	pragma comment(lib, "iphlpapi.lib")

/* Note: could also use malloc() and free() */
#	define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x)) 
#	define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

typedef SOCKET osl_socket;

#endif

#ifdef __cplusplus
extern "C" {
#endif

    void OSL_EXPORT osl_platform_once(void);
    void OSL_EXPORT osl_platform_ignore_sigpipe(void);
    int OSL_EXPORT osl_platform_use_socket(void);
    void OSL_EXPORT osl_platform_finish(void);
    void OSL_EXPORT osl_idle(unsigned long timeout);
    unsigned long OSL_EXPORT osl_tick_milli(void);
    char OSL_EXPORT * osl_getcwd(void);

#ifdef __cplusplus
}
#endif

#endif
