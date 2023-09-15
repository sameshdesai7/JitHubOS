#ifndef MPX_CTYPE_H
#define MPX_CTYPE_H

extern struct pcb *queue_head;

typedef struct pcb {
        char *name_ptr;
        char name_arr[20];
} pcb;

pcb* pcb_allocate(void);

int pcb_free(pcb* pcb);

pcb* pcb_setup(const char * name, int clas, int priority);

pcb* pcb_find(const char * name);

void pcb_insert(pcb* pcb);

void pcb_remove(pcb* pcb);

#endif
