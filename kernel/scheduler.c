#include <scheduler.h>
#include <dataStructs.h>
#include <sys_call.h>

extern pcb* cop;
extern queue* blocked;

int ioSchedule(iocb* request, dcb* device){
    
    // if device is busy
    if (device->status == 1){
        iocbEnqueue(device->iocbQ, request);
        return 0;
    } else{
        device->op = request->op;
        device->size = request->buffaSize;
        device->beginning = request->buffa;
        device->end = request->buffa + sizeof(char*)-1;
        return 0;
    }

    return 0;
}

int ioComplete(dcb* device){
    return device->status;
}
