#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/shm.h>

#define SHM_SIZE 1024

void panic(const char* msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

int main() {

  // chave para adquirir/criar o segmento de memória
  key_t key = ftok("./key", 'B');

  // adquire o identificador de um segmento de memória com base em key de tamanho SHM_SIZE com as permissões 0644 (-rw-r--r--)
  int shm_id = shmget(key, SHM_SIZE, IPC_CREAT | 0644);
  if(shm_id == -1)
    panic("shmget");

  // adquire um ponteiro para a região de memória compartilhada
  char* shm_ptr = shmat(shm_id, 0, 0);
  
  // verifica se foi possível adquirir o ponteiro
  if(shm_ptr == (char*) -1)
    panic("shmat");

  char buffer[SHM_SIZE];
  char* temp;

  printf("Digite um texto: ");
  fgets(buffer, SHM_SIZE, stdin);

  // caso tenha um \n, substitui por \0
  if((temp = strchr(buffer, '\n')) != NULL)
    *temp = '\0';
  
  // copia o texto digitado para o segmento
  strncpy(shm_ptr, buffer, SHM_SIZE);

  puts("Digite ENTER para desanexar o segmento e excluí-lo!");
  getchar();

  shmdt(shm_ptr); // desanexa o ponteiro para o segmento de memória compartilhada
  shmctl(shm_id, IPC_RMID, 0); // marca o segmento para ser excluído. Ele só é excluído caso ninguém esteja anexado a ele 

  return EXIT_SUCCESS;
}