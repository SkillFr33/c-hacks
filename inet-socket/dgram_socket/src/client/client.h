#ifndef __CLIENT_H
#define __CLIENT_H

#ifndef __USE_XOPEN2K
#define __USE_XOPEN2K
#endif
#include <netdb.h>

int new_udp_client(const char* addr, const char* port, struct addrinfo** ai);

#endif