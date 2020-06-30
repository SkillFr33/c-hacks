#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h> // O_RDONLY e O_NDELAY

/*
  Lê o conteúdo do named pipe
*/

void panic(char* err) {
  perror(err);
  exit(EXIT_FAILURE);
}

int main() {

  int fd; // file descriptor do named pipe

  // abro o named pipe criado por main.c em modo leitura. Se nenhum processo estiver usando o pipe em modo escrita (O_WRONLY),
  // o open entrará em estado de espera
  if((fd = open("named_pipe", O_RDONLY)) == -1)
    panic("open");

  int bytes; // bytes lidos por read
  char buffer[16]; // buffer para armazenar as mensagens

  puts("Aguardando por mensagens!\n");
  while((bytes = read(fd, buffer, 16)) > 0)
    printf("Mensagem recebida: %s\n", buffer);

  if(bytes == -1)
    panic("read");
  else
    puts("\nNão há mais ninguém escrevendo no pipe!");

  close(fd);

  return EXIT_SUCCESS;
}