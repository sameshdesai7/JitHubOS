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

    for (;;)
    {

        char buf[100] = {0};
        printMenu();
        sys_req(READ, COM1, buf, sizeof(buf));

        // Shutdown Command
        if ((strcmp_ic(buf, "shutdown") == 0) || (strcmp(buf, "7") == 0))
        {
            if (shutdown(buf))
                return;
        }

        // Version Command
        else if ((strcmp_ic(buf, "version") == 0) || (strcmp(buf, "1") == 0))
        {
            version();
        }

        // Get Time Command
        else if ((strcmp_ic(buf, "Get Time") == 0) || (strcmp(buf, "2") == 0))
        {
            getTime();
        }

        // Get Date Command
        else if ((strcmp_ic(buf, "Get Date") == 0) || (strcmp(buf, "4") == 0))
        {
            getDate();
        }

        // TODO: Set Time
        else if ((strcmp_ic(buf, "Set Time") == 0) || (strcmp(buf, "3") == 0))
        {
            setTime();
        }

        // TODO: Set Date
        else if ((strcmp_ic(buf, "Set Date") == 0) || (strcmp(buf, "5") == 0))
        {
            setDate();
        }

        else if ((strcmp_ic(buf, "help") == 0) || strcmp(buf, "6") == 0)
        {

            help();
        }

        else if (strcmp_ic(buf, "help version") == 0 || strcmp_ic(buf, "6 version") == 0)
            puts("Type \"version\" to retrieve the current version of the operating system\n");
        else if (strcmp_ic(buf, "help help") == 0 || strcmp_ic(buf, "6 help") == 0)
            puts("Type \"help\" to see a list of commands you can run\n");
        else if (strcmp_ic(buf, "help shutdown") == 0 || strcmp_ic(buf, "6 shutdown") == 0)
            puts("Type \"shutdown\" to exit the operating system\n");
        else if (strcmp_ic(buf, "help get time") == 0 || strcmp_ic(buf, "6 get time") == 0)
            puts("Type \"get time\" to retrieve the current system time\n");
        else if (strcmp_ic(buf, "help set time") == 0 || strcmp_ic(buf, "6 set time") == 0)
            puts("Type \"set time\" to set the system time\n");
        else if (strcmp_ic(buf, "help get date") == 0 || strcmp_ic(buf, "6 get date") == 0)
            puts("Type \"get date\" to retrieve the current system date\n");
        else if (strcmp_ic(buf, "help set date") == 0 || strcmp_ic(buf, "6 set date") == 0)
            puts("Type \"set date\" to set the system date\n");
        else
        {
            printf("\033[0;31m");
            puts("Invalid query\n");
            printf("\033[0;0m");
        }
    }
}

void printMenu()
{
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

int shutdown()
{
    char buf[100] = {0};
    printf("\033[0;36m");
    printf("Confirm Shutdown? Y/N\n");
    printf("\033[0;0m");
    // if shutdown is confirmed
    sys_req(READ, COM1, buf, sizeof(buf));
    if (strcmp_ic(buf, "Y") == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void version(void)
{
    printf("\033[0;36m");
    printf("Release Number: %d\n", VERSION);
    printf("Date of most recent compile: %s\n", COMPILE_DATE);
    printf("Time of most recent compile: %s\n", COMPILE_TIME);
    printf("\033[0;0m");
}

void getTime(void)
{
    printf("\033[0;36m");
    printf("Time is: ");

    outb(0x70, 0x04);
    int hours = inb(0x71);
    int formatedHours = fromBCD(hours);

    outb(0x70, 0x02);
    int minutes = inb(0x71);
    int formatedMinutes = fromBCD(minutes);

    outb(0x70, 0x00);
    int seconds = inb(0x71);
    int formatedSeconds = fromBCD(seconds);

    // Add padding 0 if seconds is 1 digit
    // Account for single digit minutes or seconds
    // If minutes and seconds are 1 digit
    if ((formatedMinutes <= 9) && (formatedSeconds <= 9))
    {
        printf("%d:0%d:0%d", formatedHours, formatedMinutes, formatedSeconds);
    }
    // if seconds are 1 digit
    else if (formatedSeconds <= 9)
    {

        printf("%d:%d:0%d", formatedHours, formatedMinutes, formatedSeconds);
    }
    // if minutes are 1 digit
    else if (formatedMinutes <= 9)
    {
        printf("%d:0%d:%d", formatedHours, formatedMinutes, formatedSeconds);
    }

    else
    {
        printf("%d:%d:%d", formatedHours, formatedMinutes, formatedSeconds);
    }
    printf("\033[0;0m\n");
}

void setTime(void)
{

    // Ask for user input

    printf("\033[0;36m");
    printf("Enter the time. (hh:mm:ss)\n");
    printf("\033[0;0m");
    printf(">> ");
    char buf[100] = {0};
    sys_req(READ, COM1, buf, sizeof(buf));

    if (buf[1] == ':')
    {
        buf[1] = buf[0];
        buf[0] = '0';
        for (int i = 7; i > 2; i--)
        {
            buf[i] = buf[i - 1];
        }
        buf[2] = ':';
    }

    if (isdigit(buf[0]) && isdigit(buf[1]) && isdigit(buf[3]) && isdigit(buf[4]) && isdigit(buf[6]) && isdigit(buf[7]))
    {

        // Set Hours
        int hours = atoi(&buf[0]);
        int minutes = atoi(&buf[3]);
        int seconds = atoi(&buf[6]);
        int isInvalid = 0;

        if (hours > 23 || minutes > 59 || seconds > 59)
        {
            printf("\033[0;31m");
            if (hours > 23)
            {
                printf("Invalid time format. Hours must be 1-23.\n");
            }

            if (minutes > 59)
            {
                printf("Invalid time format. Minutes must be 1-59.\n");
            }

            if (seconds > 59)
            {
                printf("Invalid time format. Seconds must be 1-59.\n");
            }
            printf("\033[0;0m");
            isInvalid = 1;
        }

        if (isInvalid)
        {
            return;
        }

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

void getDate(void)
{

    printf("\033[0;36m");
    printf("Date: ");

    // Get Year and Format
    outb(0x70, 0x09);
    int year = inb(0x71);
    int formatedYear = fromBCD(year);

    // Get Day and Format
    outb(0x70, 0x07);
    int day = inb(0x71);
    int formatedDay = fromBCD(day);

    // Get Month and format
    outb(0x70, 0x08);
    int month = inb(0x71);
    int formatedMonth = fromBCD(month);

    // If minutes and seconds are 1 digit
    if ((formatedDay <= 9) && (formatedYear <= 9))
    {
        printf("%d/0%d/0%d", formatedMonth, formatedDay, formatedYear);
    }
    // if seconds are 1 digit
    else if (formatedDay <= 9)
    {

        printf("%d/0%d/%d", formatedMonth, formatedDay, formatedYear);
    }
    // if minutes are 1 digit
    else if (formatedYear <= 9)
    {
        printf("%d/%d/0%d", formatedMonth, formatedDay, formatedYear);
    }

    else
    {
        printf("%d/%d/%d", formatedMonth, formatedDay, formatedYear);
    }
    printf("\033[0;0m\n");
}

void setDate(void)
{
    // TODO: Set Date
    char buf[100] = {0};

    // Ask for user input

    printf("\033[0;36m");
    printf("Enter the date. (mm/dd/yy)\n");
    printf("\033[0;0m");
    printf(">> ");
    sys_req(READ, COM1, buf, sizeof(buf));
    
    //changes the month to 2 digits if they are not inputted as 2
    if (buf[1] == '/')
    {
        buf[1] = buf[0];
        buf[0] = '0';
        for (int i = 7; i > 2; i--)
        {
            buf[i] = buf[i - 1];
        }
        buf[2] = '/';
    }
     //changes the day to 2 digits if they are not inputted as 2
    if (buf[4] == '/')
    {
        buf[4] = buf[3];
        buf[3] = '0';
        for (int i = 7; i > 5; i--)
        {
            buf[i] = buf[i - 1];
        }
        buf[5] = '/';
    }

    if (isdigit(buf[0]) && isdigit(buf[1]) && isdigit(buf[3]) && isdigit(buf[4]) && isdigit(buf[6]) && isdigit(buf[7]))
    {

        cli();
        int month = atoi(&buf[0]);
        int day = atoi(&buf[3]);
        int year = atoi(&buf[6]);

        printf("\033[0;31m");

        if (month < 1 || day < 1 || year < 1)
        {
            puts("Date cannot be inputted as 0 or lower\n");
            printf("\033[0;0m");
            return;
        }
        else if (month > 12)
        {
            puts("Month cannot be greater than 12\n");
            printf("\033[0;0m");
            return;
        }
        else if ((month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) && day > 31)
        {
            puts("The month you inputted cannot have over 31 days\n");
            printf("\033[0;0m");
            return;
        }
        else if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30)
        {
            puts("The month you inputted cannot have over 30 days\n");
            printf("\033[0;0m");
            return;
        }
        else if (month == 2 && day > 28)
        {
            if (year % 4 != 0)
            {
                puts("February cannot have over 28 days when it is not a leap year\n");
                printf("\033[0;0m");
                return;
            }
            else if (day > 29)
            {
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
    }
    else
    { // if the overall format does not match a proper format
        printf("\033[0;31m");
        puts("Invalid date format");
        printf("\033[0;0m");
    }
}

void help(void)
{
    // If "help" was the only word, print a list of all the commands and what they do
    printf("\033[0;36m");
    puts("Type \"version\" or type ‘1’to retrieve the current version of the operating system\n");
    puts("Type \"get time\" or type ‘2’ to retrieve the current system time\n");
    puts("Type \"set time\" or type ‘3’ to set the system time\n");
    puts("Type \"get date\" or type ‘4’ to retrieve the current system date\n");
    puts("Type \"set date\" or type ‘5’ to set the system date\n");
    puts("Type \"help\" or type ‘6’ to see a list of commands you can run\n");
    puts("Type \"shutdown\" or type ‘7’ to exit the operating system\n");
    printf("\033[0;0m");
}
