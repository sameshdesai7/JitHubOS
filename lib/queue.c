#include <dataStructs/queue.h>
#include "../include/stdlib.h"
#include <pcb.h>
#include <stdio.h>
#include <string.h>

void enqueue(queue* q, pcb* newPCB){
 
    if(q->head == NULL){
        q->head = newPCB;
        q->tail = newPCB;
        return;
    }

    if(q->head->priority > newPCB->priority){
        // newPCB will be head
        newPCB->next = q->head;
        q->head = newPCB;
        return;
    }

    pcb* temp = q->head;

    while(temp->next != NULL && newPCB->priority >= temp->next->priority){
        temp = temp->next;
    }
    
    if(temp->next == NULL) {
        // newPCB will be tail
        q->tail->next = newPCB;
        q->tail = newPCB;
    }
    else{
        newPCB->next = temp->next;
        temp->next = newPCB;
    }
    return;
}

pcb* dequeue(queue* q){
    if(q->head == NULL){
        return NULL;
    }

    pcb* temp = q->head;

    if(q->head->next != NULL){
        q->head = q->head->next;
        return temp;
    }

    q->head = NULL;
    q->tail = NULL;
    return temp;
}

void printq(queue* q){
    pcb* temp = q->head;
    printf("[");
    while(temp->next != NULL){
        printf("%s, ", temp->name_ptr);
        temp = temp->next;
    }
    printf("%s]\n", temp->name_ptr);
}
