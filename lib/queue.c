#include <dataStructs/queue.h>
#include <stdlib.h>
#include <pcb.h>
#include <stdio.h>

void enqueue(queue* q, node* newNode){
    
    if(q->head == NULL){
        q->head = newNode;
        q->tail = newNode;
        return;
    }
    
    q->tail->next = newNode;
    q->tail = newNode;
}

pcb* dequeue(queue* q){
    if(q->head == NULL){
        return NULL;
    }

    node* temp = q->head;

    if(q->head->next != NULL){
        q->head = q->head->next;
        return temp->pcb;
    }

    q->head = NULL;
    q->tail = NULL;
    return temp->pcb;
}

void printq(queue* q){
    node* temp = q->head;
    printf("[");
    while(temp->next != NULL){
        printf("%s, ", temp->pcb->name_ptr);
        temp = temp->next;
    }
    printf("%s]", temp->pcb->name_ptr);
}
