#include <stdlib.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <poll.h>
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

  // pollfd para monitorar evento de entrada
  struct pollfd pfd[2];
  pfd[0].fd = 0;          // entrada padrão
  pfd[0].events = POLLIN; // evento de entrada

  pfd[1].fd = sockfd; // socket p/ comunicar com servidor
  pfd[1].events = POLLIN; // evento de entrada

  char buffer[1024]; // buffer para armazenar as mensagens enviadas/recebidas
  int bytes; // quantidade de bytes enviados/recebidos

  // lê uma mensagem da entrada padrão e envia para o servidor
  int poll_ret = 0;
  while(fprintf(stderr, "Mensagem( [CTRL+D] para sair): "),
        (poll_ret = poll(pfd, 2, -1)) != -1 ) {
    
    // adquire o fd que sofreu o evento
    int fd_event = get_fd_by_event(pfd, 2, POLLIN);
    if(fd_event == -1)
      continue; // evento inesperado

    // servidor me enviou alguma mensagem
    if(fd_event == sockfd) {
      bytes = recv(sockfd, buffer, sizeof(buffer), MSG_NOSIGNAL);
      if(bytes == -1)
        break;
      
      if(strncmp(buffer, "timeout", 7) == 0) {
        puts("\nDesconectado por timeout!");
        exit(0);
      }
    }
    else { // evento de entrada na entrada padrão

      if(fgets(buffer, sizeof(buffer), stdin) == NULL)
        break;

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
  } // end while

  if(poll_ret == -1)
    panic("poll");

  return EXIT_SUCCESS;
}