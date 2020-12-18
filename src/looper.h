#ifndef __OSL_LOOPER_H__
#define __OSL_LOOPER_H__

#include "osl.h"
#include "list.h"
#include "mutex.h"

struct _osl_looper_t;

typedef void (*osl_looper_task_cb)(struct _osl_looper_t *, void *);
typedef void (*osl_looper_on_start_cb)(struct _osl_looper_t *);
typedef void (*osl_looper_on_stop_cb)(struct _osl_looper_t *);


typedef struct _osl_looper_schedule_t
{
    unsigned long delay;
    unsigned long interval;
    int repeat;
} osl_looper_schedule_t;

typedef struct _osl_looper_t
{
    void * user;
    char * name;
    osl_mutex_t * mutex;
    osl_bool finishing;
    osl_bool running;
    int retcode;
    osl_looper_on_start_cb on_start;
    osl_looper_on_start_cb on_stop;
    osl_list_t * task_queue;
} osl_looper_t;

#ifdef __cplusplus
extern "C" {
#endif

    osl_looper_t OSL_EXPORT * osl_looper_new(const char * name, void * user);
    void OSL_EXPORT osl_looper_free(osl_looper_t * looper);
    void OSL_EXPORT osl_looper_add_task(osl_looper_t * looper, osl_looper_task_cb task, void * user);
    void OSL_EXPORT osl_looper_add_task_schedule(osl_looper_t * looper, osl_looper_task_cb task, osl_looper_schedule_t * schedule, void * user);
    int OSL_EXPORT osl_looper_run(osl_looper_t * looper);
    osl_bool OSL_EXPORT osl_looper_is_running(osl_looper_t * looper);
    void OSL_EXPORT osl_looper_finish(osl_looper_t * looper);
    void OSL_EXPORT osl_looper_lock(osl_looper_t * looper);
    void OSL_EXPORT osl_looper_unlock(osl_looper_t * looper);

#ifdef __cplusplus
}
#endif

#endif
