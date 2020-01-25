/*
 * KERNEL.F SOURCE CODE
 * Author: Filipe Chagas Ferraz
 */

#ifndef kernelf_panic_h
#define kernelf_panic_h

#include <types.h>
#include <hardware/aarch64/rpi3/uart.h>

void panic_log(char *title, char *description);

#endif