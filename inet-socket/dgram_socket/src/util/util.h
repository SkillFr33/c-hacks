#ifndef __UTIL_H
#define __UTIL_H

void net_panic(const char* func, const int err);
void panic(const char* func_name);
void replace(char* buffer, char old, char new);

#endif