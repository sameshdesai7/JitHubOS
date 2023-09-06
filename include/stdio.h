#ifndef STDIO_H
#define STDIO_H

/**
 * @file stdio.h
 * @brief A set of functions for input and output interactions
*/

/**
 * @author Samesh Desai
 * @author Noah Marner
 * @author Jackson Monk
 * @author Blake Wagner
 * @brief Prints a single character to the console
 * @details This function utilizes sys_req(WRITE) to print a single character to the
 * COM1 output register
 * @param c The character to print to the console
*/
void putc(char c);

/**
 * @author Samesh Desai
 * @author Noah Marner
 * @author Jackson Monk
 * @author Blake Wagner
 * @brief Prints a string the console
 * @details This function utilizes sys_req(WRITE) to print a string to the
 * COM1 output register
 * @param s The string to print to the console
*/
void puts(const char* s);

/**
 * @author Samesh Desai
 * @author Noah Marner
 * @author Jackson Monk
 * @author Blake Wagner
 * @brief Prints string to the console with insertable values specified by subsequent parameters
 * @details Creates a pointer to look at the first argument. Loops while there are characters remaining in the format string.
 * When we encounter a % sign in the format string, pulls from the argument pointer and increments it.
 * @param format The format in which the string is specified. Insertable values are specified with a % symbol
*/
void printf(const char* format, ...);

#endif
