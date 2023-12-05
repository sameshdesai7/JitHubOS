#include <stdio.h>
#include <string.h>
#include <sys_req.h>
#include <stdlib.h>
#include <memory.h> 
#include <string.h>

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
                    char* intPta = itoa(*argumentPta, buffa);
                    while(*intPta){
                        *builder = *intPta;
                        builder++;
                        intPta++;
                    }
                    break;
                } case 's':
                {   
                    char* strPta = (char*)*argumentPta;
                    while(*strPta){
                        *builder = *strPta;
                        builder++;
                        strPta++;
                    }
                    break;
                } case 'c':
                {
                    *builder = *argumentPta;
                    builder++;
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
    sys_free_mem(stringBuild);
    stringBuild = NULL;
    builder = NULL;
    argumentPta = NULL;
    format = NULL;
}
