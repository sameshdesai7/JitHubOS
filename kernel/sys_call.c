#include <context.h>
#include <sys_call.h>
#include <dataStructs.h>
#include <sys_req.h>

extern queue* ready;

pcb* cop = NULL;
context* original_context = NULL;

context* sys_call(context* proc_context) {
    int EAX = proc_context->EAX;
    
    if (original_context == NULL && EAX == IDLE) {
        original_context = proc_context;
    }

    
    
    //If EAX is IDLE, meaning the process is only giving up control of the CPU for the time being, we save the context of the current process (if there is one) and put it back in the ready queue
    if (EAX == IDLE) {
        pcb* temp = dequeue(ready);
        if (temp == NULL) {
            proc_context->EAX = 0;
            return original_context;
        }
        else {
            //If there is already a running process, save its state and add it back to the ready queue
            if (cop != NULL) {
                cop->stack_ptr = (unsigned char *)proc_context;
                cop->state = "ready";
                enqueue(ready, cop);
            }
            cop = temp;
            cop->state = "running";
            //proc_context->EAX = 0;
            return (context *)cop->stack_ptr;
        }
    }

    //If EAX is EXIT, meaning the process is terminating, we simply load in the next process. If the ready queue is empty, we return the context of the original process
    else if (EAX == EXIT) {
        pcb* temp = dequeue(ready);
        if (temp == NULL) {
            proc_context->EAX = 0;
            return original_context;
        }
        else {
            cop = temp;
            proc_context->EAX = 0;
            return (context *)cop->stack_ptr;
        }
    }

    else {
        proc_context->EAX = -1;
        //Delete this once comhand is a process
        if (EAX != WRITE) original_context = NULL;
        return proc_context;
    }
}
