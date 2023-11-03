#include <scheduler.h>
#include <dataStructs.h>
#include <sys_call.h>

extern pcb* cop;
extern queue* blocked;

int ioSchedule(iocb* request, dcb* device){
    
    // if device is busy
    if (device->status == 1){
        iocbEnqueue(device->iocbQ, request);
        enqueue(blocked, cop);
        cop->state = "blocked";
        return 0;
    } else{
        return 0;
    }

    return 0;
}

int ioComplete(dcb* device){
    return device->status;
}
