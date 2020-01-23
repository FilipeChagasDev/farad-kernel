/*
 * KERNEL.F SOURCE CODE
 * Author: Filipe Chagas Ferraz
 */

#ifndef kernelf_memory_physical_h
#define kernelf_memory_physical_h

#include <types.h>
#include <panic.h>

void init_physical_memory_manager();

//physical_addr_t alloc_physical_page();

//void free_physical_page(physical_addr_t addr);

uint_t physical_page_length();

ulong_t physical_pages_quantity();

ulong_t free_physical_pages_quantity();

#endif //end module