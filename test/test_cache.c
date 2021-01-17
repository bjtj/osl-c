#include "cache.h"
#include "osl.h"
#include "str.h"
#include <assert.h>

static void on_remove_cache(osl_cache_t * cache)
{
    printf("remove cache -- %s\n", (char*)cache->data);
}

void test_cache(void)
{

#define MILLISECOND_MINUTE (1000 * 60)

    printf("== test cache ==\n");
    
    osl_cache_manager_t * manager = osl_cache_manager_init(osl_cache_manager_new(), NULL);
    osl_cache_t * cache = osl_cache_manager_get_cache(manager, "hello");
    assert(cache == NULL);

    const char * msg = "hello";
    char * uid = osl_cache_manager_set_cache(manager, (void*)msg, strlen(msg) + 1, osl_tick_milli() + MILLISECOND_MINUTE, NULL, NULL)->uid;

    printf("uid -- %s\n", uid);

    cache = osl_cache_manager_get_cache(manager, uid);
    assert(cache != NULL);
    assert(strcmp((char*)cache->data, msg) == 0);
    
    osl_cache_manager_resolve_expired(manager);

    msg = "hello2";
    uid = osl_safe_strdup(osl_cache_manager_set_cache(manager, (void*)msg, strlen(msg) + 1, osl_tick_milli() + 1000, NULL, on_remove_cache)->uid);

    osl_idle(1500);
    osl_cache_manager_resolve_expired(manager);

    cache = osl_cache_manager_get_cache(manager, uid);
    assert(cache == NULL);

    osl_safe_free(uid);
    
    osl_cache_manager_free(manager);
}

int main()
{
    osl_init_once();
    osl_ignore_sigpipe();

    test_cache();
    
    osl_finish();
    
    return 0;
}
