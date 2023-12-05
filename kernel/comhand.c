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
#include <dataStructs.h>
#include <processes.h>
#include <alarm.h>

// compile constants to be used in version to show when the most recent compilation was
#define COMPILE_DATE __DATE__
#define COMPILE_TIME __TIME__

extern queue *ready;
extern queue *blocked;
extern queue *susReady;
extern queue *susBlocked;
int logoFlag = 0;

void comhand()
{
    
    if(logoFlag == 0){
        logoFlag= 1;
        printf("\033[94;1;6m                                                                                             \n");
        printf("                                                                                             \n");
        printf("   `7MMF'`7MMF'MMP\"\"MM\"\"YMM `7MMF'  `7MMF'`7MMF'   `7MF'`7MM\"\"\"Yp,           .g8\"\"8q.    .M\"\"\"bgd \n");
        printf("     MM    MM  P'   MM   `7   MM      MM    MM       M    MM    Yb         .dP'    `YM. ,MI    \"Y \n");
        printf("     MM    MM       MM        MM      MM    MM       M    MM    dP         dM'      `MM `MMb.     \n");
        printf("     MM    MM       MM        MMmmmmmmMM    MM       M    MM\"\"\"bg.         MM        MM   `YMMNq. \n");
        printf("     MM    MM       MM        MM      MM    MM       M    MM    `Y         MM.      ,MP .     `MM \n");
        printf(" 1   MM    MM       MM        MM      MM    YM.     ,M    MM    ,9         `Mb.    ,dP' Mb     dM \n");
        printf(" `6mm9'  .JMML.   .JMML.    .JMML.  .JMML.   `bmmmmd\"'  .JMMmmmd9            `\"bmmd\"'   \"Ybmmd\"  \n");
        printf("\033[0m                                                                                            \n");
    } 
    // infinite loop
    for (;;)
    {
        // buffer for first input read in
        char buf[100] = {0};
        // prints the menu that shows all of the choices
        printMenu();
        // reads in the first input
        sys_req(READ, COM1, buf, sizeof(buf));

        // Version Command
        if ((strcmp_ic(buf, "version") == 0) || (strcmp(buf, "1") == 0))
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

        // Set Time Command
        else if ((strcmp_ic(buf, "Set Time") == 0) || (strcmp(buf, "3") == 0))
        {
            setTime();
        }

        // Set Date Command
        else if ((strcmp_ic(buf, "Set Date") == 0) || (strcmp(buf, "5") == 0))
        {
            setDate();
        }

        // Delete PCB Command
        else if ((strcmp_ic(buf, "Delete PCB") == 0) || (strcmp(buf, "6") == 0))
        {
            deletePCB(ready, blocked, susReady, susBlocked);
        }

        // Block PCB Command
        else if ((strcmp_ic(buf, "Block PCB") == 0) || (strcmp(buf, "7") == 0))
        {
            blockPCB(ready, blocked, susReady, susBlocked);
        }

        // Unblock PCB Command
        else if ((strcmp_ic(buf, "Unblock PCB") == 0) || (strcmp(buf, "8") == 0))
        {
            unblockPCB(ready, blocked, susReady, susBlocked);
        }

        // Suspend PCB Command
        else if ((strcmp_ic(buf, "Suspend PCB") == 0) || (strcmp(buf, "9") == 0))
        {
            suspendPCB(ready, blocked, susReady, susBlocked);
        }

        // Resume PCB Command
        else if ((strcmp_ic(buf, "Resume PCB") == 0) || (strcmp(buf, "10") == 0))
        {
            resumePCB(ready, blocked, susReady, susBlocked);
        }

        // Set PCB Priority Command
        else if ((strcmp_ic(buf, "Set PCB Priority") == 0) || (strcmp(buf, "11") == 0))
        {
            setPCBPriority(ready, blocked, susReady, susBlocked);
        }

        // Show PCB Command
        else if ((strcmp_ic(buf, "Show PCB") == 0) || (strcmp(buf, "12") == 0))
        {
            showPCB(ready, blocked, susReady, susBlocked);
        }

        // //Show Ready Command
        else if ((strcmp_ic(buf, "Show Ready") == 0) || (strcmp(buf, "13") == 0))
        {
            showReady(ready);
        }

        // //Show Blocked Command
        else if ((strcmp_ic(buf, "Show Blocked") == 0) || (strcmp(buf, "14") == 0))
        {
            showBlocked(blocked);
        }
        // //Show All command
        else if ((strcmp_ic(buf, "Show All") == 0) || (strcmp(buf, "15") == 0))
        {
            showAll(ready, blocked, susReady, susBlocked);
        }

        else if ((strcmp_ic(buf, "Load R3") == 0) || strcmp(buf, "16") == 0)
        {
            loadR3();
        }
        // help command
        else if ((strcmp_ic(buf, "help") == 0) || strcmp(buf, "18") == 0)
        {
            help();
        }

        // Shutdown Command
        else if ((strcmp_ic(buf, "shutdown") == 0) || (strcmp(buf, "19") == 0))
        {
            // if shutdown is confirmed, empty the ready queue and exit comhand
            if (shutdown(buf))
            {
                while (ready->head != NULL)
                {
                    dequeue(ready);
                }
                sys_req(EXIT);
            }
        }

        else if ((strcmp_ic(buf, "Alarm") == 0) || strcmp(buf, "17") == 0)
        {
            alarm();
        }
        // individual help commands
        else if (strcmp_ic(buf, "help version") == 0 || strcmp_ic(buf, "17 version") == 0)
        {
            printf("\033[0;36m");
            puts("Type \"version\" to retrieve the current version of the operating system\n");
            printf("\033[0;0m");
        }
        else if (strcmp_ic(buf, "help help") == 0 || strcmp_ic(buf, "17 help") == 0)
        {
            printf("\033[0;36m");
            puts("Type \"help\" to see a list of commands you can run\n");
            printf("\033[0;0m");
        }
        else if (strcmp_ic(buf, "help shutdown") == 0 || strcmp_ic(buf, "17 shutdown") == 0)
        {
            printf("\033[0;36m");
            puts("Type \"shutdown\" to exit the operating system\n");
            printf("\033[0;0m");
        }
        else if (strcmp_ic(buf, "help get time") == 0 || strcmp_ic(buf, "17 get time") == 0)
        {
            printf("\033[0;36m");
            puts("Type \"get time\" to retrieve the current system time\n");
            printf("\033[0;0m");
        }
        else if (strcmp_ic(buf, "help set time") == 0 || strcmp_ic(buf, "17 set time") == 0)
        {
            printf("\033[0;36m");
            puts("Type \"set time\" to set the system time\n");
            printf("\033[0;0m");
        }
        else if (strcmp_ic(buf, "help get date") == 0 || strcmp_ic(buf, "17 get date") == 0)
        {
            printf("\033[0;36m");
            puts("Type \"get date\" to retrieve the current system date\n");
            printf("\033[0;0m");
        }
        else if (strcmp_ic(buf, "help set date") == 0 || strcmp_ic(buf, "17 set date") == 0)
        {
            printf("\033[0;36m");
            puts("Type \"set date\" to set the system date\n");
            printf("\033[0;0m");
        }
        else if (strcmp_ic(buf, "help delete pcb") == 0 || strcmp_ic(buf, "17 delete pcb") == 0)
        {
            printf("\033[0;36m");
            puts("Type \"delete pcb\" to delete a PCB\n");
            puts("The name provided must be a PCB that exists\n");
            printf("\033[0;0m");
        }
        else if (strcmp_ic(buf, "help block pcb") == 0 || strcmp_ic(buf, "17 block pcb") == 0)
        {
            printf("\033[0;36m");
            puts("Type \"block pcb\" to block a PCB\n");
            puts("Only user processes can be blocked.\n");
            printf("\033[0;0m");
        }
        else if (strcmp_ic(buf, "help unblock pcb") == 0 || strcmp_ic(buf, "17 unblock pcb") == 0)
        {
            printf("\033[0;36m");
            puts("Type \"block pcb\" to unblock a PCB\n");
            printf("\033[0;0m");
        }
        else if (strcmp_ic(buf, "help suspend pcb") == 0 || strcmp_ic(buf, "17 suspend pcb") == 0)
        {
            printf("\033[0;36m");
            puts("Type \"block pcb\" to suspend a PCB\n");
            puts("Only user processes can be suspended.\n");
            printf("\033[0;0m");
        }
        else if (strcmp_ic(buf, "help resume pcb") == 0 || strcmp_ic(buf, "17 resume pcb") == 0)
        {
            printf("\033[0;36m");
            puts("Type \"retume pcb\" to resume a PCB\n");
            printf("\033[0;0m");
        }
        else if (strcmp_ic(buf, "help set pcb priority") == 0 || strcmp_ic(buf, "17 set pcb priority") == 0)
        {
            printf("\033[0;36m");
            puts("Type \"set pcb priority \" to set a pcb priority\n");
            puts("A priotity must be between 0 and 9 with 0 being the highest priority.");
            printf("\033[0;0m");
        }
        else if (strcmp_ic(buf, "help show pcb") == 0 || strcmp_ic(buf, "17 show pcb") == 0)
        {
            printf("\033[0;36m");
            puts("Type \"show pcb\" to show all the information associated with a PCB\n");
            printf("\033[0;0m");
        }
        else if (strcmp_ic(buf, "help show ready") == 0 || strcmp_ic(buf, "17 show ready") == 0)
        {
            printf("\033[0;36m");
            puts("Type \"show ready\" to show all the PCB's in the ready queue. \n");
            printf("\033[0;0m");
        }
        else if (strcmp_ic(buf, "help show blocked ") == 0 || strcmp_ic(buf, "17 show blocked") == 0)
        {
            printf("\033[0;36m");
            puts("Type \"show blocked\" to show all the PCB's in the blocked queue. \n");
            printf("\033[0;0m");
        }
        else if (strcmp_ic(buf, "help show all") == 0 || strcmp_ic(buf, "17 show all") == 0)
        {
            printf("\033[0;36m");
            puts("Type \"show all\" to show all the PCB's and all the queues (Ready, Blocked, SusReady, SusBlocked). \n");
            printf("\033[0;0m");
        }

        else if (strcmp_ic(buf, "help load r3") == 0 || strcmp_ic(buf, "17 load r3") == 0)
        {
            printf("\033[0;36m");
            puts("Type \"load r3\" to load the test processes into the ready queue. \n");
            printf("\033[0;0m");
        }

        else if (strcmp_ic(buf, "help alarm") == 0 || strcmp_ic(buf, "17 alarm") == 0)
        {
            printf("\033[0;36m");
            puts("Type \"alarm\" to create a new alarm process with a set time and message. \n");
            printf("\033[0;0m");
        }
        else if (strcmp_ic(buf, "clear") == 0){
            const char* clearScreen = "\033[H\n\033[J";
            sys_req(WRITE, COM1, clearScreen, 14);
            printf("\033[0;0m");
        }

        else
        {
            printf("\033[0;31m");
            printf("Invalid query \'%s\'\n  ", buf);
            printf("\033[0;0m");
        }

        sys_req(IDLE);
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
    printf("6. Delete PCB\n");
    printf("7. Block PCB\n");
    printf("8. Unblock PCB\n");
    printf("9. Suspend PCB\n");
    printf("10. Resume PCB\n");
    printf("11. Set PCB Priority\n");
    printf("12. Show PCB\n");
    printf("13. Show Ready\n");
    printf("14. Show Blocked\n");
    printf("15. Show All\n");
    printf("16. Load R3\n");
    printf("17. Alarm\n");
    printf("18. Help\n");
    printf("19. Shutdown\n");
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
        // exit comhand
        return 1;
    }
    else
    {
        // continue loop
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

    // Attaching the hours register to port 70
    outb(0x70, 0x04);
    // Reading the data from the hours register through port 71
    int hours = inb(0x71);
    int formatedHours = fromBCD(hours);

    // Attaching the minutes register to port 70
    outb(0x70, 0x02);
    // Reading the data from the minutes register through port 71
    int minutes = inb(0x71);
    int formatedMinutes = fromBCD(minutes);

    // Attaching the seconds register to port 70
    outb(0x70, 0x00);
    // Reading the data from the seconds register through port 71
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

    // checks to see hours are only 1 digit and will replace with the equivalent 2 digits
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

    if (!(isdigit(buf[6]) && isdigit(buf[7])))
    {
        printf("\033[0;31m");
        printf("Error. Did not enter seconds. Please use the hh:mm:ss format.");
        printf("\033[0;0m");
    }

    if (isdigit(buf[0]) && isdigit(buf[1]) && isdigit(buf[3]) && isdigit(buf[4]) && isdigit(buf[6]) && isdigit(buf[7]))
    {

        // Set Hours
        int hours = atoi(&buf[0]);
        int minutes = atoi(&buf[3]);
        int seconds = atoi(&buf[6]);
        int isInvalid = 0;

        // verifies input
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
        // disables interrupts and sets the time to each register
        cli();

        outb(0x70, 0x04);
        outb(0x71, toBCD(hours));

        outb(0x70, 0x02);
        outb(0x71, toBCD(minutes));

        outb(0x70, 0x00);
        outb(0x71, toBCD(seconds));

        printf("\033[0;32mTime set to %s.\n\033[0;0m", buf);
        // enables interrupts
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

    if (strlen(buf) > 8)
    {
        printf("\033[0;31m");
        printf("Invalid Date Format. Please use the mm/dd/yy format.");
        printf("\033[0;0m");
        return;
    }

    // changes the month to 2 digits if they are not inputted as 2
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
    // changes the day to 2 digits if they are not inputted as 2
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
        // disables interrupts
        cli();
        int year = atoi(&buf[6]);
        int month = atoi(&buf[0]);
        int day = atoi(&buf[3]);

        // int days[]= {31, 28, }
        printf("\033[0;31m");
        // verifies input
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

        // sets the date to each of the registers
        if (month == 2 | month == 4 || month == 6 || month == 9 || month == 11)
        {
            outb(0x70, 0x09);
            outb(0x71, toBCD(year));

            outb(0x70, 0x07);
            outb(0x71, toBCD(day));

            outb(0x70, 0x08);
            outb(0x71, toBCD(month));
        }
        else
        {
            outb(0x70, 0x09);
            outb(0x71, toBCD(year));

            outb(0x70, 0x08);
            outb(0x71, toBCD(month));

            outb(0x70, 0x07);
            outb(0x71, toBCD(day));
        }

        printf("\033[0;32m");
        printf("Date set to %s\n", buf);
        printf("\033[0;0m");

        // enables interrupts
        sti();
    }
    else
    { // if the overall format does not match a proper format
        printf("\033[0;31m");
        puts("Invalid date format");
        printf("\033[0;0m");
    }
}

void deletePCB(queue *ready, queue *blocked, queue *susReady, queue *susBlocked)
{
    char buf[100] = {0};

getName:
    printf("\033[0;36mEnter PCB name:\033[0;0m\n>> ");
    // Read in name the user entered
    sys_req(READ, COM1, buf, sizeof(buf));
    if (strlen(buf) < 1 || strlen(buf) > 12)
    {
        printf("\033[0;31mInvalid name for process. Name must be 1 - 12 characters long. \nEnter \"EXIT\" to exit to main menu.\033[0;0m\n");
        goto getName;
    }
    char *name = (char *)sys_alloc_mem(strlen(buf) + 1);
    // Check to see if the memory allocation was successful
    if (name == NULL)
    {
        printf("Memory allocation failed.");
        return;
    }
    str_copy(name, buf, 0, strlen(buf));

    if (strcmp_ic(name, "exit") == 0)
    {
        return;
    }

    pcb *toRemove = pcb_find(ready, blocked, susReady, susBlocked, name);
    if (toRemove == NULL)
    {
        printf("\033[0;31mCould not find process \"%s\" to delete.\033[0;0m\n", name);
        return;
    }
    if (toRemove->clas == 0)
    {
        printf("\033[0;31mError: \"%s\" is a system process. Cannot request to delete a system process.\033[0;0m\n", name);
        return;
    }
    // Remove the PCB and free its memory
    pcb_remove(ready, blocked, susReady, susBlocked, toRemove);
    // pcb_free(toRemove);
    printf("\033[0;32mSuccessfully deleted process \"%s\".\033[0;0m\n", toRemove->name_ptr);
}

void blockPCB(queue *ready, queue *blocked, queue *susReady, queue *susBlocked)
{
    char buf[100] = {0};

getName:
    printf("\033[0;36mEnter PCB name:\033[0;0m\n>> ");
    // Read in name the user entered
    sys_req(READ, COM1, buf, sizeof(buf));
    if (strlen(buf) < 1 || strlen(buf) > 12)
    {
        printf("\033[0;31mInvalid name for process. Name must be 1 - 12 characters long. \nEnter \"EXIT\" to exit to main menu.\033[0;0m\n");
        goto getName;
    }
    char *name = (char *)sys_alloc_mem(strlen(buf) + 1);
    // Check to see if the memory allocation was successful
    if (name == NULL)
    {
        printf("Memory allocation failed.");
        return;
    }
    str_copy(name, buf, 0, strlen(buf));

    if (strcmp_ic(name, "exit") == 0)
    {
        return;
    }

    // Check to see if the process is inside of a queue already
    pcb *toBlock = pcb_find(ready, blocked, susReady, susBlocked, name);
    if (toBlock == NULL)
    {
        printf("\033[0;31mCould not find process \"%s\" to block.\033[0;0m\n", name);
        return;
    }

    if (toBlock->clas == 0)
    {
        printf("\033[0;31mCannot block system process \"%s\".\033[0;0m\n", name);
        return;
    }

    // Check to see if the process is already blocked
    if (strcmp_ic(toBlock->state, "blocked") == 0 || strcmp_ic(toBlock->state, "susBlocked") == 0)
    {
        printf("\033[0;33mThe specified PCB \"%s\"is already in the blocked state. Nothing will be done.\n\033[0;0m", name);
        return;
    }
    // Check to see if the process is suspended, but not blocked
    pcb_remove(ready, blocked, susReady, susBlocked, toBlock);

    if (strcmp_ic(toBlock->state, "susReady") == 0)
    {
        toBlock->state = "susBlocked";
    }
    // remove the pcb from a queue, change the state to blocked and insert it into the blocked queue.
    else if (strcmp_ic(toBlock->state, "ready") == 0)
    {
        toBlock->state = "blocked";
    }

    pcb_insert(ready, blocked, susReady, susBlocked, toBlock);
    printf("\033[0;32mSuccessfully blocked process \"%s\".\033[0;0m\n", toBlock->name_ptr);
}

void unblockPCB(queue *ready, queue *blocked, queue *susReady, queue *susBlocked)
{
    char buf[100] = {0};

getName:
    printf("\033[0;36mEnter PCB name:\033[0;0m\n>> ");
    // Read in name the user entered
    sys_req(READ, COM1, buf, sizeof(buf));
    if (strlen(buf) < 1 || strlen(buf) > 12)
    {
        printf("\033[0;31mInvalid name for process. Name must be 1 - 12 characters long. \nEnter \"EXIT\" to exit to main menu.\033[0;0m\n");
        goto getName;
    }
    char *name = (char *)sys_alloc_mem(strlen(buf) + 1);
    // Check to see if the memory allocation was successful
    if (name == NULL)
    {
        printf("Memory allocation failed.");
        return;
    }
    str_copy(name, buf, 0, strlen(buf));

    if (strcmp_ic(name, "exit") == 0)
    {
        return;
    }

    // Check to see if the PCB exists in a queue.
    pcb *toReady = pcb_find(ready, blocked, susReady, susBlocked, name);
    if (toReady == NULL)
    {
        printf("\033[0;31mCould not find process \"%s\" to unblock.\033[0;0m\n", name);
        return;
    }
    // Check to see if the process is already unblocked
    if (strcmp_ic(toReady->state, "ready") == 0 || strcmp_ic(toReady->state, "susReady") == 0)
    {
        printf("\033[0;33mThe specified PCB \"%s\" is already in the ready state. Nothing will be done.\n\033[0;0m", name);
        return;
    }
    // Check to see if the process is suspended, but not blocked
    pcb_remove(ready, blocked, susReady, susBlocked, toReady);
    if (strcmp_ic(toReady->state, "susBlocked") == 0)
    {
        toReady->state = "susReady";
    }
    // remove the pcb from a queue, change the state to blocked and insert it into the blocked queue.
    if (strcmp_ic(toReady->state, "blocked") == 0)
    {
        toReady->state = "ready";
    }
    pcb_insert(ready, blocked, susReady, susBlocked, toReady);
    printf("\033[0;32mSuccessfully unblocked process \"%s\".\033[0;0m\n", toReady->name_ptr);
}

void suspendPCB(queue *ready, queue *blocked, queue *susReady, queue *susBlocked)
{
    char buf[100] = {0};

getName:
    printf("\033[0;36mEnter PCB name:\033[0;0m\n>> ");
    // Read in name the user entered
    sys_req(READ, COM1, buf, sizeof(buf));
    if (strlen(buf) < 1 || strlen(buf) > 12)
    {
        printf("\033[0;31mInvalid name for process. Name must be 1 - 12 characters long. \nEnter \"EXIT\" to exit to main menu.\033[0;0m\n");
        goto getName;
    }
    char *name = (char *)sys_alloc_mem(strlen(buf) + 1);
    // Check to see if the memory allocation was successful
    if (name == NULL)
    {
        printf("Memory allocation failed.");
        return;
    }
    str_copy(name, buf, 0, strlen(buf));

    if (strcmp_ic(name, "exit") == 0)
    {
        return;
    }

    // Check to see if the PCB is in a queue
    pcb *toSuspend = pcb_find(ready, blocked, susReady, susBlocked, name);
    if (toSuspend == NULL)
    {
        printf("\033[0;31mCould not find process \"%s\" to suspend.\033[0;0m\n", name);
        return;
    }

    if (toSuspend->clas == 0)
    {
        printf("\033[0;31mCannot suspend system process \"%s\".\033[0;0m\n", name);
        return;
    }

    // check to see if the process is already in the suspended state
    if (strcmp(toSuspend->state, "susReady") == 0 || strcmp(toSuspend->state, "susBlocked") == 0)
    {
        printf("\033[0;33m%s is already in the state of being suspended. Nothing will be done.\n\033[0;0m", toSuspend->name_ptr);
        return;
    }
    // if not remove them, set their approrpriate state and insert them back into the queues.
    else
    {
        pcb_remove(ready, blocked, susReady, susBlocked, toSuspend);
    }

    if (strcmp(toSuspend->state, "ready") == 0)
        toSuspend->state = "susReady";
    else if (strcmp(toSuspend->state, "blocked") == 0)
        toSuspend->state = "susBlocked";

    pcb_insert(ready, blocked, susReady, susBlocked, toSuspend);
    printf("\033[0;32mSuccessfully suspended process \"%s\".\033[0;0m\n", toSuspend->name_ptr);
}

void resumePCB(queue *ready, queue *blocked, queue *susReady, queue *susBlocked)
{
    char buf[100] = {0};

getName:
    printf("\033[0;36mEnter PCB name:\033[0;0m\n>> ");
    // Read in name the user entered
    sys_req(READ, COM1, buf, sizeof(buf));
    if (strlen(buf) < 1 || strlen(buf) > 12)
    {
        printf("\033[0;31mInvalid name for process. Name must be 1 - 12 characters long. \nEnter \"EXIT\" to exit to main menu.\033[0;0m\n");
        goto getName;
    }
    char *name = (char *)sys_alloc_mem(strlen(buf) + 1);
    // Check to see if the memory allocation was successful
    if (name == NULL)
    {
        printf("Memory allocation failed.");
        return;
    }
    str_copy(name, buf, 0, strlen(buf));

    if (strcmp_ic(name, "exit") == 0)
    {
        return;
    }

    // Check to see if the PCB is in a queue
    pcb *toReady = pcb_find(ready, blocked, susReady, susBlocked, name);
    if (toReady == NULL)
    {
        printf("\033[0;31mCould not find process \"%s\" to resume.\033[0;0m\n", name);
        return;
    }
    // check to see if the process is already in the suspended state
    if (strcmp(toReady->state, "ready") == 0 || strcmp(toReady->state, "blocked") == 0)
    {
        printf("\033[0;33mThe specified PCB \"%s\"is already in the resumed state. Nothing will be done.\n\033[0;0m", name);
        return;
    }
    // if not remove them, set their appropriate state and insert them back into the queues.
    else
    {
        pcb_remove(ready, blocked, susReady, susBlocked, toReady);
    }

    if (strcmp(toReady->state, "susReady") == 0)
        toReady->state = "ready";
    else if (strcmp(toReady->state, "susBlocked") == 0)
        toReady->state = "blocked";

    pcb_insert(ready, blocked, susReady, susBlocked, toReady);
    printf("\033[0;32mSuccessfully resumed process \"%s\".\033[0;0m\n", toReady->name_ptr);
}

void setPCBPriority(queue *ready, queue *blocked, queue *susReady, queue *susBlocked)
{
    char buf[100] = {0};

getName:
    printf("\033[0;36mEnter PCB name:\033[0;0m\n>> ");
    // Read in name the user entered
    sys_req(READ, COM1, buf, sizeof(buf));
    if (strlen(buf) < 1 || strlen(buf) > 12)
    {
        printf("\033[0;31mInvalid name for process. Name must be 1 - 12 characters long. \nEnter \"EXIT\" to exit to main menu.\033[0;0m\n");
        goto getName;
    }
    char *name = (char *)sys_alloc_mem(strlen(buf) + 1);
    // Check to see if the memory allocation was successful
    if (name == NULL)
    {
        printf("Memory allocation failed.");
        return;
    }
    str_copy(name, buf, 0, strlen(buf));

    if (strcmp_ic(name, "exit") == 0)
    {
        return;
    }

    // Check to see if a PCB with the given name exists
    pcb *toSetP = pcb_find(ready, blocked, susReady, susBlocked, name);
    if (toSetP == NULL)
    {
        printf("\033[0;31mCould not find process \"%s\" to change priority.\n\033[0;0m", name);
        return;
    }

    if (toSetP->clas == 0)
    {
        printf("\033[0;31mCannot change priority of system process \"%s\".\n\033[0;0m", name);
        return;
    }

getPriority:
    printf("\033[0;36mEnter PCB priority:\033[0;0m\n>> ");
    // read in the priority the user inputted
    sys_req(READ, COM1, buf, sizeof(buf));
    if (strcmp_ic(buf, "") == 0)
    {
        printf("\033[0;31mInvalid priority for process \"%s\". Priority must be valid integer from 0 to 9. \nEnter \"EXIT\" to exit to main menu.\033[0;0m\n", name);
        goto getPriority;
    }
    if (strcmp_ic(buf, "exit") == 0)
    {
        return;
    }
    int priority = atoi(buf);
    // Make sure the priority is valid
    if (priority < 0 || priority > 9)
    {
        printf("\033[0;31mInvalid priority for process \"%s\". Priority must be valid integer from 0 to 9. \nEnter \"EXIT\" to exit to main menu.\033[0;0m\n", name);
        goto getPriority;
    }
    if (priority == toSetP->priority)
    {
        printf("\033[0;33mThe priority of the specific PCB \"%s\" is already %d. Nothing will be done.\n\033[0;0m", name, priority);
        return;
    }
    // Set the priority to the new priority entered in
    int prevPriority = toSetP->priority;
    toSetP->priority = priority;

    if (strcmp(toSetP->state, "ready") == 0 || strcmp(toSetP->state, "susReady") == 0)
    {
        pcb_remove(ready, blocked, susReady, susBlocked, toSetP);
        pcb_insert(ready, blocked, susReady, susBlocked, toSetP);
    }

    printf("\033[0;32mSuccessfully changed priority for %s from %d to %d.\033[0;0m\n", toSetP->name_ptr, prevPriority, toSetP->priority);
}

void showPCB(queue *ready, queue *blocked, queue *susReady, queue *susBlocked)
{
    char buf[100] = {0};
    char *susStatus = "No";

getName:
    printf("\033[0;36mEnter PCB name:\033[0;0m\n>> ");
    // Read in name the user entered
    sys_req(READ, COM1, buf, sizeof(buf));
    if (strlen(buf) < 1 || strlen(buf) > 12)
    {
        printf("\033[0;31mInvalid name for process. Name must be 1 - 12 characters long. \nEnter \"EXIT\" to exit to main menu.\033[0;0m\n");
        goto getName;
    }
    char *name = (char *)sys_alloc_mem(strlen(buf) + 1);
    // Check to see if the memory allocation was successful
    if (name == NULL)
    {
        printf("Memory allocation failed.");
        return;
    }
    str_copy(name, buf, 0, strlen(buf));

    if (strcmp_ic(name, "exit") == 0)
    {
        return;
    }

    // check to see if the name of the PCB is a name of a PCB in one of the queues
    pcb *toShow = pcb_find(ready, blocked, susReady, susBlocked, name);
    if (toShow == NULL)
    {
        printf("\033[0;31mCould not find process \"%s\" to show.\033[0;0m\n", name);
        return;
    }
    // get the status of the PCB
    if (strcmp(toShow->state, "susReady") == 0 || strcmp(toShow->state, "susBlocked") == 0)
    {
        susStatus = "Yes";
    }
    // print out the PCB information
    printf("\033[0;36mPCB \"%s\"--\n"
           "\tName:      %s\n"
           "\tClass:     %d\n"
           "\tState:     %s\n"
           "\tSuspended: %s\n"
           "\tPriority:  %d\033[0;0m\n",
           toShow->name_ptr, toShow->name_ptr, toShow->clas, toShow->state, susStatus, toShow->priority);
}

void showReady(queue *ready)
{
    printf("\033[0;36mReady:\n");
    printq(ready);
    printf("\033[0;0m");
}

void showBlocked(queue *blocked)
{
    printf("\033[0;36mBlocked:\n");
    printq(blocked);
    printf("\033[0;0m");
}

void showAll(queue *ready, queue *blocked, queue *susReady, queue *susBlocked)
{
    printf("\033[0;36mReady:\n");
    printq(ready);

    printf("\nBlocked:\n");
    printq(blocked);

    printf("\nSuspended Ready:\n");
    printq(susReady);

    printf("\nSuspended Blocked:\n");
    printq(susBlocked);
    printf("\033[0;0m");
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
    puts("Type \"Delete PCB\" or type ‘6’ to delete a PCB\n");
    puts("Type \"Block PCB\" or type ‘7’ to block a PCB\n");
    puts("Type \"Unblock PCB\" or type ‘8’ to unblock a PCB\n");
    puts("Type \"Suspend PCB\" or type ‘9’ to suspend a PCB\n");
    puts("Type \"Resume PCB\" or type ‘10’ to resume PCB\n");
    puts("Type \"Set PCB Priority\" or type ‘11’ to update a PCB's priority\n");
    puts("Type \"Show PCB\" or type ‘12’ to show details of a specific PCB\n");
    puts("Type \"Show Ready\" or type ‘13’ to show PCB's in the ready queue\n");
    puts("Type \"Show Blocked\" or type ‘14’ to show PCB's in the blocked queue\n");
    puts("Type \"Show All\" or type ‘15’ to show all PBC's in all the queues\n");
    puts("Type \"Load R3\" or type ‘16’ to load the processes created to test R3 into the ready queue\n");
    puts("Type \"Alarm\" or type ‘17’ to set an alarm that prints a message at a given time\n");
    puts("Type \"help\" or type ‘18’ to see a list of commands you can run\n");
    puts("Type \"shutdown\" or type ‘19’ to exit the operating system\n");
    printf("\033[0;0m");
}

void yield()
{
    sys_req(IDLE);
    printf("\033[0;32m");
    printf("Process has changed to idle.\n");
    printf("\033[0;0m");
}

void loadR3()
{
    // CREATE PROC 1
    pcb *proc1PCB = pcb_setup("proc1", 1, 2);
    if (proc1PCB == NULL)
    {
        return;
    }
    // Assign a new context pointer to point to the space in the stack we have reserved for the context
    context *proc1Context = (context *)proc1PCB->stack_ptr;
    // Initialize segment registers to 0x10
    proc1Context->gs = 0x10;
    proc1Context->es = 0x10;
    proc1Context->ds = 0x10;
    proc1Context->ss = 0x10;
    proc1Context->fs = 0x10;

    // EIP points to our function name, which is where execution will start when the process is loaded
    proc1Context->EIP = (int)proc1;
    proc1Context->CS = 0x08;
    proc1Context->EFLAGS = 0x0202;

    // All other registers set to 0
    proc1Context->EAX = 0;
    proc1Context->EBX = 0;
    proc1Context->ECX = 0;
    proc1Context->EDX = 0;
    proc1Context->ESI = 0;
    proc1Context->EDI = 0;

    // Set ESP to be the top of the stack
    proc1Context->ESP = (int)proc1PCB->stack_ptr;
    // Set EBP to be the bottom of the stack
    proc1Context->EBP = (int)proc1PCB->stack;
    // enqueue the process
    enqueue(ready, proc1PCB);

    // CREATE PROC2
    pcb *proc2PCB = pcb_setup("proc2", 1, 2);
    if (proc2PCB == NULL)
    {
        return;
    }
    // Assign a new context pointer to point to the space in the stack we have reserved for the context
    context *proc2Context = (context *)proc2PCB->stack_ptr;
    // Initialize segment registers to 0x10
    proc2Context->gs = 0x10;
    proc2Context->es = 0x10;
    proc2Context->ds = 0x10;
    proc2Context->ss = 0x10;
    proc2Context->fs = 0x10;

    // EIP points to our function name, which is where execution will start when the process is loaded
    proc2Context->EIP = (int)proc2;
    proc2Context->CS = 0x08;
    proc2Context->EFLAGS = 0x0202;

    // All other registers set to 0
    proc2Context->EAX = 0;
    proc2Context->EBX = 0;
    proc2Context->ECX = 0;
    proc2Context->EDX = 0;
    proc2Context->ESI = 0;
    proc2Context->EDI = 0;

    // Set ESP to be the top of the stack
    proc2Context->ESP = (int)proc2PCB->stack_ptr;
    // Set EBP to be the bottom of the stack
    proc2Context->EBP = (int)proc2PCB->stack;
    // enqueue the process
    enqueue(ready, proc2PCB);

    // CREATE PROC 3
    pcb *proc3PCB = pcb_setup("proc3", 1, 2);
    if (proc3PCB == NULL)
    {
        return;
    }
    // Assign a new context pointer to point to the space in the stack we have reserved for the context
    context *proc3Context = (context *)proc3PCB->stack_ptr;
    // Initialize segment registers to 0x10
    proc3Context->gs = 0x10;
    proc3Context->es = 0x10;
    proc3Context->ds = 0x10;
    proc3Context->ss = 0x10;
    proc3Context->fs = 0x10;

    // EIP points to our function name, which is where execution will start when the process is loaded
    proc3Context->EIP = (int)proc3;
    proc3Context->CS = 0x08;
    proc3Context->EFLAGS = 0x0202;

    // All other registers set to 0
    proc3Context->EAX = 0;
    proc3Context->EBX = 0;
    proc3Context->ECX = 0;
    proc3Context->EDX = 0;
    proc3Context->ESI = 0;
    proc3Context->EDI = 0;

    // Set ESP to be the top of the stack
    proc3Context->ESP = (int)proc3PCB->stack_ptr;
    // Set EBP to be the bottom of the stack
    proc3Context->EBP = (int)proc3PCB->stack;
    // enqueue the process
    enqueue(ready, proc3PCB);

    // CREATE PROC 3
    pcb *proc4PCB = pcb_setup("proc4", 1, 2);
    if (proc4PCB == NULL)
    {
        return;
    }
    // Assign a new context pointer to point to the space in the stack we have reserved for the context
    context *proc4Context = (context *)proc4PCB->stack_ptr;
    // Initialize segment registers to 0x10
    proc4Context->gs = 0x10;
    proc4Context->es = 0x10;
    proc4Context->ds = 0x10;
    proc4Context->ss = 0x10;
    proc4Context->fs = 0x10;

    // EIP points to our function name, which is where execution will start when the process is loaded
    proc4Context->EIP = (int)proc4;
    proc4Context->CS = 0x08;
    proc4Context->EFLAGS = 0x0202;

    // All other registers set to 0
    proc4Context->EAX = 0;
    proc4Context->EBX = 0;
    proc4Context->ECX = 0;
    proc4Context->EDX = 0;
    proc4Context->ESI = 0;
    proc4Context->EDI = 0;

    // Set ESP to be the top of the stack
    proc4Context->ESP = (int)proc4PCB->stack_ptr;
    // Set EBP to be the bottom of the stack
    proc4Context->EBP = (int)proc4PCB->stack;
    // enqueue the process
    enqueue(ready, proc4PCB);

    // CREATE PROC 5
    pcb *proc5PCB = pcb_setup("proc5", 1, 2);
    if (proc5PCB == NULL)
    {
        return;
    }
    // Assign a new context pointer to point to the space in the stack we have reserved for the context
    context *proc5Context = (context *)proc5PCB->stack_ptr;
    // Initialize segment registers to 0x10
    proc5Context->gs = 0x10;
    proc5Context->es = 0x10;
    proc5Context->ds = 0x10;
    proc5Context->ss = 0x10;
    proc5Context->fs = 0x10;

    // EIP points to our function name, which is where execution will start when the process is loaded
    proc5Context->EIP = (int)proc5;
    proc5Context->CS = 0x08;
    proc5Context->EFLAGS = 0x0202;

    // All other registers set to 0
    proc5Context->EAX = 0;
    proc5Context->EBX = 0;
    proc5Context->ECX = 0;
    proc5Context->EDX = 0;
    proc5Context->ESI = 0;
    proc5Context->EDI = 0;

    // Set ESP to be the top of the stack
    proc5Context->ESP = (int)proc5PCB->stack_ptr;
    // Set EBP to be the bottom of the stack
    proc5Context->EBP = (int)proc5PCB->stack;
    // enqueue the process
    enqueue(ready, proc5PCB);
}

void alarm()
{
    printf("\033[0;36mEnter alarm name:\033[0;0m\n>> ");
    // Read in name the user entered
    char name[15] = {0};
    sys_req(READ, COM1, name, sizeof(name));
    if (strlen(name) < 1 || strlen(name) > 12)
    {
        printf("\033[0;31mInvalid name for process. Name must be 1 - 12 characters long.\033[0;0m\n");

    }

    if (strcmp_ic(name, "exit") == 0)
    {
        return;
    }

    // Check to see if the PCB is in a queue
    pcb *toCreate = pcb_find(ready, blocked, susReady, susBlocked, name);
    if (toCreate != NULL)
    {   
        printf("\033[0;31mAlarm '%s' already exists.\033[0;0m\n", name);
        return;
    }

    printf("Enter a time for the alarm (hh:mm:ss):\n");
    printf(">> ");
    // buffer for input
    char buf[100] = {0};
    int hours = 0;
    int minutes = 0;
    int seconds = 0;
    char message[100] = {0};
    // reads in the time for the alarm
    sys_req(READ, COM1, buf, sizeof(buf));

    if (isdigit(buf[0]) && isdigit(buf[1]) && isdigit(buf[3]) && isdigit(buf[4]) && isdigit(buf[6]) && isdigit(buf[7]))
    {

        // Set Hours
        hours = atoi(&buf[0]);
        minutes = atoi(&buf[3]);
        seconds = atoi(&buf[6]);
        int isInvalid = 0;

        // verifies input
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
            printf("Entered time was invalid. Exiting command...\n");
            return;
        }
    }

    else
    {
        printf("Did not enter a time in the valid format hh:mm:ss. Exiting command...\n");
        return;
    }

    // Reading in the message for the alarm
    printf("Enter a message for the alarm to display when the timer goes off (100 character limit):\n");
    printf(">> ");
    sys_req(READ, COM1, message, sizeof(message));

    pcb *alarmPCB = pcb_setup(name, 1, 2);
    if (alarmPCB == NULL)
    {
        return;
    }

    // Assign a new context pointer to point to the space in the stack we have reserved for the context
    context *alarmContext = (context *)alarmPCB->stack_ptr;

    // Initialize segment registers to 0x10
    alarmContext->gs = 0x10;
    alarmContext->es = 0x10;
    alarmContext->ds = 0x10;
    alarmContext->ss = 0x10;
    alarmContext->fs = 0x10;

    // EIP points to our function name, which is where execution will start when the process is loaded
    alarmContext->EIP = (int)alarmExecution;
    alarmContext->CS = 0x08;
    alarmContext->EFLAGS = 0x0202;

    // All other registers set to 0
    alarmContext->EAX = 0;
    alarmContext->EBX = 0;
    alarmContext->ECX = 0;
    alarmContext->EDX = 0;
    alarmContext->ESI = 0;
    alarmContext->EDI = 0;

    // Set ESP to be the top of the stack
    alarmContext->ESP = (int)alarmPCB->stack_ptr;
    // Set EBP to be the bottom of the stack
    alarmContext->EBP = (int)alarmPCB->stack;

    // Initialize an alarm struct with the values input by the user
    // ISSUE LIKELY CAUSED BY LOCAL DEFINITION OF STRUCT
    alarm_struct *newAlarm = sys_alloc_mem(sizeof(alarm_struct));
    newAlarm->hours = hours;
    newAlarm->minutes = minutes;
    newAlarm->seconds = seconds;
    newAlarm->message = sys_alloc_mem(sizeof(message));
    str_copy(newAlarm->message, message, 0, strlen(message));

    // Have the alarm pcb's alarm_ptr point to the struct we just made
    alarmPCB->alarm_ptr = newAlarm;
    // enqueue the process
    enqueue(ready, alarmPCB);
}
