#ifndef __OSL_SEMAPHORE_H__
#define __OSL_SEMAPHORE_H__

#include "osl.h"

typedef struct _osl_semaphore_t
{
    SEM_HANDLE handle;
} osl_semaphore_t;

#ifdef __cplusplus
extern "C" {
#endif

    extern OSL_EXPORT osl_semaphore_t * osl_semaphore_new(void);
    extern OSL_EXPORT osl_semaphore_t * osl_semaphore_init(osl_semaphore_t *, int);
    extern OSL_EXPORT void osl_semaphore_wait(osl_semaphore_t *);
    extern OSL_EXPORT void osl_semaphore_post(osl_semaphore_t *);
    extern OSL_EXPORT void osl_semaphore_free(osl_semaphore_t *);

#ifdef __cplusplus
}
#endif

#endif
