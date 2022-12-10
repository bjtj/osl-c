#include "cache.h"
#include "str.h"

static unsigned long _idx = 0;

static char * _gen_uid(void)
{
  char buf[1024] = {0,};
  snprintf(buf, sizeof(buf), "cache-%010lu-%010lu", osl_tick_milli(), ++_idx);
  return osl_safe_strdup(buf);
}

static osl_bool _compare_uid(osl_cache_t * cache, const char * uid)
{
  return (strcmp(cache->uid, uid) == 0);
}

static osl_bool _cache_expired(osl_cache_t * cache, void * user)
{
  (void)user;
  if (cache->expire_date == 0)
  {
    /* infinite */
    return 0;
  }
  return (osl_tick_milli() >= cache->expire_date);
}

osl_cache_t * osl_cache_copy(const osl_cache_t * other)
{
  osl_cache_t * cache = (osl_cache_t*)malloc(sizeof(osl_cache_t));
  OSL_HANDLE_MALLOC_ERROR(cache);
  memset(cache, 0, sizeof(osl_cache_t));
  cache->uid = osl_safe_strdup(other->uid);
  cache->data = other->data;
  cache->size = other->size;
  cache->create_date = other->create_date;
  cache->expire_date = other->expire_date;
  cache->user = other->user;
  cache->on_remove = other->on_remove;
  return cache;
}

void osl_cache_free(osl_cache_t * cache)
{
  if (cache == NULL)
  {
    return;
  }

  if (cache->on_remove)
  {
    cache->on_remove(cache);
  }
    
  osl_safe_free(cache->uid);
  osl_safe_free(cache);
}

osl_cache_manager_t * osl_cache_manager_new(void)
{
  osl_cache_manager_t * manager = (osl_cache_manager_t*)malloc(sizeof(osl_cache_manager_t));
  OSL_HANDLE_MALLOC_ERROR(manager);
  memset(manager, 0, sizeof(osl_cache_manager_t));
  return manager;
}

osl_cache_manager_t * osl_cache_manager_init(osl_cache_manager_t * manager, void * user)
{
  manager->user = user;
  return manager;
}

void osl_cache_manager_free(osl_cache_manager_t * manager)
{
  if (manager == NULL)
  {
    return;
  }
  osl_list_free(manager->caches, (osl_free_cb)osl_cache_free);
  osl_safe_free(manager);
}

osl_cache_t * osl_cache_manager_set_cache(osl_cache_manager_t * manager, void * data, size_t size, unsigned long expire_date, void * user, osl_cache_on_remove_cb on_remove)
{
  osl_cache_t temp = {
    .uid = _gen_uid(),
    .data = data,
    .size = size,
    .create_date = osl_tick_milli(),
    .expire_date = expire_date,
    .user = user,
    .on_remove = on_remove,
  };
  osl_cache_t * cache = osl_cache_copy(&temp);
  manager->caches = osl_list_append(manager->caches, cache);
  osl_safe_free(temp.uid);
  return cache;
}

osl_cache_t * osl_cache_manager_get_cache(osl_cache_manager_t * manager, const char * uid)
{
  return (osl_cache_t*)osl_list_find(manager->caches, (osl_compare_cb)_compare_uid, (void*)uid);
}

void osl_cache_manager_remove_cache(osl_cache_manager_t * manager, const char * uid)
{
  osl_list_remove_if(manager->caches, (osl_compare_cb)_compare_uid, (void*)uid, (osl_free_cb)osl_cache_free);
}

void osl_cache_manager_resolve_expired(osl_cache_manager_t * manager)
{
  osl_list_remove_if(manager->caches, (osl_compare_cb)_cache_expired, NULL, (osl_free_cb)osl_cache_free);
}
