#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include "server.h"
#include "../util/util.h"

/*
  Esse servidor não faz nada de mais, só converte as letras das mensagens recebidas de maiúscula para
  minúscula e vice-versa, reenviando para o cliente. Escrevi esse código só pra demonstrar a criação
  e configuração de um servidor tcp.
*/

int main(int argc, char* argv[]) {

  if(argc != 2) {
    fprintf(stderr, "Uso: %s [PORT]\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  char* port = argv[1];

  // addr NULL: wildcard address
  int server_fd = new_tcp_server(NULL, port);
  printf("Servidor escutando por conexões na porta %s\n", port);

  // configurando handler para o sinal SIGCHLD
  struct sigaction sigchld_sa;
  sigchld_sa.sa_flags = SA_RESTART;
  sigchld_sa.sa_handler = sigchld_handler;
  sigemptyset(&sigchld_sa.sa_mask);
  sigaction(SIGCHLD, &sigchld_sa, NULL);

  // estrutura para armazenar o endereço do cliente (que pode ser IPv4 ou IPv6)
  struct sockaddr_storage client_ss;
  
  // buffer para armazenar o endereço IP do cliente
  char str_ip[INET6_ADDRSTRLEN];

  // loop infinito para aceitar conexões
  while(1) {
    unsigned size = sizeof(client_ss);

    int client_fd = accept(server_fd, (struct sockaddr*) &client_ss, &size);
    if(client_fd == -1)
      panic("accept");

    int port;
    get_addr_and_port((struct sockaddr*) &client_ss, &port, str_ip, sizeof(str_ip));
    printf("Cliente %s:%d conectado!\n", str_ip, port);

    // cria um processo filho para lidar com o cliente
    switch(fork()) {
      case -1: // erro
        panic("fork");
      case 0: { // processo filho
        close(server_fd); // o processo filho não precisa desse socket
        handle_client(client_fd);
      }
      default: // processo pai
        close(client_fd); // o processo pai não precisa desse socket
    }
  }

  return EXIT_FAILURE; // nunca deve chegar aqui
}