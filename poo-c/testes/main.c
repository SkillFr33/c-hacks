#include <stdlib.h>
#include <stdio.h>
#include "interface.h"

// dps vou por isso em _Person, por enquanto vai ficar como extern :]
extern void* new(char* const this);
extern void _info(const struct person* const this);

int main() {

  Person* p = (Person*) new("Gabriel");
  _info(p);

  putchar('\n');
  p->eat(p, HoneyHam);
  putchar('\n');

  _info(p);

  putchar('\n');
  p->play(p, 10);
  putchar('\n');

  _info(p);

  return EXIT_SUCCESS;
}
