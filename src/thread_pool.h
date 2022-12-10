#ifndef __THREAD_POLL_H__
#define __THREAD_POLL_H__

#include "thread.h"
#include "list.h"
#include "mutex.h"
#include "queue.h"
#include "event.h"

typedef void (*osl_thread_pool_func)(void*);

typedef struct _osl_thread_pool_job_t
{
  osl_thread_pool_func func;
  void * arg;
} osl_thread_pool_job_t;

typedef struct _osl_thread_pool_job_queue_t
{
  osl_event_t * signal;
  osl_mutex_t * mutex;
  osl_queue_t * queue;
} osl_thread_pool_job_queue_t;

typedef struct _osl_thread_pool_node_t
{
  int idx;
  struct _osl_thread_pool_t * pool;
  osl_thread_t * thread;
} osl_thread_pool_node_t;

typedef struct _osl_thread_pool_t
{
  osl_bool done;
  osl_mutex_t * mutex;
  osl_list_t * nodes;
  osl_thread_pool_job_queue_t * jobqueue;
  int thread_count;
  int working_count;
} osl_thread_pool_t;


#ifdef __cplusplus
extern "C" {
#endif

  extern OSL_EXPORT osl_thread_pool_t * osl_thread_pool_new(void);
  extern OSL_EXPORT osl_thread_pool_t * osl_thread_pool_init(osl_thread_pool_t *, int);
  extern OSL_EXPORT void osl_thread_pool_start(osl_thread_pool_t *);
  extern OSL_EXPORT void osl_thread_pool_stop(osl_thread_pool_t *);
  extern OSL_EXPORT void osl_thread_pool_free(osl_thread_pool_t *);
  extern OSL_EXPORT void osl_thread_pool_count_working_up(osl_thread_pool_t *);
  extern OSL_EXPORT void osl_thread_pool_count_working_down(osl_thread_pool_t *);
  extern OSL_EXPORT void osl_thread_pool_count_thread_up(osl_thread_pool_t *);
  extern OSL_EXPORT void osl_thread_pool_count_thread_down(osl_thread_pool_t *);

  extern OSL_EXPORT osl_thread_pool_job_t * osl_thread_pool_job_new(void);
  extern OSL_EXPORT osl_thread_pool_job_t * osl_thread_pool_job_init(osl_thread_pool_job_t *, void (*)(void*), void *);
  extern OSL_EXPORT void osl_thread_pool_job_free(osl_thread_pool_job_t *);

  extern OSL_EXPORT osl_thread_pool_job_queue_t * osl_thread_pool_job_queue_new(void);
  extern OSL_EXPORT osl_thread_pool_job_queue_t * osl_thread_pool_job_queue_init(osl_thread_pool_job_queue_t *);
  extern OSL_EXPORT void osl_thread_pool_job_queue_free(osl_thread_pool_job_queue_t *);
  extern OSL_EXPORT osl_bool osl_thread_pool_job_queue_is_empty(osl_thread_pool_job_queue_t *);
  extern OSL_EXPORT void osl_thread_pool_job_queue_push(osl_thread_pool_job_queue_t *, osl_thread_pool_job_t *);
  extern OSL_EXPORT osl_thread_pool_job_t * osl_thread_pool_job_queue_pop(osl_thread_pool_job_queue_t *);

  extern OSL_EXPORT osl_thread_pool_node_t * osl_thread_pool_node_new(void);
  extern OSL_EXPORT osl_thread_pool_node_t * osl_thread_pool_node_init(osl_thread_pool_node_t *, osl_thread_pool_t *);
  extern OSL_EXPORT void osl_thread_pool_node_free(osl_thread_pool_node_t *);
  extern OSL_EXPORT void osl_thread_pool_node_start_iter(int, osl_thread_pool_node_t *, void*);
    
  extern OSL_EXPORT void osl_thread_pool_call(osl_thread_pool_t *, osl_thread_pool_func, void *);

#ifdef __cplusplus
}
#endif

#endif /* __THREAD_POLL_H__ */
