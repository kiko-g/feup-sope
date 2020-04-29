#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <pthread.h> 
#include <errno.h>
#include "U1.h"
#include "../parser/parser.h"
#include "../utils/utils.h"

int i = 1;
int end = 0;

void *client_thread_task(void *arg) {
    // open public fifo
    char *public_fifo = arg;
    int fd_public = open(public_fifo, O_WRONLY);
    if(fd_public == -1) {
        end = 1;
        log_operation(i, (int) getpid(), (long) pthread_self(), -1, -1, FAILD); //TODO: check if it's this operation
        return NULL;
    }

    // send random request
    int time_client = (rand() %  (UPPER_TIME - LOWER_TIME + 1));
    log_operation(i, (int) getpid(), (long) pthread_self(), -1, -1, IWANT);
    send_message(fd_public, i, (int) getpid(), (long) pthread_self(), time_client, -1);
    close(fd_public);


    // create private fifo
    char private_fifo[MAX_LEN];
    make_private_fifo(private_fifo);
    if(mkfifo(private_fifo, 0660) < 0) {
        if(errno == EEXIST) {
        }
        else {
            char error_msg[MAX_LEN];
            sprintf(error_msg, "Error creating private FIFO %s\n", private_fifo);
            write(STDOUT_FILENO, error_msg, strlen(error_msg));
            exit(1);
        }
    }

    // open private fifo
    int fd_private = open(private_fifo, O_RDONLY); //rw
    if(fd_private == -1) {
            char error_msg[MAX_LEN];
            sprintf(error_msg, "Error opening private FIFO %s with O_RDONLY\n", private_fifo);
            write(STDOUT_FILENO, error_msg, strlen(error_msg));
            exit(1);
    }

    // read the server's response from the private fifo
    char server_response[MAX_LEN];
    if(read(fd_private, server_response, MAX_LEN) < 0) {
        log_operation(i, (int) getpid(), (long) pthread_self(), time_client, -1, FAILD);
        return NULL;
    }

    // parse the server's response
    int idup, pid, duration, place;
    long tid;
    sscanf(server_response, "[ %d, %d, %ld, %d, %d ]", &idup, &pid, &tid, &time_client, &place);

    // check if a place was given and log it
    if(time_client == -1 && place == -1) {
        log_operation(i, (int) getpid(), (long) pthread_self(), time_client, -1, CLOSD);
    }   
    else {
        log_operation(i, (int) getpid(), (long) pthread_self(), time_client, -1, IAMIN);
    }

    close(fd_private);
    unlink(private_fifo);
    return NULL;
}

void make_private_fifo(char *fifo_name) {
    sprintf(fifo_name, "/tmp/%d.%d", (int) getpid(), (int) pthread_self());
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
        pthread_create(&threads[num_threads], NULL, client_thread_task, client_args.fifoname);
        i++;
        num_threads++;
        usleep(50*1000);
    }

    pthread_exit(0);
}


