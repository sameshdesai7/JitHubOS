#ifndef SYS_CALL_H
#define SYS_CALL_H
#include <context.h>
#include <dataStructs.h>

/**
 * @file sys_call.h
 * @brief Header file for the sys_call function
*/

/**
 * @author Jackson Monk
 * @author Sam Desai
 * @author Noah Marner
 * @author Blake Wagner
 * @param proc_context process context
 * @brief Based on process's opcode, changes the context of the system
 * @details Takes in a process's context, reads the value of the EAX register to determine the opcode. 
 * Then breaks into idle, exit, or other (write/read). Upon first idle, sets original context to return to later.
 * If there is a process in the ready queue, dequeue. If there is a currently operating process save its context, 
 * add it back to the ready queue, and set the currently operating process to the process recently dequeued. Return a
 * context pointer to the currently operating process's stack pointer. If ready queue is empty, return the process's context.
 * If opcode is exit, dequeue from the ready queue. If there is nothing in the ready queue, return to original context and
 * refresh original context variable to NULL. Otherwise, free the currently operating process from memory and return the pointer to
 * the current operating process's stack. Otherwise, if performing a read/write, set the EAX to -1 and return the 
 * process's context.
 */
context* sys_call(context* proc_context);

#endif
