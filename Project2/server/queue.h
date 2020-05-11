/**
 * @brief Struct for a bathroom entry request
 * 
 */
typedef struct BathroomRequest {
    int index;
    int pid;
    int tid;
    int dur;
} BathroomRequest;

/**
 * @brief Struct that simulates a queue
 * 
 */
typedef struct Queue {
    int front;
    int back;
    int length;
    int capacity;
    BathroomRequest* requests;
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
BathroomRequest front_queue(Queue* queue);

/**
 * @brief Pushes a request to the queue
 * 
 * @param queue Queue object to operate on
 * @param request to be pushed
 */
void push_queue(Queue* queue, BathroomRequest request);

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
BathroomRequest pop_queue(Queue* queue);

/**
 * @brief Frees the memory taken by the queue
 * 
 * @param queue Queue object to destroy
 */
void destroy_queue(Queue* queue);
