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

void comhand()
{

    for (;;){

        printMenu();
        char buf[100] = {0};
        sys_req(READ, COM1, buf, sizeof(buf));

        //Shutdown Command
        if((strcmp_ic(buf, "shutdown") == 0)|| (strcmp(buf, "7") == 0)){
            if(shutdown(buf))
                return;
        }

        //Version Command
        else if((strcmp_ic(buf, "version") == 0) || (strcmp(buf, "1") == 0)){
            version();
        }

        //Get Time Command
        else if((strcmp_ic(buf, "Get Time") == 0) || (strcmp(buf, "2") == 0)){
            getTime(); 
        }

        //Get Date Command
        else if((strcmp_ic(buf, "Get Date") == 0)|| (strcmp(buf, "4") == 0)){
            getDate();
        }

        //TODO: Set Time
        else if((strcmp_ic(buf, "Set Time") == 0)|| (strcmp(buf, "3") == 0)){
            setTime();
        }

        //TODO: Set Date
        else if((strcmp_ic(buf, "Set Date") == 0)|| (strcmp(buf, "5") == 0)){
            setDate();
        }

        else if ((strcmp_ic(buf, "help") == 0) || strcmp(buf, "6") == 0) {
        
            help();
        }

        else if (strcmp_ic(buf, "help version") == 0 || strcmp_ic(buf, "6 version") == 0) puts("Type \"version\" to retrieve the current version of the operating system\n");
        else if (strcmp_ic(buf, "help help") == 0 || strcmp_ic(buf, "6 help") == 0) puts("Type \"help\" to see a list of commands you can run\n");
        else if (strcmp_ic(buf, "help shutdown") == 0 || strcmp_ic(buf, "6 shutdown") == 0) puts("Type \"shutdown\" to exit the operating system\n");
        else if (strcmp_ic(buf, "help get time") == 0 || strcmp_ic(buf, "6 get time") == 0) puts("Type \"get time\" to retrieve the current system time\n");
        else if (strcmp_ic(buf, "help set time") == 0 || strcmp_ic(buf, "6 set time") == 0) puts("Type \"set time\" to set the system time\n");
        else if (strcmp_ic(buf, "help get date") == 0 || strcmp_ic(buf, "6 get date") == 0) puts("Type \"get date\" to retrieve the current system date\n");
        else if (strcmp_ic(buf, "help set date") == 0 || strcmp_ic(buf, "6 set date") == 0) puts("Type \"set date\" to set the system date\n");
        
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

int shutdown(char* buf){
    printf("Confirm Shutdown? Y/N\n");
            //if shutdown is confirmed
            sys_req(READ, COM1, buf, sizeof(buf));
            if(strcmp_ic(buf, "Y") == 0){
                return 1;
            }
            else{
                return 0;
            }
}

void version(void){
    printf("Release Number: %d\n",VERSION);
}

void getTime(void){
    printf("Time is: ");

    outb(0x70,0x04);
    int hours = inb(0x71);
    int formatedHours = ((hours & 0x30) >> 4) * 10 + (hours & 0x0F);

    //printf("%d",hours);

    outb(0x70,0x02);
    int minutes = inb(0x71);
    int formatedMinutes = ((minutes & 0x70) >> 4)*10 + (minutes & 0x0F);

    outb(0x70,0x00);
    int seconds = inb(0x71);
    int formatedSeconds = ((seconds & 0x70) >> 4)*10 + (seconds & 0x0F);

    //Add padding 0 if seconds is 1 digit 


    //Account for single digit minutes or seconds

    //If minutes and seconds are 1 digit
    if((formatedMinutes <=9)&&(formatedSeconds <=9)){
        printf("%d:0%d:0%d",formatedHours,formatedMinutes,formatedSeconds);
    }
    //if seconds are 1 digit
    else if(formatedSeconds <=9){

        printf("%d:%d:0%d",formatedHours,formatedMinutes,formatedSeconds);
    }
    //if minutes are 1 digit
    else if(formatedMinutes <=9){
        printf("%d:0%d:%d",formatedHours,formatedMinutes,formatedSeconds);
    }
    
    else{
        printf("%d:%d:%d",formatedHours,formatedMinutes,formatedSeconds);
    }

    printf("\n"); 
}

void setTime(void){

    //Ask for user input
    printf("Enter the time. (hh:mm:ss)\n");
    char buf[100] = {0};
    sys_req(READ, COM1, buf, sizeof(buf));

    if(buf[1] == ':'){
        buf[1] = buf[0];
        buf[0] = '0';
        for(int i = 7; i > 2; i--){
            buf[i] = buf[i - 1];
        }
        buf[2] = ':';
    }

    if(isdigit(buf[0]) && isdigit(buf[1]) && isdigit(buf[3]) && isdigit(buf[4]) && isdigit(buf[6]) && isdigit(buf[7])){
        
        //Set Hours
        int hours = atoi(&buf[0]);
        int minutes = atoi(&buf[3]);
        int seconds = atoi(&buf[6]);
        int isInvalid = 0;

        if(hours > 23 || minutes > 59 || seconds > 59){
            printf("\033[0;31m");
            if(hours > 23){
                printf("Invalid time format. Hours must be 1-23.\n");
            }
            
            if(minutes > 59){
                printf("Invalid time format. Minutes must be 1-59.\n");
            }

            if(seconds > 59){
                printf("Invalid time format. Seconds must be 1-59.\n");
            }
            printf("\033[0;0m");
            isInvalid = 1;

            
        }

        if(isInvalid){
            return;
        }

        cli();
        int convertedHours = ((hours/10) << 4 ) | (hours %10);
        
        outb(0x70, 0x04);
        outb(0x71, convertedHours);

        //Conversion needed due to BCD (Binary Coded Decimal)
        int convertedMinutes = ((minutes/10) << 4 ) | (minutes %10);

        outb(0x70, 0x02);
        outb(0x71, convertedMinutes);

        int convertedSeconds = ((seconds/10) << 4 ) | (seconds %10);

        outb(0x70, 0x00);
        outb(0x71, convertedSeconds);
        
        // printf("\033[0;32m");
        printf("\033[0;32mTime set to %s.\n\033[0;0m", buf);
        // printf("\033[0;0m");

        sti();
    }


}

void getDate(void){
    printf("Date: ");

            //Get Year and Format
            outb(0x70,0x09);
            int year = inb(0x71);
            int formatedYear = ((year & 0x70) >> 4)*10 + (year & 0x0F);

            
            
            //Get Day and Format
            outb(0x70,0x07);
            int day = inb(0x71);
            int formatedDay = ((day & 0x70) >> 4)*10 + (day & 0x0F);
            
            //Get Month and format
            outb(0x70,0x08);
            int month = inb(0x71);
            int formatedMonth = ((month & 0x70) >> 4)*10 + (month & 0x0F);
            
            //Print Date
            //printf("%d/%d/%d",formatedMonth,formatedDay,formatedYear);

            //If minutes and seconds are 1 digit
            if((formatedDay <=9)&&(formatedYear <=9)){
                printf("%d/0%d/0%d",formatedMonth,formatedDay,formatedYear);
            }
            //if seconds are 1 digit
            else if(formatedDay <=9){

                printf("%d/0%d/%d",formatedMonth,formatedDay,formatedYear);
            }
            //if minutes are 1 digit
            else if(formatedYear <=9){
                printf("%d/%d/0%d",formatedMonth,formatedDay,formatedYear);
            }
            
            else{
                printf("%d/%d/%d",formatedMonth,formatedDay,formatedYear);
            }

            printf("\n");
}

void setDate(void){
    //Ask for user input
    printf("Enter the date. (mm/dd/yyyy)\n");
    char buf[100] = {0};
    sys_req(READ, COM1, buf, sizeof(buf));

    if(isdigit(buf[0]) && isdigit(buf[1]) && isdigit(buf[3]) && isdigit(buf[4]) && isdigit(buf[6]) && isdigit(buf[7])){

        cli();
        //Month
        int month = atoi(&buf[0]);

        int convertedMonth = ((month/10) << 4 ) | (month %10);
        outb(0x70, 0x08);
        outb(0x71, convertedMonth);

        //Day
        int day = atoi(&buf[3]);

        int convertedDay = ((day/10) << 4 ) | (day %10);
        
        outb(0x70, 0x07);
        outb(0x71, convertedDay);

        int year = atoi(&buf[6]);
        printf("%d\n",year);

        int convertedYear = ((year/10) << 4 ) | (year %10);
        
        outb(0x70, 0x09);
        outb(0x71, convertedYear);

        sti();

    }


}

void help(void){
    //If "help" was the only word, print a list of all the commands and what they do
    puts("Type \"version\" to retrieve the current version of the operating system\n");
    puts("Type \"get time\" to retrieve the current system time\n");
    puts("Type \"set time\" to set the system time\n");
    puts("Type \"get date\" to retrieve the current system date\n");
    puts("Type \"set date\" to set the system date\n");
    puts("Type \"help\" to see a list of commands you can run\n");
    puts("Type \"shutdown\" to exit the operating system\n");
}
