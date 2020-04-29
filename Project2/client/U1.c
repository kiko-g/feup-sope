#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>
#include <pthread.h> 
#include "../parser/parser.h"
#include "../utils/utils.h"

#define MAX_THREADS 128
#define UPPER_TIME 30
#define LOWER_TIME 5

int i = 1;
int end = 0;

void *thread_task(void *arg) {
    char *fifo = arg;
    int fd_public = open(fifo, O_WRONLY);

    int time_client = (rand() %  (UPPER_TIME - LOWER_TIME + 1));

    if(fd_public == -1) {
        end = 1;
        log_operation(i, (int) getpid(), (long) pthread_self(), -1, -1, CLOSD);
        return NULL;
    }

    send_message(fd_public, i, (int) getpid(), (long) pthread_self(), time_client, -1);
    close(fd_public);


    // create private fifo

    // open private fifo

    // read from private fifo the server's response

    // parse the server's response

    // check if a place was given and log it

    // close fd and unlink fifo
}

int main(int argc, char* argv[]){

    srand(time(0));

    struct ClientArgs client_args; 
    if(parse_client_args(argc, argv, &client_args)) {
        perror("Error parsing client args");
        exit(1);
    }

    int num_threads = 0;
    pthread_t threads[MAX_THREADS];

    timer_begin();
    while(timer_duration() < client_args.nsecs) {
        pthread_create(&threads[num_threads], NULL, thread_task, client_args.fifoname);
        i++;
        num_threads++;
        usleep(50*1000);
    }

    pthread_exit(0);
}


