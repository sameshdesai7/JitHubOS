#include <stdio.h>
#include <stdlib.h>
#include <dataStructs.h>
#include <sys_req.h>
#include <mpx/io.h>

extern pcb* cop;


//The process that executes the alarm. This will trigger when the "time" parameter is greater than or equal to the current time
void alarmExecution() {

    printf("Entered alarm\n");

    // Attaching the hours register to port 70
    outb(0x70, 0x04);
    // Reading the data from the hours register through port 71
    int hours = inb(0x71);
    int formattedHours = fromBCD(hours);



    // Attaching the minutes register to port 70
    outb(0x70, 0x02);
    // Reading the data from the minutes register through port 71
    int minutes = inb(0x71);
    int formattedMinutes = fromBCD(minutes);

    // Attaching the seconds register to port 70
    outb(0x70, 0x00);
    // Reading the data from the seconds register through port 71
    int seconds = inb(0x71);
    int formattedSeconds = fromBCD(seconds);

    printf("Current Time: %d:%d:%d\n",formattedHours, formattedMinutes,formattedSeconds);
    printf("Alarm   Time: %d:%d:%d\n",cop->alarm_ptr->hours,cop->alarm_ptr->minutes,cop->alarm_ptr->seconds);

    //Checking to see if the timer has gone off
    //If it has, we display the message and exit the process

    if(formattedHours > cop->alarm_ptr->hours){
        printf(cop->alarm_ptr->message);
        sys_req(EXIT);
    }
    else if(formattedHours == cop->alarm_ptr->hours && formattedMinutes > cop->alarm_ptr->minutes){
        printf(cop->alarm_ptr->message);
        sys_req(EXIT);
    }
    else if(formattedHours >= cop->alarm_ptr->hours && formattedMinutes >= cop->alarm_ptr->minutes && formattedSeconds>=cop->alarm_ptr->seconds){
        printf(cop->alarm_ptr->message);
        sys_req(EXIT);
    }

    //If it hasn't, we yield back to the other processes in the ready queue
    else {
        printf("Idle Case");
        sys_req(IDLE);
    }
    
}
