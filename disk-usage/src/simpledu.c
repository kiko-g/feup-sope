#include "simpledu.h"

int main(int argc, char *argv[])
{
    char *dirname = ".";
    // char flags[argc][MAX_FLAG_LEN];
    DIR *dir = opendir(dirname);
    struct dirent *ent;
    struct stat buf;
    

    // parse flags
    // for (int i = 1; i < argc; ++i)
    // {
    //     strcpy(flags[i - 1], argv[i]);
    // }

    // printf("FLAGS\n");
    // for (int i = 0; i < argc - 1; ++i)
    //     printf("%s%s%s\n", BLUE_TEXT, flags[i], RESET_TEXT_COLOR);

    
    //Isto funciona com outras pastas ou so com o diretorio atual?
    // int index = pathProvided(flags, (size_t)argc - 1);
    // if (index != -1){
    //     //Nunca esta a entrar aqui!
    //     dir = opendir(flags[index]);
    //     printf("%s zas2",flags[index]);
    // }

    if(!parseArguments(argv,argc,&args)){
        printf("Error Parsing arguments");
        return 1;
    }
    dir = opendir(args.path);
    

    while ((ent = readdir(dir)) != NULL)
    {
        // skip showing current folder and parent folder
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
            continue;

        stat(ent->d_name, &buf);
        

        // apply different behaviors for files and directories

        // char str[1000] ; 
        // sprintf(str,"cd %s",args.path);
        // system(str);


        if (isFile(ent->d_name)){
             printFile(ent->d_name, buf.st_size);
        }
        else if (isDirectory(ent->d_name))
        {
            printf("Dir:  %-30s (can go deeper)\n", ent->d_name);
        }
        else if(isSymbolicLink(ent->d_name))
        {
            printf("Is symbolic link");
        }
        
    }

    return 0;
}

bool parseArguments(char * argv[],int argc, struct Arguments * args){
        
    //TODO: Falta adicionar o path!
    for (int i = 1; i < argc; i++)
        if(!activateFlag(argv[i],args)){
            if(isPath(argv[i]) != -1){
                strcpy(args->path,argv[i]);
            } else return false;
        }
    return true;
}

int pathProvided(char flags[][MAX_FLAG_LEN], size_t len)
{
    printf("\n");
    short index = -1;
    for (size_t i = 0; i < len; ++i)
        for (size_t j = 0; j < strlen(flags[i]); ++j)
            if (flags[i][j] == '/' || flags[i][j] == '~')
                index = i;

    if (index == -1)
    {
        // path not provided
        printf("%sAnalysing current working directory.%s\n", GREEN_TEXT, RESET_TEXT_COLOR);
        return -1;
    }

    if (opendir(flags[index]) == NULL)
    {
        // path provided but invalid
        printf("%sNo valid path to directory provided %s('%s').\n", RED_TEXT, RESET_TEXT_COLOR, flags[index]);
        printf("%sAnalysing current working directory.%s\n", GREEN_TEXT, RESET_TEXT_COLOR);
        return -1;
    }

    else
    {
        // path provided and valid
        printf("%sAnalysing '%s'%s\n", GREEN_TEXT, flags[index], RESET_TEXT_COLOR);
        return index;
    }
}

void details(char dirname[])
{
    DIR *dir = opendir(dirname);
    struct dirent *ent;
    struct stat buf;

    while ((ent = readdir(dir)) != NULL)
    {
        // skip showing current folder and parent folder
        if (strcmp(dirname, ".") == 0 || strcmp(dirname, "..") == 0)
            continue;

        stat(dirname, &buf);

        // apply different behaviors for files and directories
        if (isFile(dirname))
            printf("File: %-30s %ld Bytes\n", dirname, buf.st_size);
        else if (isDirectory(dirname))
        {
            printf("Directory: %-30s (can go deeper)\n", dirname);
            details(dirname);
        }
    }
}

bool isFile(const char *path)
{
    struct stat buf;
    stat(path, &buf);
    if (S_ISREG(buf.st_mode)) // File
        return true;
    return false;
}

bool isDirectory(const char *path)
{
    struct stat buf;
    stat(path, &buf);
    if (S_ISDIR(buf.st_mode)) // Directory
        return true;
    return false;
}

bool isSymbolicLink(const char *path)
{
    struct stat buf;
    stat(path, &buf);
    if (S_ISLNK(buf.st_mode)) // Symbolic Link
        return true;
    return false;
}




int validFlag(char *flag)
{
    for (int i = 0; i < 13; i++)
    {
        if (!strcmp(flag, ARGUMENTS[i])){
            return i; //Returns position in the ARGUMENTS array
        }
    }
    return -1;
}

bool activateFlag(char * flag, struct Arguments * args){
    int index = validFlag(flag);
    if(index == -1) return false;
    else if(!strcmp(ARGUMENTS[index],ALL_FLAG_SHORT) || !strcmp(ARGUMENTS[index],ALL_FLAG_LONG)){
        args->all = true;
        return true;
    }
    else if(!strcmp(ARGUMENTS[index],BYTES_FLAG_SHORT) || !strcmp(ARGUMENTS[index],BYTES_FLAG_LONG)){
        args->bytes = true;
        return true;
    }
    else if(!strcmp(ARGUMENTS[index],BLOCKSIZE_FLAG_SHORT) || !strcmp(ARGUMENTS[index],BLOCKSIZE_FLAG_LONG)){
        args->block_size = true;
        return true;
    }
    else if(!strcmp(ARGUMENTS[index],COUNTLINKS_FLAG_SHORT) || !strcmp(ARGUMENTS[index],COUNTLINKS_FLAG_LONG)){
        args->countLinks = true;
        return true;
    }
    else if(!strcmp(ARGUMENTS[index],LINK_FLAG_SHORT) || !strcmp(ARGUMENTS[index],LINK_FLAG_LONG)){
        args->deference = true;
        return true;
    }
    else if(!strcmp(ARGUMENTS[index],SEPARATEDIRS_FLAG_SHORT) || !strcmp(ARGUMENTS[index],SEPARATEDIRS_LONG)){
        args->separateDirs = true;
        return true;
    }
    else if(!strcmp(ARGUMENTS[index],MAX_DEPTH_FLAG)){
        args->max_depth = 1; //TODO
        return true;
    }
    return false;
}

// if path is not valid return -1
// if file return 0
// if directory return 1
int isPath(const char *path) {
    struct stat path_stat;
    if (lstat(path, &path_stat) < 0) // Invalid path
        return -1;
    else if (isDirectory(path)) // Directory
        return 1;
    else if (isFile(path)) // File
        return 0;
    else if(isSymbolicLink(path))
        return 2;
    else
        return -1;
}

void printFile(char * file, long size){
    //Como descobrir o block size?
    // if(args.bytes)
    //     printf("File: %-30s %ld Bytes\n", file, size);
    // else printf("File: %-30s %ld real Bytes\n", file, size / args.block_size);
    printf("File: %-30s %ld Bytes\n", file, size);

}