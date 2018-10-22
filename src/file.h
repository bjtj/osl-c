#ifndef __FILE_H__
#define __FILE_H__

#include "osl.h"

#if defined(USE_UNIX_STD)
typedef off_t osl_filesize_t;
#elif defined(USE_MS_WIN)
typedef unsigned long long osl_filesize_t;
#else
// unknown platform
typedef unsigned long osl_filesize_t;
#endif

#ifdef __cplusplus
extern "C" {
#endif

    FILE OSL_EXPORT * osl_file_open(const char * path, const char * flags);
    void OSL_EXPORT osl_file_close(FILE * fp);
    osl_filesize_t OSL_EXPORT osl_file_size(const char * path);
    char OSL_EXPORT * osl_file_dump(FILE * fp);


#ifdef __cplusplus
}
#endif

#endif
