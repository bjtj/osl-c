#ifndef __OSL_ENVIRON_H__
#define __OSL_ENVIRON_H__

#include "osl.h"

#ifdef __cplusplus
extern "C" {
#endif

    char OSL_EXPORT * osl_environ_get_full(void);
    char OSL_EXPORT * osl_environ_get(const char * key);
    void OSL_EXPORT osl_environ_set(const char * key, const char * value);
    void OSL_EXPORT osl_environ_unset(const char * key);

#ifdef __cplusplus
}
#endif

#endif
