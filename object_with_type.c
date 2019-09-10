#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

/*
 * Mais um 'hack', em que é criado uma struct Object que representa um objeto que armazena
 * um determinado valor. É possível acessar o tipo desse objeto através da função type_of, 
 * mesmo que a struct Object não possua nenhum membro que seja designado para armazenamento
 * de tipo.
*/

// apenas uma abstração para void*
typedef void* var;

// Struct do objeto
typedef struct _object {
  var value; // valor armazenado pelo objeto
} Object;

// Header do objeto, é aqui que o seu tipo é armazenado
typedef struct _header {
  var type;
} Header;

// Construtor de Object
Object* alloc(var type, var value) {

  /*
   * Essa função não só aloca memória para o objeto, pois isso poderia ser feito diretamente com um malloc.
   * Seu objetivo principal é de colocar tanto o Object, quanto seu Header no mesmo bloco de memória, dessa
   * forma, com algumas operações de artimética de ponteiro será possível acessar o tipo do objeto sem que 
   * este o armazene diretamente. Basicamente eu estou alocando memória que equivale a soma dos tamanhos dos
   * tipos Header e Object, retornando o endereço dessa memória para um ponteiro para Header:
   *
   *     HEADER        OBJECT
   *   +--------+-----------------+
   *   |  ????  |      ?????      | MEMÓRIA TOTAL ALOCADO POR MALLOC
   *   +--------+-----------------+
   *   ^
   *    `- ponteiro head 
  */
  Header* head = (Header*) malloc(sizeof(Header) + sizeof(Object)); // alocando

  /*
   * O endereço de memória retornado tem que passar a ser apontado por um ponteiro para Header, pois, inicialmente,
   * irei configurar o tipo do objeto, mas também para fazer corretamente o deslocamente para o byte que inicia meu Object.
   *
   *     HEADER        OBJECT
   *   +--------+-----------------+
   *   |  type  |      ?????      | MEMÓRIA TOTAL ALOCADO POR MALLOC 
   *   +--------+-----------------+
   *   ^
   *    `- ponteiro head 
  */
  head->type = type; // configurando o tipo do objeto

  /* 
   * Após isso, através de aritmética de ponteiro, eu pego o offset(deslocamento) necessário para chegar na região
   * que armazenará os dados de Object, por isso eu somo a memória atual apontada pelo ponteiro head com o tamanho de
   * um Header, com isso, pegarei o byte em que Object "inicia" e retorno o endereço para o ponteiro obj (que aponta
   * para um struct Object, podendo manipular corretamente esse tipo de estrutura)
   *
   *     HEADER        OBJECT
   *   +--------+-----------------+
   *   |  type  |      ?????      | MEMÓRIA TOTAL ALOCADO POR MALLOC
   *   +--------+-----------------+
   *            ^
   *             `- ponteiro obj (note que obj aponta para uma região mais a frente do bloco alocado, essa região é quem conterá meu Object).
  */
  Object* obj = (Object*)(head + sizeof(Header));
  
  /*
   * Agora com o ponteiro obj, consigo manipular os bytes para configurar o valor armazenado pelo meu objeto
   *
   *     HEADER        OBJECT
   *   +--------+-----------------+
   *   |  type  |      value      | MEMÓRIA TOTAL ALOCADO POR MALLOC
   *   +--------+-----------------+
   *            ^
   *             `- ponteiro obj 
  */
  obj->value = value; // configurando o valor que meu objeto deve armazenar

  // como eu estou alocando memória para um Objeto, eu retorno o endereço apontado por obj
  return obj; // retorna apenas o endereço que inicia meu Object, tecnicamente obj não poderá acessar seu tipo ao fazer obj->type
} // aqui encerra a função alloc (ela não é grande, ficou por conta dos comentários :] )


/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */


// Essa função desloca para traz o endereço apontado por object, dessa forma, eu passarei a ter acesso ao Header
// do meu objeto, conseguindo pegar seu tipo 
char* type_of(var object) {

  // Explicando a expressão
  // 1. ((Header*) object) = converto meu ponteiro object para o tornar um ponteiro para Header, dessa forma, conseguirei,
  // deslocar corretamente para chegar no byte que inicia o Header do objeto. Chamaremos essa parte de 'A'

  // 2. A - sizeof(Header) = subtraio a memória apontada por A pelo tamanho do Header, para chegar no byte que o inicia
  // Exemplo: digamos que meu meu Header começa na posição 0x3000, meu objeto aponta para 0x3008 (que é o endereço que meu
  // objeto propriamente inicia), e o tamanho de um Header seja de 8 bytes, ao fazer A - sizeof(Header), dessa forma
  // eu terei: 0x3008 - 0x0008 = 0x3000, sendo o resultado o byte que inicia meu Header. Chamaremos o resultado de 
  // A - sizeof(Header) de B

  // 3. B->type = como B é um endereço de memória que contém um Header, eu apenas derreferencio essa memória para obter
  // o tipo do meu objeto, logo B->type é a mesma coisa que head->type na função alloc.

  return ( ((Header*) object) - sizeof(Header) )->type; // retorna o tipo
}

int main() {
  
  Object* obj = alloc("String", "Olá, Mundo!");
  printf("Tipo do 'obj': %s\n", type_of(obj));
  printf("Valor armazenado por 'obj': %s\n\n", obj->value);

  Object* obj_dois = alloc("Inteiro", (int*) 10);
  printf("Tipo do 'obj_dois': %s\n", type_of(obj_dois));
  printf("Valor armzenado por 'obj_dois': %d\n", (int) obj_dois->value);

  return 0;
}
