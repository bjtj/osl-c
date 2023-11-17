#include "osl.h"
#include "pathname.h"
#include <assert.h>

void test_pathname(void)
{
  printf("== test pathname ==\n");
  char * name = osl_pathname_basename("/abc/d");
  assert(strcmp(name, "d") == 0);
  osl_safe_free(name);

  name = osl_pathname_dirname("/abc/d");
  assert(strcmp(name, "/abc/") == 0);
  osl_safe_free(name);

  name = osl_pathname_dirname("abc/d");
  assert(strcmp(name, "abc/") == 0);
  osl_safe_free(name);
}

int main(void)
{
  osl_init_once();
  osl_ignore_sigpipe();
  osl_use_socket();

  test_pathname();
    
  osl_finish();
    
  return 0;
}
