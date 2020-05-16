#define MAX_THREADS 10000
#define UPPER_TIME 30
#define LOWER_TIME 5
#define REQUEST_INTERVAL 10
#define MAX_ATTEMPTS 10

/**
 * @brief Task for the client's threads
 * Receives the name of the public fifo and sends a placement request to it
 * Then it creates a private fifo according to protocol, and tries to read 
 * the server's answer from it given a max number of attempts
 * 
 */
void *client_thread_task(void *arg);


