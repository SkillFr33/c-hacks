#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include "auto_management.h"

static void* pointers_to_heap[MAX_POINTERS] = {NULL};

void free_memory() {
  int counter = 0;
  while(pointers_to_heap[counter] && counter < MAX_POINTERS) {
    free(pointers_to_heap[counter]);
    pointers_to_heap[counter] = NULL;
    ++counter;
  }

  puts("\n\n>>> Memória automaticamente desalocada! <<<");
}

void* alloc(size_t bytes) {
  static int ptr_to_heap_counter = 0;
  
  void* ptr = NULL;
  if(ptr_to_heap_counter == MAX_POINTERS)
    printf("Não é possível alocar mais memória! ptr retornará NULL!\n");
  else {
    ptr = malloc(bytes);
    pointers_to_heap[ptr_to_heap_counter] = ptr;
    ++ptr_to_heap_counter;
  }

  return ptr;
}