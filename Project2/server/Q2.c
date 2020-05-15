#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <pthread.h> 
#include <semaphore.h>
#include "Q1.h"
#include "../parser/parser.h"
#include "../utils/utils.h"
#include "queue.h"

int current_place = 1;
pthread_mutex_t mutex_place = PTHREAD_MUTEX_INITIALIZER;
sem_t sem_nthreads,sem_nPlaces;
struct ServerArgs server_args; 
Queue * queue;

void *server_thread_task(void *arg) {
    char* client_msg = (char*) arg;

    // parse private fifo name and client's message
    int i, pid, dur;
    long tid;
    sscanf(client_msg, "[ %d, %d, %ld, %d, -1 ]", &i, &pid, &tid, &dur);
    log_operation(i, pid, tid, dur, -1, RECVD);

    char private_fifo[MAX_LEN];
    build_private_fifo(private_fifo, pid, tid);

    // open the private fifo (if possible)
    int fd_private;
    if((fd_private = open(private_fifo, O_WRONLY | O_NONBLOCK)) == -1) {
        log_operation(i, pid, tid, dur, -1, GAVUP);

        if(server_args.nthreads) sem_post(&sem_nthreads);
        return NULL;
    }

    // check if client was too late 
    // (rare case that client places request before closing, but the request is only handled after closing)
    if(timer_duration() >= (int) server_args.nsecs) {    
        send_message(fd_private, i, (int) getpid(), pthread_self(), -1, -1);
        close(fd_private);
        log_operation(i, (int) getpid(), pthread_self(), -1, -1, TLATE);

        if(server_args.nthreads) sem_post(&sem_nthreads);
        return NULL;
    }

    // allocate client and send message
    
    int allocated_place;
    if(server_args.nplaces){
        sem_wait(&sem_nPlaces);
        pthread_mutex_lock(&mutex_place);
        allocated_place = pop_queue(queue);
        pthread_mutex_unlock(&mutex_place);
    }else{
        pthread_mutex_lock(&mutex_place);
        allocated_place = current_place;
        current_place++;
        pthread_mutex_unlock(&mutex_place);
    }


    send_message(fd_private, i, (int) getpid(), pthread_self(), dur, allocated_place);
    
    if(close(fd_private)){
        fprintf(stderr, "Error closing private fifo\n");
        pthread_exit(NULL);
    }
    log_operation(i, (int) getpid(), pthread_self(), dur, allocated_place, ENTER);

    // wait for the client in the bathroom
    usleep(dur*1000);

    if(server_args.nplaces){
        sem_post(&sem_nPlaces);
        pthread_mutex_lock(&mutex_place);
        push_queue(queue,allocated_place);
        pthread_mutex_unlock(&mutex_place);
    }

    log_operation(i, (int) getpid(), pthread_self(), dur, allocated_place, TIMUP);

    if(server_args.nthreads) sem_post(&sem_nthreads);
    return NULL;
}

int main(int argc, char* argv[]){

    if(parse_server_args(argc, argv, &server_args)){
        perror("Error parsing server args");
        exit(1);
    }

    // create fifo
    timer_begin();
    if(mkfifo(server_args.fifoname, 0660) < 0) {
        if(errno == EEXIST) {
            fprintf(stderr, "FIFO already exists\n");
        }
        else {
            char error_msg[MAX_LEN];
            sprintf(error_msg, "Error creating public FIFO\n");
            write(STDOUT_FILENO, error_msg, strlen(error_msg));
            exit(1);
        }
    }

    // open fifo
    int fd_public = open(server_args.fifoname, O_RDONLY | O_NONBLOCK);
    if(fd_public == -1) {
        char error_msg[MAX_LEN];
        sprintf(error_msg, "Error opening public FIFO\n");
        write(STDOUT_FILENO, error_msg, strlen(error_msg));
        if (unlink(server_args.fifoname)<0){
            fprintf(stderr, "Error destroying FIFO");
        }
        exit(1);
    }

    // initialize semaphores, if number of threads/places are limited
    if(server_args.nthreads) sem_init(&sem_nthreads, 0, server_args.nthreads);

    if(server_args.nplaces){
        sem_init(&sem_nPlaces,0,server_args.nplaces);
        queue = create_queue(server_args.nplaces);
    }


    char client_msg[MAX_STR_LEN];
    pthread_t t;

    // receive and answer request
    while(timer_duration() < (int) server_args.nsecs) {        
        if(read(fd_public, &client_msg, MAX_STR_LEN) > 0 && client_msg[0] == '[') {
            // create thread if there's one available
            if(server_args.nthreads) sem_wait(&sem_nthreads);

            pthread_create(&t, NULL, server_thread_task, (void *) &client_msg);
            pthread_detach(t);
        }
    }
    


    if (unlink(server_args.fifoname)<0)
        fprintf(stderr, "Error destroying FIFO\n");

    while(read(fd_public, &client_msg, MAX_STR_LEN) > 0 && client_msg[0] == '[') {        
        // create thread if there's one available
        if(server_args.nthreads) sem_wait(&sem_nthreads);
        pthread_create(&t, NULL, server_thread_task, (void *) &client_msg);
        pthread_detach(t);
        
    }

    if(close(fd_public)<0)
        fprintf(stderr, "Error closing FIFO\n");

    destroy_queue(queue);

    pthread_exit(0);


}


void build_private_fifo(char *fifo_name, int pid, int tid) {
    sprintf(fifo_name, "/tmp/%d.%d", pid, tid);
}   