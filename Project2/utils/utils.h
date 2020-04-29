 #include <time.h>

// Operation types
#define IWANT 0
#define RECVD 1
#define ENTER 2
#define IAMIN 3
#define TIMUP 4
#define TLATE 5
#define CLOSD 6
#define FAILD 7
#define GAVUP 8

#define MAX_LEN 128

/**
 * @brief Initializes the timer
 * 
 */
void timer_begin();

/**
 * @brief Returns the current duration
 * 
 * @return int 
 */
int timer_duration();

/**
 * @brief Writes to stdout a log of an operation
 * 
 */
void log_operation(int i, int pid, long tid, int dur, int pl, int op);