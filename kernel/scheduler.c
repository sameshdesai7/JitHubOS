#include <scheduler.h>
#include <dataStructs.h>
#include <sys_call.h>
#include <sys_req.h>

extern pcb* cop;
extern queue* blocked;

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

int ioComplete(dcb* device){
    return device->status;
}
