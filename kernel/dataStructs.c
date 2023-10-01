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

    if(q->pFlag == 1){
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
    }
    else{
        q->tail->next = newPCB;
        q->tail = newPCB;
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
    if(q->head == NULL){
        printf(" ___\n| \n| \n| \n| \n|___\n");
        return;
    }

    pcb* temp = q->head;

    if(temp->next == NULL){
        printf(" ___\n
                |\n
                |  Name:      %s\n
                |  Class:     %s\n
                |  State:     %s\n
                |  Suspended: %s\n
                |  Priority:  %s\n
                |___");
    }

    char* susStatus = "No";

    printf(" ___\n| \n|  ");
    while(temp->next != NULL){
        printf("Name: %s ", temp->name_ptr);
        temp = temp->next;
    }
    
    temp = q->head;
    printf("\n|  ");
    while(temp->next != NULL){
        printf("Class: %s ", temp->clas);
        temp = temp->next;
    }

    temp = q->head;
    printf("\n|  ");
    while(temp->next != NULL){
        printf("State: %s ", temp->state);
        temp = temp->next;
    }

    temp = q->head;
    printf("\n|  ");
    while(temp->next != NULL){
        if(strcmp(temp->state, "susReady") == 0 || strcmp(temp->state, "susBlocked") == 0){
            susStatus = "Yes";
        } else susStatus = "No";
        
        printf("Suspended: %s ", susStatus);
        temp = temp->next;
    }

    temp = q->head;
    printf("\n|  ");
    while(temp->next != NULL){
        printf("Priority: %s, ", temp->priority);
        temp = temp->next;
    }
    printf("%s___\n", temp->name_ptr);
    return;
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

pcb* pcb_find(queue* ready, queue* blocked, queue* susReady, queue* susBlocked, const char* name){

    pcb* temp = ready->head; 
    if(temp != NULL){  
        if(strcmp(temp->name_ptr, name) == 0){
                return temp;
            }
        while(temp->next != NULL && strcmp(temp->name_ptr, name) != 0){
            temp = temp->next;
            if(strcmp(temp->name_ptr, name) == 0){
                return temp;
            }
        }
    }

    temp = blocked -> head;
    if(temp != NULL){
        if(strcmp(temp->name_ptr, name) == 0){
                return temp;
            }
        while(temp->next != NULL && strcmp(temp->name_ptr, name) != 0){
            temp = temp->next;
            if(strcmp(temp->name_ptr, name) == 0){
                return temp;
            }
        }
    }
    
    temp = susReady->head;
    if(temp != NULL){
        if(strcmp(temp->name_ptr, name) == 0){
                return temp;
            }
        while(temp->next != NULL && strcmp(temp->name_ptr, name) != 0){
            temp = temp->next;
            if(temp->name_ptr == name){
                return temp;
            }
            
        }
    }

    temp = susBlocked->head;
    if(temp != NULL){
        if(strcmp(temp->name_ptr, name) == 0){
                return temp;
            }
        while(temp->next != NULL && strcmp(temp->name_ptr, name) != 0){
            temp = temp->next;
            if(temp->name_ptr == name){
                return temp;
            }
        }
    }

    return NULL;
}

int pcb_insert(queue* ready, queue* blocked, queue* susReady, queue* susBlocked, pcb* new_pcb){
    if(strcmp_ic(new_pcb->state,"ready") == 0){
        enqueue(ready, new_pcb);
    }
    else if(strcmp_ic(new_pcb->state,"blocked") == 0){
        enqueue(blocked, new_pcb);
    }
    else if(strcmp_ic(new_pcb->state,"susReady") == 0){
        enqueue(susReady, new_pcb);
    }
    else if(strcmp_ic(new_pcb->state,"susBlocked") == 0){
        enqueue(susBlocked, new_pcb);
    }
    else{
        return -1;
    }
    return 0;
    
}

void pcb_remove(queue* ready, queue* blocked, queue* susReady, queue* susBlocked, pcb* pcb){

    queue* q = ready;
    
    if(strcmp_ic(pcb->state, "ready") == 0){
    }
    else if (strcmp_ic(pcb->state, "blocked") == 0){
        q = blocked;
    }
    else if (strcmp_ic(pcb->state, "susReady") == 0){
        q = susReady;
    }
    else if (strcmp_ic(pcb->state, "susBlocked") == 0){
        q = susBlocked;
    }
    else{
        return;
    }

    struct pcb* current = q->head;
    struct pcb* previous = NULL;

    if(current->next == NULL){
        q->head = NULL;
        q->tail = NULL;
        return;
    }

    while(current != NULL){
        if(strcmp(current->name_ptr, pcb->name_ptr) == 0){
            
            if(current == q->head){
                q->head = current->next;
                current->next = NULL;
            } else {
                previous->next = current->next;
                current->next = NULL;
            }

            if(current == q->tail){
                q->tail = previous;
                previous->next = NULL;
            }

            //TODO: Free memory

            return;
        }

        previous = current;
        current = current->next;
    }

    return;
}

