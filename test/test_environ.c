#include "environ.h"
#include "osl.h"
#include <assert.h>

void test_environ(void)
{
    printf("== test environ ==\n");
    char * val = osl_environ_get("USER");
    printf("user: %s\n", val);
    osl_safe_free(val);

    val = osl_environ_get("__MYVAL__");
    printf("myval: %s\n", val);
    osl_safe_free(val);

    osl_environ_set("__MYVAL__", "hello");

    val = osl_environ_get("__MYVAL__");
    printf("myval: %s\n", val);
    osl_safe_free(val);

    osl_environ_unset("__MYVAL__");

    val = osl_environ_get("__MYVAL__");
    printf("myval: %s\n", val);
    osl_safe_free(val);

    printf(" -- full environment --\n");
    char * full = osl_environ_get_full();
    printf("%s\n", full);
    osl_safe_free(full);
}

int main()
{
    osl_init_once();

    test_environ();
    
    osl_finish();
    
    return 0;
}
