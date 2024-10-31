#pragma once
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

class Monitor
{
public:
    Monitor();
    void write_file(char data_send[100], char data_rcv[100], int client_id);
    void read_file();
    ~Monitor();

private:
    pthread_mutex_t mutex;
    FILE *log_file;
};

