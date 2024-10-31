// Implementacao do servidor
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "Monitor.h"

#define PORT 8080
#define QTD_CLIENTES 10
#define BACKLOG_SIZE 10
#define BUFFER_SIZE 100

int cliente_fd[QTD_CLIENTES];
Monitor monitor_log;
clock_t start;

// Thread para exibir os logs do sistema a cada 10 segundos
void* thread_log_function(void* p){
	while(true){
		sleep(10);
		monitor_log.read_file();
	}
	return NULL;
}

void* thread_cliente(void *p){
	int fd = (int)(long) p; //-> Recebe o file descriptor do cliente

	// Variaveis para as mensagens
	char send_msg[BUFFER_SIZE];
	char welcome_msg[] = "Hello, Client!\n";

	time_t t;
	srand(time(&t));
	

	// Mensagem de Boas Vindas
	send(fd, welcome_msg, strlen(welcome_msg), 0);
	
	// Loop para recebimento de forma intermitente de mensagens de cada cliente
	while(true){
		// Recendo a mensagem do cliente
		char rcv_msg[BUFFER_SIZE];
		recv(fd, rcv_msg, BUFFER_SIZE, 0);

		// DATETIME
		if(rcv_msg[0] == 'D'){
			
			// Calculo da hora
			time_t segundos = time(NULL);
			struct tm *tm_info = localtime(&segundos);	
			strftime(send_msg, 100, "%Y-%m-%d %H:%M:%S\n", tm_info);
			
			// Envio mensagem para cliente
			send(fd, send_msg, strlen(send_msg), 0);

			//Preenchendo o log
			monitor_log.write_file(send_msg, rcv_msg, fd);
		}
		// RNDNUMBER
		else if(rcv_msg[0] == 'R'){
			int valor = rand()%100 + 1;
			sprintf(send_msg, "%d\n", valor);

			send(fd, send_msg, strlen(send_msg), 0);

			//Preenchendo o log
			monitor_log.write_file(send_msg, rcv_msg, fd);
		}
		// UPTIME
		else if(rcv_msg[0] == 'U'){
			//strcpy(send_msg)
			double tempo_execucao;
			
			tempo_execucao = ((double) (clock() - start) / CLOCKS_PER_SEC) * 10000;

			sprintf(send_msg, "Servidor em execucao ha %.1f segundos\n",tempo_execucao);

			send(fd, send_msg, strlen(send_msg), 0);
			//Preenchendo o log
			monitor_log.write_file(send_msg, rcv_msg, fd);
		}
		// INFO
		else if(rcv_msg[0] == 'I'){
			strcpy(send_msg, "Servidor TCP v1.0\n");
			
			send(fd, send_msg, strlen(send_msg), 0);

			//Preenchendo o log
			monitor_log.write_file(send_msg, rcv_msg, fd);
		}
		// BYE
		else if(rcv_msg[0]== 'B'){
			strcpy(send_msg, "Conexao Encerrada\n");
			send(fd, send_msg, strlen(send_msg), 0);
			
			//Preenchendo o log
			monitor_log.write_file(send_msg, rcv_msg, fd);
			break;
		}
	}
	close(fd);

	return NULL;
}

int main(){
	int serv_fd;
    int server_fd;
    struct sockaddr_in serv_addr;
	struct sockaddr_in cli_addr[QTD_CLIENTES];
	int cli_addr_size[QTD_CLIENTES];

	int cli_count=0;

	pthread_t cli_threads[QTD_CLIENTES];
	pthread_t thread_log;

	start = clock();

	pthread_create(&thread_log, NULL, thread_log_function, NULL);

	if ((serv_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("socket()");
		exit(EXIT_FAILURE);
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	serv_addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(serv_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
		perror("bind()");
		exit(EXIT_FAILURE);
	}

	if (listen(serv_fd, BACKLOG_SIZE) < 0) {
		perror("listen()");
		exit(EXIT_FAILURE);
	}

	printf("Servidor aguardando conexoes na porta %d\n", PORT);
	
	// Loop de execucao do servidor
	while(true){
		struct sockaddr_in cli_addr_temp;
		int cli_addr_c_temp = 0;
		cliente_fd[cli_count] = accept(serv_fd, (struct sockaddr*)&cli_addr_temp,  (socklen_t*)&cli_addr_c_temp);

		if (cliente_fd[cli_count] < 0) {
			perror("accept()");
			exit(EXIT_FAILURE);
		}
		pthread_create(&cli_threads[cli_count], 0, thread_cliente, (void*)(long)cliente_fd[cli_count]);
		cli_count++;
	}
}