#include <stdlib.h>
#include <stdio.h>
#include <string.h>

size_t my_strspn(const char* restrict str, const char* restrict match) {
  
  // quantidade de caracteres lidos de 'str' até que não seja mais compatível
  // com nenhum caractere de 'match'.
  size_t len = 0;

  // ponteiro temporário para match para que eu possa percorrer a string
  // incrementanto o ponteiro sem alterar o ponteiro original.
  const char* temp_match = match;

  // Percorro 'str' por completo ou até que algum caractere de
  // 'str' não seja compatível com nenhum caractere de 'match'
  for(; *str; str++) {

    // verifica *str com cada caractere de 'temp_match'
    for(; *temp_match; temp_match++) {
      if(*str == *temp_match) {
        ++len;
        break;
      }
    } // for *temp_match

    // se 'temp_match' apontar para '\0', significa que eu percorri toda 
    // a string e nenhum de seus caracteres era compatível com o caractere
    // atualmente apontado por 'str'
    if(*temp_match == '\0')
      return len; // retorno a quantidade de caracteres de 'str' que foram percorridos

    // 'temp_match' aponta novamente para o inicio da string apontada por 'match'
    temp_match = match;
  } // for *str

  // NÃO DEVE CHEGAR AQUI
  return 0;
}


int main(int argc, char* argv[]) {

  size_t x = my_strspn("123456789","42531");
  printf("%ld\n", x); 

  return EXIT_SUCCESS;
}