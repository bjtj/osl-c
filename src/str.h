#ifndef __OSL_STR_H__
#define __OSL_STR_H__

#include "osl.h"
#include "list.h"


#ifdef __cplusplus
extern "C" {
#endif

    extern OSL_EXPORT const char * osl_string_end_ptr(const char *);
    extern OSL_EXPORT osl_bool osl_string_is_empty(const char *);
    extern OSL_EXPORT osl_bool osl_string_equals(const char *, const char *);
    extern OSL_EXPORT osl_bool osl_string_equals_ignorecase(const char *, const char *);
    extern OSL_EXPORT char * osl_string_substr(const char *, size_t, size_t);
    extern OSL_EXPORT osl_bool osl_string_starts_with(const char *, const char *);
    extern OSL_EXPORT osl_bool osl_string_starts_with_ignorecase(const char *, const char *);
    extern OSL_EXPORT osl_bool osl_string_ends_with(const char *, const char *);
    extern OSL_EXPORT osl_bool osl_string_ends_with_ignorecase(const char *, const char *);
    extern OSL_EXPORT const char * osl_string_find(const char *, const char *);
    extern OSL_EXPORT const char * osl_string_find_last_of(const char *, const char *);
    extern OSL_EXPORT const char * osl_string_find_last_not_of(const char *, const char *);
    extern OSL_EXPORT const char * osl_string_find_first_of(const char *, const char *);
    extern OSL_EXPORT const char * osl_string_find_first_not_of(const char *, const char *);
    extern OSL_EXPORT char * osl_string_replace_all(const char *, const char *, const char *);
    extern OSL_EXPORT char * osl_string_uppercase(const char *);
    extern OSL_EXPORT char * osl_string_lowercase(const char *);
    extern OSL_EXPORT char * osl_string_capital(const char *);
    extern OSL_EXPORT size_t osl_strlen (const char *);
    extern OSL_EXPORT char * osl_strcat(char *, const char *);
    extern OSL_EXPORT char * osl_safe_strdup(const char *);
    extern OSL_EXPORT char * osl_strdup(const char *);
    extern OSL_EXPORT char * osl_strndup(const char *, size_t);
    extern OSL_EXPORT char * osl_strdup_for(const char *, const char *);
    extern OSL_EXPORT const char * osl_string_safe(const char *, const char *);
    extern OSL_EXPORT int osl_strcmp(const char *, const char *);
    extern OSL_EXPORT int osl_strcmp_ignorecase(const char *, const char *);
    extern OSL_EXPORT osl_list_t * osl_string_split(const char *, const char *);
    extern OSL_EXPORT osl_list_t * osl_string_split_limit(const char *, const char *, int);
    extern OSL_EXPORT char * osl_string_trim(const char *);
    extern OSL_EXPORT char * osl_string_ltrim(const char *);
    extern OSL_EXPORT char * osl_string_rtrim(const char *);
    extern OSL_EXPORT char * osl_string_append(const char *, const char *);

#ifdef __cplusplus
}
#endif

#endif
