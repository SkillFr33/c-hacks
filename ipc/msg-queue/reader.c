#include <stdlib.h>
#include <errno.h>
#include <sys/msg.h>
#include <stdio.h>
#include "person_msgbuf.h"

int main() {

  // gerando uma key única baseada nos metadados do arquivo e num inteiro positivo
  key_t key = ftok("./msg", 1);
  int msg_id = msgget(key, 0666); // abre a fila de mensagens com permissões 0666

  if(msg_id == -1)
    panic("msgget");

  // estrutura que armazenará a mensagem recebida
  struct person_msgbuf person;

  while(1) {
    // recebe a mensagem e trata possíveis erros
    if(msgrcv(msg_id, &person, sizeof(struct person_info), 0, 0) == -1)
      // se o erro for EIDRM significa que o writer removeu a fila de mensagens
      if(errno == EIDRM) {
        puts("Message queue removida do sistema! Não há mais nada para ser lido.");
        break;
      }
      else
        panic("msgrcv");

    puts("Mensagem recebida:");
    printf(
      "  Nome: %s\n"     \
      "  Idade: %d\n"      \
      "  Gênero: %c\n"   \
      "  Peso: %.2lf\n\n",
      person.info.name, 
      person.info.age, 
      person.info.gender, 
      person.info.weight
    );
  }

  return EXIT_SUCCESS;
}