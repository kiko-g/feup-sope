#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "parser.h"

// ------------------- Argument Parsing ------------------------- //

struct Arguments args = { .max_depth = INT_MAX};

bool parseArguments(char *argv[], int argc)
{
    bool foundPath = false;
    char *flag;

    for (int i = 1; i < argc; i++)
    {
        //remove possible unnecessary slash bar at the end
        if(argv[i][strlen(argv[i])-1] == '/')
            argv[i][strlen(argv[i]) - 1] = 0; 

        flag = strtok(argv[i], "=");
        int flagIndex = validFlag(flag);
        if (flagIndex != -1)
        {
            int number = -1;

            if (flagIndex == 5) { // block size
                number = atoi(strtok(NULL, "="));
                if(number == 0) {
                    printf("Invalid block size number argument\n");
                    exit(-1);
                }
            }
            else if (flagIndex == 4) { // block size short (-B)
                number = atoi(argv[i + 1]);
                if(number == 0) {
                    printf("Invalid block size (-B) number argument\n");
                    exit(-1);
                } 
                i++;
            }
            else if (flagIndex == 12) {  // max depth
                number = atoi(strtok(NULL, "="));
                if(number == 0) {
                    printf("Invalid max depth number argument\n");
                    exit(-1);
                }
            }
            if (!activateFlag(flag, number))
                printf("Error na flag %s", flag);
        }
        else if (isPath(argv[i]) != -1) {
            foundPath = true;
            strcpy(args.path, argv[i]);
        }
        else return false;
    }

    if (!foundPath) strcpy(args.path, ".");

    return true;
}


int validFlag(char *flag)
{
    for (int i = 0; i < 13; i++)
    {
        if (!strcmp(flag, ARGUMENTS[i]))
        {
            return i; //Returns position in the ARGUMENTS array
        }
    }
    return -1;
}



bool activateFlag(char *flag, int number)
{
    int index = validFlag(flag);

    if (index == -1) return false;
    else if (!strcmp(ARGUMENTS[index], ALL_FLAG_SHORT) || !strcmp(ARGUMENTS[index], ALL_FLAG_LONG)) {
        args.all = true;
        return true;
    }
    else if (!strcmp(ARGUMENTS[index], BYTES_FLAG_SHORT) || !strcmp(ARGUMENTS[index], BYTES_FLAG_LONG)) {
        args.bytes = true;
        return true;
    }
    else if (!strcmp(ARGUMENTS[index], BLOCKSIZE_FLAG_SHORT) || !strcmp(ARGUMENTS[index], BLOCKSIZE_FLAG_LONG)) {
        args.block_size_flag = true;
        args.block_size = number;
        return true;
    }
    else if (!strcmp(ARGUMENTS[index], COUNTLINKS_FLAG_SHORT) || !strcmp(ARGUMENTS[index], COUNTLINKS_FLAG_LONG)) {
        args.countLinks = true;
        return true;
    }
    else if (!strcmp(ARGUMENTS[index], LINK_FLAG_SHORT) || !strcmp(ARGUMENTS[index], LINK_FLAG_LONG)) {
        args.deference = true;
        return true;
    }
    else if (!strcmp(ARGUMENTS[index], SEPARATEDIRS_FLAG_SHORT) || !strcmp(ARGUMENTS[index], SEPARATEDIRS_LONG)) {
        args.separateDirs = true;
        return true;
    }
    else if (!strcmp(ARGUMENTS[index], MAX_DEPTH_FLAG)) {
        args.max_depth = number;
        args.max_depth_flag = true;
        return true;
    }
    return false;
}


int isPath(const char *path)
{
    struct stat path_stat;
    if (lstat(path, &path_stat) < 0) return -1; // Invalid path
    else if (isFile(path)) return 0;            // File
    else if (isDirectory(path)) return 1;       // Directory
    else if (isSymbolicLink(path)) return 2;    // Symbolic link
    else return -1;
}

// ------------------- File Type Checking -------------------------
bool isFile(const char *path)
{
    struct stat buf;
    int status = stat(path, &buf);

    if (status != 0) {
        printf("\nError in stat: %d\n", status);
        return false;
    }
    if (S_ISREG(buf.st_mode)) return true; // File
    return false;
}

bool isDirectory(const char *path)
{
    struct stat buf;
    int status = stat(path, &buf);

    if (status != 0) {
        printf("\nError in stat: %d\n", status);
        return false;
    }

    if (S_ISDIR(buf.st_mode)) return true; // Directory
    return false;
}

bool isSymbolicLink(const char *path)
{
    struct stat buf;
    int status = stat(path, &buf);

    if (status != 0) {
        printf("\nError in stat: %d\n", status);
        return false;
    }

    if (S_ISLNK(buf.st_mode)) return true; // Symbolic Link
    return false;
}
