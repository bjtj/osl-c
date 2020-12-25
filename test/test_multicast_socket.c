#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "osl.h"
#include "selector.h"
#include "socket.h"
#include "thread.h"
#include <assert.h>

/* multicast socket */
static void * multicast_server_thread(void * arg);
static void test_multicast_client(void);

void test_multicast_socket(void)
{

    printf("== test multicast socket ==\n");
    
    /*  */
    osl_thread_t * server_thread = osl_thread_new(multicast_server_thread, NULL);
    osl_thread_start(server_thread);

    osl_idle(100);

    test_multicast_client();

    osl_thread_join(server_thread);
    osl_thread_free(server_thread);
}

int main(int argc, char *argv[])
{
    osl_init_once();
    osl_ignore_sigpipe();
    osl_use_socket();

    test_multicast_socket();
    
    osl_finish();
    
    return 0;
}

void * multicast_server_thread(void * arg)
{
    int i = 0;
    osl_inet_address_t * addr = osl_inet_address_new(osl_inet4, NULL, 1900);
    osl_socket sock = osl_datagram_socket_bind(addr, 1);
    osl_inet_address_free(addr);
    if (osl_datagram_socket_join_group(sock, "239.255.255.250") != 0)
    {
	fprintf(stderr, "join group failed\n");
	return 0;
    }

    char buffer[1024] = {0,};
    struct sockaddr_in remote_addr;
    socklen_t remote_addr_len = sizeof(remote_addr);
    memset(&remote_addr, 0, sizeof(remote_addr));
    int ret = (int)recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr*)&remote_addr, &remote_addr_len);
    if (ret <= 0)
    {
	perror("recvfrom() failed");
	return 0;
    }

    char ip_buffer[INET6_ADDRSTRLEN] = {0,};
    printf("[multicast server] recvfrom -- %s:%d\n", osl_ip_string((struct sockaddr*)&remote_addr, ip_buffer, sizeof(ip_buffer)), ntohs(remote_addr.sin_port));
    printf("[multicast server] data -- %s\n", buffer);

    for (i = 0; i < 3; ++i) {
	printf("[multicast server] sendto()\n");
	ret = (int)sendto(sock, buffer, ret, 0, (struct sockaddr*)&remote_addr, remote_addr_len);
	if (ret <= 0)
	{
	    perror("sento() failed");
	    return 0;
	}
	osl_idle(150);
    }

    osl_socket_close(sock);
    
    return 0;
}

void test_multicast_client(void)
{
    int i = 0;
    struct sockaddr_in remote_addr;
    socklen_t remote_addr_len = sizeof(remote_addr);
    osl_socket sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (!osl_socket_is_valid(sock))
    {
	perror("socket() failed");
	return;
    }

    memset(&remote_addr, 0, remote_addr_len);
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_addr.s_addr = inet_addr("239.255.255.250");
    remote_addr.sin_port = htons(1900);

    printf("[multicast client] sendto()\n");
    if (sendto(sock, "hello", 5, 0, (struct sockaddr*)&remote_addr, remote_addr_len) <= 0)
    {
	perror("sendto() failed");
	return;
    }

    char buffer[1024] = {0,};
    printf("[multicast client] recvfrom()\n");
    if (recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr*)&remote_addr, &remote_addr_len) <= 0)
    {
	perror("recvfrom() failed");
	return;
    }

    assert(strcmp(buffer, "hello") == 0);

    osl_socket_close(sock);
}
