#ifndef __EVENT_H__
#define __EVENT_H__

#include "osl.h"
#include "mutex.h"

typedef struct _osl_event_t {
    osl_mutex_t * mutex;
#if defined(USE_PTHREAD)
    pthread_cond_t _cond;
#elif defined(USE_MS_WIN)
    HANDLE _evt;
#endif
} osl_event_t;


#ifdef __cplusplus
extern "C" {
#endif

    extern OSL_EXPORT osl_event_t * osl_event_new(void);
    extern OSL_EXPORT osl_event_t * osl_event_init(osl_event_t *);
    extern OSL_EXPORT void osl_event_free(osl_event_t *);

    extern OSL_EXPORT int osl_event_lock(osl_event_t *);
    extern OSL_EXPORT int osl_event_unlock(osl_event_t *);
    extern OSL_EXPORT int osl_event_wait(osl_event_t *);
    extern OSL_EXPORT int osl_event_wait_with_timeout(osl_event_t *, unsigned long);
    extern OSL_EXPORT int osl_event_notify(osl_event_t *);
    extern OSL_EXPORT int osl_event_notify_all(osl_event_t *);
    

#ifdef __cplusplus
}
#endif

#endif /* __EVENT_H__ */
