#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define READ 0
#define WRITE 1
#define MAX_LEN 256

/**
 * @brief checks if a path was provided through command line 
 * and sets the working directory to the correct path
 * @param flags parsed flags array
 * @param len length of the flags array
 * @return positive index if successfully read a path
 */

int recursiveScan(char* directory_name, int current_depth);

/** 
 * @brief scans an entity about its size
 * @param file_path path to file
 * @return entity size depending on parameter stbuf
 */
long scanEntity(struct stat stbuf);

/**
 * @brief prints directory/file given
 * @param path path to file
 * @param size size of the file
 */
void printEntity(long size, char* path);



