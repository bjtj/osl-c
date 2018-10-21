#include "osl.h"

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
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
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

void idle(unsigned long timeout)
{
#if defined(USE_UNIX_STD)
    struct timespec ts;
    ts.tv_sec = timeout / 1000;
    ts.tv_nsec = (timeout % 1000) * (1000 * 1000);
    nanosleep(&ts, NULL);
#elif defined(USE_MS_WIN)
    Sleep(timeout);
#else
    /* no implementation */
#endif
}

unsigned long tick_milli()
{
#if defined(USE_APPLE_STD)
    // @ref http://stackoverflow.com/a/11681069
    clock_serv_t cclock;
    mach_timespec_t mts;
    host_get_clock_service(mach_host_self(), SYSTEM_CLOCK, &cclock);
    clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);
    return (mts.tv_sec * 1000) + (mts.tv_nsec / 1000000);
#elif defined(USE_POSIX_STD)
    struct timespec spec;
    clock_gettime(CLOCK_MONOTONIC, &spec);
    return (spec.tv_sec * 1000) + (spec.tv_nsec / 1000000);
#elif defined(USE_MS_WIN)
    return GetTickCount();
#else
    /* no implementation */
#endif
}
