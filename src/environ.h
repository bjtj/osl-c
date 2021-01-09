#ifndef __OSL_ENVIRON_H__
#define __OSL_ENVIRON_H__

#include "osl.h"

#ifdef __cplusplus
extern "C" {
#endif

    extern OSL_EXPORT char * osl_environ_get_full(void);
    extern OSL_EXPORT char * osl_environ_get(const char * key);
    extern OSL_EXPORT void osl_environ_set(const char * key, const char * value);
    extern OSL_EXPORT void osl_environ_unset(const char * key);

#ifdef __cplusplus
}
#endif

#endif
