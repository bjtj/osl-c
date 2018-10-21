#ifndef __THREAD_H__
#define __THREAD_H__

#include "osl.h"

typedef struct _osl_thread_t {
    uint32_t id;
    uint8_t running;
    size_t stack_size;
    THREAD_HANDLE handle;
    void * (*func)(void*);
    void * arg;
} osl_thread_t;

#ifdef __cplusplus
extern "C" {
#endif

    osl_thread_t OSL_EXPORT * osl_thread_new(void * (*func)(void*), void * arg);
    int OSL_EXPORT osl_thread_start(osl_thread_t * thread);
    void OSL_EXPORT osl_thread_join(osl_thread_t * thread);
    void OSL_EXPORT osl_thread_free(osl_thread_t * thread);

#ifdef __cplusplus
}
#endif

#endif
