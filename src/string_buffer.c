#include "string_buffer.h"

void osl_string_buffer_clear(osl_string_buffer_t * sb)
{
    if (sb->ptr)
    {
	free(sb->ptr);
    }
}

void osl_string_buffer_append(osl_string_buffer_t * sb, const char * str)
{
    if (sb->size == 0)
    {
	sb->ptr = strdup(str);
	sb->size = strlen(str);
	return;
    }

    char * new_ptr = malloc(sb->size + strlen(str) + 1);
    memcpy(new_ptr, sb->ptr, sb->size);
    memcpy(new_ptr + sb->size, str, strlen(str));
    new_ptr[sb->size + strlen(str)] = '\0';
    sb->size = sb->size + strlen(str);
    free(sb->ptr);
    sb->ptr = new_ptr;
}

char * osl_string_buffer_to_string(osl_string_buffer_t * sb)
{
    char * str = (char*)malloc(sb->size + 1);
    memcpy(str, sb->ptr, sb->size);
    str[sb->size] = '\0';
    return str;
}
