#include <stdlib.h>
#include <stdio.h>
#ifndef __USE_XOPEN2K
#define __USE_XOPEN2K
#endif
#include <netdb.h>
#include <sys/socket.h>
#include "client.h"
#include "../util/util.h"

int new_udp_client(const char* addr, const char* port, struct addrinfo** ai) {

  struct addrinfo hints = {0};
  hints.ai_family   = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;

  struct addrinfo* res;
  int err = getaddrinfo(addr, port, &hints, &res);
  if(err != 0)
    net_panic("new_udp_client", err);

  int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  if(sockfd == -1)
    panic("socket");

  *ai = res;

  return sockfd;
}