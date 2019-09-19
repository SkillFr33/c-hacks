#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "string.h"

// Função que pega o header de um objeto de String. No header possui os membros private
static Header* get_header(String* object) {
  return (Header*) ((char*) object - sizeof(Header));
}

// função que retorna o tamanho da string
static size_t __length(void* object) {
  Header* header = get_header(object);
  return header->__size;
}

static bool __isEmpty(void* object) {
  Header* header = get_header(object);
  return header->__size == 0;
}

// Função que configura a string
static bool __setString(struct _string* object, char* new_str) {
  Header* header = get_header(object);
  memset(object->ptr, 0, header->__size + 1); // zera o conteúdo da minha string

  // se a nova string for NULL
  if(new_str == NULL){
    header->__size = 0; // tamanho da string vai pra 0
    return 1; // sucesso
  }
  else {
    size_t new_size = strlen(new_str); // pega

    if(new_size > header->__size)
      object->ptr = realloc(object->ptr, new_size + 1);
    
    header->__size = new_size;
    memcpy(object->ptr, new_str, new_size);
    return 1; // sucesso
  }

  return 0; // falha
}

static void __info(struct _string* object) {
  Header* header = get_header(object);

  printf(          \
    "=======| Info |=======\n" \
    "String: %s\n" \
    "Size: %ld\n\n"  \
  , object->ptr, header->__size);
}

String* new_string(char* str) {

  // pegando tamanho da string. Se str for NULL, o tamanhao será 0
  size_t size = str ? strlen(str) : 0;

  Header* header = (Header*) malloc(sizeof(Header) + sizeof(String));
  header->__size = size;

  // deslocando para o byte que inicia o objeto de string
  String* string = (String*) ((char*) header + sizeof(Header));

  // configurando os membros public
  string->ptr = (char*) malloc(sizeof(char) * size + 1);
  string->length = __length;
  string->isEmpty = __isEmpty;
  string->setString = __setString;
  string->info = __info;

  // Copiando str para string->str. Se str for NULL, string->ptr apontará para um '\0'
  if(str != NULL)
    memcpy(string->ptr, str, size);
  else
    string->ptr[0] = '\0';

  return string;
}