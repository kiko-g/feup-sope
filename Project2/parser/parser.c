#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "parser.h"
#include <stdbool.h>
#include <string.h>


int parse_client_args(int argc, char *argv[], struct ClientArgs * client_args){
    if (argc != NUMBER_FLAGS) {
        printf("Please insert arguments in the following format: ");
        printf("U1 <-t secs> fifoname\n");
        return -1;
    }

    for (int i = 0; i < argc; i++)
    {
        if(!strcmp(argv[i],"-t")){

            if(atoi(argv[i+1])){

                client_args->nsecs=atoi(argv[i+1]);
                i++;
            }
            else return 1;
            
        }
        else  strncpy(client_args->fifoname, argv[i],sizeof(client_args->fifoname));
        
    }

    return 0;
}


int parse_server_args(int argc, char *argv[], struct ServerArgs * server_args){
    if (argc < NUMBER_FLAGS || argc > NUMBER_SERVER_FLAGS) {
        printf("Please insert arguments in the following format: ");
        printf("Qn <-t nsecs> [-l nplaces] [-n nthreads] fifoname\n");
        return -1;
    }
    bool foundSeconds = false;
    bool foundFifoname = false;

    for (int i = 0; i < argc; i++)
    {
        if(!strcmp(argv[i],"-t")){
            foundSeconds = true;

            if(server_args->nsecs > 0){
                printf("Already Defined nsecs\n");
                return 1;
            }
            if(atoi(argv[i+1])){
                server_args->nsecs=atoi(argv[i+1]);
                i++;
            }
            else{
                printf("Please inster a natural number after -t \n");
                return 1;
            }
            
        }
        else if(!strcmp(argv[i],"-l")){

            if(server_args->nplaces > 0){
                printf("Already Defined nplaces\n");
                return 1;
            }
            if(atoi(argv[i+1])){
                server_args->nplaces = atoi(argv[i+1]);
                i++;
            }
            else{
                printf("Please inster a natural number after -l \n");
                return 1;
            }
        }
        else if(!strcmp(argv[i],"-n")){

            if(server_args->nthreads > 0){
                printf("Already Defined nthreads\n");
                return 1;
            }
            if(atoi(argv[i+1])){
                server_args->nthreads = atoi(argv[i+1]);
                i++;
            }
            else{
                printf("Please inster a natural number after -n \n");
                return 1;
            }
        }
        else if(argv[i][0]!='-' && i != 0){
            foundFifoname = true;
            strncpy(server_args->fifoname,argv[i],sizeof(server_args->fifoname));
        }

    }

    if(!foundFifoname){
        printf("Please insert Fifoname\n");
        return 1;
    }
    if (!foundSeconds){
        printf("Please define nsecs\n");
        return 1;
    }

    return 0;
}