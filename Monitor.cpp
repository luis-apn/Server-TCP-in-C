#include "Monitor.h"

Monitor::Monitor()
{
    pthread_mutex_init(&mutex, NULL);
}

Monitor::~Monitor()
{
    pthread_mutex_destroy(&mutex);
}

void Monitor::read_file(){

    pthread_mutex_lock(&mutex);
    log_file = fopen("log.txt", "r");

    if(log_file == NULL){
        perror("Error fopen\n");
        exit(EXIT_FAILURE);
    }

    char log_line[200];

    printf("--------------------------------------- LOGs DO SERVIDOR ---------------------------------------\n");
    while(fgets(log_line, 200, log_file) != NULL){
        printf("%s", log_line);
    }

    printf("-------------------------------------------------x-----------------------------------------------\n");

    fclose(log_file);

    pthread_mutex_unlock(&mutex);
}

void Monitor::write_file(char data_send[100], char data_rcv[100], int client_id){

    // Aciona a trava
    pthread_mutex_lock(&mutex);

    // Cria/Abre o arquivo para adicionar no final
    log_file = fopen("log.txt", "a+");

    char date[26];
    time_t segundos = time(NULL);
    struct tm *tm_info = localtime(&segundos);	
    strftime(date, 100, "[%Y-%m-%d %H:%M:%S]", tm_info);

    if(log_file == NULL){
        perror("Error fopen\n");
        exit(EXIT_FAILURE);
    }
    // Ajustando as string para escrever no arquivo
    char new_char[20];
    if(data_rcv[0] == 'D')
        strcpy(new_char, "DATETIME");
    else if(data_rcv[0] == 'R')
        strcpy(new_char, "RNDNUMBER");
    else if(data_rcv[0] == 'U')
        strcpy(new_char, "UPTIME");
    else if(data_rcv[0] == 'D')
        strcpy(new_char, "DATETIME");
    else if(data_rcv[0]== 'I')
        strcpy(new_char, "INFO");
    else if(data_rcv[0] == 'B')
        strcpy(new_char, "BYE");    


    fprintf(log_file, "%s Cliente: %d: Comando =  %s, Resposta = %s", date, client_id, new_char, data_send);

    fclose(log_file);

    pthread_mutex_unlock(&mutex);
}