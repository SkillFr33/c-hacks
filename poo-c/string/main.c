#include <stdlib.h>
#include <stdio.h>
#include "string.h"

int main() {

  string str = String.empty();

  if(not str->at(str, 0))
    puts("String vazia!");
  else
    printf("A string é '%s'\n", str->get_string(str));


  string str2 = String.new("Ola, meu nome eh Gabriel!");
  printf("A string é '%s' e tem %ld letras\n", str2->get_string(str2), str->get_size(str2));
  str2->set_string(str2, "Agora essa eh a nova string do objeto!");
  printf("A string é '%s' e tem %ld letras\n", str2->get_string(str2), str->get_size(str2));
  
  int index = 0;
  printf("Esse é o caractere na posição %d da minha string: '%c'\n", index, str2->at(str2, index));
  printf("Esse é o caractere na posição %d da minha string: '%c'\n", index + 2, str2->at(str2, index + 2));
  printf("Esse é o último caractere da minhas string: '%c'\n", str2->at(str2, str2->get_size(str2) - 1));

  str2->clear(str2);
  printf("Essa é minha string após do clear: '%s'\n", str2->get_string(str2));
  printf("Como você pode ver, ela foi zerada e agora possui %ld caracter(es)\n", str2->get_size(str2));

  str = String.delete(str);
  str2 = String.delete(str2);

  return EXIT_SUCCESS;
}
