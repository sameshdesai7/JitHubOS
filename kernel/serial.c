#include <mpx/io.h>
#include <mpx/serial.h>
#include <sys_req.h>
#include <stdio.h>

enum uart_registers
{
	RBR = 0, // Receive Buffer
	THR = 0, // Transmitter Holding
	DLL = 0, // Divisor Latch LSB
	IER = 1, // Interrupt Enable
	DLM = 1, // Divisor Latch MSB
	IIR = 2, // Interrupt Identification
	FCR = 2, // FIFO Control
	LCR = 3, // Line Control
	MCR = 4, // Modem Control
	LSR = 5, // Line Status
	MSR = 6, // Modem Status
	SCR = 7, // Scratch
};

static int initialized[4] = {0};

static int serial_devno(device dev)
{
	switch (dev)
	{
	case COM1:
		return 0;
	case COM2:
		return 1;
	case COM3:
		return 2;
	case COM4:
		return 3;
	}
	return -1;
}

int serial_init(device dev)
{
	int dno = serial_devno(dev);
	if (dno == -1)
	{
		return -1;
	}
	outb(dev + IER, 0x00);			// disable interrupts
	outb(dev + LCR, 0x80);			// set line control register
	outb(dev + DLL, 115200 / 9600); // set bsd least sig bit
	outb(dev + DLM, 0x00);			// brd most significant bit
	outb(dev + LCR, 0x03);			// lock divisor; 8bits, no parity, one stop
	outb(dev + FCR, 0xC7);			// enable fifo, clear, 14byte threshold
	outb(dev + MCR, 0x0B);			// enable interrupts, rts/dsr set
	(void)inb(dev);					// read bit to reset port
	initialized[dno] = 1;
	return 0;
}

int serial_out(device dev, const char *buffer, size_t len)
{
	int dno = serial_devno(dev);
	if (dno == -1 || initialized[dno] == 0)
	{
		return -1;
	}
	for (size_t i = 0; i < len; i++)
	{
		outb(dev, buffer[i]);
	}
	return (int)len;
}

int serial_poll(device dev, char *buffer, size_t len)
{
	int pos = 0;
	int end = 0;
	while (pos < (int)len - 1)
	{
		if (inb(dev + LSR) & 1)
		{
			char c = inb(dev); // Reads in one byte
			if (c == 13)
			{ // If character is return, add terminator character to buffer
				outb(dev, 10);
				break;
			}
			else if(c == 127){ 		//backspace character 
				if (pos > 0) {
					if (pos < end){
						int posTemp = pos;

						while(buffer[pos]){
							outb(dev, '\b');
							outb(dev, buffer[pos]);
							outb(dev, ' ');
							buffer[pos - 1] = buffer[pos];
							pos++;
						}
						buffer[pos - 1] = '\0';
						end--;
						pos = posTemp - 1;
						
						for(int i = end; i > pos - 1; i--) 
							outb(dev, '\b');
					}

				else {
				buffer[--pos] = '\0';
				end--;
				outb(dev, '\b');
				outb(dev, ' ');
				outb(dev, '\b');
				}
	
				}
			}

			else if(c == 27 ){
				if (inb(dev + LSR) & 1){
					char c2 = inb(dev);
					if (c2 == 91){ //checks to see if an arrow key was hit
						if (inb(dev + LSR) & 1){
							char c3 = inb(dev);
							if (c3 == 68){ //left arrow key
								if (pos > 0){
									 pos--;
									outb(dev, '\b');
								}
							}
							if (c3 == 67 && pos < end){ //right arrow key
								outb(dev, buffer[pos++]);
							}
						}
					}
				}
				//how to implement the esc key?
			}
			


			else
			{
				if (pos < end){
					int posTemp = pos;
					char temp1 = buffer[pos];
					char temp2 = buffer[pos + 1];
					buffer[pos++] = c;
					outb(dev, c);
					while(temp2){
						buffer[pos++] = temp1;
						outb(dev, temp1);
						temp1 = temp2;
						temp2 = buffer[pos];
					}
					buffer[pos++] = temp1;
					outb(dev, temp1);
					end++;
					pos = ++posTemp;
				for(int i = end; i > pos; i--) 
					outb(dev, '\b');
				}
				else{
					buffer[pos++] = c;
					end++;
					outb(dev, c);
				}
				
			}
		}
	}
	buffer[end] = '\0';
	// insert your code to gather keyboard input via the technique of polling.
	// You must validate each key and handle special keys such as delete, back space, and
	// arrow keys
	return (int)pos;
}
