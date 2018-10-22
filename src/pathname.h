#ifndef __PATHNAME_H__
#define __PATHNAME_H__

#include "osl.h"

#ifdef __cplusplus
extern "C" {
#endif

    int OSL_EXPORT osl_pathname_exists(const char * path);
    int OSL_EXPORT osl_pathname_is_dir(const char * path);
    int OSL_EXPORT osl_pathname_is_file(const char * path);
    char OSL_EXPORT * osl_pathname_merge(const char * a, const char * b);
    char OSL_EXPORT * osl_pathname_basename(const char * path);
    char OSL_EXPORT * osl_pathname_dirname(const char * path);

#ifdef __cplusplus
}
#endif

#endif
