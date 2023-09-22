#ifndef DATASTRUCTS_H
#define DATASTRUCTS_H

#include <memory.h>

typedef struct pcb{
    char *name_ptr;
    int priority;
    int clas;
    char* state;
    struct pcb* next;
} pcb;

typedef struct queue{
    pcb* head;
    pcb* tail;
    int pFlag;
} queue;

void enqueue(queue* q, pcb* newPCB);

pcb* dequeue(queue* q);

void printq(queue* q);

pcb* pcb_allocate(void);

int pcb_free(pcb* pcb);

pcb* pcb_setup(const char * name, int clas, int priority);

pcb* pcb_find(queue* ready, queue* blocked, queue* susReady, queue* susBlocked, const char * name);

int pcb_insert(queue* ready, queue* blocked, queue* susReady, queue* susBlocked, pcb* pcb);

void pcb_remove(queue* ready, queue* blocked, queue* susReady, queue* susBlocked, pcb* pcb);


#endif
