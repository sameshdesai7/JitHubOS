#include <stdio.h>
#include <string.h>
#include <sys_req.h>
#include <stdlib.h>

void putc(char c){
    sys_req(WRITE, COM1, &c, strlen(&c));
}

void puts(const char* s){
    sys_req(WRITE, COM1, s, strlen(s));
}


void printf(const char* format, ...){

    int* argumentPta = (int*)&format;
    argumentPta++;

    while(*format){
        if(*format == '%'){
            format++;
            switch(*format){
                case 'd':
                {
                    char buffa[12] = {0};
                    itoa(*argumentPta, buffa);
                    puts(buffa);
                } case 's':
                {
                    puts((char*) argumentPta);
                } case 'c':
                {
                    putc(*argumentPta);
                }
            }
            argumentPta++;
        }
        else{
            putc(*format);
        }
        format++;
    }
}
