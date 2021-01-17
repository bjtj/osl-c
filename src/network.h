#ifndef __OSL_NETWORK_H__
#define __OSL_NETWORK_H__

#include "osl.h"
#include "list.h"

typedef struct _osl_network_interface_t
{
    char * name;
    char * description;
    unsigned char * mac_address;
    size_t mac_address_size;
    osl_list_t * addr_list;
    osl_bool is_loopback;
} osl_network_interface_t;

#ifdef __cplusplus
extern "C" {
#endif

    extern OSL_EXPORT osl_network_interface_t * osl_network_interface_new(void);
    extern OSL_EXPORT osl_network_interface_t * osl_network_interface_init_with_name(osl_network_interface_t *, const char *);
    extern OSL_EXPORT void osl_network_interface_free(osl_network_interface_t *);
    extern OSL_EXPORT unsigned char * osl_network_interface_set_mac_address(osl_network_interface_t *, const unsigned char *, size_t);
    extern OSL_EXPORT osl_list_t * osl_network_all_interfaces(void);

#ifdef __cplusplus
}
#endif

#endif
