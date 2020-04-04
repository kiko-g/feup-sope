#include <stdbool.h>
#include <unistd.h>


#define CREATE 0
#define EXIT 1
#define RECV_SIGNAL 2
#define SEND_SIGNAL 3
#define RECV_PIPE 4
#define SEND_PIPE 5
#define ENTRY 6


struct Register {
    long instant;
    pid_t pid;
    int action;
    char info[1000]; //Quanto se deve dar?
};

void setInitialTime();
bool createRegistersFile();
struct Register createRegister(int action);
const char * actionString(int action);
void registerExit(int exitStatus);
void registerCreate(char *argv[], int argc);
void registerRecvSignal(int signal);
void registerSendvSignal(int signal);
void registerRecPipe(char *message);
void registerSendPipe(char *message);
void registerEntry(int bytes);