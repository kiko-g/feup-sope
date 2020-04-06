#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "register.h"

FILE *registersFile;
clock_t initialTime;

bool createRegistersFile()
{

    //Aqui ja e suposto estar definido? ou e suposto definirmos nos?
    setenv("LOG_FILENAME", "registers.txt", 0);
    registersFile = fopen(getenv("LOG_FILENAME"), "a");

    if (registersFile == NULL)
    {
        printf("Error opening file");
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
    reg.instant = clock();
    //reg.info = O que deveria sero info?
    reg.pid = getpid();
    return reg;
}

void writeRegister(const struct Register *reg)
{
    fprintf(registersFile, "%.2ld - %.8d - %s - %s\n", reg->instant, reg->pid, actionString(reg->action), reg->info);
}

void registerCreate(char *argv[], int argc) {
    struct Register reg =  createRegister(CREATE);
    for(int i = 1; i < argc; i++){
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
}

void registerRecvSignal(int signal)
{
    struct Register reg = createRegister(RECV_SIGNAL);
    sprintf(reg.info, "%d", signal);
    writeRegister(&reg);
}

void registerSendSignal(int signal)
{
    struct Register reg = createRegister(SEND_SIGNAL);
    sprintf(reg.info, "%d", signal);
    writeRegister(&reg);
}

void registerRecPipe(char *message)
{
    struct Register reg = createRegister(RECV_PIPE);
    strcpy(reg.info, message);
    writeRegister(&reg);
}

void registerSendPipe(char *message)
{
    struct Register reg = createRegister(SEND_PIPE);
    strcpy(reg.info, message);
    writeRegister(&reg);
}

void registerEntry(int bytes)
{
    // número de bytes (ou blocos) seguido do caminho.
    struct Register reg = createRegister(ENTRY);
    sprintf(reg.info, "%d", bytes);
    writeRegister(&reg);
}

const char *actionString(int action)
{
    switch (action)
    {
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