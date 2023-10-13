#ifndef DATASTRUCTS_H
#define DATASTRUCTS_H
#include <memory.h>
#include <context.h>

/**
 * @file dataStructs.h
 * @brief Data structures associated with processes and the functions to go with them. 
*/



typedef struct pcb{
    char *name_ptr;
    int priority;
    int clas;
    char* state;
    struct pcb* next;
    unsigned int stack[1024];
    context* stack_ptr;
} pcb;

typedef struct queue{
    struct pcb* head;
    struct pcb* tail;
    int pFlag;
} queue;




/**
 * @author Samesh Desai
 * @author Noah Marner
 * @author Jackson Monk
 * @author Blake Wagner
 * @param q
 * @param newPCB
 * @brief Adds a new PCB to the queue.
 * @details Receives a new PCB and an existing queue. Checks if the queue is empty by checking if the head and tail pointers are null. 
 * If so the new PCB is the new head and tail. Checks the priortiy of the PCB received and itterates through the queue 
 * until we find processes with the same priority. Process that have the same priority are first in first out. The last PCB with 
 * the same priority now has its next pointer looking at the PCB we are inserting. The new PCB next now looks at the old next of 
 * the last PCB with the same priority.
*/
void enqueue(queue* q, pcb* newPCB);
 

/**
 * @author Samesh Desai
 * @author Noah Marner
 * @author Jackson Monk
 * @author Blake Wagner
 * @param q
 * @return Returns the removed PCB
 * @brief Removes a pcb from the front of the queue.
 * @details Checks if the queue is empty (Head is NULL). Creates a temp pcb to point to the head. If heads next 
 * exists, set head to point to head next and returns the temp pcb which points to the old head. Otherwise the queue 
 * only has 1 item so the head and tail are set to NULL and we return temp
 * 
*/
pcb* dequeue(queue* q);


/**
 * @author Samesh Desai
 * @author Noah Marner
 * @author Jackson Monk
 * @author Blake Wagner
 * @param q
 * @brief Prints the contents of the queue
 * @details Given a queue, creates a temp PCB pointer to look at head, prints the contents of that PCB, looks at the 
 * current PCBs next, and prints the conents. The functions keeps iterating through the queue until next points to NULL. 
*/
void printq(queue* q);


/**
 * @author Samesh Desai
 * @author Noah Marner
 * @author Jackson Monk 
 * @author Blake Wagner
 * @return PCB pointer
 * @brief Allocate memory for a PCB 
 * @details Uses sysallocmem function to allocate memeory for a pcb based on the size of the PCB struct. Then returns a 
 * pointer to the newly created PCB. If memory fails to allocate returns NULL and prints an error message. 
*/
pcb* pcb_allocate(void);

/**
 * @author Samesh Desai
 * @author Noah Marner
 * @author Blake Wagner
 * @author Jackson Monk
 * @param pcb
 * @return 1 or -1 
 * @brief deallocates the memory associated with a PCB
 * @details Receives a PCB pointer and uses sysmemfree to deallocate the memory associated with the passed in PCB.
 * If the memory freeing is sucessful, returns 1. If not returns -1 and prints and error message. 
*/
int pcb_free(pcb* pcb);

/**
 * @author Jackson Monk
 * @author Blake Wagner
 * @author Noah Marner
 * @author Samesh Desai
 * @param name
 * @param clas
 * @param priority
 * @brief Creates a new PCB
 * @details Takes in a char* for name, int for clas, and int for prioirty. Calls PCB allocate to create a new PCB, 
 * then assigns the passed in parameters to the data members of the PCB struct. When creating a new PCB, the defualt state is ready. 
 * Calls sysallocatemem for the name and state. Returns a pointer to the newly created PCB. 
*/
pcb* pcb_setup(const char * name, int clas, int priority);

/**
 * @author Blake Wagner
 * @author Jackson Monk
 * @author Noah Marner
 * @author Samesh Desai
 * @param ready
 * @param blocked
 * @param susReady
 * @param susBlocked
 * @param name
 * @brief Given a name, returns a pointer to the PCB
 * @details Given a name, iterates through all the queues (ready, blocked, susReady, susBlocked) by creating a temp PCB pointer 
 * starting at the head of each queue. If a PCB has a name that matches the provided name immediatly returns the temp pointer 
 * which is pointing to that PCB. 
*/
pcb* pcb_find(queue* ready, queue* blocked, queue* susReady, queue* susBlocked, const char * name);

/**
 * @author Noah Marner 
 * @author Jackson Monk
 * @author Blake Wagner
 * @author Samesh Desai
 * @param ready
 * @param blocked
 * @param susReady
 * @param susBlocked
 * @param pcb
 * @brief Inserts a PCB into the proper queue.
 * @details Based on the state of the provided PCB calls enqueue with the provided PCB to add the PCB into either the ready, 
 * blocked, susReady, or susBlocked. Returns 0 for a successful insertion and -1 if failed.
*/
int pcb_insert(queue* ready, queue* blocked, queue* susReady, queue* susBlocked, pcb* pcb);

/**
 * @author Noah Marner 
 * @author Jackson Monk
 * @author Blake Wagner
 * @author Samesh Desai
 * @param ready
 * @param blocked
 * @param susReady
 * @param susBlocked
 * @param pcb
 * @brief Removes a PCB.
 * @details Determines the queue the process is in based on the state of the passed in PCB. Creates a temp current PCB pointer. Iterates through the queue to 
 * find the PCB to remove. If PCB to remove is the head set head to point to next and return temp. Otherwise previous now pointer to current next. This circumvents the 
 * PCB we wish to remove. Lastly, check if the desired PCB is the tail and if so set the new tail to be the previous PCB since we are removing the tail. 
*/
void pcb_remove(queue* ready, queue* blocked, queue* susReady, queue* susBlocked, pcb* pcb);


#endif
