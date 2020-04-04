#include "simpledu.h"

int main(int argc, char *argv[])
{
    if(!parseArguments(argv,argc)){
        printf("Error Parsing arguments");
        return 1;
    }

    return recursiveScan(args.path);
}


int recursiveScan(char* directory_name) {
    DIR *dir = opendir(args.path);
    struct dirent *ent;
    
    while ((ent = readdir(dir)) != NULL)
    {
        // skip showing current folder and parent folder
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
            continue;
        
        if (ent->d_type == DT_REG) {
            printf("FILE\n");
            printf("Name: %s\n", ent->d_name);

            char * file_path = (char*) malloc(MAX_CHAR_LEN);
            sprintf(file_path, "%s/%s", directory_name, ent->d_name);

            printf("Path: %s\n", file_path);
            scanFile(file_path);

            printf("\n");
            free(file_path);
        }    
        else if (ent->d_type == DT_DIR) {
            printf("DIRECTORY\n");
            printf("Name: %s\n\n", ent->d_name);            
        }
        else if (ent->d_type == DT_LNK) {
            printf("SYMBOLIC LINK\n");
            printf("Name: %s\n\n", ent->d_name);            
        }
        else {
            printf("UNKNOWN\n");
        }     
        
    }

    return 0;
}


int scanFile(char* file_path) {
    struct stat st;
    stat(file_path, &st);
    printf("Size: %ld Bytes\n", st.st_size);

    return 0;
}



bool parseArguments(char * argv[],int argc){


    bool foundPath = false;
    for (int i = 1; i < argc; i++)
        if(!activateFlag(argv[i])){
            if(isPath(argv[i]) != -1){
                foundPath = true;
                strcpy(args.path,argv[i]);
            } else return false;
        }
    if(!foundPath){
        strcpy(args.path,".");
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


bool isFile(const char *path)
{
    struct stat buf;
    int status = stat(path, &buf);
    
    if(status != 0) {
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
    
    if(status != 0) {
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
    
    if(status != 0) {
        printf("\nError in stat: %d\n", status);
    }

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

bool activateFlag(char * flag){
    int index = validFlag(flag);
    if(index == -1) return false;
    else if(!strcmp(ARGUMENTS[index],ALL_FLAG_SHORT) || !strcmp(ARGUMENTS[index],ALL_FLAG_LONG)){
        args.all = true;
        return true;
    }
    else if(!strcmp(ARGUMENTS[index],BYTES_FLAG_SHORT) || !strcmp(ARGUMENTS[index],BYTES_FLAG_LONG)){
        args.bytes = true;
        return true;
    }
    else if(!strcmp(ARGUMENTS[index],BLOCKSIZE_FLAG_SHORT) || !strcmp(ARGUMENTS[index],BLOCKSIZE_FLAG_LONG)){
        args.block_size = true;
        return true;
    }
    else if(!strcmp(ARGUMENTS[index],COUNTLINKS_FLAG_SHORT) || !strcmp(ARGUMENTS[index],COUNTLINKS_FLAG_LONG)){
        args.countLinks = true;
        return true;
    }
    else if(!strcmp(ARGUMENTS[index],LINK_FLAG_SHORT) || !strcmp(ARGUMENTS[index],LINK_FLAG_LONG)){
        args.deference = true;
        return true;
    }
    else if(!strcmp(ARGUMENTS[index],SEPARATEDIRS_FLAG_SHORT) || !strcmp(ARGUMENTS[index],SEPARATEDIRS_LONG)){
        args.separateDirs = true;
        return true;
    }
    else if(!strcmp(ARGUMENTS[index],MAX_DEPTH_FLAG)){
        args.max_depth = 1; //TODO
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