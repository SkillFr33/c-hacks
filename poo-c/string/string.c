#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "string.h"

static Header* get_header(String* object) {
  return (Header*) ((char*) object - sizeof(Header));
}

static size_t length(void* object) {
  Header* header = get_header(object);
  return header->__size;
}

String* new_string(char* str) {

  size_t size = strlen(str);

  Header* header = (Header*) malloc(sizeof(Header) + sizeof(String));
  header->__size = size;

  String* string = (String*) ((char*) header + sizeof(Header));
  string->ptr = (char*) malloc(sizeof(char) * size + 1);
  string->length = length;
  memcpy(string->ptr, str, size);

  return string;
}
