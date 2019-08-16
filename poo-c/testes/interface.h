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

// Struct que representa uma comida, contendo seus atributos
typedef struct food {
  const char* food_name;
  double sleep_point; 
  double happiness;
  double hunger;
} Food;

/* ------------------------------------------------------------------------- */

// Struct que representa uma pessoa
typedef struct person Person;
struct person {
  // funções-membro
  void (*sleep)(void* const _this, double hours);
  void (*play)(void* const _this, double hours);
  void (*eat)(void* const _this, FoodType food);
  void (*info)(const void* const);

  // membros de dado
  const char* name;
  double sleep_point;
  double hunger;
  double happiness;

};

// struct que contém alguns ponteiros para funções utilitárias da classe,
// como o construtor e destrutor
typedef struct _person _person;
struct _person {
  void* (*new)(char* const name);
  void* (*delete)(void* _object);
};

/* ------------------------------------------------------------------------- */

extern _person _Person;

#endif