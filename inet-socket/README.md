# Socket

Algumas anotações que eu fiz sobre sockets pra fixar o conteúdo. É muito provável que tenha algum conceito meio errado.

## O que é um socket

Um socket é um endpoint de comunicação bidirecional, ou seja, a partir de um socket é possível tanto enviar, quanto receber informações. Em sistemas baseados em Unix, um socket é representado através de um **file descriptor\***.

Há diversos domínios de comunicação de sockets, isto é, propriedades que regem convenção de nomes, família de protocolos etc. O domínio mais comum é o de Internet, que permite comunicação entre diferentes dispositivos através de uma rede. O domínio também pode ser local (Unix domain socket), para comunicação entre processos de uma mesma máquina.

Como foi dito que um socket é representado por um file descriptor, funções que lidam com um fd geralmente funcionam em um socket, como as funções **read** e **write** que proporcionam operações de entrada e saída, respectivamente.

Um socket também tem seu tipo, que define outras propriedades da comunicação, como a semântica, ou seja, a forma como os dados enviados e recebidos serão interpretados. Além disso, o tipo do socket define se a comunicação será feita através de uma conexão bem estabelecida, que proporciona certa segurança e ordem das informações. Geralmente um socket é dividido em dois principais tipos: **Stream Socket** e **Datagram Socket**.

---

**\* file descriptor:** em sistemas Unix-Like, um file descriptor é o identificador único de qualquer recurso que permita entrada e saída de dados, como um arquivo, pipe, network socket etc.

---

### Stream Socket

Stream Socket é um tipo de socket orientado à conexão, isto é, uma conexão/sessão é estabelecida entre um cliente e servidor antes que qualquer dado possa ser transmitido por ambas as partes. Essa conexão garante certas vantagens, como a confiabilidade e ordem em que os dados são enviados/recebidos. A confiabilidade se dá, pois esse tipo de socket verifica se o pacote transmitido realmente chegou em seu destino, se não, ele é reenviado. Geralmente o protocolo TCP é utilizado em conjunto com esse tipo.

### Datagram Socket

Datagram Socket é outro tipo de socket altamente utilizado. Esse tipo - diferente do Stream Socket - não possui uma conexão estabelecida entre remetente e destinaráio, ou seja, cada pacote(**datagrama\***) é individualmente endereçado e roteado(direcionado) para seu destino. Como vários datagramas podem ser roteados individualmente de máquinas diferentes, o destinatário os recebe sem uma ordem estabelecida. Além disso, devido à falta de uma conexão concreta entre ambos, é possível que datagramas se percam no meio do caminho. Geralmente o protocolo UDP é utilizado em conjunto com esse tipo.

Por mais que esse tipo de socket seja considerado sem conexão, ainda é possível se conectar a um socket remoto utilizando ele; isso é útil em casos que você enviará datagramas sempre para o mesmo destinatário. Além disso, é possível construir uma camada de segurança mesmo usando datagramas. Isso pode ser feito da seguinte forma: cada datagrama que o servidor recebe, ele envia uma mensagem ao "cliente", informando que o datagrama foi recebido com sucesso. Datagram Sockets mais rápidos que Stream Sockets.

---

**\* datagrama:** basicamente um pacote de dados, contudo independente de qualquer conexão.

---

## Modelo OSI

OSI, do inglês Open Systems Interconnection, é um modelo conceitual que estabelece regras e orientações para que seja possível a comunicação entre dois dispositivos de rede, sem que haja dependência de um hardware ou sistema operacional específico. Todas as ações necessárias para a comunicação entre esses dispositivos foram divididas em partes, chamadas de camadas, totalizando 7. Cada camada é responsável por uma parte da comunicação.

### 1. Camada Física

Essa camada é a de mais baixo nível, pois se refere à comunicação real entre os dispositivos, ou seja, define as características do hardware. Nessa camada ocorre a transmissão e recepção dos dados brutos, isto é, a conversão de sinais elétricos, de rádio, ópticos etc em bits digitais; o contrário também ocorre, logo, bits digitais são convertidos em sinais.

Para que os dados brutos enviados sejam interpretados da mesma forma quando recebidos, é especificada características como tipo de cabeamento, tensão, taxa de transmissão/recepção máxima etc.

### 2. Camada de Enlace

Nessa camada os dados vindos das camadas superiores são encapsulados em quadros(frames). Cada início e final de transmissão de um frame é identificado a partir de uma sequência específica de bits. Além disso, essa camada especifica os endereços físicos das **interfaces de rede\*** envolvidas na transmissão, ou seja, o endereço MAC(Media Access Control); esse endereço é uma sequência única e fixa de 48 bits que está gravado em um chip na própria interface de rede.

Essa camada também é responsável pela integridade dos dados (reconhecendo erros através de um **checksum\*\***) e por traduzir os bits lógicos em sinais (e visse-versa).

---

**\* interface de rede:** trata-se do hardware usado para que o dispositivo se conecte em uma rede; pode ser uma placa de rede convencional, um adaptador wifi etc.

**\*\* checksum:** é o resultado derivado da soma dos dados de um bloco. É amplamente utilizado para detectar erros em canais que podem sofrer de ruídos.

---

### 3. Camada de Rede

A camada de rede é responsável por rotear(direcionar) o tráfego de um nó da rede para outro, ou seja, é capaz de endereçar cada nó da rede utilizando o IP. Nessa camada é onde o pacote(ou datagrama) é propriamente construído, contendo o endereço da origem(source), do destino(destination) e demais informações.

Ela também é responsável por dividir o pacote em fragmentos caso este supere o tamanho máximo de um frame; esses fragmentos são posteriormente montados pela camada de rede do nó destino. É da responsabilidade dessa camada definir o protocolo de roteamento que pode, ou não, ser confiável.

### 4. Camada de Transporte

Essa camada que permite a comunicação entre dispositivos, pois é ela quem controla o fluxo dos dados da origem até o destino. Nela também ocorre checagens e eliminações de erros que podem ter ocorrido nas camadas inferiores e, em caso de perca de pacote, é solicitada a retransmissão.

Além disso, essa camada permite que ocorra a multiplexação, ou seja, uso de portas para que seja possível a comunicação entre processos que executam em computadores diferentes, como exemplo um navegador que requisita um recurso de um webserver; tanto o navegador quanto o webserver são identificados pelas portas que eles utilizam.

O tipo de **protocolo de transporte\*** utilizado altera a forma como os pacotes/datagramas serão transportados da origem para o destino. Contudo, por mais que eles sejam diferentes, a camada de transporte ainda faz o papel principal, *transportar os dados*.

Caso os dados a serem transmitidos sejam muito grandes, é possível que ultrapassam o limite de tamanho de uma **PDU(Protocol Data Unit)\*\*** da camada de rede. Dessa forma, os dados precisam ser segmentados para que possam ser transformados em pacotes e assim enviados para as camadas de mais baixo nível para que ocorra a transmissão. Na outra ponta, ou seja, no receptor, a camada de transporte também é responsável por agrupar os segmentos e montar o dado original.

---

**\* protocolo de transporte:** o modelo OSI define 5 classes de protocolos de transporte, numeradas de 0 à 4, com o prefixo TP(TP0, TP1 etc). Quanto menor a classe, menos funcionalidades ela implementa, como conectividade entre origem e destino, retransmissão de pacotes perdidos, ordem dos pacotes, confiabilidade entre outros. Os protocolos mais utilizados são TCP e UDP.

**\*\* PDU:** é uma unidade de informação com a qual a camada em questão trabalha. A PDU da camada de transporte é o segmento ou datagrama; da camada de rede é o pacote; da camada de enlace é o frame etc. Cada camada tem um tamanho máximo de PDU com a qual pode trabalhar.

---

### 5. Camada de Sessão

Sua função é de estabelecer, manter e gerenciar a conexão entre aplicações de origem e de destino. Ela também é responsável por terminar apropriadamente a comunicação entre essas aplicações, além de permitir a criação de pontos de sincronização, que são úteis para identificação de erros e re-sincronização, sem que a conexão seja interrompida.

Essa camada possibilita que os processos escolham os modos de comunicação, ou seja, se será half-duplex, simplex ou full-duplex.

### 6. Camada de Apresentação

Essa camada lida com a apresentação dos dados, ou seja, questões como encoding de caracteres, criptografia, compressão etc.

### 7. Camada de Aplicação

É a camada de mais alto nível, ou seja, mais próxima da aplicação do usuário. Ela é justamente a interface utilizada pelos programas para que estes acessem a rede, e é nela que estão os protocolos de alto nível, como protocolo de envio de email, de arquivo etc.

IMPORTANTE: hoje em dia, o Modelo OSI é apenas uma referência para outros modelos; o modelo mais utilizado para comunicação em rede é o Modelo Internet, também conhecido como Modelo TCP/IP.

## Ordem dos bytes

Ordenação dos bytes se refere a ordem que os bytes estão armazenados na memória e, posteriormente, a ordem em que eles serão transmitidos pela rede. Há dois tipos de sistema de ordenação: o sistema little-endian e big-endian. Caso dois **hosts\*** se comuniquem utilizando ordenações diferentes, os dados enviados por um não serão legiveis pelo outro, pois serão interpretados de formas diferentes.

---

**\* host:** um host(hospedeiro) se refere a qualquer dispositivo conectado em uma rede.

---

### Sistema Big-endian

Esse sistema é o mais intuitivo de ser entendido, pois basicamente os números que lemos no nosso dia a dia estão organizados dessa forma. Nele, o dígito mais significativo vem primeiro caso você o leia da direita para a esquerda. Por exemplo, o número hexadecimal 0x0A03 possui dois bytes; o byte 0x0A é inserido primeiro na memória, seguido do byte 0x03, ficando assim:

| Endereço | 0x1010   | 0x1011   |
|---------:|---------:|---------:|
| **Byte** |  `0x0A`  |  `0x03`  |

Dessa forma, o byte mais significativo (0x0A) está armazenado no endereço de menor valor (0x1010); em sequência está o byte menos significativo (0x03) que está armazenado no endereço de maior valor (0x1011). Basicamente o big-endian é isso: o byte mais significativo fica armazenado no menor endereço de memória.

### Sistema Little-endian

Em contraste ao sistema big-endian, o little-endian armazena o byte menos significativo no menor endereço de memória, dessa forma, a leitura é inversa à leitura do big-endian. Exemplo com o número 0x0A03:

| Endereço | 0x1010   | 0x1011  |
|---------:|---------:|--------:|
| **Byte** |  `0x03`  |  `0x0A` |

Note que o menor endereço contém o byte menos significativo (0x03), enquanto o maior endereço contém o byte mais significativo (0x0A). Esse sistema é menos intuitívo, pois é como se tivéssemos que ler o número 1056 (base decimal) em sua forma invertida (6501), contudo representando a mesma quantidade.

A maioria dos processadores atuais utilizam o sistema little-endian para manipular os dados. É possível descobrir a ordem dos bytes com um simples programa em C:

```C
  int x = 0x1020;
  char* xptr = (char*) &x;

  // menor endereço; se o sistema for little-endian, aqui conterá
  // o byte 0x20 (byte menos significativo)
  printf("(%p) xptr[0]: %x\n", &xptr[0], xptr[0]);

  // maior endereço; se o sistema for little-endian, aqui conterá
  // o byte 0x10 (byte mais significativo)
  printf("(%p) xptr[1]: %x\n", &xptr[1], xptr[1]);

  if(xptr[0] == 0x10)
    puts("Big-endian");
  else
    puts("Little-endian");
```

A ordem que os bytes são organizados no seu computador é chamada de **Host Byte Order**, geralmente sendo little-endian. A ordem que os bytes são organizadas na rede é chamada de **Network Byte Order**, geralmente sendo big-endian. Com o intuito de alterar a ordem dos bytes de determinados tipos/tamanhos de dados, existem funções para auxiliar o processo, como a função `htons` (Host to Network Short). Essa função recebe como argumento um `unsigned short`(2 bytes) no formato little-endian e o converte para sua representação em big-endian, basicamente invertendo os bytes (0x0A03 -> 0x030A, por exemplo). Há diversas variações desse tipo de função, como listado abaixo:

|Função    | Descrição             | Bits          |
|----------|-----------------------|---------------|
|`htons()` | host to network short | 16 (2 bytes)  |
|`htonl()` | host to network long  | 32 (4 bytes)  |
|`ntohs()` | network to host short | 16 (2 bytes)  |
|`ntohl()` | network to host long  | 32 (4 bytes)  |

Um dos números que é necessário conversão de ordem de bytes é a porta do processo. A porta é um número de 16 bits, dessa forma a conversão pode ser feita com a função `htons`.

## Structs importantes para trabalhar com sockets

É importante entender as structs envolvidas ao utilizar a interface de sockets. Essas structs são responsáveis por conter informações sobre os hosts que fazem parte de uma comunicação e como essa comunicação será realizada.

## `struct addrinfo`

Essa struct armazena informações de um host ou serviço em uma representação que é legível ao sistema operacional, ou seja, que pode ser utilizada pela API de socket. Para preencher essa struct é usada a função `getaddrinfo`.

Definição:

```C
struct addrinfo {
  int    ai_flags;      // AI_PASSIVE, AI_CANONNAME, etc.
  int    ai_family;     // AF_INET, AF_INET6, AF_UNSPEC
  int    ai_socktype;   // SOCK_STREAM, SOCK_DGRAM
  int    ai_protocol;   // tipo do protocolo; 0 = padrão da família definida em ai_family
  size_t ai_addrlen;    // tamanho, em bytes, de ai_addr
  struct sockaddr* ai_addr; // struct sockaddr_in ou _in6 (IPv4 ou IPv6, basicamente)
  char   *ai_canonname; // nome canônico do host
  struct addrinfo* ai_next; // próximo nó da lista
};
```

Essa struct, na verdade, é uma lista encadeada, tanto que seu membro `ai_next` é um ponteiro para uma `struct addrinfo`. Dessa forma, ao usar a função `getaddrinfo`, é possível que um host possua mais de um endereço de IP, e cada um deles é ligado através dessa lista. Isso será visto posteriormente.

## `struct sockaddr`

Essa struct é muito importante, pois ela é uma interface genérica que representa o endereço de um socket.

Definição

```C
struct sockaddr {
  sa_family_t sa_family;   // AF_INET = IPv4; AF_INET6 = IPv6; AF_UNIX = unix domain
  char        sa_data[14]; // endereço do socket, que depende da família de endereço,
                           // Seu tamanho é arbitrário, inclusive pode ser maior que 14
}
```

Como a `struct sockaddr` é um interface genérica, significa que na verdade ela é usada para representar outros tipos mais "concretos", que, de fato, representam o endereço de um socket. Alguns exemplos dessas structs concretas são `struct sockaddr_un`, `struct sockaddr_in` e `struct sockaddr_in6`. A primeira não interessa muito, pois é usada para IPC, pois é um domínio local identificado através de um arquivo no próprio sistema. As duas últimas são importantes para comunicação em rede, pois são elas que realmente representam o endereço na rede.

Definição `struct sockaddr_in`:

```C
struct sockaddr_in {
  short    sin_family;     // deve ser AF_INET, pois é IPv4
  ushort_t sin_port;       // número da porta (big-endian)
  struct in_addr sin_addr; // endereço IPv4
  uchar_t  sin_zero[8];    // padding para ficar com o mesmo tamanho de struct sockaddr
};
```

Essa struct contém um endereço IPv4. Note que um dos membros dessa struct é chamado de `sin_addr` e é ele quem representa o endereço do host/serviço. Antigamente esse membro era uma union, contudo, atualmente é uma `struct in_addr`, que basicamente é um inteiro de 4 bytes. Isso foi feito para fins de compatibilidade.

```C
struct in_addr {
  in_addr_t s_addr; // endereço de IP (versão 4)
};
```

O tipo `in_addr_t` é um typedef de um `uint32_t`. Lembre-se, um IP versão 4 é divido em quatro octetos (AAA.BBB.CCC.DDD), ou seja, quatro partes de 1 byte cada, com isso, o endereço completo possui 4 bytes de tamanho (os bytes devem estar ordenados pelo sistema big-endian).

Note, também, que a `struct sockaddr_in` possui um padding de 8 bytes. Esse padding faz com que essa struct possua o mesmo tamanho de uma `struct sockaddr`, logo, é totalmente seguro (e necessário) realizar um cast entre essas structs ao utilizá-las na API de sockets.

Definição `struct sockaddr_in6`:

```C
struct sockaddr_in6 {
  u_int16_t sin6_family;     // família de endereço, deve ser AF_INET6
  u_int16_t sin6_port;       // número da porta (big-endian)
  u_int32_t sin6_flowinfo;   // fluxo de informação do IPv6
  struct in6_addr sin6_addr; // enderço IPv6
  u_int32_t sin6_scope_id;   // identificador de escopo
};
```

Essa struct contém um endereço IPv6. Essa struct diverge um pouco, pois ela possui alguns membros específicos para endereço IPv6, como `sin6_flowinfo` e `sin6_scope_id`, que serão vistos posteriormente. O resto é bem padrão, a struct contém a família de endereço, número da porta e o endereço IP. Este último é uma `struct in6_addr`, que basicamente é um "número" de 16 bytes.

```C
struct in6_addr {
  uchar_t s6_addr[16]; // endereço de IP (versão 6)
};
```

Por mais que uma `struct sockaddr_int6` seja maior do que uma `struct sockaddr`, ainda é possível realizar o cast entre elas de forma segura, pois o tipo de estrutura é identificado pelo primeiro membro(família de endereço), que possui o mesmo offset para todas as structs sockaddr*. Além disso, quando se envia como parâmetro o ponteiro da struct convertido para o tipo desejado, é também passado o tamanho da struct.

Também há uma terceira struct para representar endereços: a `struct sockaddr_storage`. Essa struct contém tamanho suficiente para armazenar diversos tipos de endereço, inclusive IPv4 ou IPv6. Ela é útil quando não se sabe qual a família de endereço com que se está trabalhando. Sua única desvantagem é que ela é muito grande comparada com uma `struct sockaddr`.

Definição `struct sockaddr_storage`:

```C
struct sockaddr_storage {
  sa_family_t ss_family; // família de endereço
  char __ss_pad1[_SS_PAD1SIZE];
  int64_t __ss_align;
  char __ss_pad2[_SS_PAD2SIZE];
};
```

Os seus três últimos membros são paddings, ou seja, preenchimentos para que a struct tenha tamanho suficiente para todos as famílias de endereço. A definição de seus membros não importa muito, tanto que pode variar de sistema pra sistema, mas é importante entender seu conceito.

## Endereço de IP

Um IP nada mais é que uma sequência de bytes que identificam um dispositivo. Essa sequência varia em tamanho dependendo da versão, podendo ser de 4 bytes no caso do IPv4 ou 16 bytes no caso do IPv6. Exemplo:

| Versão | Tamanho  | Represetanção                             |
|:------:|:--------:|-------------------------------------------|
| IPv4   | 4 bytes  | `192.168.42.219`                          |
| IPv6   | 16 bytes | `2001:0db8:c9d2:0012:0000:0000:0000:0051` |

O IPv4 é dividido em quatro partes, cada uma contendo um número de 1 byte; por isso cada parte pode conter entre 0 e 255. Já o IPv6 é dividido em oito partes, cada uma contendo um número de 2 bytes; dessa forma cada parte pode conter entre 0 e 65535, ou 0 e 0xFFFF, já que é usada a base hexadecimal para representá-lo. Note que a representação mostrada acima é usada apenas por nós - humanos - pois fica mais fácil a visualização, contudo, os dispositivos "enxergam" o IP como uma sequência única de bits, sem divisórias. Para adquirir essa sequência, há funções que fazem a conversão da nossa representação, para a representação usada pelos dispositivos, como a função `inet_pton` (presentable to network). Essa função possui o seguinte cabeçalho:

```C
int inet_pton(int af, const char* src, void* dst);
```

| Parâmetro | Descrição |
|:---------:|-----------|
| `af`      | é a família de endereço; AF_INET ou AF_INET6 |
| `src`     | string que contém a representação visual do IP (127.0.0.1, por exemplo)  |
| `dst`     | buffer onde será armazenado o resultado binário ordenado como big-endian |

Retorna 1 em caso de sucesso; 0 caso a representação em `src` seja inválida; -1 se `af` for uma família de endereço inválida.

Exemplo de uso:

```C
struct sockaddr_in sa; // IPv4
inet_pton(AF_INET, "127.0.0.1", &sa.sin_addr);
```

Essa função só trabalha com endereços de IP e não com hostnames, como www.google.com. Há também as funções `inet_addr` e `inet_aton` para fazer essa conversão, contudo o uso é desencorajado, já que elas não suportam IPv6.

Para transformar o IP em sua representação binária para a nossa, basta usar a função `inet_ntop`, que possui o seguinte cabeçalho:

```C
const char *inet_ntop(int af, const void* src, char* dst, socklen_t size);
```

| Parâmetro | Descrição |
|----------:|-----------|
| `af`      | é a família de endereço; AF_INET ou AF_INET6 |
| `src`     | buffer onde está armazenado o resultado binário  |
| `dst`     | buffer onde será armazenado o resultado em represetanção legível |
| `size`    | tamanho do buffer `dst`, deve ser grande o suficiente para conter o resultado |

Retorna um ponteiro válido caso seja possível fazer a conversão; NULL caso `af` não seja uma família valida ou `size` seja muito pequeno para conter o resultado.

Exemplo de uso:

```C
char buf[32];
inet_ntop(AF_INET, &sa.sin_addr, buf, 32);
printf("IP: %s\n", buf);
```

## System Calls e Library Calls

A partir de syscalls ou da biblioteca padrão, é possível interagir com as funcionalidades de rede disponibilizadas pelo seu sistema.

## `getaddrinfo`

Essa função adquire informações de um determinado host e **serviço\*** que podem ser usadas em chamadas posteriores à API de sockets. Essas informações são carregadas em uma `struct addrinfo`, já vista anteriormente.

Definição `getaddrinfo`:

```C
int getaddrinfo(const char* node,
                const char* service,
                const struct addrinfo* hints,
                struct addrinfo** res);
```

| Parâmetro | Descrição |
|----------:|-----------|
| `node`    | é o host, exemplo `"www.google.com"` |
| `service` | especifica um serviço em particular, como `"http"` ou o número de porta  |
| `hints`   | critérios para o tipo de endereços retornados, como família de endereço, protocolo etc |
| `res`     | lista encadeada onde será retornado o resultado da função  |

Exemplo de uso:

```C
// Código sem validação de erros!!!

struct addrinfo* res;
struct addrinfo hints = {.ai_family = AF_UNSPEC, .ai_socktype = SOCK_STREAM};

getaddrinfo("www.google.com", "http", &hints, &res);

...

freeaddrinfo(res);
```

Caso não ocorra nenhum erro, a variável `res` conterá uma lista encadeada de `struct addrinfo`, isto é, uma lista de endereços com informações que podem ser utilizadas pela API de sockets. Nesse caso, estamos pegando apenas os endereços de sockets do tipo Socket Stream (SOCK_STREAM) independente da versão do IP (AF_UNSPEC), como especificado por `hints`. Note, também, que usamos a função `freeaddrinfo`, para liberar a memória da lista encadeada.

Lembre-se, uma `struct addrinfo` possui como um de seus membros uma `struct sockaddr`, que contém informações sobre o endereço do host. Dessa forma, é possível utilizar a função `inet_ntop` para adquirir a versão legível do IP. Como o resultado de `getaddrinfo` é um lista encadeada, podemos percorrer a lista e exibir cada um dos IPs do host.

Exemplo de uso:

```C
// buffer que contém o IP em formato legível
char buf[INET6_ADDRSTRLEN];

// se 'res' for NULL, chegamos ao final da lista
while(res) {
  // ponteiro para a struct de endereço; void*, pois pode ser tanto IPv4 quanto IPv6
  void* addr;

  // verifica a família de endereço
  if(res->ai_family == AF_INET)
    // se for IPv4, converte res->ai_addr para uma struct sockaddr_in*, depois pega
    // o endereço de sin_addr
    addr = &((struct sockaddr_in*)res->ai_addr)->sin_addr;
  else
    // se for IPv6, converte res->ai_addr para uma struct sockaddr_in6*, depois pega
    // o endereço de sin6_addr
    addr = &((struct sockaddr_in6*)res->ai_addr)->sin6_addr;

  // converte o IP de binário para uma string legível; armazena o resultado em 'buf'
  inet_ntop(res->ai_family, addr, buf, sizeof(buf));
  
  // imprime o IP na tela
  puts(buf);

  // próximo item da lista
  res = res->ai_next;
}
```

Note que antes de passar o endereço para `inet_ntop`, é necessário fazer a verificação da família de endereço para poder fazer o cast apropriado. Caso isso não fosse feito, o resultado seria um IP completamente diferente e errado, pois ele não levaria em consideração as diferenças de offset entre o tipos `struct sockaddr_in` e `struct sockaddr_in6`.

---

**\* serviço:** é tudo aquilo que um host fornece. Há diversos tipos de serviços, como http, ftp, dns etc. Cada serviço possui uma porta respectiva, que é padronizada.

---

## Criando o endpoint da conexão com `socket`

Essa função é extremamente importante, pois ela é o ponto de partida para adquirirmos o identificador de um socket(file descriptor) e começarmos a usá-lo como cliente ou servidor.

Definição de `socket`:

```C
int socket(int domain, int type, int protocol)
```

| Parâmetro  | Descrição |
|-----------:|-----------|
| `domain`   | é a família de endereço, no caso, AF_INET ou AF_INET6 |
| `type`     | é o tipo do socket, ou seja, Stream Socket(SOCK_STREAM) ou Datagram Socket(SOCK_DGRAM) |
| `protocol` | tipo do protocolo para determinado `type`; se for 0, o protocolo mais apropriado é escolhido |

Se a chamada ao sistema falhar, a função retorna -1. Caso tudo ocorra corretamente, `socket` retorna um inteiro indicando o file descriptor do socket, ou seja, seu identificador. Com esse identificador, é possível fazer diversas outras syscalls.

## Criando o servidor com `bind`, `listen` e `accept`

Caso você deseja criar um servidor, é necessário abordar primeiro a syscall `bind`. Essa função associa um determinado socket com um endereço, ou seja, o par IP/porta . Após o socket possuir um respectivo endereço, é possível colocá-lo para escutar por requisições de conexões.

Definição de `bind`:

```C
int bind(int sockfd, struct sockaddr* addr, socklen_t addrlen);
```

| Parâmetro  | Descrição |
|-----------:|-----------|
| `sockfd`   | é o file descriptor que identifica o socket; você o obtém com a syscall `socket` |
| `addr`     | endereço que será associado ao socket. Pode ser uma `struct sockaddr_in` ou `struct sockaddr_in6` |
| `addrlen`  | tamanho do parâmetro `addr` |

Há duas formas de criar o endereço para ser associado à um socket: da forma antiga, empacotando manualmente o endereço; e da forma mais atualizada, usando a função `getaddrinfo`. Primeiro mostrarei a mais atualizada.

```C
// Código sem verificação de erros!

struct addrinfo* res;   // resultado de getaddrinfo

struct addrinfo  hints = {0};     // critérios do endereço
hints.ai_family   = AF_UNSPEC;    // não especificada, pode ser tanto IPv4 quanto IPv6
hints.ai_socktype = SOCK_STREAM;  // socket do tipo Stream Socket (orientado a conexão)
hints.ai_flags    = AI_PASSIVE;   // AI_PASSIVE = use o endereço desta máquina

// primeiro parâmetro é NULL, pois dessa forma o endereço de IP escolhido será o "wildcard
// address", que basicamente significa qualquer IP disponível pertencente ao host.
getaddrinfo(NULL, "8081", &hints, &res);

// cria o socket com as características do endereço retornado por getaddrinfo
int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

// liga o socket com o endereço em res->ai_addr
bind(sockfd, res->ai_addr, res->ai_addrlen);

```

Note que dessa forma meu código se torna independente de uma família de IP, ou seja, em nenhum momento eu especifíco se é IPv4 ou IPv6. Além disso, essa forma permite configurações adicionais e é mais fácil, pois não é necessário montar o endereço manualmente.

A forma antiga seria da seguinte forma:

```C
  // Código sem verificação de erros!

  struct sockaddr_in addr = {0};     // struct que contém o endereço (IPv4)
  addr.sin_family = AF_INET;         // família IPv4
  addr.sin_port   = htons(8081);     // porta do serviço (big-endian, por isso o htons)
  addr.sin_addr.s_addr = INADDR_ANY; // wildcard IP

  // cria o socket
  int sockfd = socket(addr.sin_family, SOCK_STREAM, 0);
  
  // liga o socket com o endereço em 'addr'.
  // Observe que é necessário converter o endereço de addr(struct sockaddr_in*) para um
  // struct sockaddr*, que é a interface genérica que representa um endereço qualquer  
  bind(sockfd, (struct sockaddr*) &addr, sizeof(addr));
```

É preferível a versão que usa a função `getaddrinfo`, devido sua independência de família de endereços, além da liberdade de customização.

A syscall bind só serve para fazer a ligação de um socket com um endereço, dessa forma, você pode utilizar o socket para escutar por conexões nesse determinado endereço. Isso é feito com a syscall `listen`, que marca o socket como passivo, indicando que ele é um servidor e que esperará por conexões.

Definição de `listen`:

```C
int listen(int sockfd, int backlog);
```

| Parâmetro  | Descrição |
|-----------:|-----------|
| `sockfd`   | é o file descriptor que identifica o socket; você o obtém com a syscall `socket` |
| `backlog`  | é o tamanho máximo da fila de conexões pendentes de serem atendidas              |

Essa syscall é muito simples e seu uso se da dessa forma:

```C
// marcando o socket para escutar por conexões
listen(sockfd, 1);
```

Nesse caso, o socket indicado por `sockfd` é marcado para escutar por conexões. O `backlog` = 1 significa que o tamanho da fila de conexões pendentes é 1, isto é, se o servidor não conseguir atender em tempo hábil todas as requisições de conexão, elas ficarão nessa fila de pendência; se as requisições que chegarem não conseguirem entrar na fila, serão recusadas automaticamente.

As requisições são retiradas da fila com uma chamada à `accept`. Essa syscall retira a primeira conexão pendente da fila, popula uma variável com as informações do cliente e retorna um file descriptor para que seja possível interagir com ele.

Definição de `accept`:

```C
int accept(int sockfd, struct sockaddr* addr, socklen_t* addrlen);
```

| Parâmetro  | Descrição |
|-----------:|-----------|
| `sockfd`   | é o file descriptor que identifica o socket do servidor |
| `addr`     | ponteiro para uma struct sockaddr, aqui é onde será retornado informações do cliente |
| `addrlen`  | tamanho de `addr`. Após a chamada, é alterado para o tamanho da struct retornada em `addr`|

Uma chamada à `accept` responde uma chamada à `connect`, que será vista posteriormente. Caso nenhum conexão esteja na fila de espera, `accept` bloqueia até que uma esteja à caminho. Após aceitar uma requisição de conexão, é retornado em `addr` informações do cliente que acabou de conectar. Como não é possível assumir a família de endereço do cliente, geralmente é utilizada o tipo `struct sockaddr_storage`. O valor em `addrlen` deve ser inicialmente o tamanho da struct passada em `addr`, ou seja, `sizeof(struct sockaddr_storage)`.

Depois que `accept` retornar, o valor de `addrlen` é atualizado para o real tamanho da struct que contém o endereço do cliente, ou seja, se a família de endereço do cliente for IPv4, `addrlen` é atualizado de 128 bytes para 16 (sizeof struct sockaddr_in); se for IPv6, `addrlen` é atualizado de 128 bytes para 28 (sizeof struct sockaddr_in6).

Exemplo de uso:

```C
struct sockaddr_storage ss;
int size = sizeof(ss);
int clientfd = accept(sockfd, (struct sockaddr*) &ss, &size);
```

Os passos para criar um servidor foram mostrados. Basicamente seguem essa seguinte ordem:

- configuração do endereço com `getaddrinfo`;
- chamada à `socket` para adquirir o file descriptor do servidor;
- chamada à `bind` para associar o socket ao endereço configurado com `getaddrinfo`;
- chamada à `listen` para marcar o socket como pronto para escutar por requisição de conexões;
- chamada à `accept` para aceitar requisições pendentes.

## Criando o cliente

Como dito, um `accept` responde à um `connect`. Essa última syscall geralmente é usada pelo cliente que deseja se conectar à um servidor. Ela basicamente conecta um socket com um endereço especificado, fazendo com que seja possível utilizar esse socket para se comunicar com o host remoto que reside nesse endereço.

Definição de `connect`:

```C
int connect(int sockfd, struct sockaddr* serv_addr, int addrlen);
```

| Parâmetro  | Descrição |
|-----------:|-----------|
| `sockfd`   | é o file descriptor que identifica o socket que se conectará à `addr`|
| `serv_addr`| informações de endereço do servidor |
| `addrlen`  | tamanho de `serv_addr`|

Ser um cliente é muito simples, primeiro é necessário adquirir informações do endereço que deseja se conectar, utilizando `getaddrinfo` para isso. Depois é necessário adquirir um socket apropriado à esse endereço com `socket`. Por fim, basta conectar o socket com o endereço, utilizando `connect`.

Exemplo de uso:

```C
// Código sem verificação de erros!!!

struct addrinfo hints = {0};
hints.ai_family   = AF_UNSPEC;   // IPv4 ou OPv6
hints.ai_socktype = SOCK_STREAM; // Socket orientado à conexão

struct addrinfo* res; // resultado de getaddrinfo
getaddrinfo("localhost", "8080", &hints, &res);

// cria o socket com base nas informações retornadas por getaddrinfo
int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

// conecta o socket ao endereço
connect(sockfd, res->ai_addr, res->ai_addrlen);
```

Quando chamamos `connect` passando o endereço desejado, um `accept` respectivo retira da fila a nossa conexão pendente, colocando em uma `struct sockaddr_storage` nossas informações, como endereço e porta (sim, também temos uma porta, que é selecionada automaticamente pelo kernel). Note que construímos o endereço do host remoto usando a função `getaddrinfo`, isso também poderia ser feito manualmente, atribuindo valores à uma sockaddr_in(6), contudo essa é uma forma antiga de se fazer.

## Enviando e recebendo dados com `send` e `recv`

Agora que cliente e servidor estão conectados, podemos utilizar as syscalls `send` e `recv` para enviar e receber dados, respectivamente.

Definição de `send`:

```C
ssize_t send(int sockfd, const void* buf, size_t len, int flags);
```

| Parâmetro  | Descrição |
|-----------:|-----------|
| `sockfd`   | é o file descriptor que identifica o socket para qual as informações serão enviadas |
| `buf`      | buffer que contém as informações a serem enviadas |
| `len`      | tamanho de `buf` |
| `flags`    | configurações que mudam o comportamento do envio |

A função retorna a quantidade de bytes enviados; -1 em caso de erro. Se o buffer interno de `send` estiver lotado, não será possível enviar a mensagem de imediato, dessa forma a função bloqueia, até que haja espaço disponível.

Exemplo de uso:

```C
...

char buffer[] = "Ola, seja bem vindo ao servidor!";
send(clientfd, buffer, sizeof(buffer), 0);
```

Nesse exemplo de `send`, o servidor está enviando ao cliente, representando pelo `clientfd`, uma mensagem contida em `buffer` de tamanho `sizeof(buffer)`. Foi passado 0 como flag, ou seja, sem nenhuma configuração que altere o funcionamento da função. Por exemplo, `send` pode bloquear caso seu buffer interno esteja lotado, nesse caso, a flag `MSG_DONTWAIT` poderia ser passada, fazendo com que a função retorna imediatamente.

Outra flag útil é a `MSG_NOSIGNAL`; caso o outro lado (nesse caso, o cliente) interrompa a conexão de forma repentina, um sinal de EPIPE é enviado do kernel à aplicação servidor, por tentar utilizar um socket "quebrado", com isso, o processo é encerrado prematuramente. Esse comportamente de término pode ser evitado ao passar essa flag.

O uso da syscall `recv` é bem parecido. Ela tem o seguinte cabeçalho:

```C
ssize_t recv(int sockfd, void* buf, size_t len, int flags);
```

| Parâmetro  | Descrição |
|-----------:|-----------|
| `sockfd`   | é o file descriptor que identifica o socket de as informações serão recebidas |
| `buf`      | buffer que conterá as informações recebidas |
| `len`      | tamanho de `buf` |
| `flags`    | configurações que mudam o comportamento do recebimento |

A função retorna a quantidade de bytes recebidos; 0 caso receba EOF, ou seja, a conexão foi encerrada; -1 em caso de erro. Caso não haja nada pra ser recebido, a syscall bloqueia. Note que é basicamente a syscall `send`, mas no sentido contrário. Exemplo de uso:

```C
...

char buffer[1024];
int bytes = recv(clientfd, buf, sizeof(buf), 0);
```

Nesse exemplo, a função fica esperando receber informações vindas de `clientfd`. As informações serão armazenadas em `buf`, que pode conter no máximo `sizeof(buf)` bytes de uma única vez. É retornado pra variável `bytes` a quantidade de bytes recebidos. Assim como `send`, é possível usar as flags `MSG_NOSIGNAL` e `MSG_DONTWAIT` para os mesmos propósitos.

## Encerrando conexão com `close` ou `shutdown`

Caso necessite encerrar uma conexão, você pode fazer isso com as funções `close` e `shutdown`.

Definição de `close`:

```C
int close(int fd);
```

| Parâmetro  | Descrição |
|-----------:|-----------|
| `fd`   | é o file descriptor que será fechado, ou seja, um endpoint da conexão |

Qualquer tentativa de escrita ou leitura por parte do host remoto que se comunique com esse socket falhará. Por exemplo, se um cliente chamar `close` no socket usado para se comunicar com o servidor, qualquer chamada à `recv` feita pelo servidor que se refira à este cliente retornará 0, pois a outra parte(cliente) fechou a conexão. Caso `close` seja interrompido por um sinal, falhará retornando -1 e configurando errno apropriadamente.

A função `shutdown`, por sua vez, consegue encerrar parcialmente uma conexão, isto é, apenas uma de suas "vias", ou seja, é possível encerrar a via de escrita, enquanto a de leitura continua disponível e vice-versa.

Definição de `shutdown`:

```C
int shutdown(int fd, int how);
```

| Parâmetro  | Descrição |
|-----------:|-----------|
| `fd`   | é o file descriptor que será fechado, ou seja, um endpoint da conexão |
| `how`  | a forma como esse endpoint será encerrado. SHUT_RD para encerrar a leitura; SHUT_WR para encerrar a escrita; e SHUT_RDWR para encerrar ambas as vias (basicamente um `close`). |

## Servidor e cliente UDP (Datagram Socket)

Até agora vimos apenas os Stream Sockets, ou seja, um tipo de socket que é orientado à conexão; o protocolo padrão para esse tipo de socket é o TCP. Contudo, há também o tipo Datagram Sockets, que não possui uma conexão/sessão estabelecida entre os hosts da comunicação e é menos seguro que o stream socket; o protocolo padrão desse tipo é o UDP.

Como não há uma conexão, o servidor não deve utilizar as funções `listen` e `accept`, mas ainda é necessário usar o `bind`, para atrelar o servidor com um endereço. Para que o servidor receba as informações de um cliente qualquer, ele utiliza a função `recvfrom`. Exemplo:

Definição de `recvfrom`:

```C
ssize_t recvfrom(int sockfd, void* buf, size_t len, int flags,
                 struct sockaddr* src_addr, socklen_t* addrlen);
```

| Parâmetro  | Descrição |
|-----------:|-----------|
| `sockfd`   | socket que ficará esperando por informações vindas de qualquer host remoto |
| `buf`      | buffer onde as informações recebidas serão armazenadas |
| `len`      | tamanho de `buf` |
| `flags`    | flags que alteram o comportamento da função, assim como em `recv`
| `src_addr` | ptr p/ a struct que será preenchida com informações de endereço do host remoto |
| `addrlen`  | inicialmente dever ser preenchido com o tamanho da struct passada em `src_addr`, depois que a função retornar, seu valor é alterado para o real tamanho de `src_addr`, assim como em `recv` |

Um servidor UDP pode ser construido dessa forma:

```C
struct addrinfo hints = {0};
hints.ai_family = AF_UNSPEC; // IPv4 ou IPv6
hints.ai_socktype = SOCK_DGRAM; // datagram socket (udp)

struct addrinfo* res;
getaddrinfo(..., &res); // chamada igual às já vistas até agora

int sockfd = socket(res->ai_family, ...); // chamada igual às já vistas até agora

bind(sockfd, ...); // chamada igual às ja vistas até agora;

char buffer[1024];

struct sockaddr_storage ss = {0};
while(1) { // loop infinito para receber os dados
  unsigned size = sizeof(ss);
  int recv_bytes = recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr*) &ss, &size);

  // após receber uma mensagem, 'ss' é preenchida com informações sobre o host remoto que
  // enviou os dados e 'size' tem seu valor alterado para o tamanho da struct que representa
  // o endereço (sockaddr_in ou sockaddr_in6).

  printf("Mensagem recebida: %s!\n", buffer); // exibe a mensagem recebida
}
```

Note que antes de esperar por dados, nenhuma conexão é estabelecida com nenhum cliente, dessa forma, `recvfrom` esperará por dados enviados de qualquer um.

Um cliente UDP é mais simples ainda, basta criar o socket para servir de endpoint da comunicação e usar a função `sendto`.

Definição de `sendto`:

```C
ssize_t sendto(int sockfd, const void* buf, size_t len, int flags,
               const struct sockaddr* dest_addr, socklen_t addrlen);
```

| Parâmetro  | Descrição |
|-----------:|-----------|
| `sockfd`   | socket que serve de endpoint para enviar os dados |
| `buf`      | buffer que contém os dados a serem enviados |
| `len`      | tamanho de `buf` |
| `flags`    | flags que alteram o comportamento da função, assim como em `send`
| `dest_addr`| ptr p/ a struct que será preenchida com informações de endereço do servidor |
| `addrlen`  | tamanho da struct passada em `dest_addr` |

Exemplo de cliente UDP:

```C
struct addrinfo hints = {0};
hints.ai_family = AF_UNSPEC; // IPv4 ou IPv6
hints.ai_socktype = SOCK_DGRAM; // Datagram Socket (UDP)

struct addrinfo* res;
getaddrinfo(..., &res); // assim como já demonstrado

int sockfd = socket(res->ai_family, ...); // assim como já demonstrado

char buffer[1024] = "ola, exemplo de mensagem!";

int send_bytes = sendto(sockfd, buffer, strlen(buffer) + 1, 0, res->ai_addr, res->ai_addrlen);
```

Uma coisa interessante é que após o servidor receber os dados de um host remoto, é possível que ele utilize o mesmo endpoint de comunicação(socket) para enviar dados para esse mesmo host utilizando as informações de endereço retornadas nos dois últimos parâmetro de `recvform`. Exemplo:

```C
// Servidor

...

recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr*) &ss, &size);

// utilizo o mesmo endpoint para enviar uma mensagem para o host remoto que acabou
// de se comunicar com o servidor (suas informações de enderçeo estão em 'ss')
sendto(sockfd, "ola, recebi sua msg", 20, 0, (struct sockaddr*) &ss, size);
```
