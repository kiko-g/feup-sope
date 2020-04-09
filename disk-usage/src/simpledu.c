#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include "simpledu.h"
#include "register.h"
#include "parser.h"
#include "signals.h"

extern struct Arguments args;
pid_t original_pgrp_id;
pid_t child_pid = 0;

int main(int argc, char *argv[])
{
    original_pgrp_id = getpgrp();

    createRegistersFile();
    registerCreate(argv,argc);

    install_sigactions();

    if (!parseArguments(argv, argc)) {
        printf("Error Parsing arguments");
        registerExit(1);
    }
    recursiveScan(args.path, args.max_depth);
    registerExit(0);
}

// ------------------- Directory Scanning -------------------------

int recursiveScan(char *directory_name, int max_depth)
{
    if (max_depth == -1) 
        return 0;

    DIR *dir = opendir(directory_name);
    struct dirent *ent;

    size_t current_dir_size = 0;

    while ((ent = readdir(dir)) != NULL)
    {
        // skip showing current folder and parent folder
        if (strcmp(ent->d_name, "..") == 0) continue;
        else if(strcmp(ent->d_name, ".") == 0) {
            current_dir_size += scanFile(directory_name);
            continue;
        }
        char *path = (char *)malloc(MAX_LEN);
        sprintf(path, "%s/%s", directory_name, ent->d_name);

        // regular file
        if ((isFile(path) || isSymbolicLink(path))) {
            
            long file_size = scanFile(path);
            current_dir_size += file_size;
            
            if(args.all) {
                char entryString[MAX_REG_LEN];
                sprintf(entryString, "%ld\t%s\n",file_size, path);
                write(STDOUT_FILENO, entryString, strlen(entryString));
                registerEntry(file_size,path);
            }

            free(path);
        }

        // directory
        else if (isDirectory(path)) {
            int filedes[2];

            if (pipe(filedes) < 0) {
                perror("Error in pipe creation\n");
                registerExit(-1);
            }

            pid_t pid = fork();

            if (pid < 0) {
                perror("Error in fork\n");
                registerExit(-1);
            }
            else if (pid > 0) { // parent process that waits for childs
                close(filedes[WRITE]);

                // set child's pid -> used to stop it after ctrl+c
                if(getpgrp() == original_pgrp_id) {
                    child_pid = pid;
                }

                // wait for child processes to finish
                pid_t wpid;
                while ((wpid = wait(NULL)) > 0);

                if(!args.separateDirs) {
                    int next_dir_size;
                    read(filedes[READ], &next_dir_size, sizeof(int));
                    registerRecPipe(next_dir_size);
                    current_dir_size += next_dir_size;
                }

                close(filedes[READ]);
            }
            else { // child process to analyze subdirectory
            
                close(filedes[READ]);

                // create process group for children -> used to stop them after ctrl+c
                if(getpgrp() == original_pgrp_id) {
                    setpgid(pid, getpid());
                }

                char *directory_path = (char *)malloc(MAX_LEN);
                sprintf(directory_path, "%s/%s", directory_name, ent->d_name);

                int next_dir_size = recursiveScan(directory_path, max_depth - 1);
                
                if(!args.separateDirs) {
                    registerSendPipe(next_dir_size);
                    write(filedes[WRITE], &next_dir_size, sizeof(next_dir_size));
                }

                close(filedes[WRITE]);
                free(directory_path);

                registerExit(0);
            }
        }
    }

    closedir(dir);
    char entryString[MAX_REG_LEN];
    sprintf(entryString, "%ld\t%s\n", current_dir_size, directory_name);
    write(STDOUT_FILENO, entryString, strlen(entryString));

    return current_dir_size;
}

long scanFile(char *file_path)
{
    //Checks deference flag
    struct stat st;
    if (args.deference) stat(file_path, &st);
    else lstat(file_path, &st);

    //Checks -B and -b flag
    if (args.bytes) return st.st_size;
    else if (args.block_size_flag) return st.st_blocks * 512 / args.block_size;
    else return st.st_blocks / 2;
}


