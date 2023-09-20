#ifndef PCB_H
#define PCB_H

#include <memory.h>

typedef struct pcb {
        char *name_ptr;
} pcb;

pcb* pcb_allocate(void);

int pcb_free(pcb* pcb);

pcb* pcb_setup(const char * name, int clas, int priority);

pcb* pcb_find(const char * name);

void pcb_insert(pcb* pcb);

void pcb_remove(pcb* pcb);

#endif
