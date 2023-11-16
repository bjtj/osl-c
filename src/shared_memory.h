#ifndef __OSL_SHARED_MEMORY_H__
#define __OSL_SHARED_MEMORY_H__

#include "osl.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

  extern OSL_EXPORT OSL_SHARED_MEMORY_ID osl_shared_memory_create(const char *, size_t);
  extern OSL_EXPORT OSL_SHARED_MEMORY_ID osl_shared_memory_open(const char *);
  extern OSL_EXPORT char * osl_shared_memory_map(OSL_SHARED_MEMORY_ID, size_t);
  extern OSL_EXPORT void osl_shared_memory_unmap(char *, size_t);
  extern OSL_EXPORT void osl_shared_memory_unlink(const char *);
  extern OSL_EXPORT void osl_shared_memory_close(OSL_SHARED_MEMORY_ID);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* __OSL_SHARED_MEMORY_H__ */
