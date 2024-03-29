#include "string_buffer.h"

#define _MIN(A, B) (((A) < (B)) ? (A) : (B))

static void * _realloc(void * ptr, size_t org_size, size_t new_size)
{
  void * temp;
  if (org_size == new_size)
  {
    return ptr;
  }
  temp = malloc(new_size);
  OSL_HANDLE_MALLOC_ERROR(temp);
  memset(temp, 0, new_size);
  if (ptr)
  {
    memcpy(temp, ptr, _MIN(org_size, new_size));
    osl_safe_free(ptr);
  }
  return temp;
}

osl_string_buffer_t * osl_string_buffer_new(void)
{
  osl_string_buffer_t * sb = (osl_string_buffer_t*)malloc(sizeof(osl_string_buffer_t));
  OSL_HANDLE_MALLOC_ERROR(sb);
  memset(sb, 0, sizeof(osl_string_buffer_t));
  return sb;
}

osl_string_buffer_t * osl_string_buffer_init(osl_string_buffer_t * stream)
{
  return stream;
}

void osl_string_buffer_free(osl_string_buffer_t * sb)
{
  if (sb->ptr)
  {
    osl_safe_free(sb->ptr);
  }
  osl_safe_free(sb);
}

osl_bool osl_string_buffer_is_empty(osl_string_buffer_t * sb)
{
  return OSL_BOOL(sb->len == 0);
}

void osl_string_buffer_clear(osl_string_buffer_t * sb)
{
  memset(sb->ptr, 0, sb->capacity);
  sb->len = 0;
}

void osl_string_buffer_append(osl_string_buffer_t * sb, const char * str)
{
  osl_string_buffer_append_buffer(sb, str, strlen(str));
}

void osl_string_buffer_append_buffer(osl_string_buffer_t * sb, const char * buffer, size_t size)
{
  if (sb->capacity <= (sb->len + size))
  {
    size_t new_size = (((sb->len + size) / 1024) + 1) * 1024;
    sb->ptr = _realloc(sb->ptr, sb->len, new_size);
    sb->capacity = new_size;
  }

  memcpy(sb->ptr + sb->len, buffer, size);
  sb->len += size;
}

void osl_string_buffer_append_char(osl_string_buffer_t * sb, char ch)
{
  osl_string_buffer_append_buffer(sb, &ch, 1);
}

char * osl_string_buffer_to_str(osl_string_buffer_t * sb)
{
  char * str = (char*)malloc(sb->len + 1);
  OSL_HANDLE_MALLOC_ERROR(str);
  memcpy(str, sb->ptr, sb->len);
  str[sb->len] = '\0';
  return str;
}

char * osl_string_buffer_to_str_and_free(osl_string_buffer_t * sb)
{
  char * str = osl_string_buffer_to_str(sb);
  osl_string_buffer_free(sb);
  return str;
}
