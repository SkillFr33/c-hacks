#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/msg.h>
#include "person_msgbuf.h"

int main() {

  // gerando uma key única baseada nos metadados do arquivo e num inteiro positivo
  key_t key = ftok("./msg", 1);

  int msq_id = msgget(key, IPC_CREAT | 0666); // abre/cria a fila de mensagens com permissões 0666

  if(msq_id == -1)
    panic("msgget");

  struct person_msgbuf person;
  person.mtype = 1; // tipo da mensagem, isso não importa mt nesse caso

  while(1) {
    printf("\nNome(CTRL + D para sair): ");
    if(fgets(person.info.name, 32, stdin) == NULL)
      break;

    // verifica se tem \n no nome e substitui por \0
    char* temp = strchr(person.info.name, '\n');
    if(temp) *temp = '\0';

    printf("Idade: ");
    scanf(" %d", &person.info.age);

    printf("Gênero: ");
    scanf(" %c", &person.info.gender);

    printf("Peso: ");
    scanf(" %lf", &person.info.weight);
    getchar();

    // enviando a mensagem para a fila
    if(msgsnd(msq_id, &person, sizeof(struct person_info), 0) == -1)
      panic("msgsnd");
  }

  putchar(10);

  // removendo a fila de mensagens do sistema
  if(msgctl(msq_id, IPC_RMID, NULL) == -1)
    panic("msgctl");

  return EXIT_SUCCESS;
}