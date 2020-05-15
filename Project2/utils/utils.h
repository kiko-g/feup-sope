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

#define MAX_STR_LEN 128

/**
 * @brief struct representing a message exchanged between the server and client
 * Sent in binary to improve efficiency (avoiding conversions from int to string and vice-versa)
 */
typedef struct RequestMessage {
    int i;
    int pid;
    long tid;
    int dur;
    int pl;
} RequestMessage;

/**
 * @brief Initializes the timer, in seconds
 * 
 */
void timer_begin();

/**
 * @brief Returns the current duration
 * 
 * @return int the duration between the starting time and the current time, in seconds
 */
int timer_duration();

/**
 * @brief Writes to stdout a log of an operation
 * 
 * @param i sequential number of the request
 * @param pid process id
 * @param tid thread id
 * @param dur duration of the bathroom "visit"
 * @param pl attributed place of the bathroom 
 * @param op string representing the operation
 */
void log_operation(int i, int pid, long tid, int dur, int pl, int op);

/**
 * @brief Sends a message through the FIFO
 * 
 * @param fd file descriptor for the FIFO
 * @param i sequential number of the request
 * @param pid process id
 * @param tid thread id
 * @param dur duration of the bathroom "visit"
 * @param pl attributed place of the bathroom 
 */
void send_message(int fd, int i, int pid, long tid, int dur, int pl);