#define MAX_THREADS 10000
#define MAX_STR_LEN 128

/**
 * @brief 
 * 
 */
typedef struct server_thread_arguments {
    char msg[MAX_STR_LEN];
    int max_duration;
} server_thread_arguments;

/**
 * @brief 
 * 
 * @param arg 
 * @return void* 
 */
void *server_thread_task(void *arg);

/**
 * @brief 
 * 
 * @param fifo_name 
 */
void build_private_fifo(char *fifo_name, int pid, int tid);