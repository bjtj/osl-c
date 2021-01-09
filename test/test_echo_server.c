#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "osl.h"
#include "socket.h"
#include "thread.h"
#include <assert.h>

/* socket */
static void * echo_server_thread(void * arg);
static void * echo_server2_thread(void * arg);
static void test_echo_client(int port);
static void test_echo_client2(int port);
static void test_echo_client3(int port);

void test_echo_server()
{
    /*  */
    printf("== test echo server==\n");
    int port = 0;
    osl_thread_t * server_thread = osl_thread_new(echo_server_thread, &port);
    osl_thread_start(server_thread);

    osl_idle(100);

    printf("port %d\n", port);
    test_echo_client(port);

    osl_thread_join(server_thread);
    osl_thread_free(server_thread);

    /* using osl server socket  */
    printf(" -- using osl server socket -- \n");
    port = 0;
    server_thread = osl_thread_new(echo_server2_thread, &port);
    osl_thread_start(server_thread);

    osl_idle(100);

    printf("port %d\n", port);
    test_echo_client(port);

    osl_thread_join(server_thread);
    osl_thread_free(server_thread);

    /* using osl server socket and socket connect with timeout*/
    printf(" -- using osl server socket and socket connect with timeout -- \n");
    port = 0;
    server_thread = osl_thread_new(echo_server2_thread, &port);
    osl_thread_start(server_thread);

    osl_idle(100);

    printf("port %d\n", port);
    test_echo_client2(port);

    osl_thread_join(server_thread);
    osl_thread_free(server_thread);

    /* using osl server socket and socket connect without timeout */
    printf(" -- using osl server socket and socket connect with timeout -- \n");
    port = 0;
    server_thread = osl_thread_new(echo_server2_thread, &port);
    osl_thread_start(server_thread);

    osl_idle(100);

    printf("port %d\n", port);
    test_echo_client3(port);

    osl_thread_join(server_thread);
    osl_thread_free(server_thread);
}

int main()
{
    osl_init_once();
    osl_ignore_sigpipe();
    osl_use_socket();

    test_echo_server();
    
    osl_finish();
    
    return 0;
}

static void * echo_server_thread(void * arg)
{
    int * server_port = (int*)arg;

    osl_use_socket();
    
    int yes = 1;
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    osl_socket sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (void*)&yes, sizeof(yes)) != 0)
    {
	perror("setsockopt() failed");
	return 0;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = *server_port;
    if (bind(sock, (struct sockaddr*)&addr, addr_len) != 0)
    {
	perror("bind() failed");
	return 0;
    }

    {
	struct sockaddr_in _addr;
	socklen_t _addr_len = sizeof(_addr);
	if (getsockname(sock, (struct sockaddr*)&_addr, &_addr_len) != 0)
	{
	    perror("getsockname() failed");
	    return 0;
	}
	*server_port = ntohs(_addr.sin_port);
    }

    if (listen(sock, 5) != 0)
    {
	perror("listen() failed");
	return 0;
    }

    printf("listen...\n");

    {
	struct sockaddr_in remote_addr;
	socklen_t remote_addr_len = sizeof(remote_addr);
	memset(&remote_addr, 0, sizeof(remote_addr));
	osl_socket remote_sock = osl_socket_accept(sock, (struct sockaddr*)&remote_addr, &remote_addr_len);
	
	if (!osl_socket_is_valid(remote_sock))
	{
	    perror("accept() failed");
	    return 0;
	}

	char buffer[10] = {0,};
	int len = recv(remote_sock, buffer, sizeof(buffer), 0);
	if (len <= 0)
	{
	    return 0;
	}

	send(remote_sock, buffer, len, 0);
	osl_socket_close(remote_sock);
    }

    osl_socket_close(sock);
    return 0;
}


static void * echo_server2_thread(void * arg)
{
    int * server_port = (int*)arg;

    osl_use_socket();

    osl_inet_address_t * addr = osl_inet_address_new(osl_inet4, "0.0.0.0", *server_port);
    osl_socket sock = osl_socket_bind(addr, osl_true);
    osl_inet_address_free(addr);

    addr = osl_socket_get_inet_address(sock);
    *server_port = addr->port;
    osl_inet_address_free(addr);

    if (osl_socket_listen(sock, 5) != 0)
    {
	perror("listen() failed");
	return 0;
    }

    printf("listen...\n");

    {
	struct sockaddr_in remote_addr;
	socklen_t remote_addr_len = sizeof(remote_addr);
	memset(&remote_addr, 0, sizeof(remote_addr));
	osl_socket remote_sock = osl_socket_accept(sock, (struct sockaddr*)&remote_addr, &remote_addr_len);
	
	if (!osl_socket_is_valid(remote_sock))
	{
	    perror("accept() failed");
	    return 0;
	}

	char buffer[10] = {0,};
	int len = recv(remote_sock, buffer, sizeof(buffer), 0);
	if (len <= 0)
	{
	    perror("recv() failed");
	    return 0;
	}

	send(remote_sock, buffer, len, 0);
	osl_socket_close(remote_sock);
    }

    osl_socket_close(sock);
    return 0;
}

void test_echo_client(int port)
{
    char buffer[10] = {0,};
    struct sockaddr_in addr;
    osl_socket sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(port);
    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) != 0)
    {
	perror("connect() error");
	exit(1);
    }
    send(sock, "hello", 5, 0);
    recv(sock, buffer, sizeof(buffer), 0);
    assert(strcmp(buffer, "hello") == 0);
    osl_socket_close(sock);
}

void test_echo_client2(int port)
{
    char buffer[10] = {0,};
    osl_inet_address_t * addr = osl_inet_address_new(osl_inet4, "127.0.0.1", port);
    osl_socket sock = osl_socket_connect_with_timeout(addr, 1000);
    osl_inet_address_free(addr);
    if (!osl_socket_is_valid(sock))
    {
	perror("connect() error");
	exit(1);
    }
    send(sock, "hello", 5, 0);
    recv(sock, buffer, sizeof(buffer), 0);
    assert(strcmp(buffer, "hello") == 0);
    osl_socket_close(sock);
}


void test_echo_client3(int port)
{
    char buffer[10] = {0,};
    osl_inet_address_t * addr = osl_inet_address_new(osl_inet4, "127.0.0.1", port);
    osl_socket sock = osl_socket_connect(addr);
    osl_inet_address_free(addr);
    if (!osl_socket_is_valid(sock))
    {
	perror("connect() error");
	exit(1);
    }
    send(sock, "hello", 5, 0);
    recv(sock, buffer, sizeof(buffer), 0);
    assert(strcmp(buffer, "hello") == 0);
    osl_socket_close(sock);
}
