#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char* my_strstr(const char* restrict str, const char* restrict search_str) {
  for(; *str; *str++) {
    if(*str == *search_str) {

      const char* temp_str = str + 1;
      const char* temp_search_str = search_str + 1;
      
      while(*temp_search_str == *temp_str && 
            *temp_search_str)
      {
        temp_search_str++;  
        temp_str++;  
      }

      if(*temp_search_str == '\0')
        return (char*) str;
    } // if

  } // for

  return NULL;
}

int main(int argc, char* argv[]) {
  
  const char* text = "Ola, esse eh um texto de teste para que eu possa pesquisar uma string dentro dele";
  const char* search_string = "para qu";

  const char* result = strstr(text, search_string);
  if(result)
    printf("Texto encontrado a partir da posição %ld do texto!\n\"%s\"\n", (long)(result - text), result);
  else
    puts("Texto não encontrado!");

  return 0;
}