#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#ifndef __USE_XOPEN2K
#define __USE_XOPEN2K
#endif
#include <netdb.h>
#include <arpa/inet.h>
#include "server.h"
#include "../util/util.h"

int new_udp_server(const char* addr, const char* port) {

  struct addrinfo hints = {0};
  hints.ai_family   = AF_UNSPEC;  // IPv4 ou IPv6
  hints.ai_socktype = SOCK_DGRAM; // Datagram socket (UDP)
  hints.ai_flags    = AI_PASSIVE; // use meu IP

  struct addrinfo* res;
  int err = getaddrinfo(addr, port, &hints, &res);
  if(err != 0)
    net_panic("new_udp_socket", err);

  int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  if(sockfd == -1)
    panic("new_udp_socket: socket");

  if(bind(sockfd, res->ai_addr, res->ai_addrlen) == -1)
    panic("new_udp_socket: bind");

  freeaddrinfo(res);

  return sockfd;
}

void get_addr_and_port(struct sockaddr* sa, int* port, char* addr, size_t len) {
  if(sa->sa_family == AF_INET) {
    inet_ntop(AF_INET, &((struct sockaddr_in*) sa)->sin_addr, addr, len);
    if(port)
      *port = ntohs( ((struct sockaddr_in*) sa)->sin_port );
  }
  else if(sa->sa_family == AF_INET6) {
    inet_ntop(AF_INET6, &((struct sockaddr_in6*) sa)->sin6_addr, addr, len);
    if(port)
      *port = ntohs( ((struct sockaddr_in6*) sa)->sin6_port );
  }
}