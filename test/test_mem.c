#include "osl.h"
#include <assert.h>

void test_mem(void)
{
  const char * str = "hello world";
  char * mem = (char*)osl_memdup((char*)str, (size_t)strlen(str));
  assert(strncmp(mem, str, strlen(str)) == 0);
  osl_safe_free(mem);
}

int main()
{
  osl_init_once();

  test_mem();
    
  osl_finish();
    
  return 0;
}
