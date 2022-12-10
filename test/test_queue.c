#include "queue.h"
#include <assert.h>


void test(void)
{
    osl_queue_t * queue = osl_queue_init(osl_queue_new());

    osl_queue_push(queue, (void*)"a");
    osl_queue_push(queue, (void*)"b");
    osl_queue_push(queue, (void*)"c");
    osl_queue_push(queue, (void*)"d");

    assert(queue->count == 4);
    assert(osl_queue_is_empty(queue) == osl_false);

    assert(strcmp((char*)osl_queue_pop(queue), "a") == 0);
    assert(strcmp((char*)osl_queue_pop(queue), "b") == 0);
    assert(strcmp((char*)osl_queue_pop(queue), "c") == 0);
    assert(strcmp((char*)osl_queue_pop(queue), "d") == 0);

    assert(queue->count == 0);
    assert(osl_queue_is_empty(queue) == osl_true);

    osl_queue_free(queue, NULL);
}


void test2(void)
{
    osl_queue_t * queue = osl_queue_init(osl_queue_new());

    osl_queue_push(queue, (void*)strdup("a"));
    osl_queue_push(queue, (void*)strdup("b"));
    osl_queue_push(queue, (void*)strdup("c"));
    osl_queue_push(queue, (void*)strdup("d"));

    assert(queue->count == 4);

    osl_queue_free(queue, osl_safe_free);
}


void test3(void)
{
    char * str;
    osl_queue_t * queue = osl_queue_init(osl_queue_new());

    osl_queue_push(queue, (void*)strdup("a"));
    osl_queue_push(queue, (void*)strdup("b"));
    osl_queue_push(queue, (void*)strdup("c"));
    osl_queue_push(queue, (void*)strdup("d"));

    assert(queue->count == 4);

    str = (char*)osl_queue_pop(queue);
    assert(strcmp(str, "a") == 0);
    osl_safe_free(str);
    str = (char*)osl_queue_pop(queue);
    assert(strcmp(str, "b") == 0);
    osl_safe_free(str);
    str = (char*)osl_queue_pop(queue);
    assert(strcmp(str, "c") == 0);
    osl_safe_free(str);
    str = (char*)osl_queue_pop(queue);
    assert(strcmp(str, "d") == 0);
    osl_safe_free(str);

    assert(queue->count == 0);

    osl_queue_free(queue, osl_safe_free);
}

void test4()
{
  osl_queue_t * queue = osl_queue_init_with_limit(osl_queue_new(), 10);
  int i;
  for (i = 0; i < 10; ++i) {
    assert(osl_queue_push(queue, "a" + i) == 0);
  }
  assert(osl_queue_push(queue, "x") < 0);
  assert(queue->count == 10);
  for (i = 0; i < 10; ++i) {
    assert(strcmp(osl_queue_pop(queue), "a" + i) == 0);
  }
  assert(osl_queue_pop(queue) == NULL);
  osl_queue_free(queue, osl_safe_free);
}

int main()
{
    test();
    test2();
    test3();
    test4();
    return 0;
}
