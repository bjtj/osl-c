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


void osl_ip_string(struct sockaddr * addr, char * host, size_t size)
{
    s_get_ip_address(addr, host, size);
}

osl_inet_address_t * osl_inet_address_new_with_sockaddr(struct sockaddr * addr)
{
    osl_inet_address_t * ret = (osl_inet_address_t*)malloc(sizeof(osl_inet_address_t));
    memset(ret, 0, sizeof(osl_inet_address_t));
    if (addr->sa_family == AF_INET)
    {
	ret->inet_version = osl_inet4;
    }
    else if (addr->sa_family == AF_INET6)
    {
	ret->inet_version = osl_inet6;
    }

    s_get_ip_address(addr, ret->host, sizeof(ret->host));
    ret->port = s_get_port(addr);
    return ret;
}

osl_inet_address_t * osl_inet_address_new(osl_inet_version_e version, const char * host, int port)
{
    osl_inet_address_t * addr = (osl_inet_address_t*)malloc(sizeof(osl_inet_address_t));
    memset(addr, 0, sizeof(osl_inet_address_t));
    addr->inet_version = version;
    snprintf(addr->host, sizeof(addr->host), "%s", host);
    addr->port = port;
    return addr;
}

void osl_inet_address_free(osl_inet_address_t * addr)
{
    if (addr) {
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
