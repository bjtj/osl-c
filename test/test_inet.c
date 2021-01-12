#include "inet_address.h"
#include <assert.h>

void test_resolve(void)
{
    osl_inet_address_t addr = {
	.host = "127.0.0.1",
	.port = 0,
	.inet_version = osl_inet_unspec
    };
    struct addrinfo * info = osl_inet_address_resolve(&addr, SOCK_STREAM);
    assert(info->ai_family == AF_INET);
    freeaddrinfo(info);

    addr.host = "[::1]";
    info = osl_inet_address_resolve(&addr, SOCK_STREAM);
    assert(info->ai_family == AF_INET6);
    freeaddrinfo(info);
}

int main(void)
{
    
    return 0;
}
