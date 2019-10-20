#include "string.h"

// construtor do objeto
static object new(const char* const init_buffer);

// destrutor do objeto
static void* delete(object this);

// retorna um objeto vazio
static object empty(void);

// Retorna o tamanho da string
static size_t get_size(object this);

// Configura o buffer da string com um novo valor. Retorno o tamanho da nova string
static size_t set_string(object this, const char* const new_buffer);

/*
  Retorna o endereço apontado pelo buffer da string

  ATENÇÃO: como esse método retorna o endereço que o buffer da string
  aponta, será possível manipular a string no código cliente de forma di-
  reta, o que não deveria ser correto. Dessa forma, o tipo de retorno
  desse método é um 'const char* const', para gerar um warning caso
  algo como isto ocorra:

    char* retorno = string->get_string(string);

  retorno é apenas um 'char*', e não um 'const char* const', podendo alterar
  o conteúdo do objeto string diretamente
*/
static const char* const get_string(object this);

// Retorna uma cópia do caractere contido na posição `index` da string. Valida o índece passado
static char at(object this, const int index);