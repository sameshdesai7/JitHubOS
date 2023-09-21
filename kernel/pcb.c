#include <pcb.h>
#include <stdio.h>

/**
 * @brief Allocates memory for a new PCB
*/
pcb* pcb_allocate(void){
    pcb* newPCB = sys_alloc_mem(sizeof(pcb*));
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
    newPCB->state = "ready";

    newPCB->next = NULL;
    
    return newPCB;
}

// pcb* pcb_find(const char * name){

// }

// void pcb_insert(pcb* pcb){

// }

// void pcb_remove(pcb* pcb){

// }

