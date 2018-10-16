#ifndef __SEMAPHORE_H__
#define __SEMAPHORE_H__

#include "osl.h"

#if defined(USE_APPLE_SEMAPHORE)

#	include <dispatch/dispatch.h>
typedef dispatch_semaphore_t SEM_HANDLE;

#elif defined(USE_POSIX_SEMAPHORE)

#	include <semaphore.h>
typedef sem_t SEM_HANDLE;

#elif defined(USE_WIN_SEMAPHORE)

#	define SEM_HANDLE HANDLE

#endif

typedef struct _osl_semaphore_t
{
    SEM_HANDLE handle;
} osl_semaphore_t;


osl_semaphore_t * osl_semaphore_new(int initial);
void osl_semaphore_wait(osl_semaphore_t * sem);
void osl_semaphore_post(osl_semaphore_t * sem);
void osl_semaphore_free(osl_semaphore_t * sem);

#endif
