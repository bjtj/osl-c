#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "log.h"
#include "osl.h"
#include <assert.h>

void test_log(void)
{
    osl_log_trace("hello");
    osl_log_debug("hello");
    osl_log_info("hello");
    osl_log_warn("hello");
    osl_log_error("hello");
    osl_log_fatal("hello");
}

int main(int argc, char *argv[])
{
    osl_init_once();

    test_log();
    
    osl_finish();
    
    return 0;
}

