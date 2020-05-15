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
        log_operation(index, (int) getpid(), (long) pthread_self(), -1, -1, FAILD);
        return NULL;
    }

    // create private fifo
    char private_fifo[MAX_LEN];
    make_private_fifo(private_fifo);
    if(mkfifo(private_fifo, 0660) < 0) {
        if(errno == EEXIST) {
            fprintf(stderr, "FIFO already exists\n");
        }
        else {
            char error_msg[MAX_LEN*2];
            sprintf(error_msg, "Error creating private FIFO %s\n", private_fifo);
            write(STDOUT_FILENO, error_msg, strlen(error_msg));
            return NULL;
        }
    }

    // open private fifo
    int fd_private = open(private_fifo, O_RDONLY | O_NONBLOCK); //rw
    if(fd_private == -1) {
            char error_msg[MAX_LEN*2];
            sprintf(error_msg, "Error opening private FIFO %s with O_RDONLY\n", private_fifo);
            write(STDOUT_FILENO, error_msg, strlen(error_msg));
            if(unlink(private_fifo) < 0) perror("Error deleting private FIFO\n");
            return NULL;
    }

    // send random request
    int time_client = (rand() %  (UPPER_TIME - LOWER_TIME + 1));
    log_operation(index, (int) getpid(), (long) pthread_self(), -1, -1, IWANT);
    send_message(fd_public, index, (int) getpid(), (long) pthread_self(), time_client, -1);
    
    if(close(fd_public) < 0) fprintf(stderr, "Error closing FIFO\n") ;

    // read the server's response from the private fifo
    int attempt = 0;
    char server_response[MAX_LEN];
    while (read(fd_private, server_response, MAX_LEN) <= 0 && attempt < MAX_ATTEMPTS) {
        usleep(100);
        attempt++;
    } 
    if(attempt == MAX_ATTEMPTS) {
        log_operation(index, (int) getpid(), (long) pthread_self(), time_client, -1, FAILD);
        if(close(fd_private) < 0) fprintf(stderr, "Error closing FIFO.\n");
        if(unlink(private_fifo) < 0) fprintf(stderr, "Error destroying FIFO\n");
        return NULL;
    }

    // parse the server's response
    int idup, pid, duration, place;
    long tid;
    sscanf(server_response, "[ %d, %d, %ld, %d, %d ]", &idup, &pid, &tid, &time_client, &place);

    // check if a place was given and log it
    if(time_client == -1 && place == -1) {
        log_operation(index, (int) getpid(), (long) pthread_self(), time_client, -1, CLOSD);
        pthread_mutex_lock(&mutex_server_open);
        server_open = 0;
        pthread_mutex_unlock(&mutex_server_open);
    }   
    else {
        log_operation(index, (int) getpid(), (long) pthread_self(), time_client, place, IAMIN);
    }

    if(close(fd_private) < 0) fprintf(stderr,"Error closing FIFO\n");
    if(unlink(private_fifo) < 0) fprintf(stderr, "Error destroying FIFO\n");
    return NULL;
}

void make_private_fifo(char *fifo_name) {
    sprintf(fifo_name, "/tmp/%d.%d", (int) getpid(), (int) pthread_self());
}

int main(int argc, char* argv[]){

    srand(time(0));

    struct ClientArgs client_args; 
    if(parse_client_args(argc, argv, &client_args)) {
        fprintf(stderr, "Error parsing client argss\n");
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

        pthread_create(&t, NULL, client_thread_task, client_args.fifoname);
        pthread_detach(t);
        usleep(REQUEST_INTERVAL*1000);
    }

    pthread_exit(0);
}


