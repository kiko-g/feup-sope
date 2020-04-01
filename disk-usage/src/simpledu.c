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

    printf("FLAGS\n");
    for(int i=0; i < argc-1; ++i) printf("%s%s%s\n", BLUE_TEXT, flags[i], RESET_TEXT_COLOR);

    int index = pathProvided(flags, (size_t)argc-1);
    if(index != -1) dir = opendir(flags[index]);

    while ((ent = readdir(dir)) != NULL)
    {
        // skip showing current folder and parent folder
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
            continue;

        stat(ent->d_name, &buf);

        // apply different behaviors for files and directories
        if(S_ISREG(buf.st_mode)) printf("File: %-30s %ld Bytes\n", ent->d_name, buf.st_size);
        else {
            printf("Dir:  %-30s (can go deeper)\n", ent->d_name);
            // details(ent->d_name);
        } 
    }

    return 0;
}
