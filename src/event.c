#include "event.h"


osl_event_t * osl_event_new(void)
{
    osl_event_t * event = (osl_event_t*)malloc(sizeof(osl_event_t));
    OSL_HANDLE_MALLOC_ERROR(event);
    memset(event, 0, sizeof(osl_event_t));
    return event;
}

osl_event_t * osl_event_init(osl_event_t * event)
{
    event->mutex = osl_mutex_init(osl_mutex_new());
    if (event->mutex == NULL) {
	return NULL;
    }
    
#if defined(USE_APPLE_STD)
    if (pthread_cond_init(&(event->_cond), NULL) != 0) {
	osl_mutex_free(event->mutex);
	return NULL;
    }
    return event;

#elif defined(USE_PTHREAD)
    pthread_condattr_t attr;
    
    if (pthread_condattr_init(&attr) != 0) {
	goto fail1;
    }
    
    if (pthread_condattr_setclock(&attr, CLOCK_MONOTONIC) != 0) {
	goto fail2;
    }
    
    if (pthread_cond_init(&(event->_cond), &attr) != 0) {
	goto fail2;
    }

    if (pthread_condattr_destroy(&attr) != 0) {
	goto fail2;
    }

    return event;

fail2:
    if (pthread_condattr_destroy(&attr) != 0) {
	// pthread_condattr_destroy() error
    }
fail1:
    osl_mutex_free(event->mutex);

    return NULL;

#elif defined(USE_MS_WIN)    
    InitializeConditionVariable(&(event->_cond));
    return event;
#else
    return NULL;
#endif
}

void osl_event_free(osl_event_t * event)
{
    osl_mutex_free(event->mutex);
#if defined(USE_PTHREAD)
    if (pthread_cond_destroy(&(event->_cond)) != 0) {
	// pthread_cond_destroy() error
    }
#elif defined(USE_MS_WIN)
    (void)event;    
#else
    /* TODO:  */
#endif
    osl_safe_free(event);
}

int osl_event_lock(osl_event_t * event)
{
    osl_mutex_lock(event->mutex);
    return 0;
}

int osl_event_unlock(osl_event_t * event)
{
    osl_mutex_unlock(event->mutex);
    return 0;
}

int osl_event_wait(osl_event_t * event)
{
    return osl_event_wait_with_timeout(event, 0);
}

int osl_event_wait_with_timeout(osl_event_t * event, unsigned long timeout)
{
#if defined(USE_APPLE_STD)
    if (timeout > 0) {
	/* TODO: exception - not supported */
	return -1;
    }
    pthread_cond_wait(&(event->_cond), &(event->mutex->handle));
    return 0;
#elif defined(USE_PTHREAD)
    if (timeout == 0) {
	pthread_cond_wait(&(event->_cond), &(event->mutex->handle));
    } else {
	struct timespec spec;
	int ret;
	clock_gettime(CLOCK_MONOTONIC, &spec);
	spec.tv_sec += (timeout / 1000);
	spec.tv_nsec += ((timeout % 1000) * 1000000);
	ret = pthread_cond_timedwait(&(event->_cond), &(event->mutex->handle), &spec);
	if (ret != 0) {
	    switch (ret) {
	    case ETIMEDOUT:
		return -1;
	    default:
		return -1;
	    }
	}
    }
    return 0;
#elif defined(USE_MS_WIN)

    if (!SleepConditionVariableCS(&(event->_cond), &(event->mutex->cs), timeout == 0 ? INFINITE : timeout)) {
        return -1;
    }
    return 0;
#else
    return -1;
#endif
}

int osl_event_notify(osl_event_t * event)
{
#if defined(USE_PTHREAD)
    if (pthread_cond_signal(&(event->_cond)) != 0) {
	return -1;
    }
    return 0;
#elif defined(USE_MS_WIN)
    WakeConditionVariable(&(event->_cond));
    return 0;
#else
    return -1;
#endif
}

int osl_event_notify_all(osl_event_t * event)
{
#if defined(USE_PTHREAD)
    if (pthread_cond_broadcast(&(event->_cond)) != 0) {
	return -1;
    }
    return 0;
#elif defined(USE_MS_WIN)    
    WakeAllConditionVariable(&(event->_cond));
    return 0;
#else
    return -1;
#endif
}

