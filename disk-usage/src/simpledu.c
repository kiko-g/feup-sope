#include "simpledu.h"
#include "register.h"

static pid_t ogPid;

int main(int argc, char *argv[])
{
    ogPid = getpid();

    if (!parseArguments(argv, argc))
    {
        printf("Error Parsing arguments");
        return 1;
    }
    return recursiveScan(args.path, args.max_depth);
}

// ------------------- Directory Scanning -------------------------

int recursiveScan(char *directory_name, int max_depth)
{
    if (max_depth == 0)
        return 0;

    DIR *dir = opendir(directory_name);
    struct dirent *ent;

    int current_dir_size = 0;

    while ((ent = readdir(dir)) != NULL)
    {
        // skip showing current folder and parent folder
        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
            continue;

        // regular file
        if ((ent->d_type == DT_REG || ent->d_type == DT_LNK)) {
            char *file_path = (char *)malloc(MAX_CHAR_LEN);
            sprintf(file_path, "%s/%s", directory_name, ent->d_name);
            long file_size = scanFile(file_path);
            current_dir_size += file_size;

            if(args.all) {
                printf("%ld\t%s\n", file_size, file_path);
            }

            free(file_path);
        }

        // directory
        else if (ent->d_type == DT_DIR) {
            // printf("DIRECTORY\n");
            // printf("Name: %s\n\n", ent->d_name);
            int filedes[2];

            if (pipe(filedes) < 0) {
                perror("Error in pipe creation\n");
                exit(-1);
            }

            pid_t pid = fork();

            if (pid < 0) {
                perror("Error in fork\n");
                exit(-1);
            }
            else if (pid > 0) { // parent process that waits for childs
                close(filedes[WRITE]);

                // wait for child processes to finish
                pid_t wpid;
                while ((wpid = wait(NULL)) > 0);

                if(!args.separateDirs) {
                    int next_dir_size;
                    read(filedes[READ], &next_dir_size, sizeof(int));
                    current_dir_size += next_dir_size;
                }

            }
            else { // child process to analyze subdirectory
                close(filedes[READ]);
                char *directory_path = (char *)malloc(MAX_CHAR_LEN);
                sprintf(directory_path, "%s/%s", directory_name, ent->d_name);

                int next_dir_size = recursiveScan(directory_path, max_depth - 1);
                if(!args.separateDirs) {
                    write(filedes[WRITE], &next_dir_size, sizeof(next_dir_size));
                }

                exit(0);
            }
        }
        
        // other (socket, unknown...)
        else
        {
            // printf("OTHER\n");
        }
    }

    closedir(dir);

    printf("%d\t%s\n", current_dir_size, directory_name);

    return current_dir_size;
}

long scanFile(char *file_path)
{
    //Checks deference flag
    struct stat st;
    if(args.deference) stat(file_path, &st);
    else lstat(file_path, &st);
    
    //Checks -B and -b flag
    if (args.bytes) return st.st_size;
    else if (args.block_size_flag){
        return st.st_blocks * 512 / args.block_size;
    } else return st.st_blocks / 2;
}



// ------------------- Argument Parsing -------------------------

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

// ------------------- File Printing -------------------------

void printFile(char *file, long size)
{
    //Como descobrir o block size?
    // if(args.bytes)
    //     printf("File: %-30s %ld Bytes\n", file, size);
    // else printf("File: %-30s %ld real Bytes\n", file, size / args.block_size);
    printf("File: %-30s %ld Bytes\n", file, size);
}