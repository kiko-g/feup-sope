#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

#define CREATE 0
#define EXIT 1
#define RECV_SIGNAL 2
#define SEND_SIGNAL 3
#define RECV_PIPE 4
#define SEND_PIPE 5
#define ENTRY 6

#define MAX_REG_LEN 1024

struct Register {
    long instant;
    pid_t pid;
    int action;
    char info[1000]; //Quanto se deve dar?
};

/** 
 * @brief set initial time 
 */
void setInitialTime();

/** 
 * @brief Creates the registers file and initializes the initial time
 * @return True if created successfully, False otherwise
 */
bool createRegistersFile();

/** 
 * @brief Creates a Register with pid, action and current instant initialized
 * @param action 
 * @return created register 
 */
struct Register createRegister(int action);

/** 
 * @brief Converts actions flags to text
 * @param action
 * @return Correspondent String
 */
const char* actionString(int action);

/**
 * @brief Converts signal flags to text
 * @param signal 
 * @return Correspondent String
 */
const char* signalString(int signal);

/** 
 * @brief Exits the program and Registers the Exit
 * @param exitStatus
 * @return 
 */
void registerExit(int exitStatus);

/** 
 * @brief Registers the arguments passed to the program
 * @param argv array containing the program arguments
 * @param argc total of program arguments
 * @return 
 */
void registerCreate(char *argv[], int argc);

/** 
 * @brief Registers the signal recieved
 * @param signal Signal recieved
 * @return 
 */
void registerRecSignal(int signal);

/** 
 * @brief Registers the signal sent
 * @param signal Signal Sent
 * @param pid  Current PID
 * @return 
 */
void registerSendSignal(pid_t pid,int signal);

/** 
 * @brief  Registers the recieved pipe
 * @param size Message recieved by the pipe 
 * @return 
 */
void registerRecPipe(int size);

/** 
 * @brief  Registers sent by pipe
 * @param size Message sent by the pipe 
 * @return 
 */
void registerSendPipe(int size);

/** 
 * @brief Register Entry
 * @param bytes Size of the File
 * @param path  Path of the file
 */
void registerEntry(long bytes,char * path);