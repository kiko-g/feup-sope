#include "simpledu.h"

int main(int argc, char *argv[]) 
{
    char* dirname = ".";
    char flags[argc][MAX_FLAG_LEN];
    DIR* dir = opendir(dirname);
    struct dirent* ent;

    for(int i=1; i < argc; ++i) strcpy(flags[i-1], argv[i]);
    for(int i=0; i != argc-1; ++i) printf("%s ", flags[i]);

    printf("\n\n");
    while((ent = readdir(dir)) != NULL) {
        if(strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
            continue; // ignore current folder and parent folder

        printf("File/Directory: %s\n", ent->d_name);
    }

    return 0;
}