#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "auto_management.h"

int main(int argc, char* argv[]) {

  int* my_intp = (int*) alloc(sizeof(int));
  *my_intp = 10;
  printf("my_intp(%p) = %d\n", my_intp, *my_intp);
  
  char* my_strp = (char*) alloc(sizeof(char) * 100);
  strcpy(my_strp, "Ola, meu nome eh Gabriel!");
  printf("my_strp(%p) = %s", my_strp, my_strp);

  // Note que não estou chamando nenhum free para desalocar a memória alocada para os ponteiros
  return 0;
}
