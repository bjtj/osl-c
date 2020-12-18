#include "looper.h"
#include "str.h"

#define osl_looper_task(a) ((osl_looper_task_t *)(a))

typedef struct _osl_looper_task_t
{
    void * user;
    osl_looper_schedule_t schedule;
    osl_looper_task_cb callback;
    unsigned long reg_time;
    unsigned long start_time;
} osl_looper_task_t;

static osl_looper_task_t * _osl_looper_task_new(void * user);

static osl_looper_task_t * _osl_looper_task_new_with_callback(osl_looper_task_cb task_cb, void * user)
{
    osl_looper_task_t * task = _osl_looper_task_new(user);
    task->callback = task_cb;
    return task;
}
    

static osl_looper_task_t * _osl_looper_task_new(void * user)
{
    osl_looper_task_t * task = (osl_looper_task_t*)malloc(sizeof(osl_looper_task_t));
    memset(task, 0, sizeof(osl_looper_task_t));
    task->user = user;
    return task;
}

static void _osl_looper_task_free(osl_looper_task_t * task)
{
    if (task == NULL)
    {
	return;
    }
    osl_free(task);
}

osl_looper_t * osl_looper_new(const char * name, void * user)
{
    osl_looper_t * looper = (osl_looper_t*)malloc(sizeof(osl_looper_t));
    memset(looper, 0, sizeof(osl_looper_t));
    looper->name = osl_strdup(name);
    looper->mutex = osl_mutex_new();
    looper->user = user;
    return looper;
}

void osl_looper_free(osl_looper_t * looper)
{
    if (looper == NULL)
    {
	return;
    }

    osl_free(looper->name);
    osl_mutex_free(looper->mutex);
    osl_list_free(looper->task_queue, (osl_free_cb)_osl_looper_task_free);
    osl_free(looper);
}

void osl_looper_add_task(osl_looper_t * looper, osl_looper_task_cb task, void * user)
{
    osl_looper_lock(looper);
    looper->task_queue = osl_list_append(looper->task_queue, _osl_looper_task_new_with_callback(task, user));
    osl_looper_unlock(looper);
}

void osl_looper_add_task_schedule(osl_looper_t * looper, osl_looper_task_cb task, osl_looper_schedule_t * schedule, void * user)
{
    /* todo */
}

int osl_looper_run(osl_looper_t * looper)
{
    if (osl_looper_is_running(looper))
    {
	return -1;
    }
    
    looper->finishing = osl_false;
    looper->running = osl_true;
    if (looper->on_start) {
	looper->on_start(looper);
    }

    while (looper->finishing == osl_false)
    {
	if (looper->task_queue)
	{
	    /* pop one task and do the task */
	    osl_looper_lock(looper);
	    osl_looper_task_t * task = osl_looper_task(osl_list_pop_first(&(looper->task_queue)));
	    task->callback(looper, task->user);
	    _osl_looper_task_free(task);
	    osl_looper_unlock(looper);
	}
	else
	{
	    osl_idle(10);
	}
    }
    
    if (looper->on_stop)
    {
	looper->on_stop(looper);
    }
    looper->running = osl_false;

    return looper->retcode;
}

osl_bool osl_looper_is_running(osl_looper_t * looper)
{
    return looper->running;
}

void osl_looper_finish(osl_looper_t * looper)
{
    looper->finishing = osl_true;
}

void osl_looper_lock(osl_looper_t * looper)
{
    osl_mutex_lock(looper->mutex);
}

void osl_looper_unlock(osl_looper_t * looper)
{
    osl_mutex_unlock(looper->mutex);
}
