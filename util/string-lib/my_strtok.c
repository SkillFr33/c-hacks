#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/*
  Implementação própria da função strtok, que faz um parser em uma
  'str' de acordo com os caracteres delimitadores passados em 'delim'.

  string = "banana,melancia,goiaba"
  my_strtok(string, ",") -> separa as palavras pela vírgula(,)

  Resultado:
    "banana"
    "melancia"
    "goiaba"

*/
char* my_strtok(char* str, char* delim) {

  // ponteiro static que apontará para a string passada na primeira
  // chamada dessa função, isso permite o famoso my_strtok(NULL, "xyz");
  static char* static_string = NULL;

  // quantos caracteres devem ser deslocados. Inicialmente nenhum, pois
  // é necessário que static_string inicie no primeiro char de 'str' 
  static long  offset        = 0;

  // configura a static_string
  if(str) {
    static_string = str;
    offset = 0;
  }
  else
    static_string += offset + 1;

  char* temp_str = static_string;

  for(; *temp_str; *temp_str++) {

    // ponteiro temporário para a string delimitadora dessa forma 
    // consigo utilizar aritmética de ptr se deslocar o ponteiro
    // principal, ou seja, o ponteiro 'delim'.
    char* temp_delim = delim;
    
    // percorre todos os caracteres da string delimitadora e verifica
    // se o caractere atual da string principal é algum caractere 
    // delimitador
    for(; *temp_delim; *temp_delim++)
      if(*temp_str == *temp_delim) // se for, encerra o loop
        break;
    
    // se temp_delim ainda apontar para um caractere válido (!= '\0')
    // significa que o loop foi encerrado antes de percorrer todos os
    // caracteres da string delimitadora. Isso significa que o loop foi
    // encerrado prematuramente.
    if(*temp_delim) {
      // posição onde algum caractere de 'delim' foi encontrado
      offset = (long) (temp_str - static_string);
      static_string[offset] = '\0'; // substitui o caractere por um NUL
      return static_string;
    }

  } // for

  // retorna NULL caso não há como realizar o parser da string, ou seja,
  // retornar algum token.

  if(offset != 0) {
    *temp_str = '\0';
    offset = 0;
    return static_string;
  }
  else
    return NULL;
}

int main(int argc, char* argv[]) {

  char teste[] = "laranja";


  char* token = strtok(teste, ",");
  while(token) {
    puts(token);
    token = strtok(NULL, ",");
  }

  return EXIT_SUCCESS;
}
