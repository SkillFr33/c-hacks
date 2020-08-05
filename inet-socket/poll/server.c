#include <stdlib.h>
#include <stdio.h>
#ifndef __USE_XOPEN2K
#define __USE_XOPEN2K
#endif
#include <netdb.h>
#include <unistd.h>
#include <poll.h>
#include <arpa/inet.h>
#include "server.h"

// quantidade de requisição pendentes de serem aceitas
#define BACKLOG 5

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

void create_pollfd_list(pollfd_list* pfd_list, const int cap) {
  pfd_list->pfds = malloc(sizeof(struct pollfd) * cap);
  if(pfd_list == NULL)
    panic("create_pollfd_list");

  pfd_list->capacity = cap;
  pfd_list->size = 0;
}

void add_to_pollfd_list(pollfd_list* pfd_list, const int fd) {
  
  // lista cheia
  if(pfd_list->size == pfd_list->capacity) {
    size_t bytes = sizeof(struct pollfd); // tamanho de um struct pollfd (elemento da lista)
    pfd_list->capacity += 5; // calcula a nova capacidade

    pfd_list->pfds = realloc(pfd_list->pfds, pfd_list->capacity * bytes);
    if(pfd_list->pfds == NULL) // verifica falha em realloc
      panic("add_to_pollfd_list: realloc");
  }

  pfd_list->pfds[pfd_list->size].fd = fd;
  pfd_list->pfds[pfd_list->size].events = POLLIN; // evento de leitura

  pfd_list->size++; // incrementa o tamanho da lista de pollfd
}

void del_from_pollfd_list(pollfd_list* pfd_list, const int index) {
  if(pfd_list->size == 0)
    return;

  // copia o ultimo elemento para a posição do que será excluído
  pfd_list->pfds[index] = pfd_list->pfds[pfd_list->size - 1];
  pfd_list->size--; // decrementa o tamanho
}

void panic(const char* func_name) {
  perror(func_name);
  exit(EXIT_FAILURE);
}

void net_panic(const char* func_name, int err) {
  fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(err));
  exit(EXIT_FAILURE);
}