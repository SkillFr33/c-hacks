#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#ifndef __USE_XOPEN2K
#define __USE_XOPEN2K
#endif
#include <netdb.h>
#include "util.h"

void net_panic(const char* func_name, int err) {
  fprintf(stderr, "%s: getaddrinfo: %s\n", func_name, gai_strerror(err));
  exit(EXIT_FAILURE);
}

void panic(const char* func_name) {
  perror(func_name);
  exit(EXIT_FAILURE);
}

void replace(char* buffer, char old, char new) {
  char* temp;
  temp = strchr(buffer, old);
  if(temp)
    *temp = new;
}