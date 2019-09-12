# Automatic Memory Management in C

<p>Gerenciamento automático de memória dos objetos alocados na heap</p>

Compile com:

```
gcc -O2 -Wall main.c auto_management.c -o main
```

<p>Resultado do valgrind:</p>
<img src="valgrind_result.png" />
<small>Obs: aparentemente a função atexit também faz alocação dinâmica, por isso na imagem mostra que 3 alocações foram realizadas.</small>
