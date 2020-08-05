#ifndef __SERVER_H
#define __SERVER_H

#include <stdlib.h>
#include <stdio.h>
#include <poll.h>
#ifndef __USE_XOPEN2K
#define __USE_XOPEN2K
#endif
#include <netdb.h>

typedef struct {
  struct pollfd* pfds;
  size_t size;
  size_t capacity;
} pollfd_list;

int new_tcp_server(const char* addr, const char* port);
void get_addr_and_port(struct sockaddr* sa, int* port, char* buffer, size_t len);

void panic(const char* func_name);
void net_panic(const char* func_name, int err);

void create_pollfd_list(pollfd_list* pfd_list, const int size);
void add_to_pollfd_list(pollfd_list* pfd_list, const int fd);
void del_from_pollfd_list(pollfd_list* pfd_list, const int index);

#endif