#ifndef __OSL_PATHNAME_H__
#define __OSL_PATHNAME_H__

#include "osl.h"

#if defined(USE_MS_WIN)
#	define OSL_PATH_SEPARATOR "\\"
#else
#	define OSL_PATH_SEPARATOR "/"
#endif

#ifdef __cplusplus
extern "C" {
#endif
    
  extern OSL_EXPORT char * osl_pathname_merge(const char *, const char *);
  extern OSL_EXPORT char * osl_pathname_basename(const char *);
  extern OSL_EXPORT char * osl_pathname_dirname(const char *);

#ifdef __cplusplus
}
#endif

#endif
