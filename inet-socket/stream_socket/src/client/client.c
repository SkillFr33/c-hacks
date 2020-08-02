#include <stdlib.h>
#include <stdlib.h>
#ifndef __USE_XOPEN2K
#define __USE_XOPEN2K
#endif
#include <netdb.h>
#include <memory.h>
#include "client.h"
#include "../util/util.h"

int connect_tcp_server(const char* addr, const char* port) {

  // critérios para selecionar os endereços retornados por getaddrinfo
  struct addrinfo hints = {0}; 
  hints.ai_family   = AF_UNSPEC;   // IPv4 ou IPv6
  hints.ai_socktype = SOCK_STREAM; // socket orientado à conexão

  struct addrinfo* res;
  // adquire informações de endereço do host remoto
  int err = getaddrinfo(addr, port, &hints, &res);
  if(err != 0)
    net_panic("connect_tcp_server", err);

  // adquire um socket com as informações retornadas em res 
  int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  if(sockfd == -1)
    panic("socket");

  // conecta o socket ao endereço do servidor
  if(connect(sockfd, res->ai_addr, res->ai_addrlen) == -1)
    panic("connect");

  freeaddrinfo(res); // 'res' não é mais necessário

  return sockfd; // retorna o socket para se comunicar com o servidor
}