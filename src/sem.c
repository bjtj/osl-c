#include "sem.h"


#if defined(USE_APPLE_SEMAPHORE)

static void s_sem_init(SEM_HANDLE * handle, int initial) {
    *handle = dispatch_semaphore_create(initial);
}

static void s_sem_wait(SEM_HANDLE * handle) {
    dispatch_semaphore_wait(*handle, DISPATCH_TIME_FOREVER);
}

static void s_sem_post(SEM_HANDLE * handle) {
    dispatch_semaphore_signal(*handle);
}

static void s_sem_destroy(SEM_HANDLE * handle) {
    SUPPRESS_UNUSED(handle);
}

#elif defined(USE_POSIX_SEMAPHORE)

static void s_sem_init(SEM_HANDLE * handle, int initial) {
    sem_init(handle, 0, initial);
}

static void s_sem_wait(SEM_HANDLE * handle) {
    sem_wait(handle);
}

static void s_sem_post(SEM_HANDLE * handle) {
    sem_post(handle);
}

static void s_sem_destroy(SEM_HANDLE * handle) {
    sem_destroy(handle);
}

#else

static void s_sem_init(SEM_HANDLE * handle, int initial) {
    *handle = CreateSemaphore(
	NULL,		// default security attributes
	initial,	// initial count
	initial,	// maximum count
	NULL);		// unnamed semaphore
}

static void s_sem_wait(SEM_HANDLE * handle) {
    WaitForSingleObject(*handle, INFINITE);
}

static void s_sem_post(SEM_HANDLE * handle) {
    ReleaseSemaphore( 
	*handle,	// handle to semaphore
	1,			// increase count by one
	NULL);		// not interested in previous count
}

static void s_sem_destroy(SEM_HANDLE * handle) {
    CloseHandle(*handle);
}

#endif /* SEMAPHORE */



osl_semaphore_t * osl_semaphore_new(void)
{
    osl_semaphore_t * sem = (osl_semaphore_t*)malloc(sizeof(osl_semaphore_t));
    OSL_HANDLE_MALLOC_ERROR(sem);
    memset(sem, 0, sizeof(osl_semaphore_t));
    return sem;
}

osl_semaphore_t * osl_semaphore_init(osl_semaphore_t * sem, int initial)
{
    s_sem_init(&(sem->handle), initial);
    return sem;
}

void osl_semaphore_wait(osl_semaphore_t * sem)
{
    s_sem_wait(&(sem->handle));
}

void osl_semaphore_post(osl_semaphore_t * sem)
{
    s_sem_post(&(sem->handle));
}

void osl_semaphore_free(osl_semaphore_t * sem)
{
    if (sem)
    {
	s_sem_destroy(&(sem->handle));
	osl_safe_free(sem);
    }
}
