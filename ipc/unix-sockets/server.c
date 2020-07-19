#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/un.h>
#include <sys/socket.h>

void panic(const char* msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

int main() {

  // cria um socket local com o tipo de socket SOCK_STREAM, em que há uma conexão e
  // os pacotes são enviados de forma sequencial e confiável.
  int local_fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if(local_fd == -1)
    panic("socket");

  // endereço do socket local, no caso é representado por um arquivo
  struct sockaddr_un local;
  local.sun_family = AF_UNIX;
  strcpy(local.sun_path, "./socket_path");
  
  // remove o arquivo do diretório, pois se ele já existir, não será possível realizar o bind,
  // pois o endereço (arquivo) já está em uso por outro socket
  unlink(local.sun_path);

  // associa o socket ao endereço. É necessário converter o endereço de local (struct sockaddr_un) para
  // um ponteiro para struct sockaddr. Essa última é uma interface genérica que representa qualquer endereço,
  // seja ele local (unix socket) ou de rede (ex. Internet)
  // O último argumento é o tamanho da struct que representa o endereço
  if(bind(local_fd, (struct sockaddr*) &local, sizeof(local)) == -1)
    panic("bind");

  // prepara o socket para aceitar conexões. No máximo 1 requisição de conexão pode ser enfileirada 
  // antes que próximas requisições sejam recusadas.
  listen(local_fd, 1);
  puts("Server escutando por conexões");

  // espera por uma requisição de conexão ou retira uma conexão pendente na fila de requisições.
  // Como é um unix-socket, não há necessidade de adquirir informações do cliente, por isso foi
  // passado NULL nos últimos dois argumentos.
  int remote_fd = accept(local_fd, NULL, NULL);
  if(remote_fd == -1)
    panic("accept");
  puts("Conexão aceita!");

  char buffer[1024];
  while(1) {
    // recebe no máximo 1024 bytes enviados pelo cliente e armazena em 'buffer'
    int bytes = recv(remote_fd, buffer, sizeof(buffer), 0);
    if(bytes == -1) // verifica por erro
      panic("recv");
    else if(bytes == 0) { // verifica se a conexão foi encerrada
      puts("\nCliente desconectado!");
      break;
    }

    // uso stderr, pois ele não é buffered, então exibe o texto imediatamente sem precisar de um '\n' para dar o fflush
    fprintf(stderr, "\nMensagem recebida: ");
    write(STDOUT_FILENO, buffer, bytes); // exibe na saida padrão a mensagem enviada pelo cliente
  }

  unlink(local.sun_path); // excluindo o arquivo que representa o endereço do socket

  return EXIT_SUCCESS;
}
