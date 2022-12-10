#ifndef __OSL_NUMBER_H__
#define __OSL_NUMBER_H__

#include "osl.h"

#ifdef __cplusplus
extern "C" {
#endif

  extern OSL_EXPORT int osl_parseInt(const char *, int);
  extern OSL_EXPORT long osl_parseLong(const char *, int);
  extern OSL_EXPORT float osl_parseFloat(const char *);
  extern OSL_EXPORT double osl_parseDouble(const char *);

#ifdef __cplusplus
}
#endif

#endif
