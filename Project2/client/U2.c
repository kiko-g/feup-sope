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
int server_open = 1;
pthread_mutex_t mutex_index = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_server_open = PTHREAD_MUTEX_INITIALIZER;

void *client_thread_task(void *arg) {
    // open public fifo
    char *public_fifo = (char *) arg;

    // increment index -- safely
    int index;
    pthread_mutex_lock(&mutex_index);
    index = i;
    i++;
    pthread_mutex_unlock(&mutex_index);

    int fd_public = open(public_fifo, O_WRONLY | O_NONBLOCK);
    if(fd_public == -1) {
        perror("Cannot open public FIFO with O_WRONLY and O_NONBLOCK");
        return NULL;
    }

    // create private fifo before sending message, making sure that the server does not access it before it's created
    char private_fifo[MAX_LEN];
    sprintf(private_fifo, "/tmp/%d.%ld", getpid(), pthread_self());
    if(mkfifo(private_fifo, 0660) < 0) {
        if(errno == EEXIST) {
            perror("FIFO already exists");
        }
        else {
            perror("Error creating private FIFO");
            return NULL;
        }
    }

    // send random request
    int time_client = (rand() %  (UPPER_TIME - LOWER_TIME + 1));

    RequestMessage request_msg = {index, getpid(), pthread_self(), time_client, -1};
    if(write(fd_public, &request_msg, sizeof(request_msg)) < 0) {
        if(close(fd_public)) perror("Error closing public FIFO");
        if(unlink(private_fifo)) perror("Error unlinking private FIFO");
        return NULL; 
    } else {
        log_operation(index, getpid(), pthread_self(), time_client, -1, IWANT);
        if(close(fd_public) < 0) perror("Error closing public FIFO");
    }

    // open private fifo
    int fd_private = open(private_fifo, O_RDONLY);

    if(fd_private == -1) {
        perror("Error opening private FIFO with O_RDONLY");
        if(unlink(private_fifo) < 0) perror("Error deleting private FIFO");
        return NULL;
    }
    
    // read the server's response from the private fifo
    int attempt = 0;
    RequestMessage server_msg;
    while (read(fd_private, &server_msg, sizeof(server_msg)) <= 0 && attempt < MAX_ATTEMPTS) {
        usleep(250);
        attempt++;
    } 
    if(attempt == MAX_ATTEMPTS) {
        log_operation(index, getpid(), pthread_self(), time_client, -1, FAILD);
        if(close(fd_private) < 0) perror("Error closing FIFO");
        if(unlink(private_fifo) < 0) fprintf(stderr, "Error destroying FIFO");
        return NULL;
    }

    // check if a place was given and log it
    if(server_msg.dur == -1 && server_msg.pl == -1) {
        log_operation(index, getpid(), pthread_self(), time_client, -1, CLOSD);
        pthread_mutex_lock(&mutex_server_open);
        server_open = 0;
        pthread_mutex_unlock(&mutex_server_open);
    }   
    else {
        log_operation(index, getpid(), pthread_self(), time_client, server_msg.pl, IAMIN);
    }

    if(close(fd_private) < 0) perror("Error closing FIFO");
    if(unlink(private_fifo) < 0) perror("Error destroying FIFO");

    return NULL;
}

int main(int argc, char* argv[]){

    srand(time(0));

    struct ClientArgs client_args; 
    if(parse_client_args(argc, argv, &client_args)) {
        perror("Error parsing client args");
        exit(1);
    }

    pthread_t t;
    timer_begin();
    while(timer_duration() < (int) client_args.nsecs) {
        
        pthread_mutex_lock(&mutex_server_open);
        if(!server_open) {
            pthread_mutex_unlock(&mutex_server_open);
            break;
        }
        pthread_mutex_unlock(&mutex_server_open);

        if(pthread_create(&t, NULL, client_thread_task, client_args.fifoname)) {
            perror("Failed creating client thread");
        }
        else {
            pthread_detach(t);
        }
         
        usleep(REQUEST_INTERVAL*1000);
    }

    pthread_exit(0);
}
