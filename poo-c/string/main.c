#include <stdlib.h>
#include <stdio.h>
#include "string.h"

int main() {

  String* str = new_string("oi");
  String* str2 = new_string("Ola");

  str->info(str);
  str->info(str2);

  str->copy_string(str, str2);
  str2->setString(str2, "eita, oi!?");

  str->info(str);
  str->info(str2);

  str = delete_string(str);
  str2 = delete_string(str2);

  return 0;
}
