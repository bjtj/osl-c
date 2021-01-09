#ifndef __OSL_THREAD_H__
#define __OSL_THREAD_H__

#include "osl.h"

typedef void * (*osl_thread_func)(void *);

typedef struct _osl_thread_t {
    uint32_t id;
    uint8_t running;
    size_t stack_size;
    THREAD_HANDLE handle;
    osl_thread_func func;
    void * arg;
} osl_thread_t;

#ifdef __cplusplus
extern "C" {
#endif

    extern OSL_EXPORT osl_thread_t * osl_thread_new(osl_thread_func func, void * arg);
    extern OSL_EXPORT int osl_thread_start(osl_thread_t * thread);
    extern OSL_EXPORT void osl_thread_join(osl_thread_t * thread);
    extern OSL_EXPORT void osl_thread_free(osl_thread_t * thread);

#ifdef __cplusplus
}
#endif

#endif
