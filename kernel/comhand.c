#include <comhand.h>
#include <mpx/device.h>
#include <sys_req.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void comhand(void)
{
    
    for (;;){
        char buf[100] = {0};
        sys_req(READ, COM1, buf, sizeof(buf));


        //if shutdown is selected
        if(strcmp(buf, "shutdown") == 0){
            printf("Confirm Shutdown? Y/N\n");
            //if shutdown is confirmed
            sys_req(READ, COM1, buf, sizeof(buf));
            if(strcmp(buf, "Y") == 0){
                return;
            }
        }

        //TODO: Version Command
            //Where to store version info??

        //TODO: Help Command

        //TODO: Get/Set Time

        //TODO: Get/Set Date
        
        
    }
}

