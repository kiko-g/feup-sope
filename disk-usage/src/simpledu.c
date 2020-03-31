#include "simpledu.h"

int main(int argc, char *argv[]) 
{
    char* dirname = ".";
    char flags[argc][MAX_FLAG_LEN];
    DIR* dir = opendir(dirname);
    struct dirent* ent;
    struct stat buf;

    // parse flags
    for(int i=1; i < argc; ++i) strcpy(flags[i-1], argv[i]);
    for(int i=0; i < argc-1; ++i) printf("%s\n", flags[i]);
    pathProvided(flags, (size_t)argc-1); 

    while((ent = readdir(dir)) != NULL) {
        // ignore current folder and parent folder
        if(strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) continue; 

        stat(ent->d_name, &buf);
        printf("File/Directory: %s\t%ld\tBytes\n", ent->d_name, buf.st_size);
    }
    

    return 0;
}