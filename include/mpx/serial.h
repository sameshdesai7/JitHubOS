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

/**
 * @author Noah Marner
 * @author Blake Wagner
 * @author Sam Desai
 * @author Jackson Monk
 * @brief Initializes serial port to allow I/O
 * 
 * @details Initializes the DCB by setting all the struct members and installs the custom interrupt handler to the
 * interrupt vector table. Computes baud rate divisor and stores in LCR. Enables interrupts in the PIC mask register,
 * enables serial port interrupts, enables input interrupts.
 * @param dev Device to open
 * @param speed Used to calculate baud rate of device
 * @return int to signal success
*/
int serial_open(device dev, int speed);

/**
 * @author Noah Marner
 * @author Blake Wagner
 * @author Sam Desai
 * @author Jackson Monk
 * @brief Closes associated serial port
 * 
 * @details Disables the COM1 level in the PIC register and disables interrupts by loading 0s into the
 * MSR and IER.
 * @param dev Device to close
 * @return Int to test success
*/
int serial_close(device dev);

/**
 * @author Noah Marner
 * @author Blake Wagner
 * @author Sam Desai
 * @author Jackson Monk
 * @brief Starts the read process
 * 
 * @details Initializes DCB count, sets op code to READ and eFlag to INCOMPLETE to signal incomplete IO.
 * Copies contents of ring buffer to requesters buffer, if the character is a new line or a return carriage,
 * or if the ring buffer is filled, returns. Enables global interrupts with sti.
 * @param dev Device to read from
 * @param buf The buffer to fill
 * @param len The number of characters to read
 * @return Int to test success
*/  
int serial_read(device dev, char* buf, size_t len);

/**
 * @author Noah Marner
 * @author Blake Wagner
 * @author Sam Desai
 * @author Jackson Monk
 * @brief Starts the write process
 * 
 * @details Verifies parameters, sets DCB op code to WRITE, buffer to the requesters buffer, count to input length
 * and eFlag to INCOMPLETE to indiciate IO incomplete. Writes the first character, enables interrupts to handle the
 * rest of the output.
 * @param dev Device to write to
 * @param buf Buffer to write from
 * @param len Amount of characters to write
 * @return Int to test success
*/
int serial_write(device dev, char* buf, size_t len);

/**
 * @author Noah Marner
 * @author Blake Wagner
 * @author Sam Desai
 * @author Jackson Monk
 * @brief First-level interrupt handler to determine which second-level interrupt handler to deploy
 * 
 * @details First-level interrupt handler to determine which second-level interrupt handler to deploy.
 * Checks if the status is closed. If so, returns. Otherwise, uses the IIR to determine whether the interrupt
 * is an input or output interrupt. Calls the correct handler.
*/
void serial_interrupt(void);

/**
 * @author Noah Marner
 * @author Blake Wagner
 * @author Sam Desai
 * @author Jackson Monk
 * @brief Interrupt to handle input from device
 * 
 * @details Gets a character from COM1 using INB. If the DCB operation is not read (device is writing),
 * add characters to the DCB ring buffer and return. If the character is a return carriage, outputs a new line
 * and sets the appropriate character in the DCB buffer to a null terminator. If the character is not a backspace
 * or carriage return, sets the buffer to the character and outputs the character. If the count has not been exceeded
 * and we are not writing a new line or return carriage, if the character is a backspace, backspace and remove the last character
 * from the buffer. Set the DCB OP to IDLE and the eFlag to COMPLETE.
*/
void serial_input_interrupt(struct dcb *dcb);

/**
 * @author Noah Marner
 * @author Blake Wagner
 * @author Sam Desai
 * @author Jackson Monk
 * @brief Interrupt to handle output from device
 * 
 * @details Creates an IOCB pointer to look at the DCB's IOCBQ's head. If the OP is not WRITE, return.
 * If there is something to write, increment buffer since the first character was printed by serial_write.
 * OUTB the current character, decrement the buffer size variable, and return. Otherwise, there is nothing
 * left to write. Set the DCB OP to IDLE, the eFlag to COMPLETE, and disable output interrupts.
*/
void serial_output_interrupt(struct dcb *dcb);

#endif
