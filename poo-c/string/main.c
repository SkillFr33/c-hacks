#include <stdlib.h>
#include <stdio.h>
#include "string.h"

int main() {

  String* str = new_string("Olá, mundo!");
  puts(str->ptr);
  printf("%ld\n", str->length(str));

}
