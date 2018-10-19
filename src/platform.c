#include "platform.h"

static int __platform_initilized = 0;
static int __use_socket = 0;

#if defined(USE_MS_WIN)
static WSADATA wsaData;
#endif

void osl_platform_once(void)
{
    if (__platform_initilized)
    {
	return;
    }
    __platform_initilized = 1;
}

void osl_platform_ignore_sigpipe(void)
{
#if defined(USE_SIGNAL)
    signal(SIGPIPE, SIG_IGN);
#endif
}

int osl_platform_use_socket(void)
{
    if (__use_socket)
    {
	return 0;
    }

    __use_socket = 1;
    
#if defined(USE_MS_WIN)
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
	return -1;
    }
#endif
    return 0;
}

void osl_platform_finish(void)
{
    if (__use_socket)
    {
#if defined(USE_MS_WIN)
	WSACleanup();
#endif
    }
}
