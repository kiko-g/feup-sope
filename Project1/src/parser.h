#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "arguments.h"

/** 
 * @brief check if a string corresponds to an available program flag option
 * @param path string containing path 
 * @return index of the flag if it is valid, -1 othwerise
 */
int validFlag(char *flag);

/** 
 * @brief activate a flag and thus changing the program's behavior
 * @param flag flag name
 * @param number number used for block size or max depth flags
 * @return true if succeeded, false othwerise
 */
bool activateFlag(char *flag, int number);

/** 
 * @brief parse program arguments
 * @param argv array containing the program arguments
 * @param argc total of program arguments
 * @return true if succeeded, false othwerise
 */
bool parseArguments(char *argv[],int argc);

/** 
 * @brief check if a path is valid
 * @param path string with path name
 * @return -1 if not, 0 if file, 1 if directory, 2 symbolic link
 */
int isPath(const char *path);