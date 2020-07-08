#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <memory.h>
#include <sys/sem.h>
#include <sys/types.h>

// Execute esse programa duas vezes para observar o resultado

/* 
  Cria/adquire um set de num_sem semáforos
  
  Essa função é importante, pois após criar um novo set é necessário inicializá-lo, e isso
  não é atômico, ou seja, essas duas operações não são uma só. Dessa forma, pode ocorrer uma
  race condition num processo que cria e em outro que adquire o semáforo, podendo ocasionar
  resultados inesperados como utilizar um semáforo não inicializado. Essa função faz isso de
  forma segura.

  key: chave para adquirir o set de samáforos
  sem_num: quantidade máxima de semáforos que o set conterá

  retorna o identificador do set
*/
int init_semaphore(key_t key, int sem_num) {
  int sem_id;

  // tento criar um set exclusivo, ou seja, se ele já existir retorna -1
  sem_id = semget(key, sem_num, IPC_CREAT | IPC_EXCL | 0666);
  if(sem_id >= 0) { // acabo de criar o set

    struct sembuf op; // struct que conterá a operação a ser realizada, no caso, incrementar o valor do semáforo em 1
    op.sem_op = 1; // incremento o valor do semáforo em 1 
    op.sem_flg = 0; // nenhuma flag

    // op.sem_num é o identificado do semáforo dentro do set
    for(op.sem_num = 0; op.sem_num < sem_num; op.sem_num++) {
      // realizo a operação e checo por erro
      if(semop(sem_id, &op, 1) == -1) {
        semctl(sem_id, 0, IPC_RMID); // removo o set
        return -1; // erro encontrado
      }
    }

  }
  else { // set já criado, espero outro processo inicializar
    // como o set já foi criado, eu abro ele sem as flags IPC_CREAT | IPC_EXCL
    sem_id = semget(key, sem_num, 0666);
    if(sem_id == -1)
      return -1; // erro ao abrir set de semáforos
    
    struct semid_ds sem;
    
    // loop infinito esperando o set ser inicializado, para isso eu verifico se o campo sem_otime é diferente de 0.
    // esse campo armazena a data do último semop
    for(;;) {
      if(semctl(sem_id, 0, IPC_STAT, &sem) == -1)
        return -1;

      if(sem.sem_otime != 0)
        break;

      usleep(500000); // aguarda 500 milissegundos
    }

  } // else

  return sem_id; // retorno o id do set caso tudo dê certo
}

void panic(const char* msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

int main() {

  key_t key = ftok("./key", 'A'); // gerando chave para identificar o semáforo
  
  // cria/inicializa o semáforo
  int sem_id = init_semaphore(key, 1);
  if(sem_id == -1)
    panic("init_semaphore");

  puts("Semáforo criado/inicializado!\n");
  
  struct sembuf op;
  op.sem_op = -1; // adquirir um semáforo (decremento seu valor)
  op.sem_num = 0; // identificador do semáforo

  puts("Tentando adquirir semáforo!");
  semop(sem_id, &op, 1);
  puts("Semáforo adquirido! Aperte ENTER para liberá-lo.");
  getchar();

  op.sem_op = 1; // libero o semáforo (incremento seu valor)
  semop(sem_id, &op, 1);
  puts("Semáforo liberado!");

  // deleto o semáforo
  semctl(sem_id, 0, IPC_RMID);

  return EXIT_SUCCESS;
}
