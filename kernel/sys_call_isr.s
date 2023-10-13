bits 32
global sys_call_isr

;;; System call interrupt handler. To be implemented in Module R3.
extern sys_call			; The C function that sys_call_isr will call
sys_call_isr:
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
	call sys_call
	mov esp, eax
	pop gs
	pop fs
	pop es
	pop ds
	pop ss
	pop edi
	pop esi
	pop esp
	pop ebp
	pop ebx
	pop edx
	pop ecx
	pop eax
	iret
