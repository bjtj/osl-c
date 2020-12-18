#ifndef __OSL_PATHNAME_H__
#define __OSL_PATHNAME_H__

#include "osl.h"

#ifdef __cplusplus
extern "C" {
#endif

    osl_filesize_t OSL_EXPORT osl_pathname_filesize(const char * path);
    osl_bool OSL_EXPORT osl_pathname_exists(const char * path);
    osl_bool OSL_EXPORT osl_pathname_is_dir(const char * path);
    osl_bool OSL_EXPORT osl_pathname_is_file(const char * path);
    char OSL_EXPORT * osl_pathname_merge(const char * a, const char * b);
    char OSL_EXPORT * osl_pathname_basename(const char * path);
    char OSL_EXPORT * osl_pathname_dirname(const char * path);

#ifdef __cplusplus
}
#endif

#endif
