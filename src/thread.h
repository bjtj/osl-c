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

osl_thread_t * osl_thread_new(void * (*func)(void*), void * arg);
int osl_thread_start(osl_thread_t * thread);
void osl_thread_join(osl_thread_t * thread);
void osl_thread_free(osl_thread_t * thread);

#endif
