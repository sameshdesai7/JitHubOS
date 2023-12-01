bits 32
global serial_isr

extern serial_interrupt		
extern cli
extern sti	
serial_isr:
	cli
	call serial_interrupt
	sti
	iret
	
	