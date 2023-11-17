#include "osl.h"
#include "library.h"
#include <assert.h>

typedef void (*hello_fp)(const char*);

void test_library(void)
{
  osl_lib_handle lib;
  osl_lib_symbol hello_sym;
  
  printf("== test library ==\n");
  lib = osl_library_load("./", "hello");
  assert(lib != NULL);

  hello_sym = osl_library_get_symbol(lib, "hello");

  hello_fp hello;
  OSL_PTR2FUNCPTR(hello_fp, hello_sym, hello);

  hello("World!");
  
  osl_library_close(lib);
}

int main(void)
{
  osl_init_once();

  test_library();
    
  osl_finish();
    
  return 0;
}
