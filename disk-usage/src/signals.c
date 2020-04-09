#include "signals.h"
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include "register.h"

extern pid_t child_pid;

void sigint_handler(int signo) {
    registerRecSignal(signo);
    
    if(child_pid) {
        registerSendSignal(-child_pid,SIGSTOP);
        kill(-child_pid, SIGSTOP);
    }

    if (askTerminateProgram()) {
        registerSendSignal(-child_pid,SIGTERM);
        kill(-child_pid, SIGTERM);
        registerExit(0);
    }
    else {
        registerSendSignal(-child_pid,SIGCONT);
        kill(-child_pid, SIGCONT);
    }
    
}

void sigstop_handler(int signo) {
    // register sigstop
    pause();
}

void sigcont_handler(int signo) {
    // register sigcont
}

void sigterm_handler(int signo) {
    // register sigterm
    exit(0);
}

void install_sighandlers() {
    signal(SIGINT, sigint_handler);
}

void install_sigactions() {
    struct sigaction action;
    action.sa_handler = sigint_handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    if (sigaction(SIGINT, &action, NULL) < 0) {
        fprintf(stderr, "Unable to install SIGINT handler\n");
        exit(1);
    }

/*
    action.sa_handler = sigstop_handler;
    action.sa_flags = 0;
    sigemptyset(&action.sa_mask);
    if (sigaction(SIGSTOP, &action, NULL) < 0) {
        fprintf(stderr, "Unable to install SIGSTOP handler\n");
        exit(1);
    } 

    action.sa_handler = sigcont_handler;
    action.sa_flags = 0;
    sigemptyset(&action.sa_mask);
    if (sigaction(SIGCONT, &action, NULL) < 0) {
        fprintf(stderr, "Unable to install SIGCONT handler\n");
        exit(1);
    }  

    action.sa_handler = sigterm_handler;
    action.sa_flags = 0;
    sigemptyset(&action.sa_mask);
    if (sigaction(SIGTERM, &action, NULL) < 0) {
        fprintf(stderr, "Unable to install SIGTERM handler\n");
        exit(1);
    }  
*/
}

bool askTerminateProgram() {
    char answer[1];
    if(write(STDOUT_FILENO, "\n\nTerminate program? (Y/N): ", 28) == -1) {
        perror("WRITE IN SIGINT HANDLER");
        exit(-1);
    }
    fflush(stdout);

    while (true) {
        if(read(STDIN_FILENO, answer, 1) == -1) {
            perror("READ IN SIGINT HANDLER");
            exit(-1);
        }
        //answer = getchar();
        if (answer[0] == 'y' || answer[0] == 'Y') return true;
        if (answer[0] == 'n' || answer[0] == 'N') return false;
    }
}
