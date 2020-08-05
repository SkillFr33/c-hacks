# Uso do `poll` para esperar por eventos de entrada

Servidor TCP que utiliza a syscall `poll` para esperar por eventos de entrada que possam ocorrer nos sockets do servidor ou cliente. Nesse exemplo simples, consigo aceitar e geranciar diversas conexões utilizando apenas uma thread.

Compilar:

```Bash
gcc main.c server.c -o server
./server localhost 8080
```
