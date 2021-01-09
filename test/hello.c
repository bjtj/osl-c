#include <stdio.h>
#include <stdlib.h>

#if defined(_WIN32) || defined(_WIN64) /* windows */
#	define OSL_EXPORT __declspec(dllexport)
#else
#	define OSL_EXPORT
#endif

OSL_EXPORT void hello(const char * name)
{
    printf("Hello, %s\n", name);
}
