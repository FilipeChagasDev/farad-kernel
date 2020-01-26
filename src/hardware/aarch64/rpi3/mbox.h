//Code taken and adapted from https://github.com/LdB-ECM/Raspi3-Kernel

#ifndef MBOX_H
#define MBOX_H

#include <types.h>
#include "memorymap.h"

/* a properly aligned buffer */
extern volatile uint_t mailbox[36];

#define MBOX_REQUEST    0

/* channels */
#define MBOX_CH_POWER   0
#define MBOX_CH_FB      1
#define MBOX_CH_VUART   2
#define MBOX_CH_VCHIQ   3
#define MBOX_CH_LEDS    4
#define MBOX_CH_BTNS    5
#define MBOX_CH_TOUCH   6
#define MBOX_CH_COUNT   7
#define MBOX_CH_PROP    8

/* tags */
#define MBOX_TAG_SETPOWER       0x28001
#define MBOX_TAG_SETCLKRATE     0x38002
#define MBOX_TAG_SETVOLTAGE	0x38003
#define MBOX_TAG_LAST           0

#define MBOX_TAG_GET_ARM_MEM  0x10005
#define MBOX_TAG_GET_VC_MEM  0x10006

#define MAIL_EMPTY	0x40000000		/* Mailbox Status Register: Mailbox Empty */
#define MAIL_FULL	0x80000000	/* Mailbox Status Register: Mailbox Full  */

int mailbox_call(byte_t ch);
boolean_t mailbox_tag_write(uint_t message);
uint_t mailbox_tag_read();

#endif
