#include "simpledu.h"
#include "register.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include "parser.h"

extern struct Arguments args;

int main(int argc, char *argv[])
{
    // signalHandler();

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
    if (max_depth == -1)
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
    if (args.deference)
        stat(file_path, &st);
    else
        lstat(file_path, &st);

    //Checks -B and -b flag
    if (args.bytes)
        return st.st_size;
    else if (args.block_size_flag)
    {
        return st.st_blocks * 512 / args.block_size;
    }
    else
        return st.st_blocks / 2;
}


