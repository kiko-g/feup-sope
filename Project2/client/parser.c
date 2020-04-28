#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "parser.h"


int parseClientArgs(int argc, char *argv[],struct Arguments_Client * clientArgs){
    if (argc != NUMBER_FLAGS) {
        printf("Usage: U1 <-t secs> fifoname\n");
        return -1;
    }

    
    return 0;
}