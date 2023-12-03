#include <mpx/io.h>
#include <mpx/serial.h>
#include <sys_req.h>
#include <stdio.h>
#include <dataStructs.h>
#include <mpx/interrupts.h>

extern dcb* com1DCB;
extern void serial_isr(void*);

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
		if(com1DCB->status == 1){
			//0 for open 1 for closed
			com1DCB -> status = 0;
			com1DCB -> op = IDLE;
			com1DCB -> eFlag = 0; 
			com1DCB -> ringCount = 0; 
			com1DCB -> inIndex = 0;
			com1DCB -> outIndex = 0;

			//Needs vector and pointer to function to call
			idt_install(0x24, serial_isr);

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
			mask &= (0xEF);
			outb(0x21, mask);
			sti();
			
			//enable overall serial port interrupts
			outb(dev + MCR, 0x08);

			//Enable input ready interrupts 
			outb(dev + IER, 0x01);

		}
	} 
	return 0;

}

int serial_close(device dev) {
	
	if(dev == COM1){
		if(com1DCB -> status == 1){
			return 201;
		}
		com1DCB -> status = 1;

		//Disable pic
		cli();
		int mask = inb(0x21);
		mask |= (0x10);
		outb(0x21, mask);
		sti();

		//Disble interupts
		outb(dev + MSR, 0x00);
		outb(dev + IER, 0x00);
	}

	return 0;


}

int serial_read(device dev, char* buf, size_t len){

	if(dev == COM1){
		if(com1DCB -> status == 1){
			return 301;
		}
		if(buf == NULL){
			return 302;
		}
		// CHECK FOR INVALID COUNT ADDRESS OR COUNT VALUE?? PAGE 14/15 OF DOCUMENT
		if(com1DCB -> op != IDLE){
			return 304;
		}

		com1DCB -> count = 0;
		com1DCB -> op = READ;
		com1DCB -> eFlag = 0;
		cli();
		//Copy contents of ring buffer to requestors buffer
		while(com1DCB-> inIndex != com1DCB-> outIndex){

			if(com1DCB->ringBuffer[com1DCB->outIndex] == '\n' || com1DCB->ringBuffer[com1DCB->outIndex] == '\r'){
				com1DCB->count++;
				com1DCB->outIndex++;
				com1DCB->outIndex %= sizeof(com1DCB -> ringBuffer);
				com1DCB->ringCount++;
				break;
			}
			if(com1DCB->ringCount == len){
				break;
			}
			com1DCB->count++;
			*buf = com1DCB ->ringBuffer[com1DCB->outIndex];
			com1DCB->outIndex++;
			com1DCB->outIndex %= sizeof(com1DCB -> ringBuffer);
			com1DCB->ringCount++;
			buf++;
		}
		com1DCB -> op = IDLE;
		com1DCB -> eFlag = 1;
		sti();

		return 0;
	}

	return 0;

}

int serial_write(device dev, char* buf, size_t len){
	
	if(dev == COM1){
		if(com1DCB  -> status == 1){
			return 401;
		}
		if(buf == NULL){
			return 402;
		}
		// CHECK FOR INVALID COUNT ADDRESS OR COUNT VALUE?? PAGE 14/15 OF DOCUMENT
		// if(com1DCB -> op != IDLE){
		// 	return 404;
		// }
		com1DCB -> op = WRITE;
		com1DCB -> buffer = buf;
		com1DCB -> count = len;
		com1DCB->eFlag = 1;
		outb(COM1, *com1DCB-> buffer);

		int mask = inb(dev + IER);
		mask |= (0x02);
		outb(dev + IER, mask);

	}

	return 0;

}


void serial_interrupt(void){
	cli();
		int mask = inb(COM1 + IIR);
		if ((mask & 0x01) == 0x01){
			//add return code
			sti();
			return; 
		}

		else {
			if (mask == 0x00){
				//ask nate
				//inb(LSR, )
			}
			else if(mask == 0x02){
				serial_output_interrupt(com1DCB);
			}
			else if(mask == 0x04){
				serial_input_interrupt(com1DCB);
			}
			else if(mask == 0x06){
				//also ask nate
			}
			outb(0x21, 0x20);
		}
	}

void serial_input_interrupt(struct dcb *dcb){

	char character = inb(COM1);
	if (dcb -> op != READ){


		if(dcb->ringCount > sizeof(dcb->ringBuffer)){
			dcb -> ringBuffer[dcb->ringCount] = character;

		}

		return;

	}
	*(dcb->buffer + dcb->count) = character;
	dcb->count++;

	if(dcb->count < dcb->buffer_len && character != '\n' && character != '\r'){
		return;
	}

	dcb->op = IDLE;
	dcb->eFlag = 1;

	//return dcb->count;

}



void serial_output_interrupt(struct dcb *dcb){
	if (dcb -> op != WRITE){
		return;
	}
	else{
		if (com1DCB->count != 0 ){

			outb(COM1,*com1DCB -> buffer);
			com1DCB->buffer++;
			com1DCB->count--;

			return;

		}
		else{
			com1DCB ->op = IDLE;
			com1DCB->eFlag = 1;

			int mask = inb(COM1 + IER);
			mask |= (0x00);
			outb(COM1 + IER, mask);

			//return com1DCB -> count;
		}
	}
}


