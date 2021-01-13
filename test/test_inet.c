#include "inet_address.h"
#include <assert.h>

void _test_resolve(const char * ip, int ver)
{
    osl_inet_address_t * addr;
    struct addrinfo * info;
    addr = osl_inet_address_new(osl_inet_unspec, ip, 0);
    assert(addr != NULL);
    info = osl_inet_address_resolve(addr, SOCK_STREAM);
    assert(info != NULL);
    assert(info->ai_family == ver);
    freeaddrinfo(info);
    osl_inet_address_free(addr);
}

void _test_resolve_passive(const char * ip, int ver)
{
    osl_inet_address_t * addr;
    struct addrinfo * info;
    addr = osl_inet_address_new(osl_inet_unspec, ip, 0);
    assert(addr != NULL);
    info = osl_inet_address_resolve_passive(addr, AF_UNSPEC, SOCK_STREAM);
    assert(info != NULL);
    assert(info->ai_family == ver);
    freeaddrinfo(info);
    osl_inet_address_free(addr);
}

void test_resolve(void)
{
    _test_resolve("127.0.0.1", AF_INET);
    _test_resolve("::1", AF_INET6);
    _test_resolve_passive("127.0.0.1", AF_INET);
    _test_resolve_passive("::1", AF_INET6);
}

int main(void)
{
    test_resolve();
    return 0;
}
