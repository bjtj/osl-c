#include "socket.h"
#include "selector.h"

int s_set_nonblocking_socket(osl_socket sock, int enable)
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

static int s_err_continue(void)
{
#if defined(USE_WINSOCK2)
  return (WSAGetLastError() == WSAEWOULDBLOCK);
#elif defined(USE_BSD_SOCKET)
  return (errno == EINPROGRESS);
#endif
}

osl_bool OSL_EXPORT osl_socket_is_valid(osl_socket sock)
{
#if defined(USE_BSD_SOCKET)
  if (sock < 0) {
    return osl_false;
  }

#elif defined(USE_WINSOCK2)
  if (sock == INVALID_SOCKET) {
    return osl_false;
  }
#endif
  return osl_true;
}

osl_socket osl_socket_connect(osl_inet_address_t * addr)
{
  return osl_socket_connect_with_timeout(addr, 0);
}

osl_socket osl_socket_connect_with_timeout(osl_inet_address_t * addr, unsigned long timeout_milli)
{
  osl_socket ret = INVALID_SOCKET;
  osl_socket sock;
  struct addrinfo * res = osl_inet_address_resolve(addr, SOCK_STREAM);
  if (res == NULL)
  {
    return INVALID_SOCKET;
  }

  sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  if (sock < 0)
  {
    perror("socket() failed");
    goto done;
  }

  if (timeout_milli == 0)
  {
    if (connect(sock, res->ai_addr, (int)res->ai_addrlen) != 0)
    {
	    perror("connect() failed");
	    goto done;
    }
    ret = sock;
    goto done;
  }
    
  s_set_nonblocking_socket(sock, 1);
  if (connect(sock, res->ai_addr, (int)res->ai_addrlen) != 0)
  {
    if (!s_err_continue()) {
	    perror("connect() failed");
	    goto done;
    }
  }

  osl_selector_t selector = {0,};
  osl_selector_clear(&selector);
  osl_selector_register(&selector, sock, OSL_FLAG_WRITE);
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

osl_socket osl_socket_bind(osl_inet_address_t * addr, osl_bool reuseaddr)
{
  osl_socket sock = OSL_INVALID_SOCKET;
  osl_socket ret = OSL_INVALID_SOCKET;
  struct addrinfo * res = osl_inet_address_resolve_passive(addr, osl_inet_address_get_family(addr), SOCK_STREAM);
  if (res == NULL)
  {
    return INVALID_SOCKET;
  }
  sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  if (sock < 0)
  {
    perror("socket() failed");
    goto done;
  }
  if (reuseaddr) {
    int on = 1;
    if (reuseaddr && setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(on)) != 0)
    {
	    perror("setsockopt() failed");
	    goto done;
    }
#if defined(OSL_OS_APPLE) 
    if (reuseaddr && setsockopt(sock, SOL_SOCKET, SO_REUSEPORT, (char*)&on, sizeof(on)) != 0)
    {
	    perror("setsockopt() failed");
	    goto done;
    }
#endif
  }
    
  if (bind(sock, res->ai_addr, (int)res->ai_addrlen) != 0)
  {
    perror("bind() failed");
    goto done;
  }
  ret = sock;
done:
  freeaddrinfo(res);
  return ret;
}

osl_socket OSL_EXPORT osl_socket_listen(osl_socket sock, int backlog)
{
  return listen(sock, backlog);
}

osl_socket OSL_EXPORT osl_socket_accept(osl_socket sock, struct sockaddr * addr, socklen_t * addrlen)
{
  return accept(sock, addr, addrlen);
}

int osl_socket_send(osl_socket sock, const void* buf, size_t len, int flags)
{
#if defined(USE_WINSOCK2)
  return send(sock, buf, (int)len, flags);
#else
  return send(sock, buf, len, flags);
#endif
}

int osl_socket_recv(osl_socket sock, void* buf, size_t len, int flags)
{
#if defined(USE_WINSOCK2)
  return recv(sock, buf, (int)len, flags);
#else
  return recv(sock, buf, len, flags);
#endif
}

osl_inet_address_t * osl_socket_get_inet_address(osl_socket fd)
{
  struct sockaddr_in _addr;
  socklen_t _addr_len = sizeof(_addr);
  if (getsockname(fd, (struct sockaddr*)&_addr, &_addr_len) != 0)
  {
    perror("getsockname() failed");
    return NULL;
  }
  return osl_inet_address_init_with_sockaddr(osl_inet_address_new(), (struct sockaddr*)&_addr);
}

osl_socket osl_datagram_socket_connect(osl_inet_address_t * addr)
{
  return osl_datagram_socket_connect_with_timeout(addr, 0);
}

osl_socket osl_datagram_socket_connect_with_timeout(osl_inet_address_t * addr, unsigned long timeout_milli)
{
  osl_socket ret = INVALID_SOCKET;
  osl_socket sock;
  struct addrinfo * res = osl_inet_address_resolve(addr, SOCK_DGRAM);
  if (res == NULL)
  {
    return INVALID_SOCKET;
  }

  sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  if (sock < 0)
  {
    perror("socket() failed");
    goto done;
  }

  if (timeout_milli == 0)
  {
    if (connect(sock, res->ai_addr, (int)res->ai_addrlen) != 0)
    {
	    perror("connect() failed");
	    goto done;
    }
    ret = sock;
    goto done;
  }
    
  s_set_nonblocking_socket(sock, 1);
  if (connect(sock, res->ai_addr, (int)res->ai_addrlen) != 0)
  {
    if (!s_err_continue()) {
	    perror("connect() failed");
	    goto done;
    }
  }

  osl_selector_t selector = {0,};
  osl_selector_clear(&selector);
  osl_selector_register(&selector, sock, OSL_FLAG_WRITE);
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

osl_socket osl_datagram_socket_bind(osl_inet_address_t * addr, int reuseaddr)
{
  osl_socket ret = INVALID_SOCKET;
  struct addrinfo * res = osl_inet_address_resolve_passive(addr, osl_inet_address_get_family(addr), SOCK_DGRAM);
  if (res == NULL)
  {
    return INVALID_SOCKET;
  }

  osl_socket sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
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

  if (bind(sock, res->ai_addr, (int)res->ai_addrlen) != 0) {
    perror("bind() failed");
    goto done;
  }

  ret = sock;

done:
  freeaddrinfo(res);
  return ret;
}

int osl_datagram_socket_join_group(osl_socket sock, const char * group)
{
  int ret = -1;
  osl_inet_address_t * group_addr = osl_inet_address_init(osl_inet_address_new(), osl_inet_unspec, group, -1);
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

int osl_datagram_socket_send(osl_socket sock, const void* buf, size_t len, int flags)
{
#if defined(USE_WINSOCK2)
  return send(sock, buf, (int)len, flags);
#else
  return send(sock, buf, len, flags);
#endif
}

int osl_datagram_socket_sendto(osl_socket sock, const void* buf, size_t len, int flags, const struct sockaddr* dest_addr, socklen_t addrlen)
{
#if defined(USE_WINSOCK2)
  return sendto(sock, buf, (int)len, flags, dest_addr, addrlen);
#else
  return sendto(sock, buf, len, flags, dest_addr, addrlen);
#endif
}

int osl_datagram_socket_recv(osl_socket sock, void* buf, size_t len, int flags)
{
#if defined(USE_WINSOCK2)
  return recv(sock, buf, (int)len, flags);
#else
  return recv(sock, buf, len, flags);
#endif
}

int osl_datagram_socket_recvfrom(osl_socket sock, void* buf, size_t len, int flags, struct sockaddr* src_addr, socklen_t* addrlen)
{
#if defined(USE_WINSOCK2)
  return recvfrom(sock, buf, (int)len, flags, src_addr, addrlen);
#else
  return recvfrom(sock, buf, len, flags, src_addr, addrlen);
#endif
}


void osl_socket_close(osl_socket sock)
{
#if defined(USE_BSD_SOCKET)
  close(sock);
#elif defined(USE_WINSOCK2)
  closesocket(sock);
#endif
}
