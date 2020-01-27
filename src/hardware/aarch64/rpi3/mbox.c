//Code taken and adapted from https://github.com/LdB-ECM/Raspi3-Kernel

#include "mbox.h"
#include <memory/cache.h>

/* mailbox message buffer */
volatile uint_t  __attribute__((aligned(16))) mailbox[36];

#define VIDEOCORE_MBOX  (MMIO_BASE+0x0000B880)
#define MBOX_READ       ((volatile uint_t*)(VIDEOCORE_MBOX+0x0))
#define MBOX_POLL       ((volatile uint_t*)(VIDEOCORE_MBOX+0x10))
#define MBOX_SENDER     ((volatile uint_t*)(VIDEOCORE_MBOX+0x14))
#define MBOX_STATUS     ((volatile uint_t*)(VIDEOCORE_MBOX+0x18))
#define MBOX_CONFIG     ((volatile uint_t*)(VIDEOCORE_MBOX+0x1C))
#define MBOX_WRITE      ((volatile uint_t*)(VIDEOCORE_MBOX+0x20))
#define MBOX_RESPONSE   0x80000000
#define MBOX_FULL       0x80000000
#define MBOX_EMPTY      0x40000000

struct __attribute__((__packed__, aligned(4))) mbox_registers
{
        const volatile uint_t read_0; // 0x00         Read data from VC to ARM
        uint_t unused[3];             // 0x04-0x0F
        volatile uint_t peek_0;       // 0x10
        volatile uint_t sender_0;     // 0x14
        volatile uint_t status_0;     // 0x18         Status of VC to ARM
        volatile uint_t config0;      // 0x1C
        volatile uint_t write_1;      // 0x20         Write data from ARM to VC
        uint_t unused_2[3];           // 0x24-0x2F
        volatile uint_t peek_1;       // 0x30
        volatile uint_t sender_1;     // 0x34
        volatile uint_t status_1;     // 0x38         Status of ARM to VC
        volatile uint_t config_1;     // 0x3C
};

_Static_assert((sizeof(struct mbox_registers) == 0x40), "Structure MailBoxRegisters should be 0x40 bytes in size");

#define MAILBOX_FOR_READ_WRITES ((volatile __attribute__((aligned(4))) struct mbox_registers*)(uint_t *)(MMIO_BASE + 0xB880))

/**
 * Make a mailbox call. Returns 0 on failure, non-zero on success
 */
int mailbox_call(byte_t ch)
{
	uint_t r;
	/* wait until we can write to the mailbox */
	do{asm volatile("nop");}while(*MBOX_STATUS & MBOX_FULL);
	/* write the address of our message to the mailbox with channel identifier */
	*MBOX_WRITE = (((uint_t)((ullong_t)&mailbox)&~0xF) | (ch&0xF));
	/* now wait for the response */
	while(1) {
		/* is there a response? */
		do{asm volatile("nop");}while(*MBOX_STATUS & MBOX_EMPTY);
		r=*MBOX_READ;
		/* is it a response to our message? */
		if((unsigned char)(r&0xF)==ch && (r&~0xF)==(uint_t)((ullong_t)&mailbox))
			/* is it a valid successful response? */
			return mailbox[1]==MBOX_RESPONSE;
	}

	invalidate_data_cache();

	return 0;
}

boolean_t mailbox_tag_write(uint_t message) 
{
	uint_t value;	// Temporary read value
	message &= ~(0xF); // Make sure 4 low channel bits are clear
	message |= 0x8; // OR the channel bits to the value
	do
	{
		value = MAILBOX_FOR_READ_WRITES->status_1; // Read mailbox1 status from GPU
	} 
	while ((value & MAIL_FULL) != 0); // Make sure arm mailbox is not full
	MAILBOX_FOR_READ_WRITES->write_1 = message; // Write value to mailbox

	invalidate_data_cache();

	return TRUE; // Write success
}

uint_t mailbox_tag_read () 
{
	uint_t value;	// Temporary read value
	do
	{
		do
		{
			value = MAILBOX_FOR_READ_WRITES->status_0; // Read mailbox0 status
		} 
		while ((value & MAIL_EMPTY) != 0); // Wait for data in mailbox
		value = MAILBOX_FOR_READ_WRITES->read_0; // Read the mailbox
	}
	while ((value & 0xF) != 0x8); // We have response back
	value &= ~(0xF); // Lower 4 low channel bits are not part of message

	invalidate_data_cache();
	
	return value; // Return the value
}
