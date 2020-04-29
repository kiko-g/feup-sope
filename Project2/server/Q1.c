#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../parser/parser.h"
#include "../utils/utils.h"


int main(int argc, char* argv[]){

    struct ServerArgs server_args; 
    if(parse_server_args(argc, argv, &server_args)){
        exit(1);
    }
    
}


