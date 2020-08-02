#ifndef __SERVER_H
#define __SERVER_H

#include <stdlib.h>
#include <memory.h>
#include <stdio.h>

#ifndef __USE_XOPEN2K
#define __USE_XOPEN2K
#endif

#include <netdb.h>
#include <arpa/inet.h>
#include "../util/util.h"

int new_tcp_server(const char* addr, const char* port);
void handle_client(int client_fd);
void get_addr_and_port(struct sockaddr* sa, int* port, char* buffer, size_t len);

#endif