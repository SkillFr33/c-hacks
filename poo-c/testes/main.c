#include <stdlib.h>
#include <stdio.h>
#include "interface.h"

int main() {

  Person* p = (Person*) _Person.new("Gabriel");
  p->info(p);

  putchar('\n');
  p->eat(p, HoneyHam);
  putchar('\n');

  p->info(p);

  putchar('\n');
  p->play(p, 10);
  putchar('\n');

  p->info(p);

  p = _Person.delete(p);

  return EXIT_SUCCESS;
}
