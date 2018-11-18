#ifndef __OSL_STRING_STREAM_H__
#define __OSL_STRING_STREAM_H__

#include "stream.h"

#ifdef __cplusplus
extern "C" {
#endif

    osl_stream_t OSL_EXPORT * osl_string_stream_new(const char * str);

#ifdef __cplusplus
}
#endif

#endif
