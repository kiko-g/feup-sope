#include <stdlib.h>
#include "queue.h"


Queue* create_queue(int capacity) {
    Queue* queue = (Queue*) malloc(sizeof(Queue));

    queue->capacity = capacity;
    queue->length = 0;
    queue->front = 0;
    queue->back = capacity - 1;
    queue->requests = (BathroomRequest*) malloc(capacity * sizeof(BathroomRequest));
    
    return queue;
}


int empty_queue(Queue* queue) {
    if(queue->length == 0)
        return 1;

    return 0;
}


BathroomRequest front_queue(Queue* queue) {
    return queue->requests[queue->front];
}


void push_queue(Queue* queue, BathroomRequest request) {
    if(queue->length == queue->capacity) {
        double_queue(queue);
    }

    queue->length++;
    queue->back = (queue->back + 1) % queue->capacity;

    queue->requests[queue->back] = request;
}


BathroomRequest pop_queue(Queue* queue) {
    BathroomRequest request = queue->requests[queue->front];

    queue->length--;
    queue->front = (queue->front + 1) % queue->capacity;

    return request;
}


void double_queue(Queue* queue) {
    Queue* bigger_queue = create_queue(queue->capacity * 2);
    while(queue->length > 0) {
        push_queue(bigger_queue, pop_queue(queue));
    }

    free(queue->requests);
    queue = bigger_queue;
}


void destroy_queue(Queue* queue) {
    free(queue->requests);
    free(queue);
}
