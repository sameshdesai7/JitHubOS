#include <../include/comhand.h>
#include <mpx/device.h>
#include <sys_req.h>
#include <string.h>

void comhand(void)
{
    for (;;){
        char buf[10] = {0};
        int nread = sys_req(READ, COM1, buf, sizeof(buf));

        //the following echos the input -- good for testing may need to take out later
        sys_req(WRITE, COM1, buf, nread);
        nread=nread + 1 - 1;
        //if shutdown is selected
        if(strcmp(buf, "shutdown") == 0){
            //if shutdown is confirmed
            nread = sys_req(READ, COM1, buf, sizeof(buf));
            if(strcmp(buf, "yes") == 0){
                return;
            }
        }

        
        
    }
}

