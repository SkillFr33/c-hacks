#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h> // mkfifo
#include <fcntl.h> // O_WRONLY
#include <string.h>
#include <errno.h>

/*
  Diferente de um pipe comum, um named pipe(ou FIFO) pode ser utilizado por processos que não 
  necessariamente possuem uma relação. Isso é possível, pois um named pipe é realmente um 
  arquivo no sistema de arquivo, que pode ser aberto etc.
*/

void panic(char* err) {
  perror(err);
  exit(EXIT_FAILURE);
}

void replace_if_any(char* buffer, char old, char new) {
  char* temp = strchr(buffer, old);
  if(temp)
    *temp = new;
}

int main() {

  // crio um named pipe com as permissões 0644(dono tem permissão de 'rw' e o resto só de 'r')
  if(mkfifo("named_pipe", 0644) == -1) {
    // verifica se o erro é de "arquivo já existente". Se não for, encerra o programa
    if(errno != EEXIST)
      panic("mkfifo");
  }
  else
    printf("named pipe criado com sucesso! ");

  int fd; // file descriptor do named pipe

  // abro o named pipe recém criado. Caso nenhum processo tenha aberto o pipe para leitura (O_RDONLY), 
  // o open entrará em estado de espera
  puts("Esperando por processos leitores.");
  if((fd = open("named_pipe", O_WRONLY)) == -1)
    panic("open");
  puts("Alguém abriu o pipe para leitura!\n");

  char buffer[16]; // buffer de 16 bytes
  
  puts("Digite mensagens para ser enviadas ao pipe! (CTRL + D encerra)");
  while(1) {
    if(fgets(buffer, 16, stdin) == NULL)  
      break;
    
    replace_if_any(buffer, '\n', 0);
    write(fd, buffer, 16);
  }

  close(fd);

  return EXIT_SUCCESS;
}
