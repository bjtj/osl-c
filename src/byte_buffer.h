#ifndef __OSL_BYTE_BUFFER_H__
#define __OSL_BYTE_BUFFER_H__

#include "osl.h"

typedef struct _osl_byte_buffer_t
{
  uint8_t * ptr;
  size_t len;
  size_t capacity;
} osl_byte_buffer_t;

#ifdef __cplusplus
extern "C" {
#endif

  extern OSL_EXPORT osl_byte_buffer_t * osl_byte_buffer_new(void);
  extern OSL_EXPORT osl_byte_buffer_t * osl_byte_buffer_init(osl_byte_buffer_t *);
  extern OSL_EXPORT void osl_byte_buffer_free(osl_byte_buffer_t *);
  extern OSL_EXPORT void osl_byte_buffer_append(osl_byte_buffer_t *, uint8_t *, size_t);
  extern OSL_EXPORT uint8_t * osl_byte_buffer_to_array(osl_byte_buffer_t *);

#ifdef __cplusplus
}
#endif

#endif
