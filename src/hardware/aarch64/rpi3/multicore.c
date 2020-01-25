//Code taken and adapted from https://github.com/LdB-ECM/Raspi3-Kernel

#include "multicore.h"
#include "delays.h"

boolean_t core_state[4] = { 0 };

volatile boolean_t* core0_ready = &core_state[0];
volatile boolean_t* core1_ready = &core_state[1];
volatile boolean_t* core2_ready = &core_state[2];
volatile boolean_t* core3_ready = &core_state[3];


void start_other_3_cores()
{
	asm("mov	x1, #0xe0\n"\
		"mov    x2, #0x80000\n"\
		"str    x2, [x1]\n"\
		"sev\n");
	wait_usec(100000);
	while (*core1_ready == FALSE) {};
	asm("mov	x1, #0xe8\n"\
		"mov    x2, #0x80000\n"\
		"str	x2, [x1]\n"\
		"sev\n");
	wait_usec(100000);
	while (*core2_ready == FALSE) {};
	asm("mov	x1, 0xf0\n"\
		"mov    x2, #0x80000\n"\
		"str	x2, [x1]\n"\
		"sev");
	wait_usec(100000);
	while (*core3_ready == FALSE) {};
}

/*
int core_execute(uint8_t core, void (*func)())
{
	if(core == 1)
	{
		unsigned long *a = 0xe0;	//Load core 1 hibernation address into char *a
		*a = func;	//Load the function address into the hibernation address
		asm("sev");	//Wake all cores. Cores with an address will start execution there
		return 1; //Return 0x1 for success on core 1
 	}
   else if(core == 2)
 	{
 		unsigned long *b = 0xe8;	//Load core 2 hibernation address into char *b	
 		*b = func;	//Load the function address into the hibernation address
 		asm("sev");	//Wake all cores. Cores with an address will start execution there
 		return 2; //Return 0x2 for success on core 2
 	}
 	else if(core == 3)
   {
 		unsigned long *c = 0xf0; //Load core 3 hibernation address into char *c
 		*c = func;	//Load the address of the target function into *c
 		asm("sev");	//Wake all cores. Cores with an address in their hibernation address will jump to that address
 		return 3; //Return 0x3 in the event that core 3 jumped to that address and is executing
 	}
 	return 0; //Return NULL in the event that a core other than 1, 2, or 3 was called to execute code
}
*/  
