#include "thread.h"

static inline void _notify_stop(osl_thread_t * thread)
{
    osl_event_lock(thread->signal);
    thread->running = osl_false;
    osl_event_notify_all(thread->signal);
    osl_event_unlock(thread->signal);
}

/* THREAD */
#if defined(USE_PTHREAD)

// unix or linux

static void * s_thread_wrapper(void * arg) {
    osl_thread_t * thread = (osl_thread_t*)arg;
#if defined(USE_PRCTL)
    {
	char name[16] = {0,};
	snprintf(name, sizeof(name), "tid:0x%x", thread->id);
	prctl(PR_SET_NAME, name, 0, 0, 0);
    }
#endif
    thread->func(thread->arg);

    _notify_stop(thread);
    
    return 0;
}

static int s_native_start_thread(osl_thread_t * thread) {
    pthread_attr_t attr;
    int err = 1;
    if (!thread) {
	return 1;
    }
    if (pthread_attr_init(&attr) != 0) {
	return 1;
    }

    if (pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED) != 0) {
	goto done;
    }
    if (thread->stack_size > 0) {
	if (pthread_attr_setstacksize(&attr, thread->stack_size) != 0) {
	    goto done;
	}
    }
    if (pthread_create(&(thread->handle), &attr, s_thread_wrapper, (void*)thread) != 0) {
	goto done;
    }
    err = 0;

done:
    pthread_attr_destroy(&attr);
    return err;
}

#elif defined(USE_WIN_THREAD)

// windows

static UINT WINAPI s_thread_wrapper(void * arg) {
    osl_thread_t * thread = (osl_thread_t*)arg;
    thread->func(thread->arg);
    thread->running = osl_false;
    _notify_stop(thread);
    return 0;
}


static int s_native_start_thread(osl_thread_t * thread) {
    UINT dwThreadID;
    if (!thread) {
	return 1;
    }
    thread->handle = (HANDLE)_beginthreadex(NULL,
					    (unsigned int)thread->stack_size,
					    s_thread_wrapper,
					    (void*)thread,
					    0,
					    &dwThreadID);
    return 0;
}
#else
/* unknown thread api */
#endif


static uint32_t __idx = 0;

osl_thread_t * osl_thread_new(void)
{
    osl_thread_t * thread = (osl_thread_t *)malloc(sizeof(osl_thread_t));
    OSL_HANDLE_MALLOC_ERROR(thread);
    memset(thread, 0, sizeof(osl_thread_t));
    return thread;
}

osl_thread_t * osl_thread_init(osl_thread_t * thread, osl_thread_func func, void * arg)
{
    thread->id = ++__idx;
    thread->func = func;
    thread->arg = arg;
    thread->signal = osl_event_init(osl_event_new());
    return thread;
}

int osl_thread_start(osl_thread_t * thread)
{
    thread->running = osl_true;
    return s_native_start_thread(thread);
}

void osl_thread_join(osl_thread_t * thread)
{
    osl_event_lock(thread->signal);
    while (thread->running) {
	osl_event_wait(thread->signal);
    }
    osl_event_unlock(thread->signal);
}

void osl_thread_free(osl_thread_t * thread)
{
    osl_event_free(thread->signal);
    osl_safe_free(thread);
}
