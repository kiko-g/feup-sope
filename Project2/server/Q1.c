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

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

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
    int fd_private;
    if((fd_private = open(private_fifo, O_WRONLY)) == -1) {
        log_operation(i, pid, tid, dur, -1, GAVUP);
        return NULL;
    }

    // allocate client and send message
    pthread_mutex_lock(&mutex);
    int allocated_place = current_place;
    current_place++;
    pthread_mutex_unlock(&mutex);

    send_message(fd_private, i, (int) getpid(), pthread_self(), dur, allocated_place);
    close(fd_private);

    log_operation(i, (int) getpid(), pthread_self(), dur, allocated_place, ENTER);

    // wait for the client in the bathroom
    usleep(dur*1000);

    log_operation(i, (int) getpid(), pthread_self(), dur, allocated_place, TIMUP);

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
    if(fd_public == -1) {
        char error_msg[MAX_LEN];
        sprintf(error_msg, "Error opening public FIFO\n");
        write(STDOUT_FILENO, error_msg, strlen(error_msg));
        unlink(server_args.fifoname);
        exit(1);
    }

    server_thread_arguments thread_args;
    thread_args.max_duration = server_args.nsecs;
    char client_msg[MAX_STR_LEN];
    pthread_t t;

    while(timer_duration() < server_args.nsecs) {
        if(read(fd_public, &client_msg, MAX_STR_LEN) > 0 && client_msg[0] == '[') {
            strcpy(thread_args.msg, client_msg);
            pthread_create(&t, NULL, server_thread_task, (void *) &thread_args);
            pthread_detach(t);
        }
    }

    // TODO: check if this step is necessary
    if(read(fd_public, &client_msg, MAX_STR_LEN) > 0 && client_msg[0] == '[') {

        // parse private fifo name and client's message
        int i, pid, dur;
        long tid;
        sscanf(client_msg, "[ %d, %d, %ld, %d, -1 ]", &i, &pid, &tid, &dur);
        log_operation(i, pid, tid, dur, -1, RECVD);

        char private_fifo[MAX_LEN];
        build_private_fifo(private_fifo, pid, tid);

        // open the private fifo (if possible)
        int fd_private;
        if((fd_private = open(private_fifo, O_WRONLY)) == -1) {
            log_operation(i, pid, tid, dur, -1, GAVUP);
        }
        else {
            send_message(fd_private, i, (int) getpid(), pthread_self(), -1, -1);
            close(fd_private);
            log_operation(i, pid, tid, dur, -1, TLATE);
        }

    }

    close(fd_public);
    unlink(server_args.fifoname);

    pthread_exit((void *)0);
}


void build_private_fifo(char *fifo_name, int pid, int tid) {
    sprintf(fifo_name, "/tmp/%d.%d", pid, tid);
}