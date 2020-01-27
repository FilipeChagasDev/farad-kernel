/*
 * KERNEL.F SOURCE CODE
 * Author: Filipe Chagas Ferraz
 */

#ifndef kernelf_memory_kernelsegment_h
#define kernelf_memory_kernelsegment_h

#include <types.h>
#include <memory/virtual.h>
#include <memory/physical.h>
#include <memory/linearspace.h>
#include <panic.h>

typedef struct
{
    pagetable_addr_t pagetable;
    virtual_addr_t virtual_heap_base;
    virtual_addr_t virtual_heap_end;
}kernel_segment_info_t;

extern kernel_segment_info_t kernel_segment_info;

void init_kernel_segment();
void enable_kernel_segment();
virtual_addr_t extend_kernel_segment();

virtual_addr_t kernel_segment_heap_base();
virtual_addr_t kernel_segment_heap_end();
ullong_t kernel_segment_heap_length();

#endif //end module