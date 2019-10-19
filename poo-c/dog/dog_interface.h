#ifndef _DOG_INTERFACE_H
#define _DOG_INTERFACE_H

// essas duas macros são só pra fins visuais, elas não significam nada
#define private
#define public

#define DEFAULT_NAME "Rex"

// syntax-sugar pra char*
typedef char* string;

// membros privados da classe
typedef struct dog_private_members {
	private string nome;
	private double peso;
} DogPrivateMembers;

// membros públicos da classe. Cada objeto instânciado será um ponteiro para essa struct
typedef struct dog_public_members {
	public double (*get_peso)(void* this);
  public void (*set_peso)(void* this, double peso);

  public const char* const (*get_nome)(void* this);
  public void (*set_nome)(void* this, string nome);

  public void (*latir)(void* this);
} DogPublicMembers;
typedef DogPublicMembers dog;

// struct que representa o construtor e destrutor da classe
typedef struct dog_class {
  public dog* (*new)(string nome, double peso);
  public void* (*delete)(void* dog_object);
} dog_class;

// A partir dessa variável (declarada em dog_class.c), será possível
// instanciar e deletar objetos da classe dog (aka DogPublicMembers)
extern dog_class Dog;

#endif // _DOG_INTERFACE_H