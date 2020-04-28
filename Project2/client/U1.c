#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h> 


#include "../parser/parser.h"
#define UPPER_TIME 30
#define LOWER_TIME 5


int main(int argc, char* argv[]){

    struct Arguments_Client clientArgs; 
    struct Arguments_Server serverArgs;
    parseClientArgs(argc,argv,&clientArgs);

    srand(time(0));
    
    int timeClient = (rand() %  (UPPER_TIME - LOWER_TIME + 1));
}


