#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>

void panic(const char* msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

void replace(char* buffer, char old, char new) {
  char* temp = strchr(buffer, old);
  if(temp)
    *temp = new;
}

int main() {

  // cria o socket local
  int server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if(server_fd == -1)
    panic("socket");

  // endereço do socket
  struct sockaddr_un server;
  server.sun_family = AF_UNIX;
  strcpy(server.sun_path, "./socket_path");

  // tenta estabelecer uma conexão entre o socket e o endereço contido em 'server'
  if(connect(server_fd, (struct sockaddr*) &server, sizeof(server)) == -1)
    panic("connect");

  puts("Conectado! Digite as mensagens ou CTRL+D para encerrar!");

  char buffer[1024];
  // lê um texto da entrada padrão
  while(fgets(buffer, sizeof(buffer), stdin) != NULL) {
    replace(buffer, '\n', '\0'); // substitui a quebra de linha por um null terminator

    // envia para o socket a mensagem contida em buffer
    int bytes = send(server_fd, buffer, strlen(buffer), MSG_NOSIGNAL);
    if(bytes == -1)
      panic("send");
  }

  puts("\nDesconectando!");
  close(server_fd); // fecha a conexão, recv do server retorna 0

  return EXIT_SUCCESS;
}
