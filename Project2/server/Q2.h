#define MAX_THREADS 10000
#define MAX_STR_LEN 128

/**
 * @brief Task for the server's threads
 * Receives a message from a client asking for placement. It parses it and
 * then calculates a place for it. It then opens the client's private fifo
 * according to protocol and answers to the request. It also takes into account
 * the very rare case when a request is sent before the time is over, but
 * it's answered after the time's over
 */
void *server_thread_task(void *arg);