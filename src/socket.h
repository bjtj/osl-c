#ifndef __OSL_SOCKET_H__
#define __OSL_SOCKET_H__

#include "osl.h"
#include "inet_address.h"

#ifdef __cplusplus
extern "C" {
#endif

    extern OSL_EXPORT osl_bool osl_socket_is_valid(osl_socket sock);
    extern OSL_EXPORT osl_socket osl_socket_connect(osl_inet_address_t * addr);
    extern OSL_EXPORT osl_socket osl_socket_connect_with_timeout(osl_inet_address_t * addr, unsigned long timeout_milli);
    extern OSL_EXPORT osl_socket osl_socket_bind(osl_inet_address_t * addr, osl_bool reuseaddr);
    extern OSL_EXPORT osl_socket osl_socket_listen(osl_socket sock, int backlog);
    extern OSL_EXPORT osl_socket osl_socket_accept(osl_socket sock, struct sockaddr * addr, socklen_t * addrlen);
    extern OSL_EXPORT osl_inet_address_t * osl_socket_get_inet_address(osl_socket fd);
    extern OSL_EXPORT osl_socket osl_datagram_socket_connect(osl_inet_address_t * addr);
    extern OSL_EXPORT osl_socket osl_datagram_socket_connect_with_timeout(osl_inet_address_t * addr, unsigned long timeout_milli);
    extern OSL_EXPORT osl_socket osl_datagram_socket_bind(osl_inet_address_t * addr, int reuseaddr);
    extern OSL_EXPORT int osl_datagram_socket_join_group(osl_socket sock, const char * group);
    extern OSL_EXPORT void osl_socket_close(osl_socket sock);

#ifdef __cplusplus
}
#endif

#endif
