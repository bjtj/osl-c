#ifndef __NETWORK_H__
#define __NETWORK_H__

#include "osl.h"
#include "list.h"

typedef struct _osl_network_interface_t
{
    char * name;
    char * description;
    unsigned char * mac_address;
    size_t mac_address_size;
    osl_list_t * addr_list;
} osl_network_interface_t;

osl_network_interface_t SHARED_EXPORT * osl_network_interface_new_with_name(const char * name);
void SHARED_EXPORT osl_network_interface_free(osl_network_interface_t * iface);
void SHARED_EXPORT osl_network_interface_set_mac_address(osl_network_interface_t * iface, const unsigned char * addr, size_t size);
osl_list_t SHARED_EXPORT * osl_network_all_interfaces(void);

#endif
