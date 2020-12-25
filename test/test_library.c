#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "library.h"
#include "osl.h"
#include <assert.h>

void test_library(void)
{
    printf("== test library ==\n");
#if defined(PLATFORM_WINDOWS)
    osl_lib_handle lib = osl_library_load("./", "hello");
#else
    osl_lib_handle lib = osl_library_load("./test", "hello");
#endif
    ((void (*)(void))osl_library_get_symbol(lib, "hello"))();
    osl_library_close(lib);
}

int main(int argc, char *argv[])
{
    osl_init_once();

    test_library();
    
    osl_finish();
    
    return 0;
}