/*
 * KERNEL.F SOURCE CODE
 * Author: Filipe Chagas Ferraz
 */

#ifndef kernelf_memory_virtual_h
#define kernelf_memory_virtual_h

#include <types.h>
#include "physical.h"
#include <panic.h>

typedef physical_addr_t pagetable_addr_t;

typedef enum {PA_KERNEL, PA_USER_READ_ONLY, PA_USER_READ_WRITE} page_access_enum_t;

ulong_t pagetable_length();

pagetable_addr_t create_pagetable();

void delete_pagetable(pagetable_addr_t pagetable_base);

void pagetable_map(pagetable_addr_t pagetable_base, virtual_addr_t virtual_addr, physical_addr_t physical_addr, page_access_enum_t page_access);

void pagetable_map_linear(pagetable_addr_t pagetable_base, physical_addr_t start_addr, physical_addr_t end_addr, page_access_enum_t page_access);

void pagetable_unmap(pagetable_addr_t pagetable_base, virtual_addr_t virtual_addr);

#define NO_TRANSLATION ((physical_addr_t)1) //unaligned addr
physical_addr_t pagetable_get_translation(pagetable_addr_t pagetable_base, virtual_addr_t virtual_addr);

//virtual_addr_t pagetable_find(physical_addr_t physical_addr);

void enable_pagetable(pagetable_addr_t pagetable_base);

void switch_pagetable(pagetable_addr_t next_pagetable_base);

extern pagetable_addr_t current_pagetable;

#endif //end module