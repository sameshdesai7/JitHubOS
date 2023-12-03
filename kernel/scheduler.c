#include <scheduler.h>
#include <dataStructs.h>
#include <sys_call.h>
#include <sys_req.h>
#include <context.h>

extern pcb* cop;
extern queue* blocked;
extern queue* ready;
extern queue* susBlocked;
extern queue* susReady;
extern dcb* com1DCB;

int ioSchedule(iocb* request, dcb* device){

    if(request->op == IDLE){
        //Maybe add error code?
        return -1;
    }
    
    // if device is busy
    if (device->status == 1){
        iocbEnqueue(device->iocbQ, request);
        return 0;
    } else{
        device->op = request->op;
        device->buffer_len = request->buffaSize;
        device->buffer = request->buffa;
        return 0;
    }

    return 0;
}

void ioComplete(dcb* device){

    if(com1DCB){
        if(com1DCB ->eFlag == 1){
           pcb* temp = device->iocbQ->head->pcb;
           temp -> state = "ready";
            enqueue(ready, temp);
            pcb_remove(ready,blocked, susReady, susBlocked, temp);
            ((context*)(device->iocbQ->head->pcb->stack_ptr))->EAX = device->count;

            iocb* temp1 = device->iocbQ->head;
            device->iocbQ->head = device->iocbQ->head->next;
            sys_free_mem(temp1);
        }
    }

}
