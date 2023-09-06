#include <comhand.h>
#include <mpx/device.h>
#include <sys_req.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <mpx/io.h>
#include <mpx/serial.h>
#include <ctype.h>
#include <mpx/interrupts.h>

#define COMPILE_DATE __DATE__
#define COMPILE_TIME __TIME__

void comhand()
{

    for (;;){

        printMenu();
        
        char buf[100] = {0};
        sys_req(READ, COM1, buf, sizeof(buf));


        //if shutdown is selected
        if(strcmp(buf, "shutdown") == 0){
            printf("Confirm Shutdown? Y/N\n");
            //if shutdown is confirmed
            sys_req(READ, COM1, buf, sizeof(buf));
            if((strcmp(buf, "Y") == 0) || (strcmp(buf, "y") == 0)){
                return;
            }
        }

void version(void){
    printf("Release Number: %d\n",VERSION);
    printf("Date of most recent compile: %s\n", COMPILE_DATE);
    printf("Time of most recent compile: %s\n", COMPILE_TIME);
}


<<<<<<< HEAD
    outb(0x70,0x04);
    int hours = inb(0x71);
    int formatedHours = fromBCD(hours);

    outb(0x70,0x02);
    int minutes = inb(0x71);
    int formatedMinutes = fromBCD(minutes);

    outb(0x70,0x00);
    int seconds = inb(0x71);
    int formatedSeconds = fromBCD(seconds);

    //Add padding 0 if seconds is 1 digit 
    //Account for single digit minutes or seconds
    //If minutes and seconds are 1 digit
    if((formatedMinutes <=9)&&(formatedSeconds <=9)){
        printf("%d:0%d:0%d",formatedHours,formatedMinutes,formatedSeconds);
    }
    //if seconds are 1 digit
    else if(formatedSeconds <=9){
=======
        //Get Time Command
        else if(strcmp(buf, "Get Time") == 0){

            printf("\n");

            outb(0x70,0x04);
            int hours = inb(0x71);
            int formatedHours = ((hours & 0x70) >> 4)*10 + (hours & 0x0F);

            outb(0x70,0x02);
            int minutes = inb(0x71);
            int formatedMinutes = ((minutes & 0x70) >> 4)*10 + (minutes & 0x0F);

            outb(0x70,0x00);
            int seconds = inb(0x71);
            int formatedSeconds = ((seconds & 0x70) >> 4)*10 + (seconds & 0x0F);

            //Add padding 0 if seconds is 1 digit 

>>>>>>> 2aa73bb5cb76b39023610f7756880f6ae1d83313

            //Account for single digit minutes or seconds

            //If minutes and seconds are 1 digit
            if((formatedMinutes < 9)&&(formatedSeconds < 9)){
                printf("%d:0%d:0%d",formatedHours,formatedMinutes,formatedSeconds);
            }
            //if seconds are 1 digit
            else if(formatedSeconds < 9){

                printf("%d:%d:0%d",formatedHours,formatedMinutes,formatedSeconds);
            }
            //if minutes are 1 digit
            else if(formatedMinutes < 9){
                printf("%d:0%d:%d",formatedHours,formatedMinutes,formatedSeconds);
            }
            
            else{
                printf("%d:%d:%d",formatedHours,formatedMinutes,formatedSeconds);
            }

        }

<<<<<<< HEAD
        cli();
        
        outb(0x70, 0x04);
        outb(0x71, toBCD(hours));

        outb(0x70, 0x02);
        outb(0x71, toBCD(minutes));

        outb(0x70, 0x00);
        outb(0x71, toBCD(seconds));

        printf("\033[0;32mTime set to %s.\n\033[0;0m", buf);

        sti();
    }


}
=======
        //Get Date Command
        else if(strcmp(buf, "Get Date") == 0){
>>>>>>> 2aa73bb5cb76b39023610f7756880f6ae1d83313

            printf("\n");
            //printf("Entered getDate\n");

            //Get Year and Format
            outb(0x70,0x09);
            int year = inb(0x71);
<<<<<<< HEAD
            int formatedYear = fromBCD(year);

            
=======
            int formatedYear = ((year & 0x70) >> 4)*10 + (year & 0x0F);
>>>>>>> 2aa73bb5cb76b39023610f7756880f6ae1d83313
            
            //Get Day and Format
            outb(0x70,0x07);
            int day = inb(0x71);
            int formatedDay = fromBCD(day);
            
            //Get Month and format
            outb(0x70,0x08);
            int month = inb(0x71);
<<<<<<< HEAD
            int formatedMonth = fromBCD(month);
=======
            int formatedMonth = ((month & 0x70) >> 4)*10 + (month & 0x0F);
            
            //Print Date
            printf("%d/%d/%d",formatedMonth,formatedDay,formatedYear);
>>>>>>> 2aa73bb5cb76b39023610f7756880f6ae1d83313

            //printf("finished getDate");

        }

        //TODO: Set Time
        else if(strcmp(buf, "Set Time") == 0){

            //Ask for user input
            printf("Enter the time. (hh:mm:ss)\n");
            sys_req(READ, COM1, buf, sizeof(buf));

<<<<<<< HEAD
            if(isdigit(buf[0]) && isdigit(buf[1]) && isdigit(buf[3]) && isdigit(buf[4]) && isdigit(buf[6]) && isdigit(buf[7])){

                cli();
                int month = atoi(&buf[0]);
                int day = atoi(&buf[3]);
                int year = atoi(&buf[6]);

                printf("\033[0;31m");

                if (month < 1 || day < 1 || year < 1){
                    puts("Date cannot be inputted as 0 or lower\n");
                    printf("\033[0;0m");
                    return;
                }
                else if (month > 12){
                    puts("Month cannot be greater than 12\n");
                    printf("\033[0;0m");
                    return;
                }
                else if ((month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) && day > 31){
                    puts("The month you inputted cannot have over 31 days\n");
                    printf("\033[0;0m");
                    return;
                }
                else if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30){
                    puts("The month you inputted cannot have over 30 days\n");
                    printf("\033[0;0m");
                    return;
                }
                else if (month == 2 && day > 28){
                    if (year % 4 != 0){
                        puts("February cannot have over 28 days when it is not a leap year\n");
                        printf("\033[0;0m");
                        return;
                    }
                    else if (day > 29){
                        puts("February cannot have over 29 days on a leap year\n");
                        printf("\033[0;0m");
                        return;
                    }
                }

                outb(0x70, 0x08);
                outb(0x71, toBCD(month));
                
                outb(0x70, 0x07);
                outb(0x71, toBCD(day));
                
                outb(0x70, 0x09);
                outb(0x71, toBCD(year));
                
                printf("\033[0;32m");
                printf("Date set to %s\n", buf);
                printf("\033[0;0m");
                sti();
=======
            if (buf[2] != ':' || buf[5] != ':' || sizeof(buf)/sizeof(buf[0]) != 8) {
                puts("Invalid date format, try again");
                continue;
            }

            char* output = strtok(buf, ":");
            int hourResult = atoi(output[0]);
            int minuteResult = atoi(output[1]);
            int secondResult = atoi(output[2]);
>>>>>>> 2aa73bb5cb76b39023610f7756880f6ae1d83313

            if (hourResult < 0 || hourResult > 23 || minuteResult < 0 || minuteResult > 59 || secondResult < 0 || secondResult > 59) {
                puts("One or more values for hours, minutes, or seconds is invalid. Please try again.");
                continue;
            }
             
            if (isdigit(buf[0]) && isdigit(buf[1]) && isdigit(buf[3]) && isdigit(buf[4]) && isdigit(buf[6]) && isdigit(buf[7])) {
                
                //Set Hours
                str_copy(output, buf, 0, 2);
                outb(0x70, 0x04);
                outb(0x71, strtobcd(output));

                //Set Minutes
                str_copy(output, buf, 3, 2);
                outb(0x70, 0x02);
                outb(0x71, strtobcd(output));

                //Set Seconds
                str_copy(output, buf, 6, 2);
                outb(0x70, 0x00);
                outb(0x71, strtobcd(output));
            }
        }

        //TODO: Set Date

         //TODO: Help Command
        
        
    }
}

void printMenu(){
        printf("\n");
        printf("Enter one of the following:\n");
        printf("1. Version\n");
        printf("2. Get Time\n");
        printf("3. Set Time\n");
        printf("4. Get Date\n");
        printf("5. Set Date\n");
        printf("6. Help\n");
        printf("7. Shutdown\n");
        printf("\n");
        printf(">> ");

}

