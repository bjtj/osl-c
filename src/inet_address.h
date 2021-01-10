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

    extern OSL_EXPORT const char * osl_inet_version_to_string(osl_inet_version_e ver);
    extern OSL_EXPORT char * osl_ip_string(struct sockaddr * addr, char * ip_buffer, size_t size);
    extern OSL_EXPORT osl_inet_address_t * osl_inet_address_new_with_sockaddr(struct sockaddr * addr);
    extern OSL_EXPORT osl_inet_address_t * osl_inet_address_new(osl_inet_version_e version, const char * host, int port);
    extern OSL_EXPORT void osl_inet_address_free(osl_inet_address_t * addr);
    extern OSL_EXPORT int osl_inet_address_get_family(osl_inet_address_t * addr);
    extern OSL_EXPORT struct addrinfo * osl_inet_address_resolve(osl_inet_address_t * addr, int sock_type);
    extern OSL_EXPORT struct addrinfo * osl_inet_address_resolve_numeric(osl_inet_address_t * addr, int sock_type);
    extern OSL_EXPORT struct addrinfo * osl_inet_address_resolve_passive(osl_inet_address_t * addr, int family, int sock_type);

#ifdef __cplusplus
}
#endif

#endif
