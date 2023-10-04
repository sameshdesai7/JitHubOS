#ifndef CONTEXT_H
#define CONTEXT_H

typedef struct {
    int gs;
    int fs;
    int es;
    int ds;
    int ss;
    int EDI;
    int ESI;
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