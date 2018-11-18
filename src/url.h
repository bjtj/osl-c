#ifndef __OSL_URL_H__
#define __OSL_URL_H__

#include "osl.h"
#include "list.h"

typedef struct _osl_url_t
{
    char * protocol;
    char * username;
    char * passphase;
    char * hostname;
    char * port;
    char * path;
    char * query;
} osl_url_t;

#ifdef __cplusplus
extern "C" {
#endif

    osl_url_t OSL_EXPORT * osl_url_from_string(const char * url);
    void OSL_EXPORT osl_url_free(osl_url_t * url);
    osl_url_t OSL_EXPORT * osl_url_merge(osl_url_t * base_url, const char * relative);
    char OSL_EXPORT * osl_url_to_string(osl_url_t * url);

#ifdef __cplusplus
}
#endif

#endif
