#include "inet_address.h"
#include "str.h"

static void s_get_ip_address(struct sockaddr * addr, char * ipstr, size_t size)
{
#if (!defined(USE_WINSOCK2) || _WIN32_WINNT >= 0x0600)

    inet_ntop(addr->sa_family,
	      (addr->sa_family == AF_INET ?
	       (void*)&((struct sockaddr_in*)addr)->sin_addr : (void*)&((struct sockaddr_in6*)addr)->sin6_addr),
	      ipstr, size);
        
#else
    char * ptr = NULL;
    if ((ptr = inet_ntoa(addr->sin_addr)) != NULL)
    {
	strncpy(ipstr, ptr, size - 1);
    }
#endif
}

static int s_get_port(struct sockaddr * addr)
{
    return ntohs((addr->sa_family == AF_INET ?
		  ((struct sockaddr_in*)addr)->sin_port: ((struct sockaddr_in6 *)addr)->sin6_port));
}


const char * osl_inet_version_to_str(osl_inet_version_e ver)
{
    switch (ver) {
    case osl_inet4: {
	return "inet4";
    }
    case osl_inet6: {
	return "inet6";
    }
    default:
	return "(unknown)";
    }
}

char * osl_ip_string(struct sockaddr * addr, char * ip_buffer, size_t size)
{
    s_get_ip_address(addr, ip_buffer, size);
    return ip_buffer;
}

osl_inet_address_t * osl_inet_address_new(void)
{
    osl_inet_address_t * inetaddr;
    inetaddr = (osl_inet_address_t*)malloc(sizeof(osl_inet_address_t));
    OSL_HANDLE_MALLOC_ERROR(inetaddr);
    memset(inetaddr, 0, sizeof(osl_inet_address_t));
    return inetaddr;
}

osl_inet_address_t * osl_inet_address_init(osl_inet_address_t * inetaddr, osl_inet_version_e version, const char * host, int port)
{
    inetaddr->inet_version = version;
    if (host)
    {
	inetaddr->host = osl_safe_strdup(host);
    }
    inetaddr->port = port;
    return inetaddr;
}

osl_inet_address_t * osl_inet_address_init_with_sockaddr(osl_inet_address_t * inetaddr, struct sockaddr * addr)
{
    char ipstr[INET6_ADDRSTRLEN] = {0,};
    osl_inet_version_e inetver;
    if (addr->sa_family == AF_INET) {
	inetver = osl_inet4;
    } else if (addr->sa_family == AF_INET6) {
	inetver = osl_inet6;
    } else {
	/* TODO: exception */
	return NULL;
    }
    inetaddr->inet_version = inetver;
    s_get_ip_address(addr, ipstr, sizeof(ipstr));
    inetaddr->host = osl_safe_strdup(ipstr);
    inetaddr->port = s_get_port(addr);
    return inetaddr;
}

void osl_inet_address_free(osl_inet_address_t * addr)
{
    if (addr == NULL) {
	return;
    }
    osl_safe_free(addr->host);
    osl_safe_free(addr);
}

int osl_inet_address_get_family(osl_inet_address_t * addr)
{
    if (addr->inet_version == osl_inet4)
    {
	return AF_INET;
    }
    if (addr->inet_version == osl_inet6)
    {
	return AF_INET6;
    }
    return AF_UNSPEC;
}

struct addrinfo * osl_inet_address_resolve(osl_inet_address_t * addr, int sock_type)
{
    int port;
    char port_str[10] = {0,};
    struct addrinfo * res;
    struct addrinfo hints;
    if (addr == NULL) {
	return NULL;
    }
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = sock_type;
    port = addr->port;
    snprintf(port_str, sizeof(port_str), "%d", port);
    if (getaddrinfo(addr->host, (port == -1 ? NULL : port_str), &hints, &res) != 0)
    {
#if defined(USE_MS_WIN)
        fprintf(stderr, "getaddrinfo() failed - %d\n", WSAGetLastError());
#else
	perror("getaddrinfo() failed");
#endif
	return NULL;
    }
    return res;
}

struct addrinfo * osl_inet_address_resolve_numeric(osl_inet_address_t * addr, int sock_type)
{
    int port;
    char port_str[10] = {0,};
    struct addrinfo * res;
    struct addrinfo hints;
    if (addr == NULL) {
	return NULL;
    }
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = sock_type;
    hints.ai_flags = AI_NUMERICHOST;
    port = addr->port;
    snprintf(port_str, sizeof(port_str), "%d", port);
    if (getaddrinfo(addr->host, (port == -1 ? NULL : port_str), &hints, &res) != 0)
    {
	return NULL;
    }
    return res;
}

struct addrinfo * osl_inet_address_resolve_passive(osl_inet_address_t * addr, int family, int sock_type)
{
    int port;
    char port_str[10] = {0,};
    struct addrinfo * res;
    struct addrinfo hints;
    if (addr == NULL) {
	return NULL;
    }
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = family;
    hints.ai_socktype = sock_type;
    hints.ai_flags = AI_PASSIVE;
    port = addr->port;
    snprintf(port_str, sizeof(port_str), "%d", port);
    if (getaddrinfo(addr->host, (port == -1 ? NULL : port_str), &hints, &res) != 0)
    {
	return NULL;
    }
    return res;
}
