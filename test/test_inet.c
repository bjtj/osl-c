#include "inet_address.h"
#include <assert.h>

void test_resolve(void)
{
    osl_inet_address_t * addr;
    struct addrinfo * info;
    addr = osl_inet_address_new(osl_inet_unspec, "127.0.0.1", 0);
    assert(addr != NULL);
    info = osl_inet_address_resolve(addr, SOCK_STREAM);
    assert(info != NULL);
    assert(info->ai_family == AF_INET);
    freeaddrinfo(info);
    osl_inet_address_free(addr);
    
    addr = osl_inet_address_new(osl_inet_unspec, "::1", 0);
    assert(addr != NULL);
    info = osl_inet_address_resolve(addr, SOCK_STREAM);
    assert(info != NULL);
    assert(info->ai_family == AF_INET6);
    freeaddrinfo(info);
    osl_inet_address_free(addr);
}

void test_resolve_passive(void)
{
    osl_inet_address_t * addr;
    struct addrinfo * info;
    addr = osl_inet_address_new(osl_inet_unspec, "127.0.0.1", 0);
    assert(addr != NULL);
    info = osl_inet_address_resolve_passive(addr, osl_inet_address_get_family(addr), SOCK_STREAM);
    assert(info != NULL);
    assert(info->ai_family == AF_INET);
    freeaddrinfo(info);
    osl_inet_address_free(addr);
    
    addr = osl_inet_address_new(osl_inet_unspec, "::1", 0);
    assert(addr != NULL);
    info = osl_inet_address_resolve_passive(addr, osl_inet_address_get_family(addr), SOCK_STREAM);
    assert(info != NULL);
    assert(info->ai_family == AF_INET6);
    freeaddrinfo(info);
    osl_inet_address_free(addr);
}

int main(void)
{
    test_resolve();
    test_resolve_passive();
    return 0;
}
