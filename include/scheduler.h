#ifndef SCHEDULER_H
#define SCHEDULER_H
#include <dataStructs.h>

int ioSchedule(iocb* request, dcb* device);

void ioComplete(dcb* device);

#endif
