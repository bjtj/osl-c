#ifndef __OSL_CACHE_H__
#define __OSL_CACHE_H__

#include "mutex.h"
#include "list.h"

struct _osl_cache_t;

typedef void (*osl_cache_on_remove_cb)(struct _osl_cache_t *);

typedef struct _osl_cache_t
{
    char * uid;
    void * data;
    size_t size;
    unsigned long create_date;
    unsigned long expire_date;
    void * user;
    osl_cache_on_remove_cb on_remove;
} osl_cache_t;


typedef struct _osl_cache_manager_t
{
    osl_mutex_t * lock;
    osl_list_t * caches;
    void * user;
} osl_cache_manager_t;

#ifdef __cplusplus
extern "C" {
#endif

    osl_cache_t OSL_EXPORT * osl_cache_copy(const osl_cache_t * cache);
    void OSL_EXPORT osl_cache_free(osl_cache_t * cache);
    osl_cache_manager_t OSL_EXPORT * osl_cache_manager_new(void * user);
    void OSL_EXPORT osl_cache_manager_free(osl_cache_manager_t * manager);
    osl_cache_t OSL_EXPORT * osl_cache_manager_set_cache(osl_cache_manager_t * manager, void * data, size_t size, unsigned long expire_date, void * user, osl_cache_on_remove_cb on_remove);
    osl_cache_t OSL_EXPORT * osl_cache_manager_get_cache(osl_cache_manager_t * manager, const char * uid);
    void OSL_EXPORT osl_cache_manager_remove_cache(osl_cache_manager_t * manager, const char * uid);
    void OSL_EXPORT osl_cache_manager_resolve_expired(osl_cache_manager_t * manage);

#ifdef __cplusplus
}
#endif

#endif
