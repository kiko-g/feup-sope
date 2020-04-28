#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../parser/parser.h"


int main(int argc, char* argv[]){

    struct Arguments_Client clientArgs; 
    struct Arguments_Server serverArgs;
    parseClientArgs(argc,argv,&clientArgs);
    parseClientArgs(argc,argv,&serverArgs);
}


