#ifndef UART_H
#define UART_H

#include <types.h>
#include "mbox.h"
#include "gpio.h"
#include "delays.h"

void uart_init();
void uart_send(uint_t c);
char uart_getc();
void uart_puts(char *s);
void uart_hex(uint_t d);
void uart_dump(void *ptr);

#endif
