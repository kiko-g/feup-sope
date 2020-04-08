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

    signalHandler();

    if (!parseArguments(argv, argc)) {
        printf("Error Parsing arguments");
        registerExit(1);
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
        if (strcmp(ent->d_name, "..") == 0) continue;
        else if(strcmp(ent->d_name, ".") == 0) {
            current_dir_size += scanFile(directory_name);
            continue;
        }

        // regular file
        if ((ent->d_type == DT_REG || ent->d_type == DT_LNK)) {
            char *file_path = (char *)malloc(MAX_LEN);
            sprintf(file_path, "%s/%s", directory_name, ent->d_name);
            long file_size = scanFile(file_path);
            current_dir_size += file_size;
            
            if(args.all) {
                printf("%ld\t%s\n", file_size, file_path);
                registerEntry(file_size,file_path); //Dou aqui o register?
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
                registerSendPipe(next_dir_size);
                if(!args.separateDirs) {
                    write(filedes[WRITE], &next_dir_size, sizeof(next_dir_size));
                }

                free(directory_path);

                registerExit(0);
            }
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
    if (args.deference) stat(file_path, &st);
    else lstat(file_path, &st);

    //Checks -B and -b flag
    if (args.bytes) return st.st_size;
    else if (args.block_size_flag) return st.st_blocks * 512 / args.block_size;
    else return st.st_blocks / 2;
}


