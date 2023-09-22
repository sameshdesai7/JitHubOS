#ifndef QUEUE_H
#define QUEUE_H

#include "../pcb.h"
#include <stdlib.h>

typedef struct queue {
    pcb* head;
    pcb* tail;
    int priorityFlag;
} queue;

void enqueue(queue* q, pcb* newPCB);

pcb* dequeue(queue* q);

void printq(queue* q);

#endif
