#include "osl.h"
#include "library.h"
#include <assert.h>

void test_library(void)
{
    void (*hello_funcp)(void);
    printf("== test library ==\n");
#if defined(OSL_OS_WINDOWS)
    osl_lib_handle lib = osl_library_load("./", "hello");
#else
    osl_lib_handle lib = osl_library_load("./", "hello");
#endif
    assert(lib != NULL);
    *(void **) (&hello_funcp) = osl_library_get_symbol(lib, "hello");
    hello_funcp();
    osl_library_close(lib);
}

int main()
{
    osl_init_once();

    test_library();
    
    osl_finish();
    
    return 0;
}
