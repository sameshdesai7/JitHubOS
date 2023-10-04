#include <context.h>
#include <sys_call.h>
#include <dataStructs.h>
#include <sys_req.h>
#include <dataStructs.h>

context* sys_call(context* proc_context) {
    sys_req(IDLE);


    return dequeue(ready)->stack_ptr;
}