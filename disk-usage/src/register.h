#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>

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

/** 
 * @brief set initial time 
 */
void setInitialTime();

/** 
 * @brief
 * @return 
 */
bool createRegistersFile();

/** 
 * @brief
 * @param action
 * @return created register 
 */
struct Register createRegister(int action);

/** 
 * @brief
 * @param action
 * @return 
 */
const char * actionString(int action);

/** 
 * @brief
 * @param exitStatus
 * @return 
 */
void registerExit(int exitStatus);

/** 
 * @brief
 * @param argv array containing the program arguments
 * @param argc total of program arguments
 * @return 
 */
void registerCreate(char *argv[], int argc);

/** 
 * @brief
 * @param signal
 * @return 
 */
void registerRecvSignal(int signal);

/** 
 * @brief
 * @param signal
 * @param pid
 * @return 
 */
void registerSendSignal(pid_t pid,int signal);

/** 
 * @brief
 * @param message 
 * @return 
 */
void registerRecPipe(int size);

/** 
 * @brief
 * @param message 
 * @return 
 */
void registerSendPipe(int size);

/** 
 * @brief
 * @param bytes
 */
void registerEntry(long bytes,char * path);