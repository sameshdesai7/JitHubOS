#ifndef CONTEXT_H
#define CONTEXT_H

/**
 * @file context.h
 * @brief Header file that holds the context struct
*/

/**
 * @author Noah Marner
 * @author Jackson Monk
 * @brief Defines the registers in a context struct
*/
typedef struct {
    int gs;
    int fs;
    int es;
    int ds;
    int ss;
    int EDI;
    int ESI;
    int ESP;
    int EBP;
    int EBX;
    int EDX;
    int ECX;
    int EAX;
    int EIP;
    int CS;
    int EFLAGS;
} context;

#endif
