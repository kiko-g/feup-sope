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
#include <signal.h>
#include "Q2.h"
#include "../parser/parser.h"
#include "../utils/utils.h"
#include "queue.h"


int current_place = 1;
pthread_mutex_t mutex_place = PTHREAD_MUTEX_INITIALIZER;
sem_t sem_nthreads, sem_nplaces;
struct ServerArgs server_args = {0,0,MAX_NUMBER_THREADS,""}; 
Queue * queue;
int working = 1;


void *server_thread_task(void *arg) {
    struct RequestMessage client_msg = *( struct RequestMessage*) arg;

    // blocking sigalrm for this thread
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGALRM);
    if(pthread_sigmask(SIG_BLOCK, &set, NULL)) {
        perror("Unable to set sigmask for client thread");
        return NULL;
    }

    // parse private fifo name and client's message
    log_operation(client_msg.i, client_msg.pid, client_msg.tid, client_msg.dur, -1, RECVD);

    char private_fifo[MAX_LEN];

    sprintf(private_fifo, "/tmp/%d.%ld", client_msg.pid, client_msg.tid);

    // open the private fifo (if possible)
    int fd_private;
    if((fd_private = open(private_fifo, O_WRONLY)) == -1) {
        log_operation(client_msg.i, getpid(), pthread_self(), client_msg.dur, -1, GAVUP);

        if(server_args.nthreads) sem_post(&sem_nthreads);
        free(arg);
        return NULL;
    }

    // check if client was too late -> case when queue wasn't yet destroyed, but time is up 
    if(timer_duration() >= (int) server_args.nsecs) {    

        RequestMessage response_msg = {client_msg.i, getpid(), pthread_self(), -1, -1};
        if(write(fd_private, &response_msg, sizeof(response_msg)) < 0) {
            if(errno == EPIPE) fprintf(stderr, "Failed writing to public FIFO - reading end was already closed\n");
            log_operation(client_msg.i, getpid(), pthread_self(), client_msg.dur, -1, GAVUP);
        }
        else log_operation(client_msg.i, getpid(), pthread_self(), -1, -1, TLATE);

        
        close(fd_private);
        if(server_args.nthreads) sem_post(&sem_nthreads);
        free(arg);
        return NULL;
    }

    // allocate client and send message   
    int allocated_place;
    if(server_args.nplaces){
        sem_wait(&sem_nplaces);
        pthread_mutex_lock(&mutex_place);
        allocated_place = pop_queue(queue);
        pthread_mutex_unlock(&mutex_place);
    }else{
        pthread_mutex_lock(&mutex_place);
        allocated_place = current_place;
        current_place++;
        pthread_mutex_unlock(&mutex_place);
    }

    // check again if client was too late -> when fifo was destroyed or the too much time has passed since the allocation of the place
    if(allocated_place == NO_AVAILABLE_STALL || timer_duration() >= (int) server_args.nsecs) {

        RequestMessage response_msg = {client_msg.i, getpid(), pthread_self(), -1, -1};
        if(write(fd_private, &response_msg, sizeof(response_msg)) < 0) {
            if(errno == EPIPE) fprintf(stderr, "Failed writing to public FIFO - reading end was already closed\n");
            log_operation(client_msg.i, getpid(), pthread_self(), client_msg.dur, -1, GAVUP);
        }
        else log_operation(client_msg.i, getpid(), pthread_self(), -1, -1, TLATE);

        if(server_args.nthreads) sem_post(&sem_nthreads);
        if(server_args.nplaces){
            sem_post(&sem_nplaces);
            pthread_mutex_lock(&mutex_place);
            push_queue(queue,allocated_place);
            pthread_mutex_unlock(&mutex_place);
        }

        close(fd_private);
        free(arg);
        return NULL; 
    }

    RequestMessage response_msg = {client_msg.i, getpid(), pthread_self(), client_msg.dur, allocated_place};
    if(write(fd_private, &response_msg, sizeof(response_msg)) < 0) {
        if(errno == EPIPE) fprintf(stderr, "Failed writing to private FIFO - reading end was already closed\n");
        
        log_operation(client_msg.i, getpid(), pthread_self(), client_msg.dur, -1, GAVUP);
        close(fd_private);

        if(server_args.nthreads) sem_post(&sem_nthreads);
        
        if(server_args.nplaces){
            sem_post(&sem_nplaces);
            pthread_mutex_lock(&mutex_place);
            push_queue(queue,allocated_place);
            pthread_mutex_unlock(&mutex_place);
        }

        free(arg);
        return NULL; 
    }


    
    if(close(fd_private)){
        perror("Error closing private fifo");
    }
    log_operation(client_msg.i, getpid(), pthread_self(), client_msg.dur, allocated_place, ENTER);

    // wait for the client in the bathroom
    usleep(client_msg.dur*1000);

    log_operation(client_msg.i, getpid(), pthread_self(), client_msg.dur, allocated_place, TIMUP);

    if(server_args.nthreads) sem_post(&sem_nthreads);

    if(server_args.nplaces){
        pthread_mutex_lock(&mutex_place);
        push_queue(queue,allocated_place);
        pthread_mutex_unlock(&mutex_place);
        sem_post(&sem_nplaces);
    }
    free(arg);

    return NULL;
}


int main(int argc, char* argv[]){

    install_sigpipe_handler();
    install_sigalrm_handler();

    if(parse_server_args(argc, argv, &server_args)){
        fprintf(stderr, "Error parsing server args");
        exit(1);
    }

    // create fifo
    alarm(server_args.nsecs);
    timer_begin();
    if(mkfifo(server_args.fifoname, 0660) < 0) {
        if(errno == EEXIST) {
            perror("FIFO already exists");
        }
        else {
            perror("Error creating public FIFO");
            exit(1);
        }
    }

    // open fifo
    int fd_public = open(server_args.fifoname, O_RDONLY);
    if(fd_public < 0) {
        if(errno == EINTR) fprintf(stderr, "No clients received\n");
        perror("Error opening public FIFO");
        if (unlink(server_args.fifoname)<0){
            perror("Error destroying FIFO");
        }
        exit(1);
    }

    // initialize semaphores, if number of threads/places are limited
    if(server_args.nthreads) sem_init(&sem_nthreads, 0, server_args.nthreads);

    if(server_args.nplaces){
        sem_init(&sem_nplaces,0,server_args.nplaces);
        queue = create_queue(server_args.nplaces);
    }

    RequestMessage client_msg;
    pthread_t t;

    // receive and answer request
    while(working) {   
        if(read(fd_public, &client_msg, sizeof(client_msg)) > 0) {

            // copy message to another struct to ensure safe access
            struct RequestMessage *client_msg_copy = malloc(sizeof(struct RequestMessage));
            *client_msg_copy = client_msg;

            // create thread if there's one available
            if(server_args.nthreads) sem_wait(&sem_nthreads);

            // create thread to answer the request
            if(pthread_create(&t, NULL, server_thread_task, client_msg_copy)) {
                perror("Failed creating server thread");
                free(client_msg_copy);
            }else{
                pthread_detach(t);    
            }
        }
        else if(errno == EINTR) fprintf(stderr, "Failed read because of SIGALRM\n");
    }
    
    if (unlink(server_args.fifoname)<0)
        perror("Error destroying FIFO");

    while(read(fd_public, &client_msg, sizeof(client_msg)) > 0) {  

        // copy message to another struct to ensure safe access
        struct RequestMessage *client_msg_copy = malloc(sizeof(struct RequestMessage));
        *client_msg_copy = client_msg;      

        // create thread if there's one available
        if(server_args.nthreads) sem_wait(&sem_nthreads);
        if(pthread_create(&t, NULL, server_thread_task,client_msg_copy)) {
            perror("Failed creating server thread");
            free(client_msg_copy);
        }else{
            pthread_detach(t);
        }
        
    }

    if(close(fd_public)<0)
        perror("Error closing FIFO");

    if(server_args.nplaces) {
        pthread_mutex_lock(&mutex_place);
        destroy_queue(queue);
        queue = NULL;
        pthread_mutex_unlock(&mutex_place);
    }

    pthread_exit(0);
}


void install_sigalrm_handler() {
    struct sigaction sa;
    sa.sa_handler = sigalrm_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGALRM, &sa, NULL) < 0)  {        
        fprintf(stderr,"Failed to install SIGARLM handler\n");        
        exit(1);  
    }  
}


void sigalrm_handler(int signo) {
    working = 0;
}
