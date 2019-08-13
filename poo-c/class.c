#include <stdlib.h>
#include <stdio.h>
#include "interface.h"

// funções globais para a construção e destrução do objeto

static struct point* _point_constructor();
static void* _point_destructor(struct point** object);

// struct _point (não confunda com struct point) que contém o operador new e delete da classe
struct _point Point = {
  .new = _point_constructor,
  .delete = _point_destructor
};

// funções static que serão as funções-membro da clase:
static void _setX(struct point* const this, const int _x) {
  this->x = _x;
}

static void _setY(struct point* const this, const int _y) {
  this->y = _y;
}

static double _getX(const struct point* const this) {
  return this->x;
}

static double _getY(const struct point* const this) {
  return this->y;
}

static void _printPoint(const struct point* const this) {
  printf("(%.2lf, %.2lf)\n", this->x, this->y);
}

static struct point* _point_constructor() {
  struct point* _my_point = (struct point*) malloc( sizeof(struct point) );
  
  if(_my_point != NULL){
    // inicializando membros de dado:
    _my_point->x = 0.0;
    _my_point->y = 0.0;

    // fazendo ponteiros de função apontarem para as funções static (funções-membro)
    _my_point->setX = _setX;
    _my_point->setY = _setY;
    
    _my_point->getX = _getX;
    _my_point->getY = _getY;

    _my_point->printPoint = _printPoint;
  }
    
  return _my_point;
}

static void* _point_destructor(struct point** object) {
  if(*object == NULL)
    return NULL;
  
  (*object)->x = 0;
  (*object)->y = 0;

  (*object)->getX = NULL;
  (*object)->getY = NULL;
  (*object)->setX = NULL;
  (*object)->setY = NULL;

  free(*object);
  *object = NULL;
  return NULL;
}