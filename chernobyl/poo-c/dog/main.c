#include <stdlib.h>
#include <stdio.h>
#include "dog_interface.h"

/*
  compile com:
  $ gcc main.c dog_class.c -o main
*/

int main() {

  // instanciando objeto
  dog* dog = Dog.new("Cachorro", 12.33);
  
  dog->latir(dog);

  dog->set_nome(dog, "\0");
  dog->latir(dog);
  
  dog->set_nome(dog, "Xer");
  dog->latir(dog);
  
  dog->set_peso(dog, 0);

  printf("%s está com %.2lf quilos!\n", dog->get_nome(dog), dog->get_peso(dog));

  // liberando memória do objeto
  dog = Dog.delete(dog);

  return EXIT_SUCCESS;
}