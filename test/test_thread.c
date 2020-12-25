#include "thread.h"
#include <assert.h>


void * worker(void * arg)
{
    int id = *(int*)arg;
    int i = 0;
    for (; i < 10; i++) {
	printf("[%d] idx -- %d\n", id, i);
	osl_idle(50);
    }
    return 0;
}

void test_thread(void)
{
    int id1 = 1;
    int id2 = 2;
    osl_thread_t * thread1 = osl_thread_new(worker, &id1);
    osl_thread_t * thread2 = osl_thread_new(worker, &id2);
    osl_thread_start(thread1);
    osl_thread_start(thread2);
    osl_thread_join(thread1);
    osl_thread_join(thread2);
    osl_thread_free(thread1);
    osl_thread_free(thread2);
}

int main(int argc, char *argv[])
{
    osl_init_once();
    osl_ignore_sigpipe();

    test_thread();
    
    osl_finish();
    
    return 0;
}
