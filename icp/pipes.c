#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

void panic(char* err) {
  perror(err);
  exit(EXIT_FAILURE);
}

int main() {

  char buffer[1024];
  int bytes;

  int fd[2];
  if(pipe(fd) == -1)
    panic("pipe");

  pid_t pid = fork();
  if(pid == 0) {
    // child
    printf("Frase: ");
    scanf("%1023[^\n]", buffer);

    bytes = strlen(buffer);
    write(fd[1], buffer, bytes + 1);
  }
  else if(pid > 0){
    // parent

    // read bloqueia até que alguma informação seja enviada pipe
    if((bytes = read(fd[0], buffer, 1024)) > 0) {
      for(int x = 0; x < bytes - 1; x++)
        printf("%c\n", buffer[x]);

    }
    else if(bytes == -1)
      panic("read");
  }
  else
    panic("fork");

  return EXIT_SUCCESS;
}
