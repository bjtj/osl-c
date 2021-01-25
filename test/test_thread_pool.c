#include "thread_pool.h"
#include <assert.h>

static void _func(void * arg)
{
    int * idx = (int*)arg;
    printf("idx: %d\n", *idx);
    osl_safe_free(idx);
}

void test()
{
    int size = 50;
    int i;
    osl_thread_pool_t * pool;
    pool = osl_thread_pool_init(osl_thread_pool_new(), size);
    assert(osl_list_size(pool->nodes) == size);
    osl_thread_pool_start(pool);

    osl_idle(100);

    for (i = 0; i < size; ++i) {
	int * x = (int*)malloc(sizeof(i));
	*x = i;
	osl_thread_pool_call(pool, _func, (void*)x);
    }

    osl_idle(100);

    assert(osl_list_size(pool->nodes) == size);

    osl_thread_pool_stop(pool);
    osl_thread_pool_free(pool);
}

int main()
{
    test();
    
    return 0;
}
