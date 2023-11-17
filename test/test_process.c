#include "osl.h"
#include "process.h"
#include <assert.h>

void test_process(void)
{
  puts("== test process==");
  osl_process_t * process = osl_process_init(osl_process_new(), "echo \"hello\"");
  osl_process_start(process);

  osl_stream_t * out = osl_process_out_stream(process);
  int ch;
  while ((ch = osl_stream_read(out)) > 0)
  {
    putchar(ch);
  }
  putchar('\n');
  osl_stream_free(out);

  osl_process_wait(process);
  printf("exit code: %d\n", process->exit_code);
    
  osl_process_close(process);
  printf("free\n");
  osl_process_free(process);

  printf("done -- test process\n");
}

int main(void)
{
  osl_init_once();

  test_process();
    
  osl_finish();
    
  return 0;
}
