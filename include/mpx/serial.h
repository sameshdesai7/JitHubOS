#ifndef MPX_SERIAL_H
#define MPX_SERIAL_H

#include <stddef.h>
#include <mpx/device.h>
#include <dataStructs.h>

/**
 @file mpx/serial.h
 @brief Kernel functions and constants for handling serial I/O
*/

/**
 Initializes devices for user input and output
 @param device A serial port to initialize (COM1, COM2, COM3, or COM4)
 @return 0 on success, non-zero on failure
*/
int serial_init(device dev);

/**
 Writes a buffer to a serial port
 @param device The serial port to output to
 @param buffer A pointer to an array of characters to output
 @param len The number of bytes to write
 @return The number of bytes written
*/
int serial_out(device dev, const char *buffer, size_t len);

/**
 @author Noah Marner
 @author Blake Wagner
 @brief Reads a string from a serial port
 
 @details This function is used to read in data from the console. It reads characters until either
 the length limit is reached or an enter key is read in. Special characters such as backspace, delete
 and arrow keys are also handled in this function.
 @param device The serial port to read data from
 @param buffer A buffer to write data into as it is read from the serial port
 @param count The maximum number of bytes to read
 @return The number of bytes read on success, a negative number on failure
*/   		   
int serial_poll(device dev, char *buffer, size_t len);

/**
 * @author Noah Marner
 * @author Blake Wagner
 * @brief Helper method for serial_poll that does the work of a backspace
 * 
 * @details This function is used when a backspace or delete character is input. If there is a backspace,
 * the function is simply called. When a delete character is input, the position is set 
 * forward one and then the function is called.
 * @param pos The current position in the buffer
 * @param end The farthest position that has been reached in the buffer
 * @param buffer A buffer that stores the current string
 * @param dev The serial port to read data from
*/
void backspace(int *pos, int* end, char* buffer, device dev);

int serial_open(device dev, int speed);

int serial_close(device dev);

int serial_read(device dev, char* buf, size_t len);

int serial_write(device dev, char* buf, size_t len);

void serial_input_interrupt(struct dcb *dcb);

void serial_output_interrupt(struct dcb *dcb);

#endif
