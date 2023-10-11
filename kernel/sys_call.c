#include <context.h>
#include <sys_call.h>
#include <dataStructs.h>
#include <sys_req.h>
#include <dataStructs.h>

context* sys_call(context* proc_context) {
    sys_req(IDLE);

    if (cop_context == NULL) {
        cop_context = proc_context;
    }

    int EAX = proc_context->EAX;
    
    //If EAX is IDLE, meaning the process is only giving up control of the CPU for the time being, we save the context of the process and put it back in the ready queue
    if (EAX == IDLE) {
        pcb* temp = dequeue(ready);
        if (temp == NULL) {
            return cop_context;
        }
        else {
            cop->stack_ptr = cop_context;
            cop->state = "ready";
            enqueue(ready, cop);
            cop = temp;
            cop->state = "running";
            cop_context = temp->stack_ptr;
            return cop_context;
        }
    }

    //If EAX is EXIT, meaning the process is terminating, we simply load in the next process
    else if (EAX == EXIT) {
        pcb* temp = dequeue(ready);
        if (temp == NULL) {
            return cop_context;
        }
        else {
            cop = temp;
            cop_context = temp->stack_ptr;
            return cop_context;
        }
    }

    return NULL;
}
