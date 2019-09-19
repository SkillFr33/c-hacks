#include <stdlib.h>
#include <stdio.h>
#include "string.h"

int main() {

  String* str = new_string("oi");
  str->info(str);

  str->setString(str, "Meu nome eh Gabriel Mendes");
  str->info(str);
  
  str->setString(str, "asdfasdfasdf");
  str->info(str);

  str = delete_string(str);

  return 0;
}
