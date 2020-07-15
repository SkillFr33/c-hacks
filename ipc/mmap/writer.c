#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <memory.h>
#include <sys/mman.h>
#include <sys/stat.h>

void panic(const char* msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

int main() {
  const long pagesize = sysconf(_SC_PAGESIZE);

  // abre o arquivo com permissão de leitura e escrita
  int fd = open("./file.txt", O_RDWR);
  if(fd == -1)
    panic("open");

  // armazena em buf alguns metadados do arquivo (como o tamanho)
  struct stat buf;
  if(fstat(fd, &buf) == -1)
    panic("fstat");

  // mapeia o arquivo no espaço de endereçamento virtual do processo
  char* data = mmap(NULL, buf.st_size, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
  if(data == (char*) -1)
    panic("mmap");

  printf("Conteúdo do arquivo antes da alteração: \"%s\"\n", data);
  memcpy(data, "esse arquivo de teste é :)", 27);
  puts("Arquivo alterado!");

  munmap(data, buf.st_size);

  return EXIT_SUCCESS;
}