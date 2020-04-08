#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "register.h"

int registersFileDescriptor;
clock_t initialTime;

bool createRegistersFile()
{

    //Aqui ja e suposto estar definido? ou e suposto definirmos nos?
    setenv("LOG_FILENAME", "registers.txt", 0);
    registersFileDescriptor = open(getenv("LOG_FILENAME"), O_WRONLY | O_TRUNC | O_CREAT, 0644);

    if (registersFileDescriptor < 0)
    {
        printf("Error opening file");
        exit(1);
        return false;
    }
    return true;
}

void setInitialTime()
{
    initialTime = clock();
}

struct Register createRegister(int action)
{
    struct Register reg;
    reg.action = action;
    reg.instant = clock() - initialTime;
    //reg.info = O que deveria sero info?
    reg.pid = getpid();
    return reg;
}

void writeRegister(const struct Register *reg)
{
    char regString[MAX_REG_LEN];
    sprintf(regString, "%.2ld - %.8d - %s - %s\n", reg->instant, reg->pid, actionString(reg->action), reg->info);
    write(registersFileDescriptor, regString, strlen(regString));
}

void registerCreate(char *argv[], int argc) {
    struct Register reg =  createRegister(CREATE);
    for(int i = 0; i < argc; i++){
		strcat(reg.info, argv[i]);
        strcat(reg.info," ");
    }
    writeRegister(&reg);
}

void registerExit(int exitStatus)
{
    struct Register reg = createRegister(EXIT);
    sprintf(reg.info, "%d", exitStatus);
    writeRegister(&reg);
    exit(exitStatus);
}

void registerRecSignal(int signal)
{
    struct Register reg = createRegister(RECV_SIGNAL);
    sprintf(reg.info, "%s", signalString(signal));
    writeRegister(&reg);
}

void registerSendSignal(pid_t pid, int signal)
{
    struct Register reg = createRegister(SEND_SIGNAL);
    sprintf(reg.info, "%s %d", signalString(signal), pid);
    writeRegister(&reg);
}

void registerRecPipe(int size)
{
    struct Register reg = createRegister(RECV_PIPE);
    sprintf(reg.info, "%d",size);
    writeRegister(&reg);
}

void registerSendPipe(int size)
{
    struct Register reg = createRegister(SEND_PIPE);
    sprintf(reg.info, "%d",size);
    writeRegister(&reg);
}

void registerEntry(long bytes, char * path)
{
    // número de bytes (ou blocos) seguido do caminho.
    struct Register reg = createRegister(ENTRY);
    sprintf(reg.info, "%ld\t%s", bytes,path);
    writeRegister(&reg);
}

const char *actionString(int action){
    switch (action){
        case CREATE:
            return "CREATE";
        case EXIT:
            return "EXIT";
        case RECV_SIGNAL:
            return "RECV_SIGNAL";
        case SEND_SIGNAL:
            return "SEND_SIGNAL";
        case RECV_PIPE:
            return "RECV_PIPE";
        case SEND_PIPE:
            return "SEND_PIPE";
        case ENTRY:
            return "ENTRY";
        default:
            return "";
    }
}

const char *signalString(int signal){
    switch(signal){
        case SIGINT:
            return "SIGINT";
        case SIGSTOP:
            return "SIGSTOP";
        case SIGCONT:
            return "SIGCONT";
        case SIGTERM:
            return "SIGTERM";
        default:
            return "";
    }
}