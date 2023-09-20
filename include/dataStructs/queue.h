#ifndef QUEUE_H
#define QUEUE_H

#include <../pcb.h>
#include <stdlib.h>

typedef struct node {
    pcb* pcb;
    struct node* next;
} node;

typedef struct queue {
    node* head;
    node* tail;
} queue;

void enqueue(queue* q, node* newNode);

pcb* dequeue(queue* q);

void printq(queue* q);

#endif
