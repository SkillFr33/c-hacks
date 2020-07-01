#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

void panic(const char* msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

int main() {

  // configurando lock de leitura no arquivo inteiro para o processo atual
  struct flock fl = {F_RDLCK, SEEK_SET, 0, 0, getpid()};
  
  int fd = open("example.txt", O_RDONLY);
  if(fd == -1)
    panic("open");
  
  // caso o processo writer tenha adquirido o lock do arquivo, fcntl bloqueará
  // até que o lock seja liberado 
  if(fcntl(fd, F_SETLKW, &fl) == -1)
    panic("fcntl");

  puts("Lock adquirido, lendo o arquivo\n");
  
  char buffer[32];
  int bytes;

  // lê no máximo 32 bytes do arquivo
  while((bytes = read(fd, buffer, 32)) > 0)
    write(STDOUT_FILENO, buffer, bytes); // imprime os bytes lidos

  // checa por erro
  if(bytes == -1)
    panic("read");

  // libera o lock do arquivo
  fl.l_type = F_UNLCK;
  fcntl(fd, F_SETLK, &fl);

  close(fd);

  return EXIT_SUCCESS;
}