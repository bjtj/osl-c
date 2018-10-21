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
    struct addrinfo * res = osl_inet_address_resolve(addr, SOCK_STREAM);
    if (res == NULL)
    {
	return -1;
    }

    sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sock < 0)
    {
	perror("socket() failed");
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
    struct addrinfo * res = osl_inet_address_resolve_passive(addr, osl_inet_address_get_family(addr), SOCK_STREAM);
    if (res == NULL)
    {
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

int osl_datagram_socket_connect(osl_inet_address_t * addr)
{
    return osl_datagram_socket_connect_with_timeout(addr, 0);
}

int osl_datagram_socket_connect_with_timeout(osl_inet_address_t * addr, unsigned long timeout_milli)
{
    int ret = -1;
    int sock;
    struct addrinfo * res = osl_inet_address_resolve(addr, SOCK_DGRAM);
    if (res == NULL)
    {
	return -1;
    }

    sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sock < 0)
    {
	perror("socket() failed");
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

int osl_datagram_socket_bind(osl_inet_address_t * addr, int reuseaddr)
{
    int ret = -1;
    struct addrinfo * res = osl_inet_address_resolve_passive(addr, osl_inet_address_get_family(addr), SOCK_DGRAM);
    if (res == NULL)
    {
	return -1;
    }

    int sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sock < 0)
    {
	perror("socket() failed");
	goto done;
    }

    if (reuseaddr) {
	int on = 1;
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&on, sizeof(on)) != 0)
	{
	    perror("setsockopt() failed");
	    goto done;
	}
#if defined(__APPLE__)
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, (const char*)&on, sizeof(on)) != 0)
	{
	    perror("setsockopt() failed");
	    goto done;
	}
#endif
    }

    if (bind(sock, res->ai_addr, res->ai_addrlen) != 0) {
	perror("bind() failed");
	goto done;
    }

    ret = sock;

done:
    freeaddrinfo(res);
    return ret;
}

int osl_datagram_socket_join_group(int sock, const char * group)
{
    int ret = -1;
    osl_inet_address_t * group_addr = osl_inet_address_new(osl_inet_unspec, group, -1);
    struct addrinfo * group_res = osl_inet_address_resolve_numeric(group_addr, SOCK_DGRAM);
    if (group_res->ai_family == AF_INET)
    {
	struct ip_mreq mreq;
	struct sockaddr_in * addr = (struct sockaddr_in*)group_res->ai_addr;
	memcpy(&mreq.imr_multiaddr, &(addr->sin_addr), sizeof(mreq.imr_multiaddr));
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mreq, sizeof(mreq)) != 0)
	{
	    perror("setsockopt() failed");
	    goto done;
	}
    }
    else if (group_res->ai_family == AF_INET6)
    {
	struct ipv6_mreq mreq;
	struct sockaddr_in6 * addr = (struct sockaddr_in6*)group_res->ai_addr;
	memcpy(&(mreq.ipv6mr_multiaddr), &(addr->sin6_addr), sizeof(mreq.ipv6mr_multiaddr));
	mreq.ipv6mr_interface = 0;
	if (setsockopt(sock, IPPROTO_IPV6, IPV6_JOIN_GROUP, (char *)&mreq, sizeof(mreq)) != 0)
	{
	    perror("setsockopt() failed");
	    goto done;
	}
    }
    else
    {
	fprintf(stderr, "unkonwn socket type\n");
	goto done;
    }
    ret = 0;
done:
    osl_inet_address_free(group_addr);
    freeaddrinfo(group_res);
    return ret;
}

void osl_socket_close(int sock)
{
#if defined(USE_BSD_SOCKET)
    close(sock);
#elif defined(USE_WINSOCK2)
    closesocket(sock);
#endif
}
