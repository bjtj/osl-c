#ifndef __OSL_SOCKET_H__
#define __OSL_SOCKET_H__

#include "osl.h"
#include "inet_address.h"

#if defined(USE_MS_WIN)
#	define OSL_INVALID_SOCKET INVALID_SOCKET
#else
#	define OSL_INVALID_SOCKET -1
#endif

#ifdef __cplusplus
extern "C" {
#endif

    extern OSL_EXPORT osl_bool osl_socket_is_valid(osl_socket);
    extern OSL_EXPORT osl_socket osl_socket_connect(osl_inet_address_t *);
    extern OSL_EXPORT osl_socket osl_socket_connect_with_timeout(osl_inet_address_t *, unsigned long);
    extern OSL_EXPORT osl_socket osl_socket_bind(osl_inet_address_t *, osl_bool);
    extern OSL_EXPORT osl_socket osl_socket_listen(osl_socket, int);
    extern OSL_EXPORT osl_socket osl_socket_accept(osl_socket, struct sockaddr *, socklen_t *);
    extern OSL_EXPORT int osl_socket_send(osl_socket, const void *, size_t, int);
    extern OSL_EXPORT int osl_socket_recv(osl_socket, void *, size_t, int);
    extern OSL_EXPORT osl_inet_address_t * osl_socket_get_inet_address(osl_socket);
    extern OSL_EXPORT osl_socket osl_datagram_socket_connect(osl_inet_address_t *);
    extern OSL_EXPORT osl_socket osl_datagram_socket_connect_with_timeout(osl_inet_address_t *, unsigned long);
    extern OSL_EXPORT osl_socket osl_datagram_socket_bind(osl_inet_address_t *, int);
    extern OSL_EXPORT int osl_datagram_socket_send(osl_socket, const void *, size_t, int);
    extern OSL_EXPORT int osl_datagram_socket_sendto(osl_socket, const void *, size_t, int,  const struct sockaddr *, socklen_t);
    extern OSL_EXPORT int osl_datagram_socket_recv(osl_socket, void *, size_t, int);
    extern OSL_EXPORT int osl_datagram_socket_recvfrom(osl_socket, void *, size_t, int, struct sockaddr *, socklen_t *);
    extern OSL_EXPORT int osl_datagram_socket_join_group(osl_socket, const char *);
    extern OSL_EXPORT void osl_socket_close(osl_socket);

#ifdef __cplusplus
}
#endif

#endif
