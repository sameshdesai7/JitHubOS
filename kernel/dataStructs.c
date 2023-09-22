#include <dataStructs.h>
#include <stdio.h>
#include <string.h>
#include "../include/stdlib.h"

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


/**
 * @brief Allocates memory for a new PCB
*/
pcb* pcb_allocate(void){
    pcb* newPCB = sys_alloc_mem(sizeof(pcb));
    if (newPCB == NULL) {
        puts("Error allocating memory for new PCB");
        return NULL;
    }
    return newPCB;
}

int pcb_free(pcb* pcb){
    if(sys_free_mem(pcb) == -1){
        puts("Could not free memory for PCB");
        return -1;
    }
    else return 1;
}

pcb* pcb_setup(const char * name, int clas, int priority){
    if(priority > 9 || priority < 0){
        puts("Priority must be between [0-9]");
        return NULL;
    }
    pcb* newPCB = pcb_allocate();

    newPCB->name_ptr = sys_alloc_mem(sizeof(char*));
    newPCB->name_ptr = (char*)name;

    newPCB->priority = priority;
    newPCB->clas = clas;

    newPCB->state = sys_alloc_mem(sizeof(char*));
    newPCB->state = (char*)"ready";

    newPCB->next = NULL;
    
    return newPCB;
}

pcb* pcb_find(queue* ready, queue* blocked, queue* susReady, queue* susBlocked, const char * name){

    
    pcb* temp = ready->head;
    
    while(temp->name_ptr != name && temp->next != NULL){
        temp = temp->next;
        if(temp->name_ptr == name){
            return temp;
        }
    }

    temp = blocked -> head;

    while(temp->name_ptr != name && temp->next != NULL){
        temp = temp->next;
        if(temp->name_ptr == name){
            return temp;
        }
    }
    
    temp = susReady->head;

    while(temp->name_ptr != name && temp->next != NULL){
        temp = temp->next;
        if(temp->name_ptr == name){
            return temp;
        }
        
    }

    temp = susBlocked->head;
    
    while(temp->name_ptr != name && temp->next != NULL){
        temp = temp->next;
        if(temp->name_ptr == name){
            return temp;
        }
    }
    
    return NULL;
}

int pcb_insert(queue* ready, queue* blocked, queue* susReady, queue* susBlocked, pcb* new_pcb){

    if(strcmp_ic(new_pcb->state,"ready")){
        enqueue(ready, new_pcb);
    }
    else if(strcmp_ic(new_pcb->state,"blocked")){
        enqueue(blocked, new_pcb);
    }
    else if(strcmp_ic(new_pcb->state,"susReady")){
        enqueue(susReady, new_pcb);
    }
    else if(strcmp_ic(new_pcb->state,"susBlocked")){
        enqueue(susBlocked, new_pcb);
        
    }
    else{
        return -1;
    }
    return 0;
    
    
}

pcb* pcb_remove(queue* ready, queue* blocked, queue* susReady, queue* susBlocked, pcb* pcb){

    queue* q = ready;
    struct pcb* temp = ready->head;

    if (strcmp_ic(temp->state, "blocked") == 0){
        q = blocked;
    }
    else if (strcmp_ic(temp->state, "susReady") == 0){
        q = susReady;
    }
    else if (strcmp_ic(temp->state, "susBlocked") == 0){
        q = susBlocked;
    }
    else{
        return NULL;
    }

    temp = q->head;

    if(temp == NULL){
        return NULL;
    }

    if(temp->next == NULL && temp->name_ptr == pcb->name_ptr){
        q->head = NULL;
        q->tail = NULL;
    }

    while(temp->next != NULL && temp->next->name_ptr != pcb->name_ptr){
        temp = temp->next;
    }
    
    if(temp->next->name_ptr == pcb->name_ptr){
        if(temp->next->next == NULL){
            temp->next = NULL;
            q->tail=temp;
        }
        else {
            struct pcb* temp2 = temp->next;
            temp->next = temp->next->next;
            temp2->next = NULL;
        }
            return temp->next;
    }
    else return NULL;

}

