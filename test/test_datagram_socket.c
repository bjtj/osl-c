#include "osl.h"
#include "socket.h"
#include "thread.h"
#include <assert.h>

/* datagram socket */
static void * datagram_server_thread(void * arg);
static void test_datagram_client(int port);

void test_datagram_socket(void)
{

  printf("== test datagram socket ==\n");
    
  /*  */
  int port = 0;
  osl_thread_t * server_thread = osl_thread_init(osl_thread_new(), datagram_server_thread, &port);
  osl_thread_start(server_thread);

  osl_idle(100);

  printf("port %d\n", port);
  test_datagram_client(port);

  osl_thread_join(server_thread);
  osl_thread_free(server_thread);
}

int main(void)
{
  osl_init_once();
  osl_ignore_sigpipe();
  osl_use_socket();

  test_datagram_socket();
    
  osl_finish();
    
  return 0;
}

void * datagram_server_thread(void * arg)
{
  int ret;
  int * port = (int*)arg;
  osl_inet_address_t * addr = osl_inet_address_init(osl_inet_address_new(), osl_inet4, "0.0.0.0", *port);
  osl_socket sock = osl_datagram_socket_bind(addr, 1);
  osl_inet_address_free(addr);
  assert(osl_socket_is_valid(sock));
    
  addr = osl_socket_get_inet_address(sock);
  *port = addr->port;
  osl_inet_address_free(addr);

  char buffer[1024] = {0,};
  struct sockaddr_in remote_addr;
  socklen_t remote_addr_len = sizeof(remote_addr);
  memset(&remote_addr, 0, sizeof(remote_addr));
  assert((ret = osl_datagram_socket_recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr*)&remote_addr, &remote_addr_len)) > 0);    

  char ip_buffer[INET6_ADDRSTRLEN] = {0,};
  printf("recvfrom -- %s\n", osl_ip_string((struct sockaddr*)&remote_addr, ip_buffer, sizeof(ip_buffer)));

  assert((ret = osl_datagram_socket_sendto(sock, buffer, ret, 0, (struct sockaddr*)&remote_addr, remote_addr_len)) > 0);    

  osl_socket_close(sock);
    
  return 0;
}

void test_datagram_client(int port)
{
  osl_inet_address_t * addr = osl_inet_address_init(osl_inet_address_new(), osl_inet4, "127.0.0.1", port);
  struct addrinfo * res = osl_inet_address_resolve(addr, SOCK_DGRAM);
  assert(res != NULL);
  osl_inet_address_free(addr);
    
  osl_socket sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  assert(osl_socket_is_valid(sock));    
  assert(osl_datagram_socket_sendto(sock, "hello", 5, 0, res->ai_addr, (int)res->ai_addrlen) > 0);    

  freeaddrinfo(res);

  char buffer[1024] = {0,};
  assert(osl_datagram_socket_recvfrom(sock, buffer, sizeof(buffer), 0, NULL, NULL) > 0);
  assert(strcmp(buffer, "hello") == 0);

  osl_socket_close(sock);
}
