#ifndef __OSL_MUTEX_H__
#define __OSL_MUTEX_H__

#include "osl.h"

typedef struct _osl_mutex_t
{
#if defined(USE_PTHREAD)		// apple also
    pthread_mutex_t handle;
    pthread_cond_t cond;
#elif defined(USE_MS_WIN)
    HANDLE handle;
    HANDLE evt;
#else
// not support
#endif
} osl_mutex_t;

#ifdef __cplusplus
extern "C" {
#endif

    extern OSL_EXPORT osl_mutex_t * osl_mutex_new(void);
    extern OSL_EXPORT void osl_mutex_lock(osl_mutex_t * mutex);
    extern OSL_EXPORT void osl_mutex_unlock(osl_mutex_t * mutex);
    extern OSL_EXPORT void osl_mutex_free(osl_mutex_t * mutex);

#ifdef __cplusplus
}
#endif

#endif
