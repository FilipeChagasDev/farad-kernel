//Code taken and adapted from https://github.com/LdB-ECM/Raspi3-Kernel

#ifndef DELAYS_H
#define DELAYS

#include <types.h>
#include "memorymap.h"

void wait_cycles(uint_t number_of_cycles_to_wait);
void wait_usec(uint_t n);
ullong_t get_system_timer();
void wait_usec_st(uint_t n);
ullong_t micros();

#endif
