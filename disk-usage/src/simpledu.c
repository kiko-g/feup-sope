#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>

int main(int argc, char *argv[]) {
    char* dirname = ".";

    DIR *dir = opendir(dirname);
    struct dirent *ent;

    while(((ent = readdir(dir)) != NULL)) {

        // ignore current folder and parent folder
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) 
            continue;

        printf("File/Directory: %s\n", ent->d_name);
    }

    return 0;
}