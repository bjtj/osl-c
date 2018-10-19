#ifndef __SOCKET_H__
#define __SOCKET_H__

#include "osl.h"
#include "inet_address.h"

void osl_inet_address_from_sockaddr(struct sockaddr addr);
int osl_server_socket_bind(osl_inet_address_t * addr, int reuseaddr);
osl_inet_address_t * osl_socket_get_inet_address(int fd);

#endif
