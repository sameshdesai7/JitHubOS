#ifndef MPX_STDLIB_H
#define MPX_STDLIB_H

/**
 @file stdlib.h
 @brief A subset of standard C library functions.
*/

/**
    @brief Convert an ASCII string to an integer
    @param s A NUL-terminated string
    @return The value of the string converted to an integer
*/
int atoi(const char *s);

/**
    @brief Converts a valid integer to a string and stores it in buffer
    @param i The integer to convert to a string
    @param buffer The destination string for the result
    @return The same string placed into buffer
*/
char* itoa(int i, char* buffer);

/**
    @brief Checks if the provided character is a digit
    @param c The character to compare to a digit
    @return An integer indicating whether or not the character is a digit. Non-zero if true, 0 if false
*/
int isdigit(char c);
/**
    @brief Converts a string to a binary-coded decimal (bcd)
    @param c The string to convert
    @return The bcd representation of the string as an integer
*/
int strtobcd(char* string);

#endif
