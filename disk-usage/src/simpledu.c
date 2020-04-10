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

    if(!args.is_dir) { 
        struct stat stbuf;
        if (args.deference)
            stat(args.path, &stbuf);
        else
            lstat(args.path, &stbuf);

        printEntity(scanEntity(stbuf), args.path);    
    }
    else {
        recursiveScan(args.path, 0);
    }

    registerExit(0);
}

// ------------------- Directory Scanning -------------------------

int recursiveScan(char *directory_name, int current_depth)
{

    DIR *dir = opendir(directory_name);
    struct dirent *ent;
    struct stat stbuf;

    size_t current_dir_size = 0;

    while ((ent = readdir(dir)) != NULL)
    {
        // skip showing current folder and parent folder
        if (strcmp(ent->d_name, "..") == 0) continue;

        // build new path and populate stat buffer
        char *entity_path = (char *)malloc(MAX_LEN);
        sprintf(entity_path, "%s/%s", directory_name, ent->d_name);   
        if (args.deference)
            stat(entity_path, &stbuf);
        else
            lstat(entity_path, &stbuf);

        // current directory file size
        if(strcmp(ent->d_name, ".") == 0) {
            current_dir_size += scanEntity(stbuf);
            free(entity_path);
            continue;
        }

        // regular file
        if (S_ISREG(stbuf.st_mode) || S_ISLNK(stbuf.st_mode)) {
            
            long file_size = scanEntity(stbuf);
            current_dir_size += file_size;
            
            if(args.all && (args.max_depth == INT_MAX || current_depth < args.max_depth)) {
                printEntity(file_size, entity_path);
                registerEntry(file_size, entity_path);
            }
        }

        // sub-directory
        else if (S_ISDIR(stbuf.st_mode)) {
            int filedes[2];

            if (pipe(filedes) < 0) {
                perror("Error in pipe creation\n");
                free(entity_path);
                registerExit(-1);
            }

            pid_t pid = fork();

            if (pid < 0) {
                perror("Error in fork\n");
                free(entity_path);
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

                int next_dir_size = recursiveScan(entity_path, current_depth + 1);
                
                if(!args.separateDirs) {
                    registerSendPipe(next_dir_size);
                    write(filedes[WRITE], &next_dir_size, sizeof(next_dir_size));
                }

                close(filedes[WRITE]);

                free(entity_path);
                registerExit(0);
            }
        }

        free(entity_path);
    }
    
    closedir(dir);

    if(args.max_depth == INT_MAX || current_depth <= args.max_depth) {
        
        if(current_depth == 0 && args.pathHasBar)
            strcat(directory_name, "/");

        printEntity(current_dir_size, directory_name);
    }
    return current_dir_size;
}


long scanEntity(struct stat stbuf)
{
    //Checks -B and -b flag
    if (args.bytes) 
        return stbuf.st_size;
    else if (args.block_size_flag) 
        return stbuf.st_blocks * 512 / args.block_size;
    else 
        return stbuf.st_blocks / 2;
}

void printEntity(long size, char* path)
{
    char entryString[MAX_REG_LEN];
    sprintf(entryString, "%ld\t%s\n",size, path);
    write(STDOUT_FILENO, entryString, strlen(entryString));
}


