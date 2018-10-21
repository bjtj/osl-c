#ifndef __OSL_STRING_H__
#define __OSL_STRING_H__

#include "osl.h"

typedef struct _osl_string_buffer_t
{
    char * ptr;
    uint32_t size;
} osl_string_buffer_t;


osl_string_buffer_t OSL_EXPORT * osl_string_buffer_new(void);
void OSL_EXPORT osl_string_buffer_free(osl_string_buffer_t * sb);
void OSL_EXPORT osl_string_buffer_append(osl_string_buffer_t * sb, const char * str);
char OSL_EXPORT * osl_string_buffer_to_string(osl_string_buffer_t * sb);
char OSL_EXPORT * osl_string_buffer_to_string_and_free(osl_string_buffer_t * sb);

#endif
