#ifndef __INET_ADDRESS_H__
#define __INET_ADDRESS_H__

#include "osl.h"

typedef enum _osl_inet_version_e {
    osl_inet_unspec,
    osl_inet4,
    osl_inet6
} osl_inet_version_e ;

typedef struct _osl_inet_address_t
{
    osl_inet_version_e inet_version;
    char host[INET6_ADDRSTRLEN];
    int port;
} osl_inet_address_t;

#endif

osl_inet_address_t * osl_inet_address_new_with_sockaddr(struct sockaddr * addr);
osl_inet_address_t * osl_inet_address_new(osl_inet_version_e version, const char * host, int port);
void osl_inet_address_free(osl_inet_address_t * addr);

