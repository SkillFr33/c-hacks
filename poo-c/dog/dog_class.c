#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <string.h>
#include "dog_interface.h"
#include "static_functions.h"

// função que desloca o ponteiro dos membros public para a região dos membros privados
static DogPrivateMembers* get_private_offset(void* public_offset) {
  return (DogPrivateMembers*) ( ((char*) public_offset) - sizeof(DogPrivateMembers) );
}

// função que desloca o ponteiro para a região dos membros públicos do objeto
static DogPublicMembers* get_public_offset(void* private_offset) {
  return  (DogPublicMembers*) ( ((char*) private_offset) + sizeof(DogPrivateMembers) );
}

// função-macro para alocar memória suficiente para a classe dog (struct dos membros públicos e privados)
#define alloc_dog() ( \
  (DogPrivateMembers*) malloc(sizeof(DogPrivateMembers) + sizeof(DogPublicMembers)) \
)

/*
  ************ IMPLEMENTAÇÃO DO CONSTRUTOR E DESTRUTOR DA CLASSE DOG ************
*/

// Construtor de objetos de dog
static dog* _new(string nome, double peso) {
  // membros privados
  DogPrivateMembers* dog_private = alloc_dog();

  // membros públicos
  DogPublicMembers* dog_public = get_public_offset(dog_private);

  // configurando os ponteiros para função apontarem para suas respectivas funções static
  dog_public->get_nome = _get_nome;
  dog_public->set_nome = _set_nome;
  dog_public->get_peso = _get_peso;
  dog_public->set_peso = _set_peso;
  dog_public->latir    = _latir;

  // inicializando atributos privados 
  dog_private->nome = NULL;
  _set_nome(dog_public, nome);
  _set_peso(dog_public, peso);

  // retornando ponteiro apenas para a região dos membros públicos 
  return dog_public;
}

// Destrutor de objetos de dog
static void* _delete(void* this) {
  // configurando os ponteiros para função apontarem para NULL
  ((DogPublicMembers*) this)->latir    = NULL;
  ((DogPublicMembers*) this)->set_peso = NULL;
  ((DogPublicMembers*) this)->get_peso = NULL;
  ((DogPublicMembers*) this)->set_nome = NULL;
  ((DogPublicMembers*) this)->get_nome = NULL;

  // delocando ponteiro para a região dos membros privados
  DogPrivateMembers* header = get_private_offset(this);

  // configurando peso como 0 e liberando memória apontada por header->nome
  header->peso = 0;
  free(header->nome);

  // liberando memória apontada por header, ou seja, todo o bloco que
  // engloba os membros privados e membros públicos
  free(header);

  // retorna NULL para caso queria destruir o objeto e já configurar a
  // variável que apontava para esse objeto como NULL
  return NULL; // Ex.: dog = Dog.delete(dog);
}

/*
  CRIANDO VARIÁVEL RESPONSÁVEL POR INSTANCIAR E DELETAR OBJETOS DA CLASSE DOG
*/

dog_class Dog = {
  .new = _new,       // dog rex = Dog.new("Rex", 10.3);
  .delete = _delete  // Dog.delete(rex);
};

/*
  ****************** IMPLEMENTAÇÃO DOS MÉTODOS DA CLASSE ******************
*/

static void _set_peso(void* this, double peso) {
  // validando o novo peso do cachorro
  if(peso <= 0) {
    fprintf(stderr, "O peso do cachorro não pode ser menor ou igual a zero!\n");
    return;
  }

  // tendo acesso aos membros private do objeto para fazer alteração no peso
  DogPrivateMembers* header = get_private_offset(this);
  header->peso = peso;
}

static double _get_peso(void* this) {
  // tendo acesso aos membros private para retornar o peso
  DogPrivateMembers* header = get_private_offset(this);

  return header->peso;
}

static void _latir(void* this) {
  // tendo acesso aos membros private para pegar o nome do cachorro
  DogPrivateMembers* header = get_private_offset(this);

  printf("%s: AUAUAU!\n", header->nome);
}

static void _set_nome(void* this, string nome) {
  DogPrivateMembers* header = get_private_offset(this);

  // pegando o tamanho da string atual e da string nova
  size_t tamanho_novo_nome  = nome == NULL ? 0 : strlen(nome);
  size_t bytes = tamanho_novo_nome == 0 ? strlen(DEFAULT_NAME) + 1 : tamanho_novo_nome + 1;

  header->nome = realloc(header->nome, bytes);
  memset(header->nome, '\0', bytes);

  // string passada é nula ou vazia, dessa forma, o nome do cachorro passa a ser DEFAULT_NAME
  if(nome == NULL || nome[0] == '\0') {
    fprintf(stderr, "O nome do cachorro não pode ser vazio! Nome definido como \"%s\"\n", DEFAULT_NAME);
    strcpy(header->nome, DEFAULT_NAME);
  }
  else
    strcpy(header->nome, nome);
  header->nome[bytes - 1] = '\0';

}

/* 
  Como essa função retorna o endereço do membro private nome, significa que
  seu valor poderá ser manipulado no código cliente, mesmo que seu tipo
  de acesso seja private :/

  O retorno é um 'const char* const', para pelo menos gerar um warning caso algo como 
  isto ocorra: 
    
    char* nome = dog->get_nome(dog);

  nesse exemplo, será possível manipular o atributo privado do objeto, contudo gerará um warning,
  pois o ponteiro 'nome' discartará o qualificador 'const * const' do retorno da função.
*/
static const char* const _get_nome(void* this) {
  // tendo acesso aos membros private para pegar o nome do cachorro
  DogPrivateMembers* header = get_private_offset(this);

  return header->nome; // perigoso isso aqui :c
}