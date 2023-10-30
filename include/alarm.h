#include <stdio.h>
#include <stdlib.h>
/**
 * @file alarm.h
 * @brief Header file for the process that executes the alarm.
*/

/**
 * @author Jackson Monk
 * @author Noah Marner
 * @author Sam Desai
 * @author Blake Wagner
 * @brief The process that executes the alarm. This will trigger when the "time" parameter is greater than or equal to the current time
 * @details When function is called, gets and formats system time and gets the alarm time from the currently operating processes
 * alarm struct. If the alarm time is <= the system time, the alarm will trigger and display the message. Otherwise, calls sys_req(idle) to idle
 * the process.
*/
void alarmExecution();
