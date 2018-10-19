#include "socket.h"

int osl_server_socket_bind(osl_inet_address_t * addr, int reuseaddr)
{
    int ret = -1;
    struct addrinfo * res;
    char portStr[10] = {0,};
    struct addrinfo hints;
    snprintf(portStr, sizeof(portStr), "%d", addr->port);
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = osl_inet_address_get_family(addr);
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if (getaddrinfo(addr->host, (addr->port == 0 ? NULL : portStr), &hints, &res) != 0) {
	return -1;
    }
    int sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sock < 0)
    {
	perror("socket() failed");
	goto done;
    }
    if (reuseaddr && setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr)) != 0)
    {
	perror("setsockopt() failed");
	goto done;
    }
    if (bind(sock, res->ai_addr, res->ai_addrlen) != 0)
    {
	perror("bind() failed");
	goto done;
    }
    ret = sock;
done:
    freeaddrinfo(res);
    return ret;
}

osl_inet_address_t * osl_socket_get_inet_address(int fd)
{
    struct sockaddr_in _addr;
    socklen_t _addr_len = sizeof(_addr);
    if (getsockname(fd, (struct sockaddr*)&_addr, &_addr_len) != 0)
    {
	perror("getsockname() failed");
	return NULL;
    }
    return osl_inet_address_new_with_sockaddr((struct sockaddr*)&_addr);
}
