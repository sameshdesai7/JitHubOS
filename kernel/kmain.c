#include <mpx/gdt.h>
#include <mpx/interrupts.h>
#include <mpx/serial.h>
#include <mpx/vm.h>
#include <sys_req.h>
#include <string.h>
#include <stdio.h>
#include <memory.h>
#include <../include/comhand.h>
#include <stdlib.h>
#include <mpx/io.h>
#include <dataStructs.h>

// typedef struct pcb {
//         char *name_ptr;
// } pcb;

static void klogv(device dev, const char *msg)
{
	char prefix[] = "klogv: ";
	serial_out(dev, prefix, strlen(prefix));
	serial_out(dev, msg, strlen(msg));
	serial_out(dev, "\r\n", 2);
}

void kmain(void)
{
	// 0) Serial I/O -- <mpx/serial.h>
	// If we don't initialize the serial port, we have no way of
	// performing I/O. So we need to do that before anything else so we
	// can at least get some output on the screen.
	// Note that here, you should call the function *before* the output
	// via klogv(), or the message won't print. In all other cases, the
	// output should come first as it describes what is about to happen.
	serial_init(COM1);
	klogv(COM1, "Initialized serial I/O on COM1 device...");

	// 1) Global Descriptor Table (GDT) -- <mpx/gdt.h>
	// Keeps track of the various memory segments (Code, Data, Stack, etc.)
	// required by the x86 architecture. This needs to be initialized before
	// interrupts can be configured.
	gdt_init();
	klogv(COM1, "Initializing Global Descriptor Table...");

	// 2) Interrupt Descriptor Table (IDT) -- <mpx/interrupts.h>
	// Keeps track of where the various Interrupt Vectors are stored. It
	// needs to be initialized before Interrupt Service Routines (ISRs) can
	// be installed.
	idt_init();
	klogv(COM1, "Initializing Interrupt Descriptor Table...");

	// 3) Disable Interrupts -- <mpx/interrupts.h>
	// You'll be modifying how interrupts work, so disable them to avoid
	// crashing.
	cli();
	klogv(COM1, "Disabling interrupts...");

	// 4) Interrupt Request (IRQ) -- <mpx/interrupts.h>
	// The x86 architecture requires ISRs for at least the first 32
	// Interrupt Request (IRQ) lines.
	irq_init();
	klogv(COM1, "Initializing Interrupt Request routines...");

	// 5) Programmable Interrupt Controller (PIC) -- <mpx/interrupts.h>
	// The x86 architecture uses a Programmable Interrupt Controller (PIC)
	// to map hardware interrupts to software interrupts that the CPU can
	// then handle via the IDT and its list of ISRs.
	pic_init();
	klogv(COM1, "Initializing Programmable Interrupt Controller...");

	// 6) Reenable interrupts -- <mpx/interrupts.h>
	// Now that interrupt routines are set up, allow interrupts to happen
	// again.
	sti();
	klogv(COM1, "Enabling Interrupts...");

	// 7) Virtual Memory (VM) -- <mpx/vm.h>
	// Virtual Memory (VM) allows the CPU to map logical addresses used by
	// programs to physical address in RAM. This allows each process to
	// behave as though it has exclusive access to memory. It also allows,
	// in more advanced systems, the kernel to swap programs between RAM and
	// storage (such as a hard drive or SSD), and to set permissions such as
	// Read, Write, or Execute for pages of memory. VM is managed through
	// Page Tables, data structures that describe the logical-to-physical
	// mapping as well as manage permissions and other metadata.
	vm_init();
	klogv(COM1, "Initializing Virtual Memory...");

	// 8) MPX Modules -- *headers vary*
	// Module specific initialization -- not all modules require this.
	klogv(COM1, "Initializing MPX modules...");
	// R5: sys_set_heap_functions(...);
	// R4: create commhand and idle processes

	// 9) YOUR command handler -- *create and #include an appropriate .h file*
	// Pass execution to your command handler so the user can interact with
	// the system.
	
	// Adjusting time back 4 hours to account for system time
	// This technically should adjust the month back if hours < 4 and day is = 1, and then year...
	outb(0x70, 0x04);
    int hours = inb(0x71);
    int formattedHours = fromBCD(hours);
	int adjustedHours;
	if(formattedHours > 4)
		adjustedHours = formattedHours - 4;
	else {
		adjustedHours = 24 - (4 - formattedHours);
		outb(0x70, 0x07);
		int day = inb(0x71);
		int formattedDay = fromBCD(day);
		if(formattedDay > 1){
			int adjustedDay = formattedDay - 1;
			outb(0x70, 0x07);
        	outb(0x71, toBCD(adjustedDay));
		}
	}
	outb(0x70, 0x04);
    outb(0x71, toBCD(adjustedHours));

	pcb* pcb1 = pcb_setup("priority1.1", 0, 1);
	pcb* pcb2 = pcb_setup("priority0.1", 0, 0);
	pcb* pcb3 = pcb_setup("priority2.1", 0, 2);
	pcb* pcb4 = pcb_setup("priority3.2", 0, 3);
	pcb* pcb5 = pcb_setup("priority2.2", 0, 2);
	pcb* pcb6 = pcb_setup("priority1.2", 0, 1);
	pcb* pcb7 = pcb_setup("priority1.3", 0, 1);

	queue* q = sys_alloc_mem(sizeof(queue));

	enqueue(q, pcb1);
	enqueue(q, pcb2);
	enqueue(q, pcb3);
	enqueue(q, pcb4);
	enqueue(q, pcb5);
	enqueue(q, pcb6);
	enqueue(q, pcb7);
	printq(q);

	klogv(COM1, "Transferring control to commhand...");
	comhand();
	// R4: __asm__ volatile ("int $0x60" :: "a"(IDLE));

	// 10) System Shutdown -- *headers to be determined by your design*
	// After your command handler returns, take care of any clean up that
	// is necessary.
	klogv(COM1, "Starting system shutdown procedure...");

	// 11) Halt CPU -- *no headers necessary, no changes necessary*
	// Execution of kmain() will complete and return to where it was called
	// in boot.s, which will then attempt to power off Qemu or halt the CPU.
	klogv(COM1, "Halting CPU...");
}
