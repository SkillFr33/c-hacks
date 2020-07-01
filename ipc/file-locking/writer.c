#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

void panic(const char* msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

int main() {

  struct flock fl = {
    .l_type   = F_WRLCK,  // tipo do lock, no caso, pra escrita
    .l_whence = SEEK_SET, // irei dar lock a partir do inicio do arquivo
    .l_start  = 0,        // offset pra região que sofrerá o lock, ou seja, a partir de 'l_whence + l_start'
    .l_len    = 0,        // tamanho (em bytes) da região que sofrerá o lock. 0 = até EOF, ou seja, até o final
    .l_pid    = getpid()  // pid do processo que adquiriu o lock
  };

  // abre o arquivo; se não existir, cria com as permissões 0644 ('rw' para o dono e 'r' para o resto)
  int fd = open("example.txt", O_WRONLY | O_CREAT, 0644);
  if(fd == -1)
    panic("open");
  
  // se algum outro processo (writer ou reader) tiver adquirido o lock antes, a função fcntl
  // bloqueará até que o lock seja liberado
  if(fcntl(fd, F_SETLKW, &fl) == -1)
    panic("fcntl");

  puts("Lock adquirido, escreva as mensagens para inserir no arquivo! (CTRL + D encerra)\n");
  
  char buffer[32];
  // lê até 32 bytes da entrada padrão
  while(fgets(buffer, 32, stdin) != NULL)
    if(write(fd, buffer, strlen(buffer)) == -1) // escreve os bytes lidos no arquivo
      panic("write");

  // libera o lock
  fl.l_type = F_UNLCK; // muda o tipo para F_UNLCK
  fcntl(fd, F_SETLK, &fl); // observe que agora foi utilizado F_SETLK e não F_SETLKW

  close(fd);

  return EXIT_SUCCESS;
}