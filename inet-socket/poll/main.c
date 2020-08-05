#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#ifndef __USE_XOPEN2K
#define __USE_XOPEN2K
#endif
#include <netdb.h>
#include <unistd.h>
#include <poll.h>
#include "server.h"

/*
  Utilizando poll para esperar por eventos de entrada que possam ocorrer nos sockets do servidor ou cliente
*/

int main(int argc, char* argv[]) {

  if(argc != 3) {
    fprintf(stderr, "Uso: %s [ADDR] [PORT]\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  char* addr = argv[1];
  char* port = argv[2]; 

  pollfd_list pfd_list;
  create_pollfd_list(&pfd_list, 5);

  // adquire um endpoint para escutar por conexões
  int serverfd = new_tcp_server(addr, port);

  add_to_pollfd_list(&pfd_list, serverfd); // adiciono o socket do servidor à lista de pollfd

  // espera por um evento que ocorra em qualquer um dos sockets em pfd_list.pfds
  while(poll(pfd_list.pfds, pfd_list.size, -1) != -1) {


    // Ocorreu um evento em algum socket, contudo ainda não sei em qual.
    // É necessário checar cada um dos pollfd e verificar qual recebeu um evento de POLLIN
    int fd_index; // contador que armazena o índice do socket que recebeu o evento
    for(fd_index = 0; fd_index < pfd_list.size; fd_index++)
      if(pfd_list.pfds[fd_index].revents & POLLIN)
        break; // achei o socket, dou break para parar a busca

    // verifica se foi o socket do servidor que recebeu o evento
    if(pfd_list.pfds[fd_index].fd == serverfd) {
      // se sim, significa que uma requisição de conexão está esperando para ser atendida

      int newfd = accept(serverfd, NULL, NULL); // aceita a conexão
      if(newfd == -1)
        perror("accept"); // exibo mensagem do erro que ocorreu ao aceitar conexão
      
      puts("Cliente conectado!");
      add_to_pollfd_list(&pfd_list, newfd); // adiciona novo socket ao poll
    }
    else { 
      // se não foi o socket do servidor, então é um de cliente que está disponível para leitura
      char buffer[1024];
      int clientfd = pfd_list.pfds[fd_index].fd;
      int bytes = recv(clientfd, buffer, sizeof(buffer), 0); // recebe a mensagem e armazena em 'buffer'
    
      // se recebemos 0 byte, significa que o cliente desconectou!
      if(bytes == 0) {
        printf("Cliente de socket %d desconectou!\n", clientfd);
        del_from_pollfd_list(&pfd_list, fd_index);
        close(clientfd); // fecha o socket e liberar qualquer recurso atrelado à ele
      }
      else if(bytes > 0) {
        printf("Cliente de socket %d enviou: ", clientfd);
        fflush(stdout); // força um flush em stdout para exibir a mensagem do printf acima
        write(1, buffer, bytes);
        broadcast(&pfd_list, buffer, bytes, fd_index); // replica mensagem para demais clientes
      }
      else
        perror("recv"); // exibo mensagem do erro que ocorreu receber dados do cliente
    }
  
  } // end while

  free(pfd_list.pfds);
  panic("poll");
}