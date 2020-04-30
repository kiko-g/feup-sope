#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <pthread.h> 
#include "Q1.h"
#include "../parser/parser.h"
#include "../utils/utils.h"

int current_place = 1;

void *server_thread_task(void *arg) {
    server_thread_arguments *thread_args = (server_thread_arguments*) arg;

    // parse private fifo name and client's message
    int i, pid, dur;
    long tid;
    sscanf(thread_args->msg, "[ %d, %d, %ld, %d, -1 ]", &i, &pid, &tid, &dur);
    log_operation(i, pid, tid, dur, -1, RECVD);

    char private_fifo[MAX_LEN];
    build_private_fifo(private_fifo, pid, tid);

    // open the private fifo (if possible)
    int fd_private, num_tries = 0;
    do {
        fd_private = open(private_fifo, O_WRONLY);
        num_tries++;
        usleep(500);
    } while(fd_private == -1 && num_tries < 5);
    
    if(fd_private == -1) {
        log_operation(i, pid, tid, dur, -1, GAVUP);
        return NULL;
    }

    // allocate client and send message
    char client_allocation[MAX_LEN];
    if(thread_args->max_duration = timer_duration() + dur) {
        sprintf(client_allocation, "[ %d, %d, %ld, %d, %d ]", i, (int) getpid(), (long) pthread_self(), dur, current_place);
        log_operation(i, (int) getpid(), pthread_self(), dur, current_place, ENTER);

        // TODO: mutex lock
        current_place++;
        // TODO: mutex unlock

        write(fd_private, &client_allocation, MAX_LEN);

        // wait for the client in the bathroom
        usleep(dur*1000);

        // TODO: free the space
    }
    else {
        sprintf(client_allocation, "[ %d, %d, %ld, %d, %d ]", i, (int) getpid(), (long) pthread_self(), -1, -1);
        log_operation(i, (int) getpid(), pthread_self(), -1, -1, TLATE);
        log_operation(i, (int) getpid(), pthread_self(), -1, -1, TIMUP); // TODO: is it here ???
    }

    close(fd_private);
    return NULL;
}

int main(int argc, char* argv[]){

    struct ServerArgs server_args; 
    if(parse_server_args(argc, argv, &server_args)){
        perror("Error parsing server args");
        exit(1);
    }

    timer_begin();
    if(mkfifo(server_args.fifoname, 0660) < 0) {
        if(errno == EEXIST) {
        }
        else {
            char error_msg[MAX_LEN];
            sprintf(error_msg, "Error creating public FIFO\n");
            write(STDOUT_FILENO, error_msg, strlen(error_msg));
            exit(1);
        }
    }

    int fd_public = open(server_args.fifoname, O_RDONLY | O_NONBLOCK); // NONBLOCK so that we don't need to wait for client
    printf("FIFO NAME:%s\n", server_args.fifoname);
    if(fd_public == -1) {
        char error_msg[MAX_LEN];
        sprintf(error_msg, "Error opening public FIFO\n");
        write(STDOUT_FILENO, error_msg, strlen(error_msg));
        unlink(server_args.fifoname);
        exit(1);
    }

    int num_threads = 0;
    pthread_t threads[MAX_THREADS];

    server_thread_arguments thread_args;
    thread_args.max_duration = server_args.nsecs;
    char client_msg[MAX_STR_LEN];

    while(timer_duration() < server_args.nsecs) {
        if(read(fd_public, &client_msg, MAX_STR_LEN) > 0) {
            strcpy(thread_args.msg, client_msg);
            pthread_create(&threads[num_threads], NULL, server_thread_task, (void *) &thread_args);
            num_threads++;
        }
    }

    close(fd_public);
    unlink(server_args.fifoname);

    pthread_exit(0);
}


void build_private_fifo(char *fifo_name, int pid, int tid) {
    sprintf(fifo_name, "/tmp/%d.%d", pid, tid);
}