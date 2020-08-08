#include <stdlib.h>
#include <stdio.h>
#ifndef __USE_XOPEN2K
#define __USE_XOPEN2K
#endif
#include <netdb.h>
#include <poll.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "../util/util.h"

// quantidade de requisição pendentes de serem aceitas
#define BACKLOG 5

// timeout ao ficar 10s sem receber mensagem do cliente
#define TIMEOUT 10

// inicializa um servidor tcp e retorna seu socket(file descriptor)
int new_tcp_server(const char* addr, const char* port) {

  struct addrinfo hints = {0};
  hints.ai_socktype = SOCK_STREAM;  // stream socket (orientado à conexão)
  hints.ai_family   = AF_UNSPEC;    // IPv4 ou IPv6 
  hints.ai_flags    = AI_PASSIVE;   // socket passivo (servidor) 

  struct addrinfo* res; // retorno de getaddrinfo (endereço do servidor)
  int err = getaddrinfo(addr, port, &hints, &res);
  if(err != 0)
    net_panic("new_tcp_server: getaddrinfo", err);
  
  // cria o socket com as configurações do endereço retornado por getaddrinfo
  int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  if(sockfd == -1)
    panic("new_tcp_server: socket");

  // associo o socket ao endereço
  if(bind(sockfd, res->ai_addr, res->ai_addrlen) == -1)
    panic("new_tcp_server: bind");

  // não necessito mais do 'res'
  freeaddrinfo(res);
  
  // preparando o socket para escutar por conexões
  if(listen(sockfd, BACKLOG) == -1)
    panic("new_tcp_server: listen");
  
  return sockfd; // retorna o socket do servidor para aceitar conexões
}

// adquire a porta do cliente e converte seu endereço de IP em uma string legível 
void get_addr_and_port(struct sockaddr* sa, int* port, char* buffer, size_t len) {
  if(sa->sa_family == AF_INET) { // IPv4
    inet_ntop(sa->sa_family, &((struct sockaddr_in*) sa)->sin_addr, buffer, len);
    if(port)
      *port = ntohs( ((struct sockaddr_in*) sa)->sin_port);
  }
  else { // IPv6
    inet_ntop(sa->sa_family, &((struct sockaddr_in6*) sa)->sin6_addr, buffer, len);
    if(port)
      *port = ntohs( ((struct sockaddr_in6*) sa)->sin6_port );
  }
}

void handle_client(int client_fd, struct sockaddr_storage ss) {
  char buffer[1024];
  int bytes;

  // configurando pollfd para monitorar evento de entrada em client_fd
  struct pollfd pfd[1];
  pfd[0].fd = client_fd;
  pfd[0].events = POLLIN;

  // adquirindo endereço de IP e porta do cliente 
  char str_ip[INET6_ADDRSTRLEN];
  int port;
  get_addr_and_port((struct sockaddr*) &ss, &port, str_ip, sizeof(str_ip));

  // esperando por mensagens do cliente. TIMEOUT segundos sem receber nada desconecta o cliente
  int poll_ret = 0;
  while( (poll_ret = poll(pfd, 1, TIMEOUT * 1000)) != -1 ) {
    
    // verificando se houve timeout
    if(poll_ret == 0) {
      printf("O cliente %s:%d foi desconectado por timeout!\n", str_ip, port);
      send(client_fd, "timeout!\n", 9, MSG_DONTWAIT);
      exit(0);
    }
    
    // se o socket do cliente não sofreu nenhum evento de POLLIN, algum evento inesperado ocorreu,
    // fazendo com que poll retornasse
    if(!(pfd[0].revents & POLLIN))
      continue;

    // recebe mensagem do cliente
    bytes = recv(client_fd, buffer, sizeof(buffer), MSG_NOSIGNAL);
    if(bytes > 0) {
      toggle_case(buffer); // alterna o case das
      bytes = send(client_fd, buffer, bytes, 0); // reenvia a mensagem de volta para o cliente
      if(bytes == -1)
        panic("send");
    }
    else if(bytes == 0) {     
      printf("O cliente %s:%d desconectou!", str_ip, port);
      exit(0);
    }
    else
      panic("recv");
  }

  panic("poll"); // erro ao monitorar evento de entrada em client_fd
}