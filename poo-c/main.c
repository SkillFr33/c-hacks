#include <stdlib.h>
#include <stdio.h>
#include "interface.h"

int main() {

  struct point* p = Point.new();
  p->setX(p, 10);   // configurando valores do membro de dado x
  p->setY(p, 12.5); // configurando valores do membro de dado y

  printf("p.x = %.2lf\n", p->getX(p)); // imprimindo membro de dado x com a função-membro getX 
  printf("p.y = %.2lf\n", p->getY(p)); // imprimindo membro de dado y com a função-membro getY

  p->printPoint(p); // imprimindo ponto no formato (x, y) com a função-membro printPoint 

  /* 
    A chamada de função abaixo gera um erro ao compilar(referência p/ printPoint não definida), 
    pois se trata de uma função static que só pode ser acessada indiretamente através de um objeto 
    de Point (aka: struct point) ou diretamente apenas dentro do arquivo class.c
  */
  //printPoint(p); // referência não definida para printPoint

  Point.delete(&p);
  // ou: p = Point.delete(&p);

  return 0;
}
