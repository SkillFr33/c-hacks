#ifndef _PERSON_INTERFACE_H
#define _PERSON_INTERFACE_H

#include <stdlib.h>
#include <stdio.h>

// Enum pra indexar os tipos de comida no array (ver class.c) 
typedef enum {
  MeatBalls = 0,
  HoneyHam,
  Juice,
  CookedEggs,
  Coffee
} FoodType;

// Struct que representa uma comidam, contendo seus atributos
typedef struct food {
  char*  food_name;
  double sleep_point; 
  double happiness;
  double hunger;
} Food;

/* ------------------------------------------------------------------------- */

// Struct que representa uma pessoa
typedef struct person {
  // funções-membro
  void (*sleep)(struct person* const this, double hours);
  void (*play)(struct person* const this, double hours);
  void (*eat)(struct person* const this, FoodType food);
  void (*info)(const struct person* const);

  // membros de dado
  char*  name;
  double sleep_point;
  double hunger;
  double happiness;

} Person;

typedef struct _person {
  void* (*new)(char* const name);
  void* (*delete)(void* this);
} _person;

extern _person _Person; // _Person vem de class.c

/* ------------------------------------------------------------------------- */

#endif