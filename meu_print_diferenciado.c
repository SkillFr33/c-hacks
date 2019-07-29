#include <stdio.h>
#include <stdlib.h>

/*
	Umas doideras com macros o.O
*/

#define formato(x)   \
	_Generic(x,        \
		char*:  "%s\n",  \
		char:   "%c\n",  \
		double: "%f\n",  \
		int:    "%d\n"   \
)  

#define tamanho_array(x)\
	sizeof(x) / sizeof(x[0]) 

#define meu_print_diferenciado(T, ...) {         \
	T array[] = {__VA_ARGS__};                     \
	for(int x = 0; x < tamanho_array(array); x++)  \
		printf(formato(array[x]), array[x]);         \
	putchar('\n');                                 \
}

int main() {

	meu_print_diferenciado(char, 65, 66, 67, 78);
	meu_print_diferenciado(int, 65, 'a', 67, 78, 'B');
	meu_print_diferenciado(char*, "Hello", "World!", ":)");
	meu_print_diferenciado(double, 80.1);

}