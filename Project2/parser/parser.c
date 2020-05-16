#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "parser.h"
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>



int parse_client_args(int argc, char *argv[], struct ClientArgs * client_args){
    if (argc != NUMBER_FLAGS) {
        fprintf(stderr, "Please insert arguments in the following format: ");
        fprintf(stderr, "Un <-t secs> fifoname\n");
        return -1;
    }

    for (int i = 0; i < argc; i++)
    {
        if(!strcmp(argv[i],"-t")){

            if(atoi(argv[i+1])){
                if (atoi(argv[i+1]) > 0){
                    client_args->nsecs=atoi(argv[i+1]);
                    i++;
                } else{
                    fprintf(stderr, "Please insert positive Time\n");
                    return 1;
                }
            }
            else{
                fprintf(stderr, "Please insert valid time\n");
                return 1;
            } 
            
        }
        else {
            if(strlen(argv[i]) > sizeof(client_args->fifoname)) {
                fprintf(stderr, "Fifoname is too big\n");
                return 1;
            }
            strncpy(client_args->fifoname, argv[i],sizeof(client_args->fifoname));
        }
        
    }

    return 0;
}


int parse_server_args(int argc, char *argv[], struct ServerArgs * server_args){
    if (argc < NUMBER_FLAGS || argc > NUMBER_SERVER_FLAGS) {
        fprintf(stderr, "Please insert arguments in the following format: ");
        fprintf(stderr, "Qn <-t nsecs> [-l nplaces] [-n nthreads] fifoname\n");
        return -1;
    }
    bool foundSeconds = false;
    bool foundFifoname = false;

    for (int i = 0; i < argc; i++)
    {
        if(!strcmp(argv[i],"-t")){
            foundSeconds = true;

            if(server_args->nsecs > 0){
                fprintf(stderr, "Already Defined nsecs\n");
                return 1;
            }
            if(atoi(argv[i+1])){
                if(atoi(argv[i+1]) > 0){
                    server_args->nsecs=atoi(argv[i+1]);
                    i++;
                } else{
                    fprintf(stderr, "Please insert a natural number after -t \n");
                    return 1;
                }
            }
            else{
                fprintf(stderr, "Please insert a natural number after -t \n");
                return 1;
            }
            
        }
        else if(!strcmp(argv[i],"-l")){

            if(server_args->nplaces > 0){
                fprintf(stderr, "Already Defined nplaces\n");
                return 1;
            }
            if(atoi(argv[i+1])){
                if(atoi(argv[i+1]) > 0){
                    server_args->nplaces = atoi(argv[i+1]);
                    i++;
                }else{
                    fprintf(stderr, "Please insert a natural number after -l \n");
                    return 1;
                }
            }
            else{
                fprintf(stderr, "Please insert a natural number after -l \n");
                return 1;
            }
        }
        else if(!strcmp(argv[i],"-n")){

            if(server_args->nthreads != MAX_NUMBER_THREADS){
                fprintf(stderr, "Already Defined nthreads\n");
                return 1;
            }

            int numberThreads = atoi(argv[i+1]);
            if(numberThreads > 0){
                server_args->nthreads = numberThreads;
                i++;
            }else if(numberThreads == 0){
                printf("Will assume Infinite number of threads\n");
                server_args->nthreads = numberThreads;
                i++;
            }else{
                fprintf(stderr, "Please insert a natural number after -n \n");
                return 1;
            }
        }
        else if(argv[i][0]!='-' && i != 0){
            foundFifoname = true;

            if(strlen(argv[i]) > sizeof(server_args->fifoname)) {
                fprintf(stderr, "Fifoname is too big\n");
                return 1;
            }

            strncpy(server_args->fifoname,argv[i],sizeof(server_args->fifoname));
        }

    }

    if(!foundFifoname){
        fprintf(stderr, "Please insert Fifoname\n");
        return 1;
    }
    if (!foundSeconds){
        fprintf(stderr, "Please define nsecs\n");
        return 1;
    }

    return 0;
}
