#ifndef __SEMAPHORE_H__
#define __SEMAPHORE_H__

#include "osl.h"

typedef struct _osl_semaphore_t
{
    SEM_HANDLE handle;
} osl_semaphore_t;


osl_semaphore_t OSL_EXPORT * osl_semaphore_new(int initial);
void OSL_EXPORT osl_semaphore_wait(osl_semaphore_t * sem);
void OSL_EXPORT osl_semaphore_post(osl_semaphore_t * sem);
void OSL_EXPORT osl_semaphore_free(osl_semaphore_t * sem);

#endif
