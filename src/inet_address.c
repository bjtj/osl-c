#include "inet_address.h"

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


char * osl_ip_string(struct sockaddr * addr, char * ip_buffer, size_t size)
{
    s_get_ip_address(addr, ip_buffer, size);
    return ip_buffer;
}

osl_inet_address_t * osl_inet_address_new_with_sockaddr(struct sockaddr * addr)
{
    osl_inet_address_t * ret = (osl_inet_address_t*)malloc(sizeof(osl_inet_address_t));
    OSL_HANDLE_MALLOC_ERROR(ret);
    memset(ret, 0, sizeof(osl_inet_address_t));
    if (addr->sa_family == AF_INET)
    {
	ret->inet_version = osl_inet4;
    }
    else if (addr->sa_family == AF_INET6)
    {
	ret->inet_version = osl_inet6;
    }

    char ipstr[INET6_ADDRSTRLEN] = {0,};
    s_get_ip_address(addr, ipstr, sizeof(ipstr));
    ret->host = strdup(ipstr);
    ret->port = s_get_port(addr);
    return ret;
}

osl_inet_address_t * osl_inet_address_new(osl_inet_version_e version, const char * host, int port)
{
    osl_inet_address_t * addr = (osl_inet_address_t*)malloc(sizeof(osl_inet_address_t));
    OSL_HANDLE_MALLOC_ERROR(addr);
    memset(addr, 0, sizeof(osl_inet_address_t));
    addr->inet_version = version;
    if (host)
    {
	addr->host = strdup(host);
    }
    addr->port = port;
    return addr;
}

void osl_inet_address_free(osl_inet_address_t * addr)
{
    if (addr) {
	if (addr->host)
	{
	    free(addr->host);
	}
	free(addr);	
    }
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
    char port_str[10] = {0,};
    snprintf(port_str, sizeof(port_str), "%d", addr->port);

    struct addrinfo * res;
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = sock_type;
    if (getaddrinfo(addr->host, (addr->port == -1 ? NULL : port_str), &hints, &res) != 0)
    {
	return NULL;
    }
    return res;
}

struct addrinfo * osl_inet_address_resolve_numeric(osl_inet_address_t * addr, int sock_type)
{
    char port_str[10] = {0,};
    snprintf(port_str, sizeof(port_str), "%d", addr->port);

    struct addrinfo * res;
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = sock_type;
    hints.ai_flags = AI_NUMERICHOST;
    if (getaddrinfo(addr->host, (addr->port == -1 ? NULL : port_str), &hints, &res) != 0)
    {
	return NULL;
    }
    return res;
}

struct addrinfo * osl_inet_address_resolve_passive(osl_inet_address_t * addr, int family, int sock_type)
{
    char port_str[10] = {0,};
    snprintf(port_str, sizeof(port_str), "%d", addr->port);

    struct addrinfo * res;
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = family;
    hints.ai_socktype = sock_type;
    hints.ai_flags = AI_PASSIVE;
    if (getaddrinfo(addr->host, (addr->port == -1 ? NULL : port_str), &hints, &res) != 0)
    {
	return NULL;
    }
    return res;
}
