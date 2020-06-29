#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include "string.h"

/*
  ******************** MACROS UTILITÁRIAS ********************
*/

#define null_string_error ( \
  "Erro: tentativa de iniciar a string com um ponteiro nulo!\n" \
  "Caso queira criar uma string vazia, utilize \"String.empty()\"\n" \
)

#define index_out_of_bounds_error "Erro: falha ao acessar índice %d!\n"

#define alloc_string() ( \
  (StringPrivateMembers*) malloc(sizeof(StringPrivateMembers) + sizeof(StringPublicMembers)) \
)

#define get_public_offset(private) ( \
  (StringPublicMembers*) ( ((char*) private) + sizeof(StringPrivateMembers)) \
)

#define get_private_offset(public) ( \
  (StringPrivateMembers*) ( ((char*) public) - sizeof(StringPrivateMembers)) \
)

/*
  ******************** IMPLEMENTAÇÃO DOS MÉTODOS ********************
*/

static size_t set_string(object this, const char* const new_buffer) {
  if(new_buffer == NULL) {
    fprintf(stderr, null_string_error);
    exit(1);
  }

  size_t size_new_buffer = strlen(new_buffer);

  StringPrivateMembers* string_private = get_private_offset(this);

  if(not string_private->buffer)
    string_private->buffer = malloc(size_new_buffer + 1);
  else
    string_private->buffer = realloc(string_private->buffer, size_new_buffer + 1);

  memset(string_private->buffer, 0, size_new_buffer + 1);

  strncpy(string_private->buffer, new_buffer, size_new_buffer);

  string_private->size = size_new_buffer;
  return size_new_buffer;
}

static const char* const get_string(object this) {
  StringPrivateMembers* string_private = get_private_offset(this);

  return string_private->buffer;
}

static size_t get_size(object this) {
  StringPrivateMembers* string_private = get_private_offset(this);

  return string_private->size;
}

static char at(object this, const int index) {
  StringPrivateMembers* string_private = get_private_offset(this);
  
  if(index < 0 or index > string_private->size - 1) {
    fprintf(stderr, index_out_of_bounds_error, index);
    exit(1);
  }

  return string_private->buffer[index];
}

static void clear(object this) {
  StringPrivateMembers* string_private = get_private_offset(this);

  if(string_private->buffer == NULL)
    return;

  memset(string_private->buffer, 0, string_private->size);
  string_private->size = 0;
}

static void copy(object this, object other) {
  if(this == NULL or other == NULL)
    return;

  StringPrivateMembers* other_string_private = get_private_offset(other);

  set_string(this, other_string_private->buffer);
}

/*
  ********* CONSTRUTOR, DESTRUTOR *********
*/

static object empty(void) {
  return String.new("\0");
}

static object new(const char* const init_buffer) {
  // membros private
  StringPrivateMembers* string_private = alloc_string();
  // membros public
  StringPublicMembers* string_public = get_public_offset(string_private);

  // configura a nova string e retorna o tamanho para string_private->size
  string_private->buffer = NULL;
  string_private->size = set_string(string_public, init_buffer);

  // configura os métodos do objeto
  string_public->get_string = get_string;
  string_public->set_string = set_string;
  string_public->get_size = get_size;
  string_public->at = at;
  string_public->clear = clear;
  string_public->copy = copy;

  return string_public;
}

static void* delete(object this) {
  if(this == NULL)
    return NULL;

  StringPrivateMembers* string_private = get_private_offset(this);
  
  // zerando e liberando memória dos membros privados
  memset(string_private->buffer, 0, string_private->size);
  string_private->size = 0;

  free(string_private->buffer);
  string_private->buffer = NULL;

  // apagando a referência apontada pelos métodos às funções static
  this->at = NULL;
  this->get_size = NULL;
  this->get_string = NULL;
  this->set_string = NULL;
  this->clear = NULL;
  this->copy = NULL;

  free(string_private);

  return NULL;
}

class_string String = {
  .new = new,
  .delete = delete,
  .empty = empty
};