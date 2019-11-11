#include <stdlib.h>
#include <stdio.h>
#include <string.h> // strlen

#define INIT_BUFFER_SIZE 32 // 32 bytes
#define INCREMENT_BUFFER 16 // 16 bytes
#define true 1

char* my_getline(void);

int main() {

  char* msg = NULL;

  printf("Digite uma mensagem: ");
  msg = my_getline();
  printf("Mensagem digitada: \"%s\"\n", msg);
  printf("Tamanho: %ld\n", strlen(msg));

  free(msg);

  return EXIT_SUCCESS;
}

char* my_getline(void) {

  char* buffer = NULL;
  size_t bytes = INIT_BUFFER_SIZE;

  // alocando INIT_BUFFER_SIZE bytes para o buffer
  buffer = (char*) malloc(bytes);
  if(!buffer) {
    fprintf(stderr, "Memória insuficiente! Retornando NULL.\n");
    return NULL;
  }

  char character = '\0';
  size_t position = 0;

  while(true) {
    character = getchar();
    
    if(position == bytes) {
      //puts("realloc");
      bytes += INCREMENT_BUFFER;
      buffer = realloc(buffer, bytes);
      if(!buffer) {
        fprintf(stderr, "Memória insuficiente! Retornando NULL.\n");
        return NULL;
      }
    }

    if(character == EOF || character == '\n') {
      buffer[position] = '\0';
      return buffer;
    }
    else {
      buffer[position] = character;
      ++position;
    }
  }

  return NULL; // nunca deve bater aqui!
}
