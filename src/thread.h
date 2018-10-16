#ifndef __THREAD_H__
#define __THREAD_H__

#include "osl.h"

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

typedef struct _osl_thread_t {
    uint32_t id;
    uint8_t running;
    size_t stack_size;
    THREAD_HANDLE handle;
    void * (*func)(void*);
    void * arg;
} osl_thread_t;

osl_thread_t * osl_thread_new(void);
int osl_thread_start(osl_thread_t * thread);
void osl_thread_join(osl_thread_t * thread);
void osl_thread_free(osl_thread_t * thread);

#endif
