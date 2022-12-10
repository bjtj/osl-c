#ifndef __OSL_ENVIRON_H__
#define __OSL_ENVIRON_H__

#include "osl.h"

#ifdef __cplusplus
extern "C" {
#endif

  extern OSL_EXPORT char * osl_environ_get_full(void);
  extern OSL_EXPORT char * osl_environ_get(const char *);
  extern OSL_EXPORT void osl_environ_set(const char *, const char *);
  extern OSL_EXPORT void osl_environ_unset(const char *);

#ifdef __cplusplus
}
#endif

#endif
