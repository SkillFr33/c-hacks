#include <stdlib.h>
#include <stdio.h>
#include "string.h"

int main() {

  string str = String.empty();
  string str2 = String.new("Oi, tudo bem?");

  str->copy(str, str2);

  printf("str = %p\n", str->get_string(str));
  printf("str2 = %p\n", str2->get_string(str2));

  printf("%s | size = %lu\n\n", str->get_string(str), str->get_size(str));
  printf("%s | size = %lu\n", str2->get_string(str2), str2->get_size(str2));

  str2->set_string(str2, "oi, possuo outro conteúdo agora!");
  printf("%s | size = %lu\n", str2->get_string(str2), str2->get_size(str2));

  String.delete(str);
  String.delete(str2);

  return EXIT_SUCCESS;
}
