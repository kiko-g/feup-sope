#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "parser.h"
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
    if (argc != NUMBER_FLAGS) {
        printf("Please insert arguments in the following format: ");
        printf("U1 <-t secs> fifoname\n");
        return -1;
    }

    for (int i = 0; i < argc; i++)
    {
        if(!strcmp(argv[i],"-t")){

            if(atoi(argv[i+1])){

                server_args->nsecs=atoi(argv[i+1]);
                i++;
            }
            else{
                printf("Please inster a natural number after -t \n");
                return 1;
            }
            
        }
        else  strncpy(server_args->fifoname,argv[i],sizeof(server_args->fifoname));
        
    }

    return 0;
}