#ifndef SCHEDULER_H
#define SCHEDULER_H
#include <dataStructs.h>

int ioSchedule(iocb* request, dcb* device);

int ioComplete(dcb* device);

#endif
