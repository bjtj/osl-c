#include "network.h"
#include "inet_address.h"

#if defined(USE_BSD_SOCKET)

static osl_network_interface_t * s_obtain_network_interface(osl_list_t ** list, const char * iface_name)
{
    osl_list_t * ptr = *list;
    for (; ptr; ptr = ptr->next)
    {
	osl_network_interface_t * iface = (osl_network_interface_t*)ptr->data;
	if (strcmp(iface->name, iface_name) == 0)
	{
	    return iface;
	}
    }

    osl_network_interface_t * iface = osl_network_interface_new_with_name(iface_name);
    *list = osl_list_append(*list, iface);
    return iface;
}

static osl_list_t * s_get_all_network_interfaces(void)
{
    osl_list_t * ifaces = NULL;
    struct ifaddrs * addrs, * tmp;
    getifaddrs(&addrs);
    tmp = addrs;
        
    while (tmp) {
            
	if (tmp->ifa_addr) {

	    osl_network_interface_t * iface = s_obtain_network_interface(&ifaces, tmp->ifa_name);

	    switch (tmp->ifa_addr->sa_family) {
	    case AF_INET:
	    case AF_INET6:
	    {
		iface->addr_list = osl_list_append(iface->addr_list, osl_inet_address_new_with_sockaddr((struct sockaddr*)tmp->ifa_addr));
	    }
	    break;
#if defined(USE_APPLE_STD)
	    case AF_LINK:
	    {
		unsigned char * ptr = (unsigned char *)LLADDR((struct sockaddr_dl *)(tmp->ifa_addr));
		osl_network_interface_set_mac_address(iface, ptr, 6);
	    }
	    break;
#else
	    case AF_PACKET:
	    {
		struct sockaddr_ll * s = (struct sockaddr_ll*)tmp->ifa_addr;
		osl_network_interface_set_mac_address(iface, s->sll_addr, 6);
	    }
	    break;
#endif
	    default:
		break;
	    }
                
	}
	tmp = tmp->ifa_next;
    }
    freeifaddrs(addrs);
        
    return ifaces;
}

#elif defined(USE_WINSOCK2)

static osl_list_t * s_get_all_network_interfaces(void)
{
    osl_list_t * ifaces = NULL;
    ULONG outBufLen = 0;
    DWORD dwRetVal = 0;
    IP_ADAPTER_INFO * pAdapterInfos = (IP_ADAPTER_INFO*) malloc(sizeof(IP_ADAPTER_INFO));
	OSL_HANDLE_MALLOC_ERROR(pAdapterInfos);

    // retry up to 5 times, to get the adapter infos needed
    const int retry = 5;
    for (int i = 0; i < retry && (dwRetVal == ERROR_BUFFER_OVERFLOW || dwRetVal == NO_ERROR); ++i)
    {
	// GetAdaptersInfo: https://msdn.microsoft.com/ko-kr/library/windows/desktop/aa365917%28v=vs.85%29.aspx
	dwRetVal = GetAdaptersInfo(pAdapterInfos, &outBufLen);
	if (dwRetVal == NO_ERROR)
	{
	    break;
	}
	else if (dwRetVal == ERROR_BUFFER_OVERFLOW)
	{
	    free(pAdapterInfos);
	    pAdapterInfos = (IP_ADAPTER_INFO *)malloc(outBufLen);
		OSL_HANDLE_MALLOC_ERROR(pAdapterInfos);
	}
	else
	{
	    pAdapterInfos = NULL;
	    break;
	}
    }

    if (dwRetVal == NO_ERROR)
    {
	IP_ADAPTER_INFO* pAdapterInfo = pAdapterInfos;
	while (pAdapterInfo)
	{
	    osl_network_interface_t * iface = osl_network_interface_new_with_name(pAdapterInfo->AdapterName);
	    iface->description = strdup(pAdapterInfo->Description);

	    IP_ADDR_STRING * pIpAddress = &(pAdapterInfo->IpAddressList);
	    while( pIpAddress != 0 )
	    {
		iface->addr_list = osl_list_append(iface->addr_list,
						   osl_inet_address_new(
						       osl_inet4,
						       pAdapterInfo->IpAddressList.IpAddress.String,
						       0));
		pIpAddress = pIpAddress->Next;
	    }
	    pAdapterInfo = pAdapterInfo->Next;
	    ifaces = osl_list_append(ifaces, iface);
	}
    }
	osl_safe_free(pAdapterInfos);

    return ifaces;
}

#endif

osl_network_interface_t * osl_network_interface_new_with_name(const char * name)
{
    osl_network_interface_t * iface = (osl_network_interface_t*)malloc(sizeof(osl_network_interface_t));
	OSL_HANDLE_MALLOC_ERROR(iface);
    memset(iface, 0, sizeof(osl_network_interface_t));
    iface->name = strdup(name);
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
