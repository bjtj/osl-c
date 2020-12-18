#ifndef __OSL_STR_H__
#define __OSL_STR_H__

#include "osl.h"
#include "list.h"


#ifdef __cplusplus
extern "C" {
#endif

    osl_bool OSL_EXPORT osl_string_is_empty(const char * str);
    char OSL_EXPORT * osl_string_substr(const char * str, int start, int end);
    osl_bool OSL_EXPORT osl_string_starts_with(const char * str, const char * query);
    osl_bool OSL_EXPORT osl_string_starts_with_ignorecase(const char * str, const char * query);
    osl_bool OSL_EXPORT osl_string_ends_with(const char * str, const char * query);
    osl_bool OSL_EXPORT osl_string_ends_with_ignorecase(const char * str, const char * query);
    const char OSL_EXPORT * osl_string_find_last_of(const char * str, const char * tokens);
    const char OSL_EXPORT * osl_string_find_first_not_of(const char * str, const char * tokens);
    char OSL_EXPORT * osl_string_replace_all(const char * str, const char * mat, const char * sub);
    char OSL_EXPORT * osl_string_uppercase(const char * str);
    char OSL_EXPORT * osl_string_lowercase(const char * str);
    char OSL_EXPORT * osl_string_capital(const char * str);
    char OSL_EXPORT * osl_strdup(const char * str);
    char OSL_EXPORT * osl_strndup(const char * str, size_t size);
    char OSL_EXPORT * osl_strdup_for(const char * str, const char * end_ptr);
    const char OSL_EXPORT * osl_str_if(const char * str, const char * def);
    int OSL_EXPORT osl_strcmp_ignorecase(const char * a, const char * b);
    osl_list_t OSL_EXPORT * osl_split(const char * str, const char * del);
    osl_list_t OSL_EXPORT * osl_split_limit(const char * str, const char * del, int limit);

#ifdef __cplusplus
}
#endif

#endif
