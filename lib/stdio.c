#include <stdio.h>
#include <string.h>
#include <sys_req.h>
#include <stdlib.h>
#include <memory.h> 

void putc(char c){
    sys_req(WRITE, COM1, &c, strlen(&c));
}

void puts(const char* s){
    sys_req(WRITE, COM1, s, strlen(s));
}


void printf(const char* format, ...){

    char* stringBuild = (char*)sys_alloc_mem(512);
    char* builder = stringBuild;
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
                    break;
                } case 's':
                {
                    *builder = *argumentPta;
                    builder++;
                    break;
                } case 'c':
                {
                    putc(*argumentPta);
                    break;
                }
            }
            argumentPta++;
        }
        else{
            *builder = *format;
            builder++;
        }
        format++;
    }

    sys_req(WRITE, COM1, stringBuild, strlen(stringBuild));
}
