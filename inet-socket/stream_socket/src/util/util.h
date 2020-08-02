#ifndef __MY_UTIL_H
#define __MY_UTIL_H

/*
  Funções utilitárias
*/

void panic(const char* func_name);
void net_panic(const char* func_name, int err);
void sigchld_handler(int sig);
void toggle_case(char* buffer);
void replace(char* buffer, char old, char new);

#endif