#ifndef _AUTO_MEMORY_MANAGEMENT_H_
#define _AUTO_MEMORY_MANAGEMENT_H_

// posso alocar no máximo MAX_POINTERS objetos na heap
#define MAX_POINTERS 10

// meu próprio main. Ver função-macro 'main'
int my_main(int argc, char* argv[]);

// função para alocar memória, basicamente é uma malloc com algumas coisas adicionais
void* alloc(size_t bytes);

// função que será responsável por desalocar a memória alocada no final do programa
void free_memory();

// Essa função-macro substitui o main que eu digitar no main.c
// por esse main já pré-definido com alguns comandos, como o atexit
// e o return my_main. A última linha dessa função macro é a 'minha main'
// que será montada.
#define main(...)               \
main(int argc, char* argv[]) {  \
  atexit(free_memory);          \
  return my_main(argc, argv);   \
}                               \
int my_main(__VA_ARGS__)

/* 
  Exemplo de código:

  // main.c
  int main(int argc, char* argv[]) {
    puts("Olá mundo!");

    return 100;
  }

  Quando a função-macro 'main' for expandida, vai gerar o seguinte código:

  int main(int argc, char* argv[]) {
    atexit(free_memory);
    return my_main(argc, argv); // o 'main' original chama my_main
  }
  int my_main(int argc, char* argv[]) {
    puts("Olá mundo!");
  
    return 100;
  }

  Note que tudo digitado dentro do primeiro main(que é a função-macro) vai parar dentro de
  my_main, que é chamado pelo main original.

*/

#endif // _AUTO_MEMORY_MANAGEMENT_H_