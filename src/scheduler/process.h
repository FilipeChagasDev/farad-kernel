/*
 * KERNEL.F SOURCE CODE
 * Author: Filipe Chagas Ferraz
 */

#ifndef kernelf_scheduler_process_h
#define kernelf_scheduler_process_h

#include <type.h>
#include <memory/virtual.h>
#include <scheduler/context.h>


typedef struct
{
    virtual_addr_t stack_overflow;
    virtual_addr_t segmentation_fault;
    virtual_addr_t alloc_fault;
    virtual_addr_t div_by_zero;
    virtual_addr_t deadlock;
}process_listeners_t;

typedef struct
{
    virtual_addr_t heap_break;
    virtual_addr_t stack_break;
}process_segment_t;

typedef struct
{
    physical_addr_t start;
    ulong_t length;
}process_code_t;

typedef struct
{
    ulong_t pid;
    string_t name;
    pagetable_addr_t pagetable;
    process_code_t code;
    process_segment_t segment;
    context_ptr_t context;
    context_stack_t return_point;
    process_listeners_t listeners;
    ulong_t return_value;
}process_descriptor_t;

#endif //end module