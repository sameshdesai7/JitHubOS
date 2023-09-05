#ifndef MPX_CTYPE_H
#define MPX_CTYPE_H

#define VERSION 1
/**
 * @file comhand.h
 * @brief A set of functions that allow users to interact with the OS
*/


/**
 * @brief calls all of the different functions that a user would use within the OS
 * 
 */
void comhand(void);
/**
 * @brief Prints the menu that has different functions the user can choose from to use
 * 
 */
void printMenu();
/**
 * @brief allows the user to shutdown the OS, but double checks to see if the user really wants to
 * 
 * @return int - returns a 1 if the user confirmed shutdown or a 0 if the user denied shutdown
 */
int shutdown();
/**
 * @brief Shows the user which version of the OS they are using
 * 
 */
void version(void);
/**
 * @brief Gets the current time set within the OS
 * 
 */
void getTime(void);
/**
 * @brief Allows the user to set the time of the OS, but doesn't allow them to input an invalid time
 * 
 */
void setTime(void);
/**
 * @brief Gets the current date set within the OS
 * 
 */
void getDate(void);
/**
 * @brief Allows the user to set the date within the OS, but does not allow any invalid dates
 * 
 */
void setDate(void);
/**
 * @brief Displays a list of all of the different functions and what they are made to do
 * 
 */
void help(void);

#endif

