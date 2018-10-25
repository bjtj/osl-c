#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "../src/osl.h"
#include "../src/thread.h"
#include "../src/list.h"
#include "../src/string_buffer.h"
#include "../src/network.h"
#include "../src/date.h"
#include "../src/sem.h"
#include "../src/socket.h"
#include "../src/library.h"
#include "../src/log.h"
#include "../src/pathname.h"
#include "../src/process.h"
#include "../src/url.h"
#include "../src/argparse.h"
#include "../src/str.h"
#include "../src/looper.h"
#include <assert.h>


/* socket */
static void * echo_server_thread(void * arg);
static void * echo_server2_thread(void * arg);
static void test_echo_client(int port);
static void test_echo_client2(int port);
static void test_echo_client3(int port);

/* datagram socket */
static void * datagram_server_thread(void * arg);
static void test_datagram_client(int port);

/* multicast socket */
static void * multicast_server_thread(void * arg);
static void test_multicast_client(void);

void * worker(void * arg)
{
    int id = *(int*)arg;
    int i = 0;
    for (; i < 10; i++) {
	printf("[%d] idx -- %d\n", id, i);
	osl_idle(50);
    }
    return 0;
}

void test_osl(void)
{
    char * cwd = osl_getcwd();
    printf("cwd: %s\n", cwd);
    free(cwd);
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

    list = osl_list_remove_idx(list, 2, free);
    ptr = list;
    for (; ptr; ptr = ptr->next)
    {
	printf("* %s\n", (char *)ptr->data);
    }

    printf("====\n");

    list = osl_list_remove_idx(list, 0, free);
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
	osl_idle(200);
    }
}

void test_string(void)
{
    assert(osl_string_starts_with("--hello", "--") == 1);
    assert(osl_string_ends_with("img.jpg", ".jpg") == 1);
}

void test_string_buffer(void)
{
    printf(" == test string buffer ==\n");
    osl_string_buffer_t * sb = osl_string_buffer_new();
    osl_string_buffer_append(sb, "hello");
    osl_string_buffer_append(sb, " ");
    osl_string_buffer_append(sb, "world");

    char * str = osl_string_buffer_to_string(sb);
    printf("str is '%s'\n", str);
    free(str);

    osl_string_buffer_free(sb);
}

void test_file(void)
{
    char * dump;
    osl_file_stream_t stream = osl_file_stream_open("hello", "w");
    osl_file_stream_write(stream, 'h');
    osl_file_stream_write(stream, 'e');
    osl_file_stream_write(stream, 'l');
    osl_file_stream_write(stream, 'l');
    osl_file_stream_write(stream, 'o');
    osl_file_stream_close(stream);

    assert(osl_pathname_exists("hello"));
    assert(osl_pathname_is_file("hello"));
    assert(osl_pathname_is_dir("hello") == 0);

    stream = osl_file_stream_open("hello", "r");
    dump = osl_file_stream_dump(&stream);
    printf("dump: '%s'\n", dump);
    assert(strcmp(dump, "hello") == 0);
    free(dump);
    osl_file_stream_close(stream);

    printf("file size: %d\n", (int)osl_pathname_filesize("hello"));
}

void test_pathname(void)
{
    printf("== test pathname ==\n");
    char * name = osl_pathname_basename("/abc/d");
    assert(strcmp(name, "d") == 0);
    free(name);

    name = osl_pathname_dirname("/abc/d");
    assert(strcmp(name, "/abc/") == 0);
    free(name);

    name = osl_pathname_dirname("abc/d");
    assert(strcmp(name, "abc/") == 0);
    free(name);
}

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

void test_echo_server()
{
    /*  */
    printf(" == test echo server==\n");
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

void test_datagram_socket(void)
{

    printf(" == test datagram socket ==\n");
    
    /*  */
    int port = 0;
    osl_thread_t * server_thread = osl_thread_new(datagram_server_thread, &port);
    osl_thread_start(server_thread);

    osl_idle(100);

    printf("port %d\n", port);
    test_datagram_client(port);

    osl_thread_join(server_thread);
    osl_thread_free(server_thread);
}

void test_multicast_socket(void)
{

    printf(" == test multicast socket ==\n");
    
    /*  */
    osl_thread_t * server_thread = osl_thread_new(multicast_server_thread, NULL);
    osl_thread_start(server_thread);

    osl_idle(100);

    test_multicast_client();

    osl_thread_join(server_thread);
    osl_thread_free(server_thread);
}

void test_library(void)
{
    printf(" == test library ==\n");
#if defined(PLATFORM_WINDOWS)
    osl_lib_handle lib = osl_library_load("./", "hello");
#else
    osl_lib_handle lib = osl_library_load("./test", "hello");
#endif
    ((void (*)(void))osl_library_get_symbol(lib, "hello"))();
    osl_library_close(lib);
}

void test_log(void)
{
    osl_log_trace("hello");
    osl_log_debug("hello");
    osl_log_info("hello");
    osl_log_warn("hello");
    osl_log_error("hello");
    osl_log_fatal("hello");
}

void test_process(void)
{
    puts("== test process==");
    osl_process_t * process = osl_process_new("echo \"hello\"");
    osl_process_start(process);

    osl_file_stream_t out = osl_process_out_stream(process);
    int ch;
    while ((ch = out.read(&out)) > 0)
    {
	putchar(ch);
    }
    putchar('\n');

    osl_process_wait(process);
    printf("exit code: %d\n", process->exit_code);
    
    osl_process_close(process);
    osl_process_free(process);
}

void test_url(void)
{
    printf("== test url ==\n");
    {
	osl_url_t * url = osl_url_from_string("http://localhost:5000");
	char * url_str = osl_url_to_string(url);
	printf("url -- %s\n", url_str);
	free(url_str);
	osl_url_free(url);
    }

    {
	osl_url_t * url = osl_url_from_string("http://localhost:5000/resource");
	char * url_str = osl_url_to_string(url);
	printf("url -- %s\n", url_str);
	free(url_str);
	osl_url_free(url);
    }

    {
	osl_url_t * url = osl_url_from_string("http://localhost/resource");
	char * url_str = osl_url_to_string(url);
	printf("url -- %s\n", url_str);
	free(url_str);
	osl_url_free(url);
    }

    {
	osl_url_t * url = osl_url_from_string("http://localhost/resource?query");
	char * url_str = osl_url_to_string(url);
	printf("url -- %s\n", url_str);
	free(url_str);
	osl_url_free(url);
    }

    {
	osl_url_t * url = osl_url_from_string("http://localhost/resource?a=A&b=B");
	char * url_str = osl_url_to_string(url);
	printf("url -- %s\n", url_str);
	free(url_str);
	osl_url_free(url);
    }

    {
	osl_url_t * url = osl_url_from_string("http://username:password@localhost/resource");
	char * url_str = osl_url_to_string(url);
	printf("url -- %s\n", url_str);
	free(url_str);
	osl_url_free(url);
    }

    {

	osl_url_t * base_url = osl_url_from_string("http://127.0.0.1/a/b/c");
	osl_url_t * url = osl_url_merge(base_url, "d");
	char * url_str = osl_url_to_string(url);
	printf("url -- %s\n", url_str);
	free(url_str);
	osl_url_free(url);
	osl_url_free(base_url);
    }

    {

	osl_url_t * base_url = osl_url_from_string("http://127.0.0.1/a/b/c/");
	osl_url_t * url = osl_url_merge(base_url, "d");
	char * url_str = osl_url_to_string(url);
	printf("url -- %s\n", url_str);
	free(url_str);
	osl_url_free(url);
	osl_url_free(base_url);
    }

    {
	osl_url_t * base_url = osl_url_from_string("http://127.0.0.1/a/b/c");
	osl_url_t * url = osl_url_merge(base_url, "/d");
	char * url_str = osl_url_to_string(url);
	printf("url -- %s\n", url_str);
	free(url_str);
	osl_url_free(base_url);
	osl_url_free(url);
    }


    {
	osl_url_t * base_url = osl_url_from_string("http://127.0.0.1/a/b/c?query");
	osl_url_t * url = osl_url_merge(base_url, "d");
	char * url_str = osl_url_to_string(url);
	printf("url -- %s\n", url_str);
	free(url_str);
	osl_url_free(url);
	osl_url_free(base_url);
    }

    {

	osl_url_t * base_url = osl_url_from_string("http://127.0.0.1/a/b/c/?query");
	osl_url_t * url = osl_url_merge(base_url, "d");
	char * url_str = osl_url_to_string(url);
	printf("url -- %s\n", url_str);
	free(url_str);
	osl_url_free(url);
	osl_url_free(base_url);
    }

    {
	osl_url_t * base_url = osl_url_from_string("http://127.0.0.1/a/b/c?query");
	osl_url_t * url = osl_url_merge(base_url, "/d");
	char * url_str = osl_url_to_string(url);
	printf("url -- %s\n", url_str);
	free(url_str);
	osl_url_free(base_url);
	osl_url_free(url);
    }
}

static void print_arguments(osl_arguments_t * args)
{
    printf(" -- print arguments --\n");
    printf("NAME -- %s\n", args->name);
    osl_list_t * ptr = args->arg_list;
    for (; ptr; ptr = ptr->next)
    {
	osl_argument_t * arg = (osl_argument_t*)ptr->data;
	printf("  ARGUMENT -- %s = %s\n", arg->name, arg->value);
    }
    ptr = args->text_list;
    for (; ptr; ptr = ptr->next)
    {
	char * str = (char*)ptr->data;
	printf("  TEXT -- %s\n", str);
    }
}

void test_arguments(void)
{
    printf("== test arguments ==\n");
    osl_arguments_usage_t usage = {0,};
    {
	char * argv[] = {
	    "hello",
	};
	int argc = 1;
	osl_arguments_t * args = osl_arguments_parse(&usage, argc, argv);
	print_arguments(args);
	osl_arguments_free(args);
    }

    {
	char * argv[] = {
	    "hello",
	    "--debug", "yes",
	    "--single",
	};
	int argc = 4;
	osl_arguments_t * args = osl_arguments_parse(&usage, argc, argv);
	print_arguments(args);
	osl_arguments_free(args);
    }

    {
	char * argv[] = {
	    "hello",
	    "text1",
	    "--debug", "yes",
	    "--single",
	    "text2"
	};
	usage.flag_list = osl_list_append(usage.flag_list, osl_argument_flag_new("--single", NULL, 1, NULL));
	int argc = 6;
	osl_arguments_t * args = osl_arguments_parse(&usage, argc, argv);
	print_arguments(args);
	osl_arguments_free(args);
    }

    osl_list_free(usage.flag_list, (osl_free_cb)osl_argument_flag_free);
}

void _looper_print_task_cb(osl_looper_t * looper, void * user)
{
    printf("MSG -- %s\n", (char*)user);
}

void _looper_hello_and_bye_task_cb(osl_looper_t * looper, void * user)
{
    printf("hello and bye!\n");
    osl_looper_finish(looper);
}

void test_looper(void)
{
    printf("== test looper ==\n");
    osl_looper_t * looper = osl_looper_new("looper-1", NULL);

    osl_looper_add_task(looper, _looper_print_task_cb, "a");
    osl_looper_add_task(looper, _looper_print_task_cb, "b");
    osl_looper_add_task(looper, _looper_print_task_cb, "c");
    osl_looper_add_task(looper, _looper_hello_and_bye_task_cb, NULL);
    osl_looper_add_task(looper, _looper_hello_and_bye_task_cb, NULL);
    osl_looper_run(looper);

    osl_looper_free(looper);
}

int main(int argc, char *argv[])
{
    osl_init_once();
    osl_ignore_sigpipe();
    osl_use_socket();

    test_osl();
    test_thread();
    test_list();
    test_date();
    test_string();
    test_string_buffer();
    test_file();
    test_pathname();
    test_network();
    test_echo_server();
    test_datagram_socket();
    test_multicast_socket();
    test_library();
    test_log();
    test_process();
    test_url();
    test_arguments();
    test_looper();
    
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
    osl_socket_close(sock);
}

void test_echo_client2(int port)
{
    char buffer[10] = {0,};
    osl_inet_address_t * addr = osl_inet_address_new(osl_inet4, "127.0.0.1", port);
    int sock = osl_socket_connect_with_timeout(addr, 1000);
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
    int sock = osl_socket_connect(addr);
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


void * datagram_server_thread(void * arg)
{
    int * port = (int*)arg;
    osl_inet_address_t * addr = osl_inet_address_new(osl_inet4, "0.0.0.0", *port);
    int sock = osl_datagram_socket_bind(addr, 1);
    osl_inet_address_free(addr);
    if (!osl_socket_is_valid(sock))
    {
	return 0;
    }
    
    addr = osl_socket_get_inet_address(sock);
    *port = addr->port;
    osl_inet_address_free(addr);

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
    printf("recvfrom -- %s\n", osl_ip_string((struct sockaddr*)&remote_addr, ip_buffer, sizeof(ip_buffer)));

    ret = (int)sendto(sock, buffer, ret, 0, (struct sockaddr*)&remote_addr, remote_addr_len);
    if (ret <= 0)
    {
	perror("sento() failed");
	return 0;
    }

    osl_socket_close(sock);
    
    return 0;
}

void test_datagram_client(int port)
{
    osl_inet_address_t * addr = osl_inet_address_new(osl_inet4, "127.0.0.1", port);
    struct addrinfo * res = osl_inet_address_resolve(addr, SOCK_DGRAM);
    osl_inet_address_free(addr);
    if (res == NULL)
    {
	return;
    }
    int sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (!osl_socket_is_valid(sock))
    {
	perror("socket() failed");
	return;
    }

    if (sendto(sock, "hello", 5, 0, res->ai_addr, res->ai_addrlen) <= 0)
    {
	perror("sendto() failed");
	return;
    }

    freeaddrinfo(res);

    char buffer[1024] = {0,};
    if (recvfrom(sock, buffer, sizeof(buffer), 0, NULL, NULL) <= 0)
    {
	perror("recvfrom() failed");
	return;
    }

    assert(strcmp(buffer, "hello") == 0);

    osl_socket_close(sock);
}


void * multicast_server_thread(void * arg)
{
    osl_inet_address_t * addr = osl_inet_address_new(osl_inet4, NULL, 1900);
    int sock = osl_datagram_socket_bind(addr, 1);
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
    printf("recvfrom -- %s:%d\n", osl_ip_string((struct sockaddr*)&remote_addr, ip_buffer, sizeof(ip_buffer)), ntohs(remote_addr.sin_port));
    printf("data -- %s\n", buffer);

    ret = (int)sendto(sock, buffer, ret, 0, (struct sockaddr*)&remote_addr, remote_addr_len);
    if (ret <= 0)
    {
	perror("sento() failed");
	return 0;
    }

    osl_socket_close(sock);
    
    return 0;
}

void test_multicast_client(void)
{
    struct sockaddr_in remote_addr;
    socklen_t remote_addr_len = sizeof(remote_addr);
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    if (!osl_socket_is_valid(sock))
    {
	perror("socket() failed");
	return;
    }

    memset(&remote_addr, 0, remote_addr_len);
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_addr.s_addr = inet_addr("239.255.255.250");
    remote_addr.sin_port = htons(1900);
    if (sendto(sock, "hello", 5, 0, (struct sockaddr*)&remote_addr, remote_addr_len) <= 0)
    {
	perror("sendto() failed");
	return;
    }

    char buffer[1024] = {0,};
    if (recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr*)&remote_addr, &remote_addr_len) <= 0)
    {
	perror("recvfrom() failed");
	return;
    }

    assert(strcmp(buffer, "hello") == 0);

    osl_socket_close(sock);
}
