#ifndef __SOCKET_H__
#define __SOCKET_H__

#include "osl.h"
#include "inet_address.h"

#ifdef __cplusplus
extern "C" {
#endif

    int OSL_EXPORT osl_socket_connect(osl_inet_address_t * addr);
    int OSL_EXPORT osl_socket_connect_with_timeout(osl_inet_address_t * addr, unsigned long timeout_milli);
    int OSL_EXPORT osl_server_socket_bind(osl_inet_address_t * addr, int reuseaddr);
    osl_inet_address_t OSL_EXPORT * osl_socket_get_inet_address(int fd);
    int OSL_EXPORT osl_datagram_socket_connect(osl_inet_address_t * addr);
    int OSL_EXPORT osl_datagram_socket_connect_with_timeout(osl_inet_address_t * addr, unsigned long timeout_milli);
    int OSL_EXPORT osl_datagram_socket_bind(osl_inet_address_t * addr, int reuseaddr);
    int OSL_EXPORT osl_datagram_socket_join_group(int sock, const char * group);
    void OSL_EXPORT osl_socket_close(int sock);

#ifdef __cplusplus
}
#endif

#endif
