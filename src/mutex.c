#include "mutex.h"

osl_mutex_t * osl_mutex_new(void)
{
    osl_mutex_t * mutex = (osl_mutex_t*)malloc(sizeof(osl_mutex_t));
    OSL_HANDLE_MALLOC_ERROR(mutex);
    memset(mutex, 0, sizeof(osl_mutex_t));
    return mutex;
}

osl_mutex_t * osl_mutex_init(osl_mutex_t * mutex)
{
    #if defined(USE_PTHREAD)
    if (pthread_mutex_init(&(mutex->handle), NULL) != 0) {
	/* assert */
	osl_safe_free(mutex);
	return NULL;
    }
    return mutex;
#elif defined(USE_MS_WIN)
    mutex->handle = CreateMutex(NULL, FALSE, NULL);
    return mutex;
#else
    return NULL;
#endif
}

void osl_mutex_lock(osl_mutex_t * mutex)
{
#if defined(USE_PTHREAD)
    if (pthread_mutex_lock(&(mutex->handle)) != 0) {
	/* error */
    }
#elif defined(USE_MS_WIN)
    // TODO: https://msdn.microsoft.com/ko-kr/library/windows/desktop/ms686927(v=vs.85).aspx
    WaitForSingleObject(mutex->handle, INFINITE);
#else
    /* no implementation */
#endif
}

void osl_mutex_unlock(osl_mutex_t * mutex)
{
#if defined(USE_PTHREAD)
    if (pthread_mutex_unlock(&(mutex->handle)) != 0) {
	/* error */
    }
#elif defined(USE_MS_WIN)
    ReleaseMutex(mutex->handle);
#else
    /* no implementation */
#endif
}

void osl_mutex_free(osl_mutex_t * mutex)
{
    if (mutex)
    {
#if defined(USE_PTHREAD)
	if (pthread_mutex_destroy(&(mutex->handle)) != 0) {
	    /* error */
	}
#elif defined(USE_MS_WIN)
	CloseHandle(mutex->handle);
#else
	/*  */
#endif
	osl_safe_free(mutex);
    }
}
