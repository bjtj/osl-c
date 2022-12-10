#include "thread_pool.h"
#include <assert.h>

static void * _thread_wrapper(void * arg)
{
  osl_thread_pool_node_t * node = (osl_thread_pool_node_t *)arg;
  osl_thread_pool_t * pool = node->pool;
  osl_thread_pool_job_queue_t * jobqueue = node->pool->jobqueue;

  osl_thread_pool_count_thread_up(pool);

  while (!node->pool->done) {
    osl_thread_pool_job_t * job;
    osl_event_lock(jobqueue->signal);
    osl_event_wait(jobqueue->signal);
    job = osl_thread_pool_job_queue_pop(jobqueue);
    osl_event_unlock(jobqueue->signal);
    if (job) {
	    osl_thread_pool_count_working_up(pool);
	    job->func(job->arg);
	    osl_thread_pool_count_working_down(pool);
	    osl_thread_pool_job_free(job);
    }
  }

  osl_thread_pool_count_thread_down(pool);

  return 0;
}


osl_thread_pool_t * osl_thread_pool_new(void)
{
  osl_thread_pool_t * pool = (osl_thread_pool_t*)malloc(sizeof(osl_thread_pool_t));
  OSL_HANDLE_MALLOC_ERROR(pool);
  memset(pool, 0, sizeof(osl_thread_pool_t));
  return pool;
}

osl_thread_pool_t * osl_thread_pool_init(osl_thread_pool_t * pool, int capacity)
{
  if (capacity <= 0) {
    return NULL;
  }

  pool->jobqueue = osl_thread_pool_job_queue_init(osl_thread_pool_job_queue_new());
  pool->mutex = osl_mutex_init(osl_mutex_new());
    
  for (; capacity > 0; --capacity) {
    osl_thread_pool_node_t * node;
    node = osl_thread_pool_node_init(osl_thread_pool_node_new(), pool);
    pool->nodes = osl_list_append(pool->nodes, node);
  }

  return pool;
}

void osl_thread_pool_start(osl_thread_pool_t * pool)
{
  osl_list_each(pool->nodes, (osl_iter_cb)osl_thread_pool_node_start_iter, NULL);
}

void osl_thread_pool_stop(osl_thread_pool_t * pool)
{
  pool->done = osl_true;
  while (pool->thread_count > 0) {
    osl_event_lock(pool->jobqueue->signal);
    osl_event_notify_all(pool->jobqueue->signal);
    osl_event_unlock(pool->jobqueue->signal);
    osl_idle(100);
  }
}

static void _node_join_free(osl_thread_pool_node_t * node)
{
  osl_thread_join(node->thread);
  osl_thread_pool_node_free(node);
}

void osl_thread_pool_free(osl_thread_pool_t * pool)
{
  if (pool == NULL) {
    return;
  }
  osl_thread_pool_job_queue_free(pool->jobqueue);
  osl_mutex_free(pool->mutex);
  osl_list_free(pool->nodes, (osl_free_cb)_node_join_free);
  osl_safe_free(pool);
}

void osl_thread_pool_count_working_up(osl_thread_pool_t * pool)
{
  osl_mutex_lock(pool->mutex);
  pool->working_count++;
  osl_mutex_unlock(pool->mutex);
}

void osl_thread_pool_count_working_down(osl_thread_pool_t * pool)
{
  osl_mutex_lock(pool->mutex);
  pool->working_count--;
  osl_mutex_unlock(pool->mutex);
}

void osl_thread_pool_count_thread_up(osl_thread_pool_t * pool)
{
  osl_mutex_lock(pool->mutex);
  pool->thread_count++;
  osl_mutex_unlock(pool->mutex);
}

void osl_thread_pool_count_thread_down(osl_thread_pool_t * pool)
{
  osl_mutex_lock(pool->mutex);
  pool->thread_count--;
  osl_mutex_unlock(pool->mutex);
}

osl_thread_pool_job_t * osl_thread_pool_job_new(void)
{
  osl_thread_pool_job_t * job = (osl_thread_pool_job_t*)malloc(sizeof(osl_thread_pool_job_t));
  OSL_HANDLE_MALLOC_ERROR(job);
  memset(job, 0, sizeof(osl_thread_pool_job_t));
  return job;
}

osl_thread_pool_job_t * osl_thread_pool_job_init(osl_thread_pool_job_t * job, osl_thread_pool_func func, void * arg)
{
  job->func = func;
  job->arg = arg;
  return job;
}

void osl_thread_pool_job_free(osl_thread_pool_job_t * job)
{
  osl_safe_free(job);
}

osl_thread_pool_job_queue_t * osl_thread_pool_job_queue_new(void)
{
  osl_thread_pool_job_queue_t * jqueue = (osl_thread_pool_job_queue_t*)malloc(sizeof(osl_thread_pool_job_queue_t));
  OSL_HANDLE_MALLOC_ERROR(jqueue);
  memset(jqueue, 0, sizeof(osl_thread_pool_job_queue_t));
  return jqueue;
}

osl_thread_pool_job_queue_t * osl_thread_pool_job_queue_init(osl_thread_pool_job_queue_t * jqueue)
{
  jqueue->signal = osl_event_init(osl_event_new());
  jqueue->mutex = osl_mutex_init(osl_mutex_new());
  jqueue->queue = osl_queue_init(osl_queue_new());
  return jqueue;
}

void osl_thread_pool_job_queue_free(osl_thread_pool_job_queue_t * jqueue)
{
  osl_event_free(jqueue->signal);
  osl_mutex_free(jqueue->mutex);
  osl_queue_free(jqueue->queue, (osl_free_cb)osl_thread_pool_job_free);
  osl_safe_free(jqueue);
}

osl_bool osl_thread_pool_job_queue_is_empty(osl_thread_pool_job_queue_t * jqueue)
{
  osl_bool ret;
  osl_mutex_lock(jqueue->mutex);
  ret = OSL_BOOL(jqueue->queue->count == 0);
  osl_mutex_unlock(jqueue->mutex);
  return ret;
}

void osl_thread_pool_job_queue_push(osl_thread_pool_job_queue_t * jqueue, osl_thread_pool_job_t * job)
{
  osl_event_lock(jqueue->signal);
  osl_mutex_lock(jqueue->mutex);
  osl_queue_push(jqueue->queue, (void*)job);
  osl_event_notify(jqueue->signal);
  osl_mutex_unlock(jqueue->mutex);
  osl_event_unlock(jqueue->signal);
}

osl_thread_pool_job_t * osl_thread_pool_job_queue_pop(osl_thread_pool_job_queue_t * jqueue)
{
  osl_thread_pool_job_t * ret;
  osl_mutex_lock(jqueue->mutex);
  ret = (osl_thread_pool_job_t *)osl_queue_pop(jqueue->queue);
  osl_mutex_unlock(jqueue->mutex);
  return ret;
}

osl_thread_pool_node_t * osl_thread_pool_node_new(void)
{
  osl_thread_pool_node_t * node = (osl_thread_pool_node_t*)malloc(sizeof(osl_thread_pool_node_t));
  OSL_HANDLE_MALLOC_ERROR(node);
  memset(node, 0, sizeof(osl_thread_pool_node_t));
  return node;
}

osl_thread_pool_node_t * osl_thread_pool_node_init(osl_thread_pool_node_t * node, osl_thread_pool_t * pool)
{
  static int _idx = 0;
  node->idx = ++_idx;
  node->pool = pool;
  node->thread = osl_thread_init(osl_thread_new(), _thread_wrapper, node);
  return node;
}

void osl_thread_pool_node_free(osl_thread_pool_node_t * node)
{
  osl_thread_free(node->thread);
  osl_safe_free(node);
}

void osl_thread_pool_node_start_iter(int idx, osl_thread_pool_node_t * node, void * user)
{
  (void)idx;
  (void)user;
  osl_thread_start(node->thread);
}

void osl_thread_pool_call(osl_thread_pool_t * pool, osl_thread_pool_func func, void * arg)
{
  osl_thread_pool_job_queue_push(pool->jobqueue, osl_thread_pool_job_init(osl_thread_pool_job_new(), func, arg));
}
