#include "osl.h"
#include "string_buffer.h"
#include <assert.h>

void test_string_buffer(void)
{
  printf("== test string buffer ==\n");
  osl_string_buffer_t * sb = osl_string_buffer_init(osl_string_buffer_new());
  osl_string_buffer_append(sb, "hello");
  osl_string_buffer_append(sb, " ");
  osl_string_buffer_append(sb, "world");

  char * str = osl_string_buffer_to_str(sb);
  printf("str is '%s'\n", str);
  osl_safe_free(str);

  osl_string_buffer_free(sb);
}

int main(void)
{
  osl_init_once();

  test_string_buffer();

  osl_finish();
    
  return 0;
}
