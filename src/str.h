#ifndef __STR_H__
#define __STR_H__

#include "osl.h"



#ifdef __cplusplus
extern "C" {
#endif

    int OSL_EXPORT osl_string_is_empty(const char * str);
    char OSL_EXPORT * osl_string_substr(const char * str, int start, int end);
    int OSL_EXPORT osl_string_starts_with(const char * str, const char * query);
    int OSL_EXPORT osl_string_ends_with(const char * str, const char * query);
    const char OSL_EXPORT * osl_string_find_last_of(const char * str, const char * tokens);
    const char OSL_EXPORT * osl_string_find_first_not_of(const char * str, const char * tokens);
    char OSL_EXPORT * osl_strdup(const char * str);
    char OSL_EXPORT * osl_strndup(const char * str, size_t size);
    char OSL_EXPORT * osl_strdup_for(const char * str, const char * end_ptr);

#ifdef __cplusplus
}
#endif

#endif
