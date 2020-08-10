#ifndef __MY_UTIL_H
#define __MY_UTIL_H

#include <sys/epoll.h>

/*
  Funções utilitárias
*/

#define BUFFERSIZE 1024

void panic(const char* func_name);
void net_panic(const char* func_name, int err);
void sigchld_handler(int sig);
void toggle_case(char* buffer);
void replace(char* buffer, char old, char new);
int get_fd_by_event(struct epoll_event* ev, size_t ev_size, int event);

#endif