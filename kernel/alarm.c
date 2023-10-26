#include <stdio.h>
#include <stdlib.h>
#include <mpx/io.h>

//The process tht executes the alarm. This will trigger when the "time" parameter is greater than or equal to the current time
void alarmExecution(int time, char* message) {
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


}
