#ifndef __OSL_SOCKET_H__
#define __OSL_SOCKET_H__

#include "osl.h"
#include "inet_address.h"

#ifdef __cplusplus
extern "C" {
#endif

    osl_bool OSL_EXPORT osl_socket_is_valid(osl_socket sock);
    osl_socket OSL_EXPORT osl_socket_connect(osl_inet_address_t * addr);
    osl_socket OSL_EXPORT osl_socket_connect_with_timeout(osl_inet_address_t * addr, unsigned long timeout_milli);
    osl_socket OSL_EXPORT osl_socket_bind(osl_inet_address_t * addr, int reuseaddr);
    osl_socket OSL_EXPORT osl_socket_listen(osl_socket sock, int backlog);
    osl_socket OSL_EXPORT osl_socket_accept(osl_socket sock, struct sockaddr * addr, socklen_t * addrlen);
    osl_inet_address_t OSL_EXPORT * osl_socket_get_inet_address(int fd);
    osl_socket OSL_EXPORT osl_datagram_socket_connect(osl_inet_address_t * addr);
    osl_socket OSL_EXPORT osl_datagram_socket_connect_with_timeout(osl_inet_address_t * addr, unsigned long timeout_milli);
    osl_socket OSL_EXPORT osl_datagram_socket_bind(osl_inet_address_t * addr, int reuseaddr);
    int OSL_EXPORT osl_datagram_socket_join_group(osl_socket sock, const char * group);
    void OSL_EXPORT osl_socket_close(osl_socket sock);

#ifdef __cplusplus
}
#endif

#endif
