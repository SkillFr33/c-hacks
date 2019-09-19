#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "string.h"

/*
 * Funções que iniciam com __ são funções que serão apontadas pelos ponteiros
 * para função da classe, dessa forma, representam os métodos da classe. 
*/


// Função que pega o header de um objeto de String. No header possui os membros private
static Header* get_header(String* this) {
  return (Header*) ((char*) this - sizeof(Header));
}

// função que retorna o tamanho da string
static size_t __length(void* this) {
  Header* header = get_header(this);
  return header->__size;
}

static bool __isEmpty(void* this) {
  Header* header = get_header(this);
  return header->__size == 0;
}

// Função que configura a string
static bool __setString(struct _string* this, char* new_str) {
  Header* header = get_header(this);
  memset(this->ptr, 0, header->__size + 1); // zera o conteúdo da minha string

  // se a nova string for NULL
  if(new_str == NULL){
    header->__size = 0; // tamanho da string vai pra 0
    return 1; // sucesso
  }
  else {
    size_t new_size = strlen(new_str); // pega

    if(new_size > header->__size)
      this->ptr = realloc(this->ptr, new_size + 1);
    
    header->__size = new_size;
    memcpy(this->ptr, new_str, new_size);
    return 1; // sucesso
  }

  return 0; // falha
}

static void __info(struct _string* this) {
  Header* header = get_header(this);

  printf(          \
    "=======| Info |=======\n" \
    "String: %s\n" \
    "Size: %ld\n\n"  \
  , this->ptr, header->__size);
}

static bool 
__copy_string(struct _string* this, struct _string* other) {
  return this->setString(this, other->ptr);
}


// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

// Construtor
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
  string->copy_string = __copy_string;

  // Copiando str para string->str. Se str for NULL, string->ptr apontará para um '\0'
  if(str != NULL)
    memcpy(string->ptr, str, size);
  else
    string->ptr[0] = '\0';

  return string;
}

// Destrutor
void* delete_string(struct _string* this) {
  if(this == NULL)
    return NULL;

  Header* header = get_header(this);

  // Limpando membros public e private
  memset(this->ptr, 0, header->__size);
  header->__size = 0;
  free(this->ptr);

  // Configurando ponteiros para função para apontarem para NULL
  this->info = NULL;
  this->isEmpty = NULL;
  this->length = NULL;
  this->setString = NULL;

  free(header);

  return NULL;
}