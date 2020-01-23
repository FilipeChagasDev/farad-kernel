#ifndef MULTICORE_H
#define MULTICORE_H

#include <types.h>

extern volatile boolean_t* core0_ready;
extern volatile boolean_t* core1_ready;
extern volatile boolean_t* core2_ready;
extern volatile boolean_t* core3_ready;


void start_other_3_cores();

#endif
