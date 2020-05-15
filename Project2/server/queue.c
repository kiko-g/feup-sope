#include <stdlib.h>
#include "queue.h"


Queue* create_queue(int capacity) {
    Queue* queue = (Queue*) malloc(sizeof(Queue));

    queue->capacity = capacity;
    queue->length = 0;
    queue->front = 0;
    queue->back = capacity - 1;
    queue->available_stalls = (unsigned*) malloc(capacity * sizeof(unsigned));

    for(int i = 1; i <= capacity; i++) {
        push_queue(queue,i);
    }
    
    return queue;
}


int empty_queue(Queue* queue) {
    if(queue->length == 0)
        return 1;
    return 0;
}


unsigned front_queue(Queue* queue) {
    return queue->front;
}


void push_queue(Queue* queue, unsigned available_stall) {
    queue->length++;
    queue->back = (queue->back + 1) % queue->capacity;

    queue->available_stalls[queue->back] = available_stall;
}


unsigned pop_queue(Queue* queue) {
    if(queue->length == 0)
        return NO_AVAILABLE_STALL;

    unsigned available_stall = queue->available_stalls[queue->front];

    queue->length--;
    queue->front = (queue->front + 1) % queue->capacity;

    return available_stall;
}

void destroy_queue(Queue* queue) {
    free(queue->available_stalls);
    free(queue);
}
