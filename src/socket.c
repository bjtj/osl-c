#include "socket.h"
#include "selector.h"

int s_set_nonblocking_socket(int sock, int enable)
{
#if defined(USE_WINSOCK2)
    u_long mode = enable ? 1 : 0;
    if (ioctlsocket(sock, FIONBIO, &mode) != 0) {
	return -1;
    }
#elif defined(USE_BSD_SOCKET)
    int flags = fcntl(sock, F_GETFL);
    flags = enable ? (flags | O_NONBLOCK) : (flags & ~O_NONBLOCK);
    if (fcntl(sock, F_SETFL, flags) != 0) {
	perror("fcntl() failed");
	return -1;
    }
#endif
    return 0;
}

static int s_err_continue()
{
#if defined(USE_WINSOCK2)
    return (WSAGetLastError() == WSAEWOULDBLOCK);
#elif defined(USE_BSD_SOCKET)
    return (errno == EINPROGRESS);
#endif
}

int osl_socket_connect(osl_inet_address_t * addr)
{
    return osl_socket_connect_with_timeout(addr, 0);
}

int osl_socket_connect_with_timeout(osl_inet_address_t * addr, unsigned long timeout_milli)
{
    int ret = -1;
    int sock;
    struct addrinfo * res;
    char portStr[10] = {0,};
    struct addrinfo hints;
    
    snprintf(portStr, sizeof(portStr), "%d", addr->port);
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    if (getaddrinfo(addr->host, (addr->port == 0 ? NULL : portStr), &hints, &res) != 0) {
	return -1;
    }

    sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sock < 0)
    {
	goto done;
    }

    if (timeout_milli == 0)
    {
	if (connect(sock, res->ai_addr, res->ai_addrlen) != 0)
	{
	    perror("connect() failed");
	    goto done;
	}
	ret = sock;
	goto done;
    }
    
    s_set_nonblocking_socket(sock, 1);
    if (connect(sock, res->ai_addr, res->ai_addrlen) != 0)
    {
	if (!s_err_continue()) {
	    perror("connect() failed");
	    goto done;
	}
    }

    osl_selector_t selector = {0,};
    osl_selector_clear(&selector);
    osl_selector_register(&selector, sock, osl_write);
    if (osl_selector_select(&selector, timeout_milli) <= 0)
    {
	fprintf(stderr, "connection timeout\n");
	goto done;
    }

    {
	int err;
	socklen_t len = sizeof(err);
	if (getsockopt(sock, SOL_SOCKET, SO_ERROR, (char*)&err, &len) != 0)
	{
	    perror("getsockopt() failed");
	    goto done;
	}
	if (err) {
	    fprintf(stderr, "connect() failed -- %d\n", err);
	    goto done;
	}
    }
    
    s_set_nonblocking_socket(sock, 0);
    ret = sock;
done:
    freeaddrinfo(res);
    return ret;
}

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
