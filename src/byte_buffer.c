#include "byte_buffer.h"

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

osl_byte_buffer_t * osl_byte_buffer_new(void)
{
  osl_byte_buffer_t * bb = (osl_byte_buffer_t*)malloc(sizeof(osl_byte_buffer_t));
  OSL_HANDLE_MALLOC_ERROR(bb);
  memset(bb, 0, sizeof(osl_byte_buffer_t));
  return bb;
}

osl_byte_buffer_t * osl_byte_buffer_init(osl_byte_buffer_t * stream)
{
  return stream;
}

void osl_byte_buffer_free(osl_byte_buffer_t * bb)
{
  if (bb->ptr)
  {
    osl_safe_free(bb->ptr);
  }
  osl_safe_free(bb);
}

void osl_byte_buffer_append(osl_byte_buffer_t * bb, uint8_t * buffer, size_t size)
{
  if (bb->capacity <= (bb->len + size))
  {
    size_t new_size = (((bb->len + size) / 1024) + 1) * 1024;
    bb->ptr = _realloc(bb->ptr, bb->len, new_size);
    bb->capacity = new_size;
  }

  memcpy(bb->ptr + bb->len, buffer, size);
  bb->len += size;
}


uint8_t * osl_byte_buffer_to_array(osl_byte_buffer_t * bb)
{
  uint8_t * ret = (uint8_t*)malloc(bb->len);
  OSL_HANDLE_MALLOC_ERROR(ret);
  memcpy(ret, bb->ptr, bb->len);
  return ret;
}
