#include "signals.h"
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

void sigint_handler(int signo)
{
    kill(-2,SIGTSTP); //Stops the children
    bool answer = askTerminateprogram();
    if (answer)
    {
        kill(-2,SIGTERM);
        kill(getpid(),SIGTERM); //Terminates the parent
        printf("\n\n Program Terminated");
    }
    else
    {
        kill(-2, SIGCONT); 
        printf("Continuing Program\n\n ");
    }
    
}

void signalHandler()
{
    struct sigaction action;

    action.sa_handler = sigint_handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    if (sigaction(SIGINT, &action, NULL) < 0)
    {
        fprintf(stderr, "Unable to install SIGINT handler\n");
        exit(1);
    }
}

bool askTerminateProgram()
{
    char answer;
    printf("Terminate program? (Y/N): ");

    while (true)
    {
        scanf("%c", &answer);
        if (answer == 'y' || answer == 'Y')
        {
            return true;
        }
        if ((answer == 'n' || answer == 'N'))
        {
            return false;
        }
        printf("\nPlease insert answer again: ");
    }
}
