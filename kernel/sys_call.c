#include <context.h>
#include <sys_call.h>
#include <dataStructs.h>
#include <sys_req.h>
#include <stdio.h>
extern queue* ready;

pcb* cop = NULL;
context* original_context = NULL;

context* sys_call(context* proc_context) {

    int EAX = proc_context->EAX;
    
    //If EAX is IDLE, meaning the process is only giving up control of the CPU for the time being, we save the context of the current process (if there is one) and put it back in the ready queue
    if (EAX == IDLE) {

        if (original_context == NULL) {
            original_context = proc_context;
        }

        pcb* temp = dequeue(ready);
        if (temp == NULL) {
            proc_context->EAX = 0;
            return proc_context;
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
            proc_context->EAX = 0;
            return (context *)cop->stack_ptr;
        }
    }

    //If EAX is EXIT, meaning the process is terminating, we simply load in the next process. If the ready queue is empty, we return the context of the original process
    else if (EAX == EXIT) {
        pcb* temp = dequeue(ready);
        if (temp == NULL) {
            proc_context->EAX = 0;
            context* temporary = original_context;
            cop = NULL;
            original_context = NULL;
            return temporary;
        }
        else {
            sys_free_mem(cop);
            cop = temp;
            proc_context->EAX = 0;
            return (context *)cop->stack_ptr;
        }
    }

    else{
        iocb* iocb = sys_alloc_mem(sizeof(iocb));
        iocb->op = EAX;
        iocb->pcb = cop;
        iocb->dcb = proc_context->ebx;
        iocb->buffa = proc_context->ecx;
        iocb->buffaSize = proc_context->edx;

        //Save context of current process and block it
        cop->stack_ptr = proc_context;
        cop->state = "blocked";
        enqueue(blocked, cop);

        ioSchedule(iocb, proc->ebx);

        pcb* temp = dequeue(ready);
        if (temp == NULL) {
            return proc_context;
        }
        else {
            cop = temp;
            cop->state = "running";
            proc_context->EAX = 0;
            return (context *)cop->stack_ptr;
        }

        proc_context->EAX = -1;
        return proc_context;
    }
}
