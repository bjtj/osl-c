#ifndef __OSL_STRING_H__
#define __OSL_STRING_H__

#include "osl.h"

typedef struct _osl_string_buffer_t
{
    char * ptr;
    size_t len;
    size_t capacity;
} osl_string_buffer_t;

#ifdef __cplusplus
extern "C" {
#endif

    extern OSL_EXPORT osl_string_buffer_t * osl_string_buffer_new(void);
    extern OSL_EXPORT void osl_string_buffer_free(osl_string_buffer_t * sb);
    extern OSL_EXPORT void osl_string_buffer_append(osl_string_buffer_t * sb, const char * str);
    extern OSL_EXPORT void osl_string_buffer_append_buffer(osl_string_buffer_t * sb, const char * buffer, size_t size);
    extern OSL_EXPORT char * osl_string_buffer_to_string(osl_string_buffer_t * sb);
    extern OSL_EXPORT char * osl_string_buffer_to_string_and_free(osl_string_buffer_t * sb);

#ifdef __cplusplus
}
#endif

#endif
