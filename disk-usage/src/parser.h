
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "arguments.h"

bool isFile(const char *path);

bool isDirectory(const char *path);

bool isSymbolicLink(const char *path);

int validFlag(char * flag);

bool activateFlag(char * flag, int number);

bool parseArguments(char * argv[],int argc);

int isPath(const char *path);