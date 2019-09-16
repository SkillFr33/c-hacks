#ifndef _STRING_H_
#define _STRING_H_

// struct que contém os membro e funções-membro private do objeto 
struct _string_private_members {
  size_t __size;
};
typedef struct _string_private_members Header;

struct _string {
  char* ptr;
  size_t (*length)(void* object);
};
typedef struct _string String;

String* new_string(char* str);

#endif // _STRING_H_