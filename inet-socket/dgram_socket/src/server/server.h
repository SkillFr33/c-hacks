#ifndef __SERVER_H
#define __SERVER_H

int new_udp_server(const char* addr, const char* port);
void get_addr_and_port(struct sockaddr* sa, int* port, char* addr, size_t len);

#endif