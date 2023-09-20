#include <pcb.h>

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

// int pcb_free(pcb* pcb){

// }

// pcb* pcb_setup(const char * name, int clas, int priority){

// }

// pcb* pcb_find(const char * name){

// }

// void pcb_insert(pcb* pcb){

// }

// void pcb_remove(pcb* pcb){

// }

