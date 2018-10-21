#ifndef __MUTEX_H__
#define __MUTEX_H__

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

    osl_mutex_t OSL_EXPORT * osl_mutex_new(void);
    void OSL_EXPORT osl_mutex_lock(osl_mutex_t * mutex);
    void OSL_EXPORT osl_mutex_unlock(osl_mutex_t * mutex);
    void OSL_EXPORT osl_mutex_free(osl_mutex_t * mutex);

#ifdef __cplusplus
}
#endif

#endif
