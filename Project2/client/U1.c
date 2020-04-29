#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h> 
#include "../parser/parser.h"

#define UPPER_TIME 30
#define LOWER_TIME 5


int main(int argc, char* argv[]){

    struct ClientArgs client_args; 
    parse_client_args(argc, argv, &client_args);

    srand(time(0));
    
    int timeClient = (rand() %  (UPPER_TIME - LOWER_TIME + 1));
}


