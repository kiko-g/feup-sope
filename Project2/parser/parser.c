#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "parser.h"
#include <string.h>


int parseClientArgs(int argc, char *argv[],struct Arguments_Client * clientArgs){
    if (argc != NUMBER_FLAGS) {
        printf("Please insert arguments in the following format: ");
        printf("U1 <-t secs> fifoname\n");
        return -1;
    }
    for (int i = 0; i < argc; i++)
    {
        if(!strcmp(argv[i],"-t")){

            if(atoi(argv[i+1])){

                clientArgs->nsecs=atoi(argv[i+1]);
                i++;
            }
            else return 1;
            
        }
        else  strncpy(clientArgs->fifoname,argv[i],sizeof(clientArgs->fifoname));
        
    }

    printf("Arguments parsed Correctly\n");    
    return 0;
}


int parseServerArgs(int argc, char *argv[],struct Arguments_Server * serverArgs){
    if (argc != NUMBER_FLAGS) {
        printf("Please insert arguments in the following format: ");
        printf("U1 <-t secs> fifoname\n");
        return -1;
    }
    for (int i = 0; i < argc; i++)
    {
        if(!strcmp(argv[i],"-t")){

            if(atoi(argv[i+1])){

                serverArgs->nsecs=atoi(argv[i+1]);
                i++;
            }
            else{
                printf("Please inster a natural number after -t \n");
                return 1;
            }
            
        }
        else  strncpy(serverArgs->fifoname,argv[i],sizeof(serverArgs->fifoname));
        
    }

    printf("Arguments parsed Correctly\n");    
    return 0;
}