#include "network.h"
#include "inet_address.h"
#include <assert.h>

void test_network(void)
{
  osl_list_t * ifaces;
  osl_list_t * ptr;
  printf("== test network == \n");
  ifaces = osl_network_all_interfaces();
  ptr = ifaces;
  for (; ptr; ptr = ptr->next)
  {
    osl_list_t* addr_ptr;

    osl_network_interface_t * iface = (osl_network_interface_t*)ptr->data;
    printf("* Network interface name: %s (loopback: %s)\n", iface->name, iface->is_loopback ? "Y" : "N");
    addr_ptr = iface->addr_list;
    for (; addr_ptr; addr_ptr = addr_ptr->next) {
	    osl_inet_address_t* addr = (osl_inet_address_t*)addr_ptr->data;
	    printf(" - IP: %s (%s)\n", addr->host, osl_inet_version_to_str(addr->inet_version));
    }

  }

  osl_list_free(ifaces, (void (*)(void*))osl_network_interface_free);
}

int main(void)
{
  osl_init_once();
  osl_ignore_sigpipe();
  osl_use_socket();

  test_network();
    
  osl_finish();
    
  return 0;
}
