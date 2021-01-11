#include "network.h"
#include "inet_address.h"
#include "str.h"

static osl_network_interface_t* s_obtain_network_interface(osl_list_t** list, const char* iface_name)
{
    osl_list_t* ptr = *list;
    for (; ptr; ptr = ptr->next)
    {
        osl_network_interface_t* iface = (osl_network_interface_t*)ptr->data;
        if (strcmp(iface->name, iface_name) == 0)
        {
            return iface;
        }
    }

    osl_network_interface_t* iface = osl_network_interface_new_with_name(iface_name);
    *list = osl_list_append(*list, iface);
    return iface;
}

#if defined(USE_BSD_SOCKET)

static osl_list_t * s_get_all_network_interfaces(void)
{
    osl_list_t * ifaces = NULL;
    struct ifaddrs * addrs, * ptr;
    getifaddrs(&addrs);
    ptr = addrs;
        
    for (; ptr; ptr = ptr->ifa_next) {
	osl_network_interface_t * iface;
	if (ptr->ifa_addr == NULL) {
	    continue;
	}
	
	iface = s_obtain_network_interface(&ifaces, ptr->ifa_name);

	iface->is_loopback = OSL_BOOL((ptr->ifa_flags & IFF_LOOPBACK) != 0);

	switch (ptr->ifa_addr->sa_family) {
	case AF_INET:
	case AF_INET6:
	{
	    iface->addr_list = osl_list_append(iface->addr_list, osl_inet_address_new_with_sockaddr((struct sockaddr*)ptr->ifa_addr));
	    break;
	}
#if defined(USE_APPLE_STD)
	case AF_LINK:
	{
	    unsigned char * mac_addr = (unsigned char *)LLADDR((struct sockaddr_dl *)(ptr->ifa_addr));
	    osl_network_interface_set_mac_address(iface, mac_addr, 6);
	    break;
	}
#else
	case AF_PACKET:
	{
	    struct sockaddr_ll * s = (struct sockaddr_ll*)ptr->ifa_addr;
	    osl_network_interface_set_mac_address(iface, s->sll_addr, 6);
	    break;
	}
#endif
	default:
	    break;
	}

    }
    freeifaddrs(addrs);
        
    return ifaces;
}

#elif defined(USE_WINSOCK2)

// https://docs.microsoft.com/en-us/windows/win32/api/iphlpapi/nf-iphlpapi-getadaptersaddresses
static osl_list_t * s_get_all_network_interfaces(void)
{
    osl_list_t * ifaces = NULL;
    
    DWORD dwRetVal = 0;

    unsigned int i = 0;

    // Set the flags to pass to GetAdaptersAddresses
    ULONG flags = GAA_FLAG_INCLUDE_PREFIX;

    // default to unspecified address family (both)
    ULONG family = AF_UNSPEC;

    PIP_ADAPTER_ADDRESSES pAddresses = NULL;
    ULONG outBufLen = 0;
    ULONG Iterations = 0;

    PIP_ADAPTER_ADDRESSES pCurrAddresses = NULL;
    PIP_ADAPTER_UNICAST_ADDRESS pUnicast = NULL;

    // Allocate a 15 KB buffer to start with.
    outBufLen = 15000;

    do {

        pAddresses = (IP_ADAPTER_ADDRESSES*)malloc(outBufLen);
        OSL_HANDLE_MALLOC_ERROR(pAddresses);        

        dwRetVal = GetAdaptersAddresses(family, flags, NULL, pAddresses, &outBufLen);

        if (dwRetVal == ERROR_BUFFER_OVERFLOW) {
            osl_safe_free(pAddresses);
            pAddresses = NULL;
        }
        else {
            break;
        }

        Iterations++;

    } while ((dwRetVal == ERROR_BUFFER_OVERFLOW) && (Iterations < 3));

    if (dwRetVal == NO_ERROR) {
        // If successful, output some information from the data we received
        pCurrAddresses = pAddresses;
        while (pCurrAddresses) {

            osl_network_interface_t* iface = s_obtain_network_interface(&ifaces, pCurrAddresses->AdapterName);

            iface->is_loopback = OSL_BOOL(pCurrAddresses->IfType == IF_TYPE_SOFTWARE_LOOPBACK);

            pUnicast = pCurrAddresses->FirstUnicastAddress;
            if (pUnicast != NULL) {
                for (i = 0; pUnicast != NULL; i++) {

                    osl_inet_address_t * addr = osl_inet_address_new_with_sockaddr(pUnicast->Address.lpSockaddr);
                    iface->addr_list = osl_list_append(iface->addr_list, addr);
                                        
                    pUnicast = pUnicast->Next;
                }
            }

            if (pCurrAddresses->PhysicalAddressLength != 0) {
                osl_network_interface_set_mac_address(iface, pCurrAddresses->PhysicalAddress, pCurrAddresses->PhysicalAddressLength);
            }

            pCurrAddresses = pCurrAddresses->Next;
        }
    }
    
    osl_safe_free(pAddresses);

    return ifaces;
}

#endif

osl_network_interface_t * osl_network_interface_new_with_name(const char * name)
{
    osl_network_interface_t * iface = (osl_network_interface_t*)malloc(sizeof(osl_network_interface_t));
    OSL_HANDLE_MALLOC_ERROR(iface);
    memset(iface, 0, sizeof(osl_network_interface_t));
    iface->name = osl_safe_strdup(name);
    return iface;
}

void osl_network_interface_free(osl_network_interface_t * iface)
{
    if (iface == NULL) {
	return;
    }
    if (iface->name)
    {
	osl_safe_free(iface->name);
    }
    if (iface->description)
    {
	osl_safe_free(iface->description);
    }
    if (iface->mac_address)
    {
	osl_safe_free(iface->mac_address);
    }
    osl_list_free(iface->addr_list, (void (*)(void*))osl_inet_address_free);
    free(iface);
}

unsigned char* osl_network_interface_set_mac_address(osl_network_interface_t * iface, const unsigned char * addr, size_t size)
{
    if (iface == NULL)
    {
	return NULL;
    }
    if (iface->mac_address)
    {
	osl_safe_free(iface->mac_address);
    }
    iface->mac_address = (unsigned char*)malloc(size);
    OSL_HANDLE_MALLOC_ERROR(iface->mac_address);
    memcpy(iface->mac_address, addr, size);
    iface->mac_address_size = size;
    return iface->mac_address;
}

osl_list_t * osl_network_all_interfaces(void)
{
    return s_get_all_network_interfaces();
}
