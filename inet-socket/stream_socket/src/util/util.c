#include <stdlib.h>
#include <sys/wait.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <sys/epoll.h>
#include <ctype.h>
#include <errno.h>
#include "./util.h"

/*
  Funções utilitárias
*/

void panic(const char* func_name) {
  perror(func_name);
  exit(EXIT_FAILURE);
}

void net_panic(const char* func_name, int err) {
  fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(err));
  exit(EXIT_FAILURE);
}

// handler de SIGCHLD
void sigchld_handler(int sig) {
  // Recebe o SIGCHLD do processo filho, evitando que ele se torne zumbi
  // O while evita que SIGCHLDs sejam ignorados caso eles cheguem enquanto outro
  // estiver sendo tratado. Se waitpid retornar 0, significa que não há mais nenhum
  // SIGCHLD a ser tratado nesse momento.
  while(waitpid(-1, NULL, WNOHANG) > 0);
}

void toggle_case(char* buffer) {
  while(*buffer) {
    if(isalpha(*buffer))
      *buffer ^= 32;
    buffer++;
  }
}

void replace(char* buffer, char old, char new) {
  char* temp;
  temp = strchr(buffer, old);
  if(temp)
    *temp = new;
}

int get_fd_by_event(struct epoll_event* ev, size_t ev_size, int event) {
  int i;
  for(i = 0; i < ev_size; i++)
    if(ev[i].events & event)
      return ev[i].data.fd; // retorna fd que sofreu 'event'
  
  // evento inesperado
  return -1;
}

int send_all(int fd, const char* buffer, size_t len) {
  const char* ptr = buffer; // ponteiro para o buffer. Esse ponteiro será deslocado

  // enquanto o tamanho do buffer for maior que zero, significa que ainda há bytes a serem enviados
  while(len > 0) {
    int bytes_sent = send(fd, ptr, len, MSG_NOSIGNAL);
    
    // checando por interrupção
    if(errno == EINTR)
      continue;
    else if(bytes_sent == -1)
      return -1;

    ptr += bytes_sent; // desloca ptr pela quantidade de bytes enviados
    len -= bytes_sent; // decrementa o tamanho do buffer
  }

  return 0;
}