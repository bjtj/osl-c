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

    osl_string_buffer_t OSL_EXPORT * osl_string_buffer_new(void);
    void OSL_EXPORT osl_string_buffer_free(osl_string_buffer_t * sb);
    void OSL_EXPORT osl_string_buffer_append(osl_string_buffer_t * sb, const char * str);
    void OSL_EXPORT osl_string_buffer_append_buffer(osl_string_buffer_t * sb, const char * buffer, size_t size);
    char OSL_EXPORT * osl_string_buffer_to_string(osl_string_buffer_t * sb);
    char OSL_EXPORT * osl_string_buffer_to_string_and_free(osl_string_buffer_t * sb);

#ifdef __cplusplus
}
#endif

#endif
