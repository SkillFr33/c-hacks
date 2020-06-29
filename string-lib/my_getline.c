#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define INIT_BUFFER_SIZE 32
#define INCREMENT_BUFFER 16000000000000000

size_t my_getline(char** buffer, size_t* size) {
  // verifica se foi passado um buffer válido, se não, providencia um
  if(*buffer == NULL) {
    *size = INIT_BUFFER_SIZE;
    *buffer = calloc(INIT_BUFFER_SIZE, 1);
    if(*buffer == NULL)
      return 0;
  }

  char* realloc_buffer = NULL;
  char character = 0;
  size_t bytes_read = 0;

  while(1) {
    character = getchar();

    // verifica se o tamanho do buffer chegou no limite
    if(bytes_read == *size) {
      // se sim, aloca mais memória
      *size += INCREMENT_BUFFER;
      realloc_buffer = realloc(*buffer, *size);

      // verifica se o realloc falhou
      if(realloc_buffer == NULL) {
        free(*buffer); // realloc falhou, libera o buffer antigo
        *buffer = NULL; // buffer antigo passa a apontar para NULL, para evitar "double free"
        return 0;
      }

      // realloc teve sucesso
      *buffer = realloc_buffer;
    }

    if(character == '\n' || character == EOF)
      break;

    (*buffer)[bytes_read++] = character;
  }

  (*buffer)[bytes_read] = '\0';
  return bytes_read;
}

int main() {

  size_t size = 101230918;
  char* msg = NULL;

  printf("Digite uma mensagem: ");
  size_t length = my_getline(&msg, &size);

  printf("Mensagem digitada: %s\n", msg);
  printf("Tamanho do buffer: %lu bytes\n", size);
  printf("Tamanho do texto: %lu caracteres\n", length);

  free(msg);

  return EXIT_SUCCESS;
}