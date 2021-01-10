#include "network.h"
#include "osl.h"
#include <assert.h>

void test_network(void)
{
    printf("== test network == \n");
    osl_list_t * ifaces = osl_network_all_interfaces();
    osl_list_t * ptr = ifaces;
    for (; ptr; ptr = ptr->next)
    {
	osl_network_interface_t * iface = (osl_network_interface_t*)ptr->data;
	printf("* iface name -- %s\n", iface->name);
    }

    osl_list_free(ifaces, (void (*)(void*))osl_network_interface_free);
}

int main()
{
    osl_init_once();
    osl_ignore_sigpipe();
    osl_use_socket();

    test_network();
    
    osl_finish();
    
    return 0;
}
