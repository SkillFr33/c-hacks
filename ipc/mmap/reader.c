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
  int fd = open("./file.txt", O_RDONLY);
  if(fd == -1)
    panic("open");

  // armazena em buf alguns metadados do arquivo (como o tamanho)
  struct stat buf;
  if(fstat(fd, &buf) == -1)
    panic("fstat");

  // mapeia o arquivo somente para leitura no espaço de endereçamento virtual do processo
  char* data = mmap(NULL, buf.st_size, PROT_READ, MAP_SHARED, fd, 0);
  if(data == (char*) -1)
    panic("mmap");

  printf("Conteúdo: \"%s\"\n", data);

  // >>> A LINHA ABAIXO CAUSA UMA FALHA DE SEGMENTAÇÃO <<<
  // memcpy(data, "Tentando usar o memcpy com um arquivo mapeado apenas para leitura", 65);

  munmap(data, buf.st_size);

  return EXIT_SUCCESS;
}