#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include "client.h"
#include "../util/util.h"

int main(int argc, char* argv[]) {

  if(argc != 3) {
    fprintf(stderr, "Uso: %s [ADDR] [PORT]\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  char* addr = argv[1];
  char* port = argv[2];

  struct addrinfo* ai;
  int sockfd = new_udp_client(addr, port, &ai);

  char buffer[1024];
  int bytes;
  while(printf("\nMensagem a ser enviada: "),
        fgets(buffer, sizeof(buffer), stdin) != NULL) {

    if(buffer[0] == '\n')
      continue;
    
    replace(buffer, '\n', '\0');
    bytes = sendto(sockfd, buffer, strlen(buffer) + 1, 0, ai->ai_addr, ai->ai_addrlen);
    if(bytes == -1)
      panic("sendto");
    
    // passo NULL nos últimos argumentos, pois não preciso armazenar informações de quem enviou
    bytes = recvfrom(sockfd, buffer, sizeof(buffer), MSG_NOSIGNAL, NULL, NULL);
    if(bytes == -1)
      panic("recvfrom");
    else if(bytes == 0) {
      puts("Desconectado!");
      break;
    }

    printf("Mensagem recebida de volta: %s\n", buffer);
  }

  freeaddrinfo(ai);

  /*
    Esse código poderia ter sido escrito de outra forma, usando a syscall connect para associar um endereço
    à um socket UDP e utilizar as funções send e recv. Isso pouparia o terceiro argumento em new_udp_client
    e as syscalls sendto e recvfrom, que possuem muitos argumentos.
  */

}