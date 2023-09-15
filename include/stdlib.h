#ifndef MPX_STDLIB_H
#define MPX_STDLIB_H
#define NULL ((void*)0)

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
    @author Samesh Desai
    @author Noah Marner
    @author Jackson Monk
    @author Blake Wagner
    @brief Converts a valid integer to a string and stores it in buffer
    @details First, this function checks if the number is negative and sets the negative flag
    according to the result. Next, if the number is 0, we set the buffer to 0, increment position,
    add a null terminator, and exit. While i != 0, the remainder of i % 10 is found. The buffer at
    the current position is then set to the remainder plus the '0' character to calculate the number character,
    then i is divided by 10 to go to the digit. After the loop, the number is in reverse order; we add a '-' character
    to the end if the number is negative. Last, reverse the string and add a null terminator.
    @param i The integer to convert to a string
    @param buffer The destination string for the result
    @return The same string placed into buffer
*/
char* itoa(int i, char* buffer);

/**
    @author Jackson Monk
    @brief Checks if the provided character is a digit
    @details This function checks if the character is equal to 0-9. If true it returns 1, if not,
    it returns 0
    @param c The character to compare to a digit
    @return An integer indicating whether or not the character is a digit. Non-zero if true, 0 if false
*/
int isdigit(char c);

/**
    @author Jackson Monk
    @brief Converts a string to a binary-coded decimal (bcd)
    @details Gets the 10s digit by dividing by 10 and the ones digit by modding by 10. We then shift
    the 10s digit four digits to the right (pad with four zeros). We then or the 10s digit and the ones digit
    together to get the final result.
    @param c The integer to convert
    @return The bcd representation of the integer
*/
int toBCD(int num);

/**
 * @author Sam Desai
 * @author Jackson Monk
 * @brief Converts a binary-coded decimal to an int
 * @details This function starts by getting the 10s place by anding the input value with
 * 0x70 (1111 0000). Then it gets the ones place by anding with 0x0F (0000 1111). It gets the final
 * value by adding these two values.
 * @param c The BCD value to convert
 * @return The integer representation of the integer
*/
int fromBCD(int bcd);

#endif
