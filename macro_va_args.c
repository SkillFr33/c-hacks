#include <stdio.h>
#include <stdlib.h>

#define array_size(array) (sizeof(array)/sizeof(array[0])) 

#define sum(T, ...) ({              \
  T array[] = {__VA_ARGS__};        \
  T total = 0;                      \
  size_t size = array_size(array);     \
  while(size-- > 0)                 \
    total += array[size];           \
  total; /*isso aqui é tipo um return*/\
})


int main() {

  int x = sum(int, 1, 2, 3, 4, 5, 7, 8, 9, 10);
  printf("%d\n\n", x);

  double y = sum(double, 10.2, 10.5, 19.234);
  printf("%f\n\n", y);

  char z = sum(char, 10, 20, 30, 5);
  printf("%c\n", z);

  return 0;
}
