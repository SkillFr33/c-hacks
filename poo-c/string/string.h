#ifndef _STRING_H_
#define _STRING_H_

#include <stdbool.h>

// struct que contém os membro e funções-membro private do objeto 
struct _string_private_members {
  size_t __size;
};
typedef struct _string_private_members Header;

struct _string {
  char* ptr;
  size_t (*length)(void* this);
  bool (*isEmpty)(void* this);
  bool (*setString)(struct _string* this, char* new_str);
  void (*info)(struct _string* this);
  bool (*copy_string)(struct _string* this, struct _string* other);
};
typedef struct _string String;

String* new_string(char* str);
void* delete_string(struct _string* this);

#endif // _STRING_H_