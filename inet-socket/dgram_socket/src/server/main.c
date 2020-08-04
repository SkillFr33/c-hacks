#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include "server.h"
#include "../util/util.h"


int main(int argc, char* argv[]) {

  if(argc != 2) {
    fprintf(stderr, "Uso: %s [PORT]\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  char* sport = argv[1];

  int sockfd = new_udp_server(NULL, sport);

  char buffer[1024] = {0};
  char str_ip[INET6_ADDRSTRLEN];
  int port;

  struct sockaddr_storage ss = {0};
  while(1) {
    unsigned size = sizeof(ss);
    int bytes = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*) &ss, &size);
    if(bytes == -1)
      panic("recvfrom");

    get_addr_and_port((struct sockaddr*) &ss, &port, str_ip, sizeof(str_ip));
    printf("\nMensagem recebida de %s:%d\n> \"%s\"\n", str_ip, port, buffer);

    // utilizo o mesmo endpoint(socket) para enviar os dados, contudo direcionando para o endereço do
    // host que acabou de me enviar dados(suas informações de endereço foram armazenadas em 'ss')
    sendto(sockfd, buffer, bytes, 0, (struct sockaddr*) &ss, size);
  }

  return EXIT_SUCCESS;
}