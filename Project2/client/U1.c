#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "parser.h"


int main(int argc, char* argv[]){

    struct Arguments_Client clientArgs; 
    parseClientArgs(argc,argv,&clientArgs);
}


