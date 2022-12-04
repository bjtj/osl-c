#ifndef __OSL_STRING_BUFFER_H__
#define __OSL_STRING_BUFFER_H__

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
    extern OSL_EXPORT osl_string_buffer_t * osl_string_buffer_init(osl_string_buffer_t *);
    extern OSL_EXPORT void osl_string_buffer_free(osl_string_buffer_t *);
    extern OSL_EXPORT void osl_string_buffer_append(osl_string_buffer_t *, const char *);
    extern OSL_EXPORT void osl_string_buffer_append_buffer(osl_string_buffer_t *, const char *, size_t);
    extern OSL_EXPORT char * osl_string_buffer_to_str(osl_string_buffer_t *);
    extern OSL_EXPORT char * osl_string_buffer_to_str_and_free(osl_string_buffer_t *);

#ifdef __cplusplus
}
#endif

#endif
