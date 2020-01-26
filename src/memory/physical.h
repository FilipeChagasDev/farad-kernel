/*
 * KERNEL.F SOURCE CODE
 * Author: Filipe Chagas Ferraz
 */

#ifndef kernelf_memory_physical_h
#define kernelf_memory_physical_h

#include <types.h>
#include <panic.h>


typedef struct free_page_marker
{
    ullong_t magic;
#define FREE_PAGE_MARKER_MAGIC 1234
    struct free_page_marker *next;
    struct free_page_marker *prev;
}free_page_marker_t;

typedef struct physical_mem_info_struct
{
    void *program_end;
    void *physical_heap_start;
    void *physical_heap_end;
    ulong_t physical_heap_length;
    ulong_t page_quantity;
    uint_t page_length;   
    free_page_marker_t *free_pages_list;  
    ulong_t free_pages_count;
}physical_mem_info_t;

extern physical_mem_info_t physical_mem_info;

void init_physical_memory_manager();

physical_addr_t alloc_physical_page();

void free_physical_page(physical_addr_t addr);

uint_t physical_page_length();

ulong_t physical_pages_quantity();

ulong_t free_physical_pages_quantity();

#endif //end module