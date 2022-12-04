#ifndef __OSL_BYTE_STREAM_H__
#define __OSL_BYTE_STREAM_H__

#include "stream.h"

#ifdef __cplusplus
extern "C" {
#endif

  extern OSL_EXPORT osl_stream_t * osl_byte_stream_init(osl_stream_t *, uint8_t *, size_t);

#ifdef __cplusplus
}
#endif

#endif
