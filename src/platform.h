#ifndef __OSL_PLATFORM_H__
#define __OSL_PLATFORM_H__

#if HAVE_CONFIG_H
#	include <config.h>
#endif

/*
 * Predefined osl macros
 * ====================
 * http://stackoverflow.com/questions/142508/how-do-i-check-os-with-a-preprocessor-directive
 */

#if defined(__APPLE__) || defined(__MACH__) /* mac os x */

#	define OSL_OS_APPLE

#	define USE_UNIX_STD
#	define USE_APPLE_STD
#	define USE_APPLE_SEMAPHORE
#	define USE_PTHREAD
#	define USE_BSD_SOCKET
#	define USE_SIGNAL

#	include <unistd.h>
#	include <sys/time.h>
#	include <sys/stat.h>
#	include <sys/types.h>
#	include <time.h>
#	include <dirent.h>
#	include <fcntl.h>
#	include <signal.h>
#	include <errno.h>
#	include <mach/clock.h>
#	include <mach/mach.h>
#	include <dlfcn.h>

#	define osl_vsnprintf vsnprintf

#	define OSL_EXPORT

#elif defined(unix) || defined(__unix__) || defined(__unix) /* unix or linux */

#	define OSL_OS_UNIX

#	define USE_UNIX_STD
#	define USE_POSIX_STD
#	define USE_POSIX_SEMAPHORE
#	define USE_PTHREAD
#	define USE_BSD_SOCKET
#	define USE_SIGNAL

#	if !defined(__CYGWIN__)
#		define USE_PRCTL
#	endif

#	include <unistd.h>
#	include <sys/time.h>
#	include <sys/stat.h>
#	include <sys/types.h>
#	include <sys/wait.h>
#	include <time.h>
#	include <dirent.h>
#	include <errno.h>
#	include <fcntl.h>
#	include <signal.h>
#	include <dlfcn.h>

#	define osl_vsnprintf vsnprintf

#	define OSL_EXPORT

#elif defined(_WIN32) || defined(_WIN64) /* windows */

#	define OSL_OS_WINDOWS

// #	define _WIN32_WINNT 0x501 /* force to use windows xp APIs */

#	define USE_MS_WIN
#	define USE_WIN_SEMAPHORE
#	define USE_WIN_THREAD
#	define USE_WINSOCK2

#if _MSC_VER < 1900
#	define snprintf _snprintf_s
#endif

#if _MSC_VER < 1400
#	define strncpy strncpy_s
#endif

#	define strdup _strdup

#	define strcasecmp(x,y) _stricmp((x),(y))
#	define strncasecmp(x,y,z) _strnicmp((x),(y),(z))
#	define osl_vsnprintf(b,bs,f,v) vsnprintf_s((b),(bs),_TRUNCATE,(f),(v))

#	define TIME SYSTEMTIME

#	include <direct.h>
#	include <sys/stat.h>
#	include <io.h>

#	define __func__ __FUNCTION__

// https://docs.microsoft.com/en-us/cpp/cpp/definitions-and-declarations-cpp?view=msvc-160
#	define OSL_EXPORT __declspec(dllexport)

#endif

#endif
