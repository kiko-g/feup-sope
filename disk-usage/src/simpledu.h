#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#define MAX_FLAG_LEN 50
#define MAX_CHAR_LEN 256

#define READ 0
#define WRITE 1


/**
 * @brief checks if a path was provided through command line 
 * and sets the working directory to the correct path
 * @param flags parsed flags array
 * @param len length of the flags array
 * @return positive index if successfully read a path
 */

int recursiveScan(char* directory_name, int max_depth);

long scanFile(char* file_path);



