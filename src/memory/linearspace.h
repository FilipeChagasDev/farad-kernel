/*
 * KERNEL.F SOURCE CODE
 * Author: Filipe Chagas Ferraz
 */

#ifndef kernelf_memory_linearspace_h
#define kernelf_memory_linearspace_h

#include <types.h>
#include <memory/physical.h>
#include <memory/virtual.h>
#include <panic.h>

extern boolean_t linear_space_enabled;
extern pagetable_addr_t linear_pagetable;

void init_linear_space();
void enable_linear_space();
void enter_linear_space();
void leave_linear_space();

void linear_space_drop();

#endif //end module