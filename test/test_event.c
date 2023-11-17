#include "event.h"
#include "thread.h"
#include <assert.h>

struct _arg_t
{
  int i;
  osl_event_t * event;
  unsigned long due;
  unsigned long timeout;
  int expected_ret;
};

static void * worker_thread(void * arg)
{
  struct _arg_t * x = (struct _arg_t*)arg;
  unsigned long t;
  osl_event_t * event = x->event;

  t = osl_tick_milli();

  assert(osl_event_lock(event) == 0);
  if (x->timeout == 0) {
    assert(osl_event_wait(event) == x->expected_ret);
  } else {
    assert(osl_event_wait_with_timeout(event, x->timeout) == x->expected_ret);
  }
  assert(osl_event_unlock(event) == 0);

  x->due = osl_tick_milli() - t;
  return 0;
}

void test(void)
{
  int i;
  osl_event_t * event = osl_event_init(osl_event_new());
  assert(event != NULL);
  for (i = 0; i < 2; i++) {
    osl_thread_t * thread;
    struct _arg_t arg = {
	    .event = event,
	    .due = 0,
	    .timeout = 0,
	    .expected_ret = 0,
    };
    assert(arg.event != NULL);
    thread = osl_thread_init(osl_thread_new(), worker_thread, &arg);
    assert(thread != NULL);
    osl_thread_start(thread);
	
    osl_idle(1100);

    assert(osl_event_lock(event) == 0);
    assert(osl_event_notify(event) == 0);
    assert(osl_event_unlock(event) == 0);

    osl_thread_join(thread);
    osl_thread_free(thread);
	
    assert(arg.due >= 1000);
  }

  osl_event_free(event);
}

void test2(void)
{
  osl_thread_t * threads[2];
  int i;
  osl_event_t * event = osl_event_init(osl_event_new());
  assert(event != NULL);
  struct _arg_t arg[2];

  for (i = 0; i < 2; ++i) {
    arg[i].event = event;
    arg[i].due = 0;
    arg[i].timeout = 0;
    arg[i].expected_ret = 0;
  }
    
  for (i = 0; i < 2; i++) {
    threads[i] = osl_thread_init(osl_thread_new(), worker_thread, &arg[i]);
    assert(threads[i] != NULL);
    osl_thread_start(threads[i]);
  }

  osl_idle(1100);

  assert(osl_event_lock(event) == 0);
  assert(osl_event_notify_all(event) == 0);
  assert(osl_event_unlock(event) == 0);

  for (i = 0; i < 2; ++i) {
    osl_thread_join(threads[i]);
    osl_thread_free(threads[i]);
    assert(arg[i].due >= 1000);
  }

  osl_event_free(event);
}

void test3(void)
{
  osl_event_t * event = osl_event_init(osl_event_new());
  assert(event != NULL);
  {
    osl_thread_t * thread;
    struct _arg_t arg = {
	    .event = event,
	    .due = 0,
	    .timeout = 0,
	    .expected_ret = 0,
    };
    assert(arg.event != NULL);
    thread = osl_thread_init(osl_thread_new(), worker_thread, &arg);
    assert(thread != NULL);
    osl_thread_start(thread);
	
    osl_idle(1100);

    assert(osl_event_lock(event) == 0);
    assert(osl_event_notify(event) == 0);
    assert(osl_event_unlock(event) == 0);

    osl_thread_join(thread);
    osl_thread_free(thread);
	
    assert(arg.due >= 1000);

    /*  */

    arg.due = 0;
    arg.timeout = 1010;
    arg.expected_ret = -1;
    thread = osl_thread_init(osl_thread_new(), worker_thread, &arg);
    assert(thread != NULL);
    osl_thread_start(thread);
	
    osl_idle(1100);

    osl_thread_join(thread);
    osl_thread_free(thread);
    assert(arg.due >= 1000);
  }

  osl_event_free(event);
}

int main(void)
{
  test();
  test2();
#if !defined(USE_APPLE_STD)
  test3();
#endif
  return 0;
}

