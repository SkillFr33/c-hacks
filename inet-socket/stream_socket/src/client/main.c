#include <stdlib.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/epoll.h>
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

  int epoll_fd = epoll_create1(0);
  if(epoll_fd == -1)
    panic("epoll_create");

  // epoll_event para poder adicionar os file descriptors e seus respectivos eventos a serem monitorados
  struct epoll_event event = {0};
  event.events = EPOLLIN; // evento de entrada

  event.data.fd = 0; // entrada padrão
  if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, 0, &event) == -1)
    panic("epoll_ctl");
  
  event.data.fd = sockfd; // conexão com o servidor
  if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sockfd, &event) == -1)
    panic("epoll_ctl");

  char buffer[BUFFERSIZE]; // buffer para armazenar as mensagens enviadas/recebidas
  int bytes; // quantidade de bytes enviados/recebidos

  // lê uma mensagem da entrada padrão e envia para o servidor
  int epoll_ret = 0;
  struct epoll_event evs[2] = {0}; // STDIN_FILENO e sockfd
  puts("Digite suas mensagens( [CTRL+D] para sair)!\n");
  while( (epoll_ret = epoll_wait(epoll_fd, evs, 2, -1)) != -1 ) {
    
    // zera o buffer pra evitar bugs
    memset(buffer, 0, sizeof(buffer));
    
    // adquire o fd que sofreu o evento
    int fd_event = get_fd_by_event(evs, epoll_ret, EPOLLIN);
    if(fd_event == -1)
      continue; // evento inesperado

    // servidor me enviou alguma mensagem
    if(fd_event == sockfd) {
      // recebe uma mensagem do servidor
      bytes = recv(sockfd, buffer, sizeof(buffer), MSG_NOSIGNAL);

      // bytes > 0, alguma mensagem foi recebida
      if(bytes > 0) {

        if(buffer[0] == '\0' || buffer[0] == '\n')
          continue;
        else if(strncmp(buffer, "timeout", 7) == 0) {
          puts("\nDesconectado por timeout!");
          exit(0);
        }
        else
          printf("Mensagem recebida: %s\n", buffer);

      }
      else if(bytes == 0) { // nenhum byte recebido, servidor encerrou a conexão
        puts("\nServidor fechou!");
        exit(0);
      }
      else // bytes < 0, erro no recv
        perror("recv");
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
    }
  } // end while

  if(epoll_ret == -1)
    panic("epoll_wait");

  return EXIT_SUCCESS;
}