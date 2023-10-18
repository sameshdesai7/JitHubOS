#ifndef SYS_CALL_H
#define SYS_CALL_H
#include <context.h>
#include <dataStructs.h>

pcb* cop = NULL;
context* original_context = NULL;

context* sys_call(context* proc_context);

#endif
