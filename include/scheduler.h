#ifndef SCHEDULER_H
#define SCHEDULER_H
#include <dataStructs.h>

/**
 @file scheduler.h
 @brief Helper functions to handle scheduling I/O requests
*/

/**
 * @author Sam Desai
 * @author Blake Wagner
 * @author Noah Marner
 * @author Jackson Monk
 * @brief Adds I/O request to IOCB queue
 * 
 * @details Ensures OP code is read or write. If the queue is empty, DCB directly receives IOCB request.
 * Otherwise, adds IO request to device IOCB queue. 
 * @param request IOCB request
 * @param device device to run the I/O on
 * @return int for success checking
*/
int ioSchedule(iocb* request, dcb* device);

/**
 * @author Sam Desai
 * @author Blake Wagner
 * @author Noah Marner
 * @author Jackson Monk
 * @brief Waits for I/O to complete before processing next request
 * 
 * @details If eFlag is COMPLETE, ensures there is something in the IOCB queue and moves the associated
 * process from the blocked queue (waiting for I/O) back into the ready queue and then removes the
 * I/O request from the IOCB queue.
 * @param device device to run the I/O on
*/
void ioComplete(dcb* device);

#endif
