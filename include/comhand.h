#ifndef MPX_CTYPE_H
#define MPX_CTYPE_H

#define VERSION 1
/**
 * @file comhand.h
 * @brief A set of functions that allow users to interact with the OS
*/


/**
 * @author Sam Desai
 * @author Jackson Monk
 * @brief calls all of the different functions that a user would use within the OS
 * 
 */
void comhand(void);

/**
 * @author Sam Desai
 * @author Jackson Monk
 * @brief Prints the menu that has different functions the user can choose from to use
 * 
 */
void printMenu();

/**
 * @author Sam Desai
 * @author Jackson Monk
 * @brief allows the user to shutdown the OS, but requires confirmation
 * @return int - returns a 1 if the user confirmed shutdown or a 0 if the user denied shutdown
 */
int shutdown();

/**
 * @author Sam Desai
 * @author Jackson Monk
 * @brief Shows the user which version of the OS they are using
 */
void version(void);

/**
 * @author Sam Desai
 * @author Jackson Monk
 * @brief Gets the current time set within the OS
 * @details Attaches the index of the time register (mins, seconds, hours, etc) to the index
 * register of the RTC. Then, using inb to read from the RTC data port (0x71), puts the value
 * in an integer and formats the value to decimal using the fromBCD function. This is done for
 * hours, minutes, and seconds. Finally, checks for single digits in minutes or seconds and 
 * adds padding zeros, then prints out the value.
 */
void getTime(void);

/**
 * @author Sam Desai
 * @author Jackson Monk
 * @author Noah Marner
 * @brief Allows the user to set the time of the OS, but doesn't allow them to input an invalid time
 * @details Starts by validating the minutes and seconds by checking if they are over 59 and adds padding
 * to single digit hour field. Then checks if all inputs are digits. If any validating reveals errors,
 * sets a flag to 1 to exit the function. Disables interrupts, converts the inputs into BCD format by calling
 * the toBCD function and attaches the index of the time register (mins, seconds, hours, etc) to the index
 * register of the RTC. Lastly, uses outb to write to the BCD values to the data port, prints out the time if
 * successful, and reenables interrupts.
 */
void setTime(void);

/**
 * @author Sam Desai
 * @author Jackson Monk
 * @brief Gets the current date set within the OS
 * @details Attaches the index of the time register (mins, seconds, hours, etc) to the index
 * register of the RTC. Then, using inb to read from the RTC data port (0x71), puts the value
 * in an integer and formats the value to decimal using the fromBCD function. This is done for
 * days, month, and year. Finally, checks for single digits in days or month and 
 * adds padding zeros, then prints out the value.
 */
void getDate(void);

/**
 * @author Sam Desai
 * @author Jackson Monk
 * @brief Allows the user to set the date within the OS, but does not allow any invalid dates
 * @details Starts by validating the input with numerous tests. Disables interrupts, converts the inputs into 
 * BCD format by calling the toBCD function and attaches the index of the time register (days, minutes, years, etc) 
 * to the index register of the RTC. Lastly, uses outb to write to the BCD values to the data port, 
 * prints out the time if successful, and reenables interrupts.
 */
void setDate(void);

/**
 * @author Sam Desai
 * @author Jackson Monk
 * @brief Displays a list of all of the different functions and what they are made to do
 * @details Prints a list of commands with explanations. This function will also take one input and
 * give specific details for that input.
 */
void help(void);

#endif

