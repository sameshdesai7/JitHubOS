#include <mpx/io.h>
#include <mpx/serial.h>
#include <sys_req.h>
#include <stdio.h>
#include <dataStructs.h>
#include <mpx/interrupts.h>

extern dcb* com1DCB;
extern dcb* com2DCB;
extern dcb* com3DCB;
extern dcb* com4DCB;

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

			else if(c == 127){ 	//backspace character
				if (pos > 0) {
						backspace(&pos, &end, buffer, dev);
	
				}
			}

			else if(c == 126){
				if(pos < end){
					outb(dev,buffer[pos++]);
					backspace(&pos, &end, buffer, dev);
				}
			}

			else if(c == 27 ){
				for(int i =0; i < 100000; i++);
				if (inb(dev + LSR) & 1){
					char c2 = inb(dev);
					if (c2 == 91){ 	//checks to see if an arrow key was hit
						for(int i =0; i < 100000; i++);
						if (inb(dev + LSR) & 1){
							char c3 = inb(dev);
							if (c3 == 68){ 	//left arrow key
								if (pos > 0){
									 pos--;
									outb(dev, '\b');
								}
							}
							if (c3 == 67 && pos < end){  //right arrow key
								outb(dev, buffer[pos++]);
							}
						}
					}
				}
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
	return (int)pos;
}

void backspace(int *pos, int* end, char* buffer, device dev){
	if (*pos > 0){
		if (*pos < *end){
			int posTemp = *pos;

				while(buffer[*pos]){
					outb(dev, '\b');
					outb(dev, buffer[*pos]);
					outb(dev, ' ');
					buffer[*pos - 1] = buffer[*pos];
					*pos = *pos + 1;
				}
			buffer[*pos - 1] = '\0';
			*end = *end - 1;
			*pos = posTemp - 1;
					
			for(int i = *end; i > *pos - 1; i--) 
				outb(dev, '\b');

		}
		else {
			buffer[--*pos] = '\0';
			*end = *end - 1;
			outb(dev, '\b');
			outb(dev, ' ');
			outb(dev, '\b');
		}
	}
	return;	
}

int serial_open(device dev, int baudRate) {
	serial_init(dev);
	if(dev == COM1){
		if(com1DCB->eFlag == 1){
			//0 for open 1 for closed
			com1DCB->eFlag = 0;
			//0 for idle 1 for in use
			com1DCB->status = 0; 
			//Needs vector and pointer to function to call
			idt_install(0x24, (int)serial_isr);

			//Compute baud rate divisor
			int baudRateDiv = 115200 / (long)baudRate; //find baud rate
    		int remainder = 115200 % (long)baudRate;

			//disabel interupts
			outb(dev + IER, 0x00);

			//store 0x80 in line control register
			outb(dev + LCR, 0x80);

			//store baud rate div high bits in MSB
			outb(dev + DLL, (int)baudRateDiv);
			outb(dev + DLM, remainder);

			//store 0x03 in line control register
			outb(dev + LCR, 0x03);

			//set pic mask register ????????????
			cli();
			int mask = inb(0x21);
			mask &= (0b11101111);
			outb(0x21, mask);
			sti();
			
			//enable overall serial port interrupts
			outb(dev + MCR, 0x08);

			//Enable input ready interrupts 
			outb(dev + IER, 0x01);

		}
	} else if(dev == COM2){

	} else if(dev == COM3){

	} else if(dev ==COM4){

	}


	return 0;

}

int serial_close(device dev) {
	
	if(dev == COM1){
		if(com1DCB -> eFlag == 0){
			com1DCB -> eFlag = 1;
		}

		//Disable pic
		cli();
		int mask = inb(0x21);
		mask |= (0b00010000);
		outb(0x21, mask);
		sti();

		//Disble interupts
		outb(dev + MSR, 0x00);
		outb(dev + IER, 0x00);

	} else if(dev == COM2){

	} else if(dev == COM3){

	} else if(dev ==COM4){

	}

	return 0;


}

int serial_read(device dev, char* buf, size_t len){

	if(dev == COM1){
		if(com1DCB == NULL){
			return 301;
		}
		if(buf == NULL){
			return 302;
		}
		// CHECK FOR INVALID COUNT ADDRESS OR COUNT VALUE?? PAGE 14/15 OF DOCUMENT
		if(com1DCB -> status == 1){
			return 304;
		}
		com1DCB -> size = len;
		//intialIze input buffer var??
		com1DCB -> eFlag = 0;

		char* tempBuf = buf;
		int currentSize = 0;
		int ringBufferSize = 0;
		cli();
		//Copy contents of ring buffer to requestors buffer
		while(com1DCB->beginning != com1DCB->end){

			if(com1DCB->ringBuffer[ringBufferSize] == '\n' || com1DCB->ringBuffer[ringBufferSize] == '\r'){
				tempBuf++;
				currentSize++;
				ringBufferSize++;
				com1DCB->eFlag = 1;
				break;
			}
			if(currentSize == len){
				com1DCB->eFlag = 1;
				break;
			}
			if (ringBufferSize == com1DCB->size) {
				ringBufferSize %= com1DCB->size;
			}
			*tempBuf = com1DCB->ringBuffer[ringBufferSize];
			tempBuf++;
			ringBufferSize++;
			currentSize++;

		}
		sti();

	}

}

int serial_write(device dev, char* buf, size_t len){
	
	if(dev == COM1){
		if(com1DCB == NULL){
			return 401;
		}
		if(buf == NULL){
			return 402;
		}
		// CHECK FOR INVALID COUNT ADDRESS OR COUNT VALUE?? PAGE 14/15 OF DOCUMENT
		if(com1DCB -> status == 1){
			return 404;
		}

	// INSTALL BUFFER POINTER AND COUNTERS
	com1DCB->status = 1;
	com1DCB->beginning = buf;
	com1DCB->end = buf[len-1];

	com1DCB->numTransferred = len;
	com1DCB->eFlag = 0;
	outb(COM1, *com1DCB->beginning)

	int mask = inb(dev + IER);
	mask |= (0b00000010);
	outb(dev + IER, mask);

	}

}

