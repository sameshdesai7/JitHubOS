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

//compile constants to be used in version to show when the most recent compilation was
#define COMPILE_DATE __DATE__
#define COMPILE_TIME __TIME__

void comhand()
{
    queue* ready = sys_alloc_mem(sizeof(queue));
    queue* blocked = sys_alloc_mem(sizeof(queue));
    queue* susReady = sys_alloc_mem(sizeof(queue));
    queue* susBlocked = sys_alloc_mem(sizeof(queue));
    ready->pFlag = 1;
    blocked->pFlag = 0;
    susReady->pFlag = 1;
    susBlocked->pFlag = 0;

    //infinite loop
    for (;;)
    {
        //buffer for first input read in
        char buf[100] = {0};
        //prints the menu that shows all of the choices
        printMenu();
        //reads in the first input
        sys_req(READ, COM1, buf, sizeof(buf));

        // Shutdown Command
        if ((strcmp_ic(buf, "shutdown") == 0) || (strcmp(buf, "18") == 0))
        {
            //if shutdown is confirmed, exit loop
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

        //Set Time Command
        else if ((strcmp_ic(buf, "Set Time") == 0) || (strcmp(buf, "3") == 0))
        {
            setTime();
        }

        //Set Date Command
        else if ((strcmp_ic(buf, "Set Date") == 0) || (strcmp(buf, "5") == 0))
        {
            setDate();
        }

        //create PCB Command
        else if ((strcmp_ic(buf, "Create PCB") == 0) || (strcmp(buf, "6") == 0)){
            createPCB(ready, blocked, susReady, susBlocked);
        }

        //Delete PCB Command
        else if ((strcmp_ic(buf, "Delete PCB") == 0) || (strcmp(buf, "7") == 0)){
            deletePCB(ready, blocked, susReady, susBlocked);
        }

        //Block PCB Command
        else if ((strcmp_ic(buf, "Block PCB") == 0) || (strcmp(buf, "8") == 0)){
            blockPCB(ready, blocked, susReady, susBlocked);
        }

        //Unblock PCB Command
        else if ((strcmp_ic(buf, "Unblock PCB") == 0) || (strcmp(buf, "9") == 0)){
            unblockPCB(ready, blocked, susReady, susBlocked);
        }

        //Suspend PCB Command
        else if ((strcmp_ic(buf, "Suspend PCB") == 0) || (strcmp(buf, "10") == 0)){
            suspendPCB(ready, blocked, susReady, susBlocked);
        }
        
        // //Resume PCB Command
        // else if ((strcmp_ic(buf, "Resume PCB") == 0) || (strcmp(buf, "11") == 0)){
        //     resumePCB(ready, blocked, susReady, susBlocked);
        // }
        
        //Set PCB Priority Command
        else if ((strcmp_ic(buf, "Set PCB Priority") == 0) || (strcmp(buf, "12") == 0)){
            setPCBPriority(ready, blocked, susReady, susBlocked);
        }
        
        //Show PCB Command
        else if ((strcmp_ic(buf, "Show PCB") == 0) || (strcmp(buf, "13") == 0)){
            showPCB(ready, blocked, susReady, susBlocked);
        }

        // //Show Ready Command
        else if ((strcmp_ic(buf, "Show Ready") == 0) || (strcmp(buf, "14") == 0)){
            showReady(ready);
        }

        // //Show Blocked Command
        else if ((strcmp_ic(buf, "Show Blocked") == 0) || (strcmp(buf, "15") == 0)){
            showBlocked(blocked);
        }
        // //Show All command
        else if ((strcmp_ic(buf, "Show All") == 0) || (strcmp(buf, "16") == 0)){
            showAll(ready, blocked, susReady, susBlocked);
        }

        //help command
        else if ((strcmp_ic(buf, "help") == 0) || strcmp(buf, "17") == 0)
        {
            help();
        }
        //individual help commands
        else if (strcmp_ic(buf, "help version") == 0 || strcmp_ic(buf, "6 version") == 0){
            printf("\033[0;36m");
            puts("Type \"version\" to retrieve the current version of the operating system\n");
            printf("\033[0;0m");
        }
        else if (strcmp_ic(buf, "help help") == 0 || strcmp_ic(buf, "6 help") == 0){
            printf("\033[0;36m");
            puts("Type \"help\" to see a list of commands you can run\n");
            printf("\033[0;0m");
        }
        else if (strcmp_ic(buf, "help shutdown") == 0 || strcmp_ic(buf, "6 shutdown") == 0){
            printf("\033[0;36m");
            puts("Type \"shutdown\" to exit the operating system\n");
            printf("\033[0;0m");
        }
        else if (strcmp_ic(buf, "help get time") == 0 || strcmp_ic(buf, "6 get time") == 0){
            printf("\033[0;36m");
            puts("Type \"get time\" to retrieve the current system time\n");
            printf("\033[0;0m");
        }
        else if (strcmp_ic(buf, "help set time") == 0 || strcmp_ic(buf, "6 set time") == 0){
            printf("\033[0;36m");
            puts("Type \"set time\" to set the system time\n");
            printf("\033[0;0m");
        }
        else if (strcmp_ic(buf, "help get date") == 0 || strcmp_ic(buf, "6 get date") == 0){
            printf("\033[0;36m");
            puts("Type \"get date\" to retrieve the current system date\n");
            printf("\033[0;0m");
        }
        else if (strcmp_ic(buf, "help set date") == 0 || strcmp_ic(buf, "6 set date") == 0){
            printf("\033[0;36m");
            puts("Type \"set date\" to set the system date\n");
            printf("\033[0;0m");
        }
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
    printf("6. Create PCB\n");
    printf("7. Delete PCB\n");
    printf("8. Block PCB\n");
    printf("9. Unblock PCB\n");
    printf("10. Suspend PCB\n");
    printf("11. Resume PCB\n");
    printf("12. Set PCB Priority\n");
    printf("13. Show PCB\n");
    printf("14. Show Ready\n");
    printf("15. Show Blocked\n");
    printf("16. Show All\n");
    printf("17. Help\n");
    printf("18. Shutdown\n");
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
        //exit loop
        return 1;
    }
    else
    {
        //continue loop
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

    //Attaching the hours register to port 70
    outb(0x70, 0x04);
    //Reading the data from the hours register through port 71
    int hours = inb(0x71);
    int formatedHours = fromBCD(hours);

    //Attaching the minutes register to port 70
    outb(0x70, 0x02);
    //Reading the data from the minutes register through port 71
    int minutes = inb(0x71);
    int formatedMinutes = fromBCD(minutes);

    //Attaching the seconds register to port 70
    outb(0x70, 0x00);
    //Reading the data from the seconds register through port 71
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

    //checks to see hours are only 1 digit and will replace with the equivalent 2 digits
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


    if(!(isdigit(buf[6]) && isdigit(buf[7]))){
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

        //verifies input
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
        //disables interrupts and sets the time to each register
        cli();

        outb(0x70, 0x04);
        outb(0x71, toBCD(hours));

        outb(0x70, 0x02);
        outb(0x71, toBCD(minutes));

        outb(0x70, 0x00);
        outb(0x71, toBCD(seconds));

        printf("\033[0;32mTime set to %s.\n\033[0;0m", buf);
        //enables interrupts
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

    if(strlen(buf) > 8){
        printf("\033[0;31m");
        printf("Invalid Date Format. Please use the mm/dd/yy format.");
        printf("\033[0;0m");
        return;
    }
    
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
        //disables interrupts
        cli();
        int year = atoi(&buf[6]);
        int month = atoi(&buf[0]);
        int day = atoi(&buf[3]);
        
        // int days[]= {31, 28, }
        printf("\033[0;31m");
        //verifies input
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

        //sets the date to each of the registers
        if (month == 2 | month == 4 || month == 6 || month == 9 || month == 11){
            outb(0x70, 0x09);
            outb(0x71, toBCD(year));

            outb(0x70, 0x07);
            outb(0x71, toBCD(day));
            
            outb(0x70, 0x08);
            outb(0x71, toBCD(month));
        }
        else{
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

        //enables interrupts
        sti();
    }
    else
    { // if the overall format does not match a proper format
        printf("\033[0;31m");
        puts("Invalid date format");
        printf("\033[0;0m");
    }
}

void createPCB(queue* ready, queue* blocked, queue* susReady, queue* susBlocked) {
    char buf[100] = {0};
    
    getName:
    printf("\033[0;36mEnter PCB name:\033[0;0m\n>> ");
    //Read in name the user entered
    sys_req(READ, COM1, buf, sizeof(buf));
    char* name = (char*)sys_alloc_mem(strlen(buf) + 1);
    //Check to see if the memory allocation was successful
    if(name == NULL){
        printf("Memory allocation failed.");
        return;
    }
    str_copy(name, buf, 0, strlen(buf));
    //check to see if the name already exists in a queue
    if (pcb_find(ready, blocked, susReady, susBlocked, name) != NULL) {
        printf("\033[0;31mInvalid name \"%s\". PCB with that name already exists.\033[0;0m\n", name);
        goto getName;
    }

    getClass:
    printf("\033[0;36mEnter PCB class:\033[0;0m\n>> ");
    //Read in the class the user specified in the input
    sys_req(READ, COM1, buf, sizeof(buf));
    int class = atoi(buf);
    //Check to see if the class is a valid class
    if (class < 0 || class > 1) {
        printf("\033[0;31mInvalid class value for process \"%s\". Class must be either 0 for system process or 1 for user process.\033[0;0m\n", name);
        goto getClass;
    }

    getPriority:
    printf("\033[0;36mEnter PCB priority:\033[0;0m\n>> ");
    //Read in the priority from the user input
    sys_req(READ, COM1, buf, sizeof(buf));
    int priority = atoi(buf);
    //Check to make sure the priority is valid
    if (priority < 0 || priority > 9) {
        printf("\033[0;31mInvalid priority for process \"%s\". Priority must be valid integer from 0 to 9.\033[0;0m\n", name);
        goto getPriority;
    }
    //Actually create the PCB and insert it into the queue
    pcb* newPCB = pcb_setup(name, class, priority);
    pcb_insert(ready, blocked, susReady, susBlocked, newPCB);
    printf("\033[0;32mSuccessfully inserted process \"%s\".\033[0;0m\n", newPCB->name_ptr);
}

void deletePCB(queue* ready, queue* blocked, queue* susReady, queue* susBlocked) {
    char buf[100] = {0};

    printf("\033[0;36mEnter PCB name to delete:\033[0;0m\n>> ");
    sys_req(READ, COM1, buf, sizeof(buf));
    //Read in the name of the PCB to delete
    char* name = (char*)sys_alloc_mem(strlen(buf) + 1);
    //Make sure memory allocation was successful
    if(name == NULL){
        printf("\033[0;31mMemory allocation failed.\033[0;0m\n");
        return;
    }
    str_copy(name, buf, 0, strlen(buf));
    //If the name is in a queue, remove it. If not, write a message to the console. 
    pcb* toRemove = pcb_find(ready, blocked, susReady, susBlocked, name);
    if (toRemove == NULL) {
        printf("\033[0;31mCould not find process \"%s\" to delete.\033[0;0m\n", name);
        return;
    }
    if (toRemove->clas == 0) {
        printf("\033[0;31mError: \"%s\" is a system process. Cannot request to delete a system process.\033[0;0m\n", name);
        return;
    }
    //Remove the PCB and free its memory
    pcb_remove(ready, blocked, susReady, susBlocked, toRemove);
    pcb_free(toRemove);
    printf("\033[0;32mSuccessfully deleted process \"%s\".\033[0;0m\n", toRemove->name_ptr);
}

void blockPCB(queue* ready, queue* blocked, queue* susReady, queue* susBlocked) {
    char buf[100] = {0};

    printf("\033[0;36mEnter PCB name to block:\033[0;0m\n>> ");
    //Read in the name of the PCB to block from the user input
    sys_req(READ, COM1, buf, sizeof(buf));
    char* name = (char*)sys_alloc_mem(strlen(buf) + 1);
    //Check if the memory allocation was successful
    if(name == NULL){
        printf("Memory allocation failed.");
        return;
    }
    str_copy(name, buf, 0, strlen(buf));
    //Check to see if the process is inside of a queue already
    pcb* toBlock = pcb_find(ready, blocked, susReady, susBlocked, name);
    if (toBlock == NULL) {
        printf("\033[0;31mCould not find process \"%s\" to block.\033[0;0m\n", name);
        return;
    }
    //Check to see if the process is already blocked
    if (strcmp_ic(toBlock->state, "blocked") == 0 || strcmp_ic(toBlock->state, "susBlocked") == 0 ){
        printf("\033[0;31mThe specified PCB is already in the blocked state\033[0;0m");
        return;
    }
    //Check to see if the process is suspended, but not blocked
    if (strcmp_ic(toBlock->state, "susReady") == 0){
        pcb_remove(ready, blocked, susReady, susBlocked, toBlock);
        toBlock->state = "susBlocked";
        pcb_insert(ready, blocked, susReady, susBlocked, toBlock);  
    }
    //remove the pcb from a queue, change the state to blocked and insert it into the blocked queue.
    else{
        pcb_remove(ready, blocked, susReady, susBlocked, toBlock);
        toBlock->state = "blocked";
        pcb_insert(ready, blocked, susReady, susBlocked, toBlock);
    }
    printf("\033[0;32mSuccessfully blocked process \"%s\".\033[0;0m\n", toBlock->name_ptr);
}

void unblockPCB(queue* ready, queue* blocked, queue* susReady, queue* susBlocked) {
    char buf[100] = {0};

    printf("\033[0;36mEnter PCB name to unblock:\033[0;0m\n>> ");
    //Read in the name of the PCB the user inputted
    sys_req(READ, COM1, buf, sizeof(buf));
    char* name = (char*)sys_alloc_mem(strlen(buf) + 1);
    //Check to see if the memory allocation was successful
    if(name == NULL){
        printf("Memory allocation failed.");
        return;
    }
    str_copy(name, buf, 0, strlen(buf));
    //Check to see if the PCB exists in a queue. 
    pcb* toReady = pcb_find(ready, blocked, susReady, susBlocked, name);
    if (toReady == NULL) {
        printf("\033[0;31mCould not find process \"%s\" to unblock.\033[0;0m\n.", name);
        return;
    }
    //Check to see if the process is already unblocked
    if (strcmp_ic(toBlock->state, "ready") == 0 || strcmp_ic(toBlock->state, "susReady") == 0 ){
        printf("\033[0;31mThe specified PCB is already in the ready state \033[0;0m");
        return;
    }
    //Check to see if the process is suspended, but not blocked
     pcb_remove(ready, blocked, susReady, susBlocked, toBlock);
    if (strcmp_ic(toBlock->state, "susBlocked") == 0){
        toBlock->state = "susReady";
    }
    //remove the pcb from a queue, change the state to blocked and insert it into the blocked queue.
    if (strcmp_ic(toBlock->state, "susReady") == 0){
        toBlock->state = "ready";
    }
      pcb_insert(ready, blocked, susReady, susBlocked, toBlock);
    printf("\033[0;32mSuccessfully unblocked process \"%s\".\033[0;0m\n", toReady->name_ptr);
}

void suspendPCB(queue* ready, queue* blocked, queue* susReady, queue* susBlocked) {
    char buf[100] = {0};

    printf("\033[0;36mEnter PCB name to suspend:\033[0;0m\n>> ");
    //Read in the name of the PCB the user inputted
    sys_req(READ, COM1, buf, sizeof(buf));
    char* name = (char*)sys_alloc_mem(strlen(buf) + 1);
    //Check to see if the memory allocation was successful
    if(name == NULL){
        printf("Memory allocation failed.");
        return;
    }
    str_copy(name, buf, 0, strlen(buf));
    //Check to see if the PCB is in a queue
    pcb* toSuspend = pcb_find(ready, blocked, susReady, susBlocked, name);
    if (toSuspend == NULL) {
        printf("\033[0;31mCould not find process \"%s\" to suspend.\033[0;0m\n.", name);
        return;
    }
    //check to see if the process is already in the suspended state
    if (strcmp(toSuspend->state, "susReady") == 0 || strcmp(toSuspend->state, "susBlocked") == 0){
        printf("\033[0;31m%s is already in the state of being suspended.\033m[0;0m\n", toSuspend->name_ptr);
        return;
    }
    //if not remove them, set their approrpriate state and insert them back into the queues. 
    else {
        pcb_remove(ready, blocked, susReady, susBlocked, toSuspend);
    }
    
    if (strcmp(toSuspend->state, "ready") == 0) toSuspend->state = "susReady";
    else if (strcmp(toSuspend->state, "blocked") == 0) toSuspend->state = "susBlocked";
    
    pcb_insert(ready, blocked, susReady, susBlocked, toSuspend);
    printf("\033[0;32mSuccessfully suspended process \"%s\".\033[0;0m\n", toSuspend->name_ptr);
}

void resumePCB(queue* ready, queue* blocked, queue* susReady, queue* susBlocked){
    char buf[100] = {0};

    printf("\033[0;36mEnter PCB name to resume:\033[0;0m\n>> ");
    //Read in the name of the PCB the user inputted
    sys_req(READ, COM1, buf, sizeof(buf));
    char* name = (char*)sys_alloc_mem(strlen(buf) + 1);
    //Check to see if the memory allocation was successful
    if(name == NULL){
        printf("Memory allocation failed.");
        return;
    }
    str_copy(name, buf, 0, strlen(buf));
    //Check to see if the PCB is in a queue
    pcb* toReady = pcb_find(ready, blocked, susReady, susBlocked, name);
    if (toReady == NULL) {
        printf("\033[0;31mCould not find process \"%s\" to resume.\033[0;0m\n.", name);
        return;
    }
    //check to see if the process is already in the suspended state
    if (strcmp(toReady->state, "ready") == 0 || strcmp(toReady->state, "blocked") == 0){
        printf("\033[0;31m%s is already in the resumed state.\033m[0;0m\n", toReady->name_ptr);
        return;
    }
    //if not remove them, set their approrpriate state and insert them back into the queues. 
    else {
        pcb_remove(ready, blocked, susReady, susBlocked, toReady);
    }
    
    if (strcmp(toReady->state, "susReady") == 0) toReady->state = "ready";
    else if (strcmp(toReady->state, "susBlocked") == 0) toReady->state = "blocked";
    
    pcb_insert(ready, blocked, susReady, susBlocked, toSuspend);
    printf("\033[0;32mSuccessfully resumed process \"%s\".\033[0;0m\n", toReady->name_ptr);
}

void setPCBPriority(queue* ready, queue* blocked, queue* susReady, queue* susBlocked){
    char buf[100] = {0};

    printf("\033[0;36mEnter PCB name to change priority:\033[0;0m\n>> ");
    //Read in the priority the user inputted
    sys_req(READ, COM1, buf, sizeof(buf));
    char* name = (char*)sys_alloc_mem(strlen(buf) + 1);
    //Check to see if the memory allocation was successful
    if(name == NULL){
        printf("Memory allocation failed.");
        return;
    }
    str_copy(name, buf, 0, strlen(buf));
    //Check to see if a PCB with the given name exists
    pcb* toSetP = pcb_find(ready, blocked, susReady, susBlocked, name);
    if (toSetP == NULL) {
        printf("\033[0;31mCould not find process \"%s\" to change priority.\033[0;0m\n.", name);
        return;
    }

    getPriority:
    printf("\033[0;36mEnter PCB priority:\033[0;0m\n>> ");
    //read in the priority the user inputted
    sys_req(READ, COM1, buf, sizeof(buf));
    if(strcmp_ic(buf,"exit") == 0){
        return;
    }
    int priority = atoi(buf);
    //Make sure the priority is valid
    if (priority < 0 || priority > 9) {
        printf("\033[0;31mInvalid priority for process \"%s\". Priority must be valid integer from 0 to 9. \nEnter \"EXIT\" to exit to main menu.\033[0;0m\n", name);
        goto getPriority;
    }
    //Set the priority to the new priority entered in
    int prevPriority = toSetP->priority;
    toSetP->priority = priority;
    printf("\033[0;32mSuccessfully changed priority for %s from %d to %d.\033[0;0m\n", toSetP->name_ptr, prevPriority, toSetP->priority);
}

void showPCB(queue* ready, queue* blocked, queue* susReady, queue* susBlocked){
    char buf[100] = {0};
    char* susStatus = "No";

    printf("\033[0;36mEnter PCB name to show:\033[0;0m\n>> ");
    //Reads in the PCB name the user entered
    sys_req(READ, COM1, buf, sizeof(buf));
    char* name = (char*)sys_alloc_mem(strlen(buf) + 1);
    //Check to see if memory allocation was successful
    if(name == NULL){
        printf("Memory allocation failed.");
        return;
    }
    str_copy(name, buf, 0, strlen(buf));
    //check to see if the name of the PCB is a name of a PCB in one of the queues
    pcb* toShow = pcb_find(ready, blocked, susReady, susBlocked, name);
    if (toShow == NULL) {
        printf("\033[0;31mCould not find process \"%s\" to show.\033[0;0m\n.", name);
        return;
    }
    //get the status of the PCB
     if (strcmp(toShow->state, "susReady") == 0 || strcmp(toShow->state, "susBlocked") == 0){
        susStatus = "Yes";
    }
    //print out the PCB information
    printf("\033[0;36mPCB \"%s\"--\n"
                "\tName:      %s\n"
                "\tClass:     %d\n"
                "\tState:     %s\n"
                "\tSuspended: %s\n"
                "\tPriority:  %d\033[0;0m\n", 
    toShow->name_ptr, toShow->name_ptr, toShow->clas, toShow->state, susStatus, toShow->priority);
}

void showReady(queue* ready){
    printf("\033[0;36mReady: ");
    printq(ready);
    printf("\033[0;0m");
}

void showBlocked(queue* blocked){
    printf("\033[0;36mBlocked: ");
    printq(blocked);
    printf("\033[0;0m");
}

void showAll(queue* ready, queue* blocked, queue* susReady, queue* susBlocked){
    printf("\033[0;36mReady: ");
    printq(ready);

    printf("Blocked: ");
    printq(blocked);

    printf("Suspended Ready: ");
    printq(susReady);

    printf("Suspended Blocked ");
    printq(susBlocked);
    printf("\033[0;0m");
}

void help(void){
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
