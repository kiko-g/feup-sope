#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../parser/parser.h"
#include "../utils/utils.h"


int main(int argc, char* argv[]){

    struct Arguments_Server serverArgs; 
    if(parseServerArgs(argc,argv,&serverArgs)){
        exit(1);
    }

  
    
}


