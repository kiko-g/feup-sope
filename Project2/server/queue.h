#define NO_AVAILABLE_STALL -1

/**
 * @brief Struct that simulates a queue
 * 
 */
typedef struct Queue {
    int front;
    int back;
    int length;
    int capacity;
    unsigned* available_stalls;
} Queue;

/**
 * @brief Create a queue object
 * 
 * @param capacity the maximum capacity of the queue
 * @return Queue* the created object
 */
Queue* create_queue(int capacity);

/**
 * @brief Checks if a queue is empty
 * 
 * @param queue Queue object to check
 * @return int 1 if empty, 0 if otherwise
 */
int empty_queue(Queue* queue);

/**
 * @brief Gets the front of the queue
 * 
 * @param queue Queue object to operate on
 * @return BathroomRequest the request at the front the queue
 */
unsigned front_queue(Queue* queue);

/**
 * @brief Pushes a request to the queue
 * 
 * @param queue Queue object to operate on
 * @param request to be pushed
 */
void push_queue(Queue* queue, unsigned available_stall);

/**
 * @brief Doubles the size of a queue
 * 
 * @param queue Queue object to be doubled
 */
void double_queue(Queue* queue);

/**
 * @brief Pops a request from the queue
 * 
 * @param queue Queue object to operate on
 * @return BathroomRequest 
 */
unsigned pop_queue(Queue* queue);

/**
 * @brief Frees the memory taken by the queue
 * 
 * @param queue Queue object to destroy
 */
void destroy_queue(Queue* queue);

/**
 * @brief Fills the queue with sequential numbers until capacity
 * @param queue Queue to fill
 */ 
void fill_queue(Queue * queue);

