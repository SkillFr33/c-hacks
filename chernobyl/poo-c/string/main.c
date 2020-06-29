#include <stdlib.h>
#include <stdio.h>
#include "string.h"

int main() {

  string str = String.empty();
  string str2 = String.new("Oi, tudo bem?");

  str->copy(str, str2);

  printf("str = %p\n", str->get_string(str));
  printf("str2 = %p\n", str2->get_string(str2));

  puts(str->get_string(str));
  puts(str2->get_string(str2));

  return EXIT_SUCCESS;
}
