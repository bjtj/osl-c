#include <stdio.h>
#include <stdlib.h>

#if defined(_WIN32) || defined(_WIN64) /* windows */
#	define OSL_EXPORT __declspec(dllexport)
#else
#	define OSL_EXPORT
#endif

void OSL_EXPORT hello(void)
{
    printf("hello!!!\n");
}
