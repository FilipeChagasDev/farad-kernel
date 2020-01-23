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

	/*uint8_t cause=dbg_regs[33]>>26;

	// print out interruption type
	switch(type) {
		case 0: printf("Synchronous"); break;
		case 1: printf("IRQ"); break;
		case 2: printf("FIQ"); break;
		case 3: printf("SError"); break;
	}
	printf(": ");
	// decode exception type (some, not all. See ARM DDI0487B_b chapter D10.2.28)
	switch(cause) {
		case 0b000000: printf("Unknown"); break;
		case 0b000001: printf("Trapped WFI/WFE"); break;
		case 0b001110: printf("Illegal execution"); break;
		case 0b010101: printf("System call"); break;
		case 0b100000: printf("Instruction abort, lower EL"); break;
		case 0b100001: printf("Instruction abort, same EL"); break;
		case 0b100010: printf("Instruction alignment fault"); break;
		case 0b100100: printf("Data abort, lower EL"); break;
		case 0b100101: printf("Data abort, same EL"); break;
		case 0b100110: printf("Stack alignment fault"); break;
		case 0b101100: printf("Floating point"); break;
		case 0b110000: printf("Breakpoint, lower EL"); break;
		case 0b110001: printf("Breakpoint, same EL"); break;
		case 0b111100: printf("Breakpoint instruction"); break;
		default: printf("Unknown %x", cause); break;
	}
	// decode data abort cause
	if(cause==0b100100 || cause==0b100101) {
		printf(", ");
		switch((dbg_regs[33]>>2)&0x3) {
			case 0: printf("Address size fault"); break;
			case 1: printf("Translation fault"); break;
			case 2: printf("Access flag fault"); break;
			case 3: printf("Permission fault"); break;
		}
		switch(dbg_regs[33]&0x3) {
			case 0: printf(" at level 0"); break;
			case 1: printf(" at level 1"); break;
			case 2: printf(" at level 2"); break;
			case 3: printf(" at level 3"); break;
		}
	}
	printf("\n");
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

}

