#ifndef __OSL_STRING_H__
#define __OSL_STRING_H__

#include "osl.h"

typedef struct _osl_string_buffer_t
{
    char * ptr;
    uint32_t size;
} osl_string_buffer_t;


void osl_string_buffer_clear(osl_string_buffer_t * sb);
void osl_string_buffer_append(osl_string_buffer_t * sb, const char * str);
char * osl_string_buffer_to_string(osl_string_buffer_t * sb);

#endif
