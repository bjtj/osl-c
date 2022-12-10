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

  extern OSL_EXPORT osl_url_t * osl_url_from_string(const char *);
  extern OSL_EXPORT void osl_url_free(osl_url_t *);
  extern OSL_EXPORT osl_url_t * osl_url_merge(osl_url_t *, const char *);
  extern OSL_EXPORT char * osl_url_to_str(osl_url_t *);

#ifdef __cplusplus
}
#endif

#endif
