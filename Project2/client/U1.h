#define MAX_THREADS 10000
#define UPPER_TIME 30
#define LOWER_TIME 5

/**
 * @brief Task for the client's thread
 * 
 */
void *client_thread_task(void *arg);

/**
 * @brief Creates the name of the private fifo
 * to connect the client to the server
 * 
 * @param fifoname The name of the new private fifo
 */
void make_private_fifo(char *fifo_name);

