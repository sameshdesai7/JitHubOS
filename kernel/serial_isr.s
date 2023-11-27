bits 32
global serial_isr

extern serial_interrupt			
serial_isr:
	push eax
	push ecx
	push edx
	push ebx
	push ebp
	push esp
	push esi
	push edi
	push ss
	push ds
	push es
	push fs
	push gs
	push esp
	call serial_interrupt
	mov esp, eax
	pop gs
	pop fs
	pop es
	pop ds
	pop ss
	pop edi
	pop esi
	add esp, 4
	pop ebp
	pop ebx
	pop edx
	pop ecx
	pop eax
	iret