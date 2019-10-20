#include <stdlib.h>
#include <stdio.h>
#include "string.h"

int main() {

  string str = String.empty();

  if(str->at(str, 0) == '\0') {
    puts("VAZIO");
  }

  str = String.delete(str);

  return EXIT_SUCCESS;
}
