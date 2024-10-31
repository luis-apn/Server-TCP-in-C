# Implementação de Servidor com Sockets em C++

Este projeto implementa um servidor TCP básico em C/C++ utilizando sockets, com funcionalidade de log de atividades usando threads. A comunicação pode ocorrer entre vários clientes(10 no máximo definido pela macro  ```QTD_CLIENTES```) e o servidor, com funcionalidades de resposta baseadas em comandos pré-definidos.

## Introdução a Sockets em C/C++

Sockets são interfaces de comunicação que permitem a troca de dados entre dispositivos conectados a uma rede. Na implementação com sockets TCP em C/C++, a comunicação é baseada em três etapas principais: criação do socket, ligação (`bind`) a uma porta e aceitação (`accept`) de conexões de clientes.

---

## Estrutura do Projeto

### Arquivos Principais
- `server.cpp`: Implementação do servidor, que aguarda conexões de clientes e responde a comandos específicos.
- `Monitor.cpp` e `Monitor.h`: Implementação de uma classe para manipulação de logs de atividades com controle de concorrência via mutex evitando que múltiplas threads de clientes acessem e modifiquem o arquivo de forma arbitrária. 
- `Makefile`: Script para compilar, limpar e executar o projeto.

---

## Descrição das Funções

### server.cpp

1. **Definições e Variáveis Globais**: Configura a porta, quantidade de clientes, tamanho do buffer, e inicializa variáveis e objetos para controlar clientes e logs.

2. **`void* thread_log_function(void* p)`**: 
   - Função executada em uma thread separada, que exibe o log do servidor a cada 10 segundos. Utiliza a função `read_file()` do monitor de logs.

3. **`void* thread_cliente(void *p)`**:
   - Função responsável por lidar com cada cliente conectado.
   - **Mensagens de Comandos**:
     - **`D` - DATETIME**: Retorna a data e hora atual ao cliente.
     - **`R` - RNDNUMBER**: Retorna um número aleatório entre 1 e 100.
     - **`U` - UPTIME**: Retorna o tempo em segundos desde o início da execução do servidor.
     - **`I` - INFO**: Informa a versão do servidor.
     - **`B` - BYE**: Encerra a conexão com o cliente.
   - Cada resposta é registrada no log usando a função `write_file()` do monitor de logs.

4. **`int main()`**: 
   - Função principal, onde o servidor é inicializado e configurado.
   - Configura o socket, realiza o `bind` e `listen`, e cria uma thread para cada cliente conectado, além da thread de log.

### Monitor.cpp e Monitor.h

A classe `Monitor` é responsável por gerenciar e armazenar logs de atividades do servidor, mantendo a integridade dos dados mesmo em um ambiente multithreaded. A implementação garante que apenas uma thread por vez possa acessar ou modificar o arquivo de log (`log.txt`), sendo assim **thread-safe**. 

#### Principais Funções e Estrutura

1. **Variável de Mutex (`pthread_mutex_t mutex`)**:
   - O mutex (`mutex`) é uma trava que controla o acesso concorrente ao arquivo de log.
   - Ao acessar o arquivo, a função bloqueia o mutex, garantindo que somente uma thread tenha acesso ao arquivo até que a operação termine. Após a operação, o mutex é desbloqueado, permitindo que outra thread possa acessá-lo.
  
2. **Construtor (`Monitor()`)**:
   - Inicializa o mutex usando `pthread_mutex_init`, garantindo que ele esteja pronto para uso e previne possíveis problemas de concorrência.
  
3. **Destrutor (`~Monitor()`)**:
   - Destrói o mutex ao final da execução, liberando recursos.

4. **`void read_file()`**:
   - Esta função é chamada a cada 10 segundos por uma thread específica para ler e exibir o conteúdo do log no terminal.
   - O mutex é bloqueado antes de abrir o arquivo e desbloqueado após a leitura, garantindo que o acesso seja exclusivo.
   - A função abre o `log.txt` em modo de leitura e imprime cada linha no terminal. Ela apresenta uma borda decorativa para melhorar a visualização.

5. **`void write_file(char data_send[100], char data_rcv[100], int client_id)`**:
   - Esta função é chamada toda vez que um cliente envia um comando ao servidor.
   - **Etapas da Função**:
     1. **Bloqueio do Mutex**: Garante que somente a thread atual possa acessar o arquivo durante a gravação.
     2. **Abertura do Arquivo**: O arquivo é aberto em modo de adição (`a+`), permitindo que cada nova entrada seja registrada ao final do arquivo.
     3. **Formatação e Gravação**:
        - A função coleta informações de data e hora e identifica o comando recebido.
        - Formata uma linha de log que inclui o horário, ID do cliente, comando e resposta.
        - Exemplo de linha de log: `[YYYY-MM-DD HH:MM:SS] Cliente: <client_id>: Comando = <comando>, Resposta = <resposta>`.
     4. **Desbloqueio do Mutex**: Após a gravação, o mutex é liberado, permitindo que outras threads possam acessar o arquivo.
     
   - Esse fluxo garante que logs sejam gravados de forma consistente, mesmo quando múltiplos clientes estão conectados e enviando comandos simultaneamente.

---

Essa implementação da classe `Monitor` garante que o sistema de logs seja seguro para múltiplas threads e fornece um histórico confiável das atividades dos clientes conectados ao servidor, permitindo a auditoria de ações realizadas no servidor de forma organizada e precisa.


## Compilação e Execução

### Usando o Makefile

Para facilitar a compilação e execução, o Makefile define os seguintes comandos:

- **`make all`** ou **`make monitor`**: Compila o projeto, gerando o executável `server`.
- **`make clean`**: Remove arquivos gerados durante a compilação (`server` e `Monitor.o`).
- **`make run`**: Executa o servidor.
- **`make clear`**: Limpa o terminal.

---

### Como Compilar e Executar

1. Compile o projeto:
```bash
   make all
```

2. Execute o servidor:
```bash
  make run
```

3. Para limpar os arquivos de build:
```bash
make clean
```
4. Para limpar o terminal:
```bash
make clear
```
