#include <stdlib.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include "client.h"
#include "../util/util.h"

int main(int argc, char* argv[]) {

  if(argc != 3) {
    fprintf(stderr, "Uso: %s [ADDR] [PORT]\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  char* addr = argv[1];
  char* port = argv[2];

  // conecta no host/servidor addr:port
  int sockfd = connect_tcp_server(addr, port);

  char buffer[1024]; // buffer para armazenar as mensagens enviadas/recebidas
  int bytes; // quantidade de bytes enviados/recebidos

  // lê uma mensagem da entrada padrão e envia para o servidor
  while(printf("Mensagem( [CTRL+D] para sair): "), 
        fgets(buffer, sizeof(buffer), stdin) != NULL) {
    
    // verifica se alguma coisa foi digitada
    if(buffer[0] == '\n')
      continue;

    replace(buffer, '\n', '\0');

    // envia a mensagem para o servidor
    bytes = send(sockfd, buffer, strlen(buffer) + 1, 0);
    if(bytes == -1)
      panic("send");
    
    // recebe uma mensagem do servidor
    bytes = recv(sockfd, buffer, sizeof(buffer), MSG_NOSIGNAL);
    if(bytes <= 0)
      break;
    
    printf("Mensagem recebida: %s\n", buffer);
  }

  puts("\nDesconectado!");

  return EXIT_SUCCESS;
}
