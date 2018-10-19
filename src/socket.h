#ifndef __SOCKET_H__
#define __SOCKET_H__

#include "osl.h"
#include "inet_address.h"

int osl_socket_connect(osl_inet_address_t * addr);
int osl_socket_connect_with_timeout(osl_inet_address_t * addr, unsigned long timeout_milli);
int osl_server_socket_bind(osl_inet_address_t * addr, int reuseaddr);
osl_inet_address_t * osl_socket_get_inet_address(int fd);

#endif
