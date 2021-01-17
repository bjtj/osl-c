#ifndef __OSL_INET_ADDRESS_H__
#define __OSL_INET_ADDRESS_H__

#include "osl.h"

typedef enum _osl_inet_version_e {
    osl_inet_unspec,
    osl_inet4,
    osl_inet6
} osl_inet_version_e ;

typedef struct _osl_inet_address_t
{
    osl_inet_version_e inet_version;
    char * host;
    int port;
} osl_inet_address_t;


#ifdef __cplusplus
extern "C" {
#endif

    extern OSL_EXPORT const char * osl_inet_version_to_str(osl_inet_version_e);
    extern OSL_EXPORT char * osl_ip_string(struct sockaddr *, char *, size_t);
    extern OSL_EXPORT osl_inet_address_t * osl_inet_address_new(void);
    extern OSL_EXPORT osl_inet_address_t * osl_inet_address_init(osl_inet_address_t *, osl_inet_version_e, const char *, int);
    extern OSL_EXPORT osl_inet_address_t * osl_inet_address_init_with_sockaddr(osl_inet_address_t *, struct sockaddr *);
    extern OSL_EXPORT void osl_inet_address_free(osl_inet_address_t *);
    extern OSL_EXPORT int osl_inet_address_get_family(osl_inet_address_t *);
    extern OSL_EXPORT struct addrinfo * osl_inet_address_resolve(osl_inet_address_t *, int);
    extern OSL_EXPORT struct addrinfo * osl_inet_address_resolve_numeric(osl_inet_address_t *, int);
    extern OSL_EXPORT struct addrinfo * osl_inet_address_resolve_passive(osl_inet_address_t *, int, int);

#ifdef __cplusplus
}
#endif

#endif
