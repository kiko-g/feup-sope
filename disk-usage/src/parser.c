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
        flag = strtok(argv[i], "=");
        int flagIndex = validFlag(flag);
        if (flagIndex != -1)
        {
            int number = -1;
            if (flagIndex == 12 || flagIndex == 5)
            { //If it is block-size or max-depth
                number = atoi(strtok(NULL, "="));
            }
            else if (flagIndex == 4)
            {
                number = atoi(argv[i + 1]);
                i++;
            }
            if (!activateFlag(flag, number))
                printf("Error na flag %s", flag);
        }
        else if (isPath(argv[i]) != -1)
        {
            foundPath = true;
            strcpy(args.path, argv[i]);
        }
        else
            return false;
    }

    if (!foundPath)
        strcpy(args.path, ".");

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
    if (index == -1)
        return false;
    else if (!strcmp(ARGUMENTS[index], ALL_FLAG_SHORT) || !strcmp(ARGUMENTS[index], ALL_FLAG_LONG))
    {
        args.all = true;
        return true;
    }
    else if (!strcmp(ARGUMENTS[index], BYTES_FLAG_SHORT) || !strcmp(ARGUMENTS[index], BYTES_FLAG_LONG))
    {
        args.bytes = true;
        return true;
    }
    else if (!strcmp(ARGUMENTS[index], BLOCKSIZE_FLAG_SHORT) || !strcmp(ARGUMENTS[index], BLOCKSIZE_FLAG_LONG))
    {
        args.block_size_flag = true;
        args.block_size = number;
        return true;
    }
    else if (!strcmp(ARGUMENTS[index], COUNTLINKS_FLAG_SHORT) || !strcmp(ARGUMENTS[index], COUNTLINKS_FLAG_LONG))
    {
        args.countLinks = true;
        return true;
    }
    else if (!strcmp(ARGUMENTS[index], LINK_FLAG_SHORT) || !strcmp(ARGUMENTS[index], LINK_FLAG_LONG))
    {
        args.deference = true;
        return true;
    }
    else if (!strcmp(ARGUMENTS[index], SEPARATEDIRS_FLAG_SHORT) || !strcmp(ARGUMENTS[index], SEPARATEDIRS_LONG))
    {
        args.separateDirs = true;
        return true;
    }
    else if (!strcmp(ARGUMENTS[index], MAX_DEPTH_FLAG))
    {
        args.max_depth = number;
        args.max_depth_flag = true;
        return true;
    }
    return false;
}

// if path is not valid return -1
// if file return 0
// if directory return 1
int isPath(const char *path)
{
    struct stat path_stat;
    if (lstat(path, &path_stat) < 0) // Invalid path
        return -1;
    else if (isDirectory(path)) // Directory
        return 1;
    else if (isFile(path)) // File
        return 0;
    else if (isSymbolicLink(path))
        return 2;
    else
        return -1;
}

// ------------------- File Type Checking -------------------------

bool isFile(const char *path)
{
    struct stat buf;
    int status = stat(path, &buf);

    if (status != 0)
    {
        printf("\nError in stat: %d\n", status);
    }

    if (S_ISREG(buf.st_mode)) // File
        return true;
    return false;
}

bool isDirectory(const char *path)
{
    struct stat buf;
    int status = stat(path, &buf);

    if (status != 0)
    {
        printf("\nError in stat: %d\n", status);
    }

    if (S_ISDIR(buf.st_mode)) // Directory
        return true;
    return false;
}

bool isSymbolicLink(const char *path)
{
    struct stat buf;
    int status = stat(path, &buf);

    if (status != 0)
    {
        printf("\nError in stat: %d\n", status);
    }

    if (S_ISLNK(buf.st_mode)) // Symbolic Link
        return true;
    return false;
}
