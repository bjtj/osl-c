#ifndef __STR_H__
#define __STR_H__

#include "osl.h"

#ifdef __cplusplus
extern "C" {
#endif

    int OSL_EXPORT osl_string_is_empty(const char * str);
    char OSL_EXPORT * osl_string_substr(const char * str, int start, int end);
    const char OSL_EXPORT * osl_string_find_last_of(const char * str, const char * tokens);

#ifdef __cplusplus
}
#endif

#endif
