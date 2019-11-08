#include <stdlib.h>
#include <setjmp.h>
#include <stdio.h>

/*
  Compile com: gcc -m32 exception.c -o main
*/

// jmp_buf para armazenar o contexto atual do ambiente
jmp_buf environment;

int main(int argc, char* argv[]) {

  // salva o contexto atual do programa (stack, instruction pointer etc)
  int ret_value = setjmp(environment);

  // try
  if(!ret_value) {

    double a, b;
    printf("Digite dois números para realizar uma divisão: ");
    scanf("%lf %lf", &a, &b);

    if(b == 0) {
      // throw exception. Retorna o contexto do programa quando a linha 15 estava sendo executada
      longjmp(environment, (int) "Exception: division by 0!");
    }

    printf("O resultado da divisão é: %.2lf\n", a / b);

  }
  // catch
  else { 
    printf("ERROR  |  %s\n", (char*) ret_value);
  }

  return EXIT_SUCCESS;
}