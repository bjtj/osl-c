#include "../src/osl.h"
#include "../src/thread.h"
#include "../src/list.h"
#include "../src/string_buffer.h"
#include "../src/file.h"
#include "../src/network.h"
#include "../src/date.h"
#include "../src/sem.h"
#include "../src/socket.h"
#include <assert.h>

static void * echo_server_thread(void * arg);
static void * echo_server2_thread(void * arg);
static void test_echo_client(int port);

void * worker(void * arg)
{
    int id = *(int*)arg;
    int i = 0;
    for (; i < 10; i++) {
	printf("[%d] idx -- %d\n", id, i);
	idle(50);
    }
    return 0;
}

void test_thread(void)
{
    int id1 = 1;
    int id2 = 2;
    osl_thread_t * thread1 = osl_thread_new(worker, &id1);
    osl_thread_t * thread2 = osl_thread_new(worker, &id2);
    osl_thread_start(thread1);
    osl_thread_start(thread2);
    osl_thread_join(thread1);
    osl_thread_join(thread2);
    osl_thread_free(thread1);
    osl_thread_free(thread2);
}

void test_list(void)
{

    osl_list_t * list = NULL;

    printf("count -- %lu\n", osl_list_count(list));
    printf("item of index 3 is %s\n", (char*)osl_list_get(list, 3));

    list = osl_list_append(list, strdup("hello"));
    list = osl_list_append(list, strdup("hi"));
    list = osl_list_append(list, strdup("ho"));
    list = osl_list_append(list, strdup("bye"));
    list = osl_list_prepend(list, strdup("!!!"));

    osl_list_t * ptr = list;
    for (; ptr; ptr = ptr->next)
    {
	printf("* %s\n", (char *)ptr->data);
    }

    printf("item of index 3 is %s\n", (char*)osl_list_get(list, 3));

    printf("count -- %lu\n", osl_list_count(list));

    list = osl_list_remove(list, 2, free);
    ptr = list;
    for (; ptr; ptr = ptr->next)
    {
	printf("* %s\n", (char *)ptr->data);
    }

    printf("====\n");

    list = osl_list_remove(list, 0, free);
    ptr = list;
    for (; ptr; ptr = ptr->next)
    {
	printf("* %s\n", (char *)ptr->data);
    }

    osl_list_free(list, free);
}

void test_date(void)
{
    printf(" == test date ==\n");
    int i = 0;
    for (; i < 3; i++)
    {
	osl_date_t now = osl_date_now();
	printf("%04d-%02d-%02d %02d:%02d:%02d.%03d\n",
	       now.year, now.month+1, now.day, now.hour, now.minute, now.second, now.millisecond);
	idle(1000*2);
    }
}

void test_string_buffer(void)
{
    printf(" == test string buffer ==\n");
    osl_string_buffer_t sb = {0,};
    osl_string_buffer_append(&sb, "hello");
    osl_string_buffer_append(&sb, " ");
    osl_string_buffer_append(&sb, "world");

    char * str = osl_string_buffer_to_string(&sb);
    printf("str is '%s'\n", str);
    free(str);

    osl_string_buffer_clear(&sb);
}

void test_file(void)
{
    char * dump;
    FILE * fp = fopen("hello", "w");
    fwrite("hello", 1, 5, fp);
    fclose(fp);

    fp = fopen("hello", "r");
    dump = osl_file_dump(fp);
    printf("dump: '%s'\n", dump);
    free(dump);
    fclose(fp);

    printf("file size: %lu\n", osl_file_size("hello"));
}

void test_network(void)
{
    osl_list_t * ifaces = osl_network_all_interfaces();
    osl_list_t * ptr = ifaces;
    for (; ptr; ptr = ptr->next)
    {
	osl_network_interface_t * iface = (osl_network_interface_t*)ptr->data;
	printf("* iface name -- %s\n", iface->name);
    }

    osl_list_free(ifaces, (void (*)(void*))osl_network_interface_free);
}

void test_echo_server()
{
    /*  */
    int port = 0;
    osl_thread_t * server_thread = osl_thread_new(echo_server_thread, &port);
    osl_thread_start(server_thread);

    idle(100);

    printf("port %d\n", port);
    test_echo_client(port);

    osl_thread_join(server_thread);
    osl_thread_free(server_thread);

    /*  */
    port = 0;
    server_thread = osl_thread_new(echo_server2_thread, &port);
    osl_thread_start(server_thread);

    idle(100);

    printf("port %d\n", port);
    test_echo_client(port);

    osl_thread_join(server_thread);
    osl_thread_free(server_thread);
}

int main(int argc, char *argv[])
{
    osl_platform_once();
    osl_platform_ignore_sigpipe();
    osl_platform_use_socket();
    
    test_thread();
    test_list();
    test_string_buffer();
    test_file();
    test_network();
    test_echo_server();
    test_date();
    
    osl_platform_finish();
    
    return 0;
}



static void * echo_server_thread(void * arg)
{
    int * server_port = (int*)arg;

    osl_platform_use_socket();
    
    int yes = 1;
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
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
	int remote_sock = accept(sock, (struct sockaddr*)&remote_addr, &remote_addr_len);
	
	if (remote_sock < 0)
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
	close(remote_sock);
    }

    close(sock);
    return 0;
}


static void * echo_server2_thread(void * arg)
{
    int * server_port = (int*)arg;

    osl_platform_use_socket();

    osl_inet_address_t * addr = osl_inet_address_new(osl_inet4, "0.0.0.0", *server_port);
    int sock = osl_server_socket_bind(addr, 1);
    osl_inet_address_free(addr);

    addr = osl_socket_get_inet_address(sock);
    *server_port = addr->port;
    osl_inet_address_free(addr);

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
	int remote_sock = accept(sock, (struct sockaddr*)&remote_addr, &remote_addr_len);
	
	if (remote_sock < 0)
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
	close(remote_sock);
    }

    close(sock);
    return 0;
}

void test_echo_client(int port)
{
    char buffer[10] = {0,};
    struct sockaddr_in addr;
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
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
    close(sock);
}
