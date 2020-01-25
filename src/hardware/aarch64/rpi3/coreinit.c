#include "uart.h"
#include "multicore.h"

void main_secundary(unsigned int core);
void main();

void core0_main()
{
	*core0_ready = TRUE;
	main();
}

void core1_main()
{
	*core1_ready = TRUE;
    main_secundary(1);
}

void core2_main()
{	
	*core2_ready = TRUE;
    main_secundary(2);
	
}

void core3_main()
{
	*core3_ready = TRUE;
    main_secundary(3);
}


// ------------ Exceptions ----------------

// array to store register values (see dbg_saveregs in start.S)
unsigned long long int dbg_regs[38];

/**
 * Decode exception cause
 */
void dbg_decodeexc(unsigned long long int type)
{
	switch(type) {
		case 0: uart_puts("[Synchronous]"); break;
		case 1: uart_puts("[IRQ]"); break;
		case 2: uart_puts("[FIQ]"); break;
		case 3: uart_puts("[SError]"); break;
	}

	byte_t cause = dbg_regs[33]>>26;

	switch(cause) {
		case 0b000000: uart_puts("Unknown"); break;
		case 0b000001: uart_puts("Trapped WFI/WFE"); break;
		case 0b001110: uart_puts("Illegal execution"); break;
		case 0b010101: uart_puts("System call"); break;
		case 0b100000: uart_puts("Instruction abort, lower EL"); break;
		case 0b100001: uart_puts("Instruction abort, same EL"); break;
		case 0b100010: uart_puts("Instruction alignment fault"); break;
		case 0b100100: uart_puts("Data abort, lower EL"); break;
		case 0b100101: uart_puts("Data abort, same EL"); break;
		case 0b100110: uart_puts("Stack alignment fault"); break;
		case 0b101100: uart_puts("Floating point"); break;
		case 0b110000: uart_puts("Breakpoint, lower EL"); break;
		case 0b110001: uart_puts("Breakpoint, same EL"); break;
		case 0b111100: uart_puts("Breakpoint instruction"); break;
		default: uart_puts("Unknown"); break;
	}
	// decode data abort cause
	if(cause==0b100100 || cause==0b100101) {
		uart_puts(", ");
		switch((dbg_regs[33]>>2)&0x3) {
			case 0: uart_puts("Address size fault"); break;
			case 1: uart_puts("Translation fault"); break;
			case 2: uart_puts("Access flag fault"); break;
			case 3: uart_puts("Permission fault"); break;
		}
		switch(dbg_regs[33]&0x3) {
			case 0: uart_puts(" at level 0"); break;
			case 1: uart_puts(" at level 1"); break;
			case 2: uart_puts(" at level 2"); break;
			case 3: uart_puts(" at level 3"); break;
		}
	}

	uart_puts("\n");

	while(1);	

	/*
	// if the exception happened in the debugger, we stop to avoid infinite loop
	if(dbg_running) {
		printf("Exception in debugger!\n"
			"  elr_el1: %x  spsr_el1: %x\n  esr_el1: %x  far_el1: %x\nsctlr_el1: %x  tcr_el1: %x\n",
			dbg_regs[31],dbg_regs[32],dbg_regs[33],dbg_regs[34],dbg_regs[35],dbg_regs[36]);
		while(1);
	}*/
}

void dbg_main()
{
	uart_puts("\n_|_|_|_|_|_\n");
}

