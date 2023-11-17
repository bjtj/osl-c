#include "looper.h"
#include "osl.h"
#include <assert.h>

void _looper_print_task_cb(osl_looper_t * looper, void * user)
{
  (void)looper;
  (void)user;
  printf("MSG -- %s\n", (char*)user);
}

void _looper_hello_and_bye_task_cb(osl_looper_t * looper, void * user)
{
  (void)user;
  printf("hello and bye!\n");
  osl_looper_finish(looper);
}

void test_looper(void)
{
  printf("== test looper ==\n");
  osl_looper_t * looper = osl_looper_init(osl_looper_new(), "looper-1", NULL);

  osl_looper_add_task(looper, _looper_print_task_cb, "a");
  osl_looper_add_task(looper, _looper_print_task_cb, "b");
  osl_looper_add_task(looper, _looper_print_task_cb, "c");
  osl_looper_add_task(looper, _looper_hello_and_bye_task_cb, NULL);
  osl_looper_add_task(looper, _looper_hello_and_bye_task_cb, NULL);
  osl_looper_run(looper);

  osl_looper_free(looper);
}

int main(void)
{
  osl_init_once();
  osl_ignore_sigpipe();
  osl_use_socket();

  test_looper();
    
  osl_finish();
    
  return 0;
}
