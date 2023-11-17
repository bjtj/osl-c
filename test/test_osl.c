#include "osl.h"

#include <assert.h>

void test_osl(void)
{
  char * cwd = osl_getcwd();
  printf("cwd: %s\n", cwd);
  osl_safe_free(cwd);
}


int main(void)
{
  osl_init_once();

  test_osl();
    
  osl_finish();
    
  return 0;
}
