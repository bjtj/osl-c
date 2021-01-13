#include "osl.h"
#include "socket.h"
#include "thread.h"
#include <assert.h>

struct _server_t {
    uint16_t port;
};

/* socket */
static void * echo_server_thread(void * arg);
static void * echo_server2_thread(void * arg);
static void test_echo_client(uint16_t port);
static void test_echo_client2(uint16_t port);
static void test_echo_client3(uint16_t port);

void test_echo_server()
{   
    struct _server_t server = { 0, };

    /*  */
    osl_thread_t * server_thread;
    
    printf("== test echo server==\n");    
    server_thread = osl_thread_new(echo_server_thread, &server);
    osl_thread_start(server_thread);

    osl_idle(100);

    printf("port %d\n", server.port);
    test_echo_client(server.port);

    osl_thread_join(server_thread);
    osl_thread_free(server_thread);

    /* using osl server socket  */
    printf(" -- using osl server socket -- \n");
    server.port = 0;
    server_thread = osl_thread_new(echo_server2_thread, &server);
    osl_thread_start(server_thread);

    osl_idle(100);

    printf("port %d\n", server.port);
    test_echo_client(server.port);

    osl_thread_join(server_thread);
    osl_thread_free(server_thread);

    /* using osl server socket and socket connect with timeout*/
    printf(" -- using osl server socket and socket connect with timeout -- \n");
    server.port = 0;
    server_thread = osl_thread_new(echo_server2_thread, &server);
    osl_thread_start(server_thread);

    osl_idle(100);

    printf("port %d\n", server.port);
    test_echo_client2(server.port);

    osl_thread_join(server_thread);
    osl_thread_free(server_thread);

    /* using osl server socket and socket connect without timeout */
    printf(" -- using osl server socket and socket connect with timeout -- \n");
    server.port = 0;
    server_thread = osl_thread_new(echo_server2_thread, &server);
    osl_thread_start(server_thread);

    osl_idle(100);

    printf("port %d\n", server.port);
    test_echo_client3(server.port);

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
    struct _server_t* server = (struct _server_t*)arg;
    int yes = 1;
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    osl_socket sock;

    osl_use_socket();
    
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    assert(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (void*)&yes, sizeof(yes)) == 0);

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = (uint16_t)server->port;
    assert(bind(sock, (struct sockaddr*)&addr, addr_len) == 0);

    {
	struct sockaddr_in _addr;
	socklen_t _addr_len = sizeof(_addr);
	assert(getsockname(sock, (struct sockaddr*)&_addr, &_addr_len) == 0);
	server->port = ntohs(_addr.sin_port);
    }

    assert(listen(sock, 5) == 0);

    printf("LISTEN...\n");

    {
	int len;
	char buffer[10] = {0,};
	struct sockaddr_in remote_addr;
	socklen_t remote_addr_len = sizeof(remote_addr);
	memset(&remote_addr, 0, sizeof(remote_addr));
	osl_socket remote_sock = osl_socket_accept(sock, (struct sockaddr*)&remote_addr, &remote_addr_len);
	
	assert(osl_socket_is_valid(remote_sock));
	
	len = recv(remote_sock, buffer, sizeof(buffer), 0);
	assert(len > 0);
	
	send(remote_sock, buffer, len, 0);
	osl_socket_close(remote_sock);
    }

    osl_socket_close(sock);
    return 0;
}


static void * echo_server2_thread(void * arg)
{
    struct _server_t* server = (struct _server_t*)arg;
    osl_inet_address_t * addr;
    osl_socket sock;

    osl_use_socket();

    addr = osl_inet_address_new(osl_inet4, "0.0.0.0", server->port);
    sock = osl_socket_bind(addr, osl_true);
    assert(osl_socket_is_valid(sock));
    osl_inet_address_free(addr);

    addr = osl_socket_get_inet_address(sock);
    server->port = (uint16_t)addr->port;
    osl_inet_address_free(addr);

    assert(osl_socket_listen(sock, 5) == 0);

    printf("LISTEN...\n");

    {
	int len;
	char buffer[10] = {0,};
	osl_socket remote_sock;
	struct sockaddr_in remote_addr;
	socklen_t remote_addr_len = sizeof(remote_addr);
	memset(&remote_addr, 0, sizeof(remote_addr));
	remote_sock = osl_socket_accept(sock, (struct sockaddr*)&remote_addr, &remote_addr_len);
	assert(osl_socket_is_valid(remote_sock));
	len = recv(remote_sock, buffer, sizeof(buffer), 0);
	assert(len > 0);
	send(remote_sock, buffer, len, 0);
	osl_socket_close(remote_sock);
    }

    osl_socket_close(sock);
    return 0;
}

void test_echo_client(uint16_t port)
{
    char buffer[10] = {0,};
    osl_socket sock;
    osl_inet_address_t * addr = osl_inet_address_new(osl_inet4, "127.0.0.1", port);
    assert(osl_socket_is_valid(sock = osl_socket_connect(addr)));
    osl_socket_send(sock, "hello", 5, 0);
    osl_socket_recv(sock, buffer, sizeof(buffer), 0);
    assert(strcmp(buffer, "hello") == 0);
    osl_inet_address_free(addr);
    osl_socket_close(sock);
}

void test_echo_client2(uint16_t port)
{
    char buffer[10] = {0,};
    osl_inet_address_t * addr = osl_inet_address_new(osl_inet4, "127.0.0.1", port);
    osl_socket sock = osl_socket_connect_with_timeout(addr, 1000);
    osl_inet_address_free(addr);
    assert(osl_socket_is_valid(sock));
    send(sock, "hello", 5, 0);
    recv(sock, buffer, sizeof(buffer), 0);
    assert(strcmp(buffer, "hello") == 0);
    osl_socket_close(sock);
}


void test_echo_client3(uint16_t port)
{
    char buffer[10] = {0,};
    osl_inet_address_t * addr = osl_inet_address_new(osl_inet4, "127.0.0.1", port);
    osl_socket sock = osl_socket_connect(addr);
    osl_inet_address_free(addr);
    assert(osl_socket_is_valid(sock));
    send(sock, "hello", 5, 0);
    recv(sock, buffer, sizeof(buffer), 0);
    assert(strcmp(buffer, "hello") == 0);
    osl_socket_close(sock);
}
