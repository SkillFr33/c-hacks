#include <stdlib.h>
#include <stdio.h>
#ifndef __USE_XOPEN2K
#define __USE_XOPEN2K
#endif
#include <netdb.h>
#include <sys/epoll.h>
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

  // retorno de getaddrinfo (endereço do servidor) e ponteiro temporário para 
  // percorrer a lista de endereços.
  struct addrinfo* res, *temp;
  int err = getaddrinfo(addr, port, &hints, &res);
  if(err != 0)
    net_panic("new_tcp_server: getaddrinfo", err);
  
  // cria o socket com as configurações do endereço retornado por getaddrinfo
  int sockfd;
  for(temp = res; temp != NULL; temp = temp->ai_next) {
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if(sockfd == -1) {
      perror("new_tcp_server: socket");
      continue;
    }

    // Algumas vezes ao encerrar o servidor, é possível que fique resquícios de sua existencia no kernel,
    // impossibilitando o uso do endereço e porta que o socket tinha dado bind. Se esse for o caso, tenta 
    // reutilizá-los
    int boolean = 1; // opção booleana (sim, no caso)
    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &boolean, sizeof(boolean)) == -1) { 
      perror("new_tcp_server: setsockopt");
      continue;
    }

    // associo o socket ao endereço
    if(bind(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
      close(sockfd);
      perror("new_tcp_server: bind");
      continue;
    }

    // consegui fazer todo o processo de criar o socket e associá-lo à um endereço, encerra o loop
    break;
  } // end for

  // se temp for NULL, percorri toda a lista de endereços sem conseguir criar um socket para ele
  if(temp == NULL) {
    puts("new_tcp_server: não foi possível criar servidor TCP");
    exit(EXIT_FAILURE);
  }

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
  char buffer[BUFFERSIZE]; // buffer que conterá a mensagem enviada pelo cliente
  int bytes; // retorno de recv e send

  // criando instância de epoll
  int epoll_fd = epoll_create1(0);
  if(epoll_fd == -1)
    panic("epoll_create");

  // configurando monitoramento do evento de entrada de client_fd
  struct epoll_event event = {0};
  event.events = EPOLLIN;
  event.data.fd = client_fd;

  // adicionando client_fd na lista de sockets a serem monitorados
  if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event) == -1)
    panic("epoll_ctl");

  // adquirindo endereço de IP e porta do cliente 
  char str_ip[INET6_ADDRSTRLEN];
  int port = 0;
  get_addr_and_port((struct sockaddr*) &ss, &port, str_ip, sizeof(str_ip));

  // esperando por mensagens do cliente. TIMEOUT segundos sem receber nada desconecta o cliente
  int ret = 0;
  struct epoll_event ev[1]; // vetor de eventos retornados, como monitoro apenas client_fd, o vetor tem tamanho 1
  while( (ret = epoll_wait(epoll_fd, ev, 1, TIMEOUT * 1000)) != -1 ) {
    
    // verificando se houve timeout
    if(ret == 0) {
      printf("O cliente %s:%d foi desconectado por timeout!\n", str_ip, port);
      send(client_fd, "timeout!\n", 9, MSG_DONTWAIT);
      exit(0);
    }
    
    // se o socket do cliente não sofreu nenhum evento de EPOLLIN, algum evento inesperado ocorreu,
    // fazendo com que epoll_wait retornasse
    if(!(ev[0].events & EPOLLIN))
      continue;

    // recebe mensagem do cliente
    bytes = recv(client_fd, buffer, sizeof(buffer), MSG_NOSIGNAL);
    if(bytes > 0) {
      toggle_case(buffer); // alterna o case das letras
      bytes = send(client_fd, buffer, bytes, 0); // reenvia a mensagem de volta para o cliente
      if(bytes == -1)
        panic("send");
    }
    else if(bytes == 0) {     
      printf("O cliente %s:%d desconectou!\n", str_ip, port);
      exit(0);
    }
    else
      panic("recv");
  }

  panic("epoll_wait"); // erro ao monitorar evento de entrada em client_fd
}