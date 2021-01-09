#ifndef __OSL_STR_H__
#define __OSL_STR_H__

#include "osl.h"
#include "list.h"


#ifdef __cplusplus
extern "C" {
#endif

    extern OSL_EXPORT const char * osl_string_end_ptr(const char * str);
    extern OSL_EXPORT osl_bool osl_string_is_empty(const char * str);
    extern OSL_EXPORT osl_bool osl_string_equals(const char * s1, const char * s2);
    extern OSL_EXPORT osl_bool osl_string_equals_ignorecase(const char * s1, const char * s2);
    extern OSL_EXPORT char * osl_string_substr(const char * str, size_t start, size_t end);
    extern OSL_EXPORT osl_bool osl_string_starts_with(const char * str, const char * query);
    extern OSL_EXPORT osl_bool osl_string_starts_with_ignorecase(const char * str, const char * query);
    extern OSL_EXPORT osl_bool osl_string_ends_with(const char * str, const char * query);
    extern OSL_EXPORT osl_bool osl_string_ends_with_ignorecase(const char * str, const char * query);
    extern OSL_EXPORT const char * osl_string_find(const char * str, const char * match);
    extern OSL_EXPORT const char * osl_string_find_last_of(const char * str, const char * tokens);
    extern OSL_EXPORT const char * osl_string_find_last_not_of(const char * str, const char * tokens);
    extern OSL_EXPORT const char * osl_string_find_first_of(const char * str, const char * tokens);
    extern OSL_EXPORT const char * osl_string_find_first_not_of(const char * str, const char * tokens);
    extern OSL_EXPORT char * osl_string_replace_all(const char * str, const char * mat, const char * sub);
    extern OSL_EXPORT char * osl_string_uppercase(const char * str);
    extern OSL_EXPORT char * osl_string_lowercase(const char * str);
    extern OSL_EXPORT char * osl_string_capital(const char * str);
    extern OSL_EXPORT char * osl_strcat(char * s1, const char * s2);
    extern OSL_EXPORT char * osl_strdup(const char * str);
    extern OSL_EXPORT char * osl_strndup(const char * str, size_t size);
    extern OSL_EXPORT char * osl_strdup_for(const char * str, const char * end_ptr);
    extern OSL_EXPORT const char * osl_string_safe(const char * str, const char * def);
    extern OSL_EXPORT int osl_strcmp(const char * a, const char * b);
    extern OSL_EXPORT int osl_strcmp_ignorecase(const char * a, const char * b);
    extern OSL_EXPORT osl_list_t * osl_string_split(const char * str, const char * del);
    extern OSL_EXPORT osl_list_t * osl_string_split_limit(const char * str, const char * del, int limit);
    extern OSL_EXPORT char * osl_string_trim(const char * str);
    extern OSL_EXPORT char * osl_string_ltrim(const char * str);
    extern OSL_EXPORT char * osl_string_rtrim(const char * str);
    extern OSL_EXPORT char * osl_string_append(const char * s1, const char * s2);

#ifdef __cplusplus
}
#endif

#endif
