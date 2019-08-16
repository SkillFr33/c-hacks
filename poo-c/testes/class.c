#include "interface.h"

static Food food_array[] = {
  [MeatBalls] = {"meat balls", 10, 10, 10},
  [HoneyHam] = {"honey ham", 11, 11, 11},
  [Juice] = {"juice", 12, 12, 12},
  [CookedEggs] = {"cooked eggs", 13, 13, 13},
  [Coffee] = {"coffee", 14, 14, 14}
};

static void _sleep(Person* this, double hours) {
  this->sleep_point += (hours * 5);
  this->hunger -= (hours * 3.3);
  this->happiness += (hours * 1.2);

  printf("%s dormiu por %.1lf horas!\n", this->name, hours);
}

static void _play(Person* this, double hours) {
  this->sleep_point -= (hours * 2);
  this->hunger -= (hours * 2.2);
  this->happiness += (hours * 3.3);

  printf("%s jogou por %.1lf horas!\n", this->name, hours);
}

static void _eat(Person* this, FoodType food) {
  Food* f = &food_array[food];
  this->sleep_point += f->sleep_point;
  this->hunger += f->hunger;
  this->happiness += f->happiness;

  printf("%s consumiu %s!\n", this->name, f->food_name);
}

static void _info(const Person* const this) {
  printf(
    "Nome: %s\n"          \
    "Felicidade: %.2lf\n" \
    "Sono: %.2lf\n"       \
    "Fome: %.2lf\n",
    this->name, this->happiness, this->sleep_point, this->hunger
  );
}

static void* _new(char* const name) {
  Person* object = (Person*) malloc(sizeof(Person));
  object->eat   = _eat;
  object->sleep = _sleep;
  object->play  = _play;
  object->info  = _info;

  object->happiness = 80.0;
  object->sleep_point = 80.0;
  object->hunger = 80.0;
  object->name = name;

  return object;
}

static void* _delete(struct person* object) {
  object->play = NULL;
  object->info = NULL;
  object->eat  = NULL;
  object->info = NULL;
  
  object->happiness = 0;
  object->sleep_point = 0;
  object->hunger = 0;
  object->name = NULL;

  free(object);

  return NULL;
}

struct _person _Person = {
  .new = _new,
  .delete = _delete
};