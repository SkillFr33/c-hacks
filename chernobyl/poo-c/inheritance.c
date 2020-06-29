#include <stdlib.h>
#include <stdio.h>

// syntax sugar
#define class typedef struct


/*
                        ATENÇÃO
  
  Compile com: 
    $ gcc inheritance.c -fms-extensions -o [BINARIO]
    $ ./[BINARIO]
*/

// Classe base/mãe
class {
  double largura;
  double altura; 
} Forma;

// Classe derivada/filha
class {
  Forma; // herda os atributos `largura` e `altura` da classe mãe
  double (*area_retangulo)(void* this); // método único da classe filha
} Retangulo;

// Implementa o método da classe filha
double area_retangulo(void* this) {
  return ((Retangulo*) this)->largura * ((Retangulo*) this)->altura;
}

int main() {

  Retangulo* retangulo = malloc(sizeof(Retangulo));
  
  // Como não criei um construtor, atribuo tudo aqui mesmo
  retangulo->area_retangulo = area_retangulo;
  retangulo->altura = 11.1;
  retangulo->largura = 23.2;

  // Imprime na tela a área do retângulo
  printf("Área do retângulo: %.2lf m²\n", retangulo->area_retangulo(retangulo));

  return EXIT_SUCCESS;
}
