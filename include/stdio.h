#ifndef STDIO_H
#define STDIO_H

/**
 * @file stdio.h
 * @brief A set of functions for input and output interactions
*/

/**
 * @brief Prints a single character to the console
 * @param c The character to print to the console
*/
void putc(char c);
/**
 * @brief Prints a string the console
 * @param s The string to print to the console
*/
void puts(const char* s);
/**
 * @brief Prints string to the console with insertable values specified by subsequent parameters
 * @param format The format in which the string is specified. Insertable values are specified with a % symbol
*/
void printf(const char* format, ...);

#endif
