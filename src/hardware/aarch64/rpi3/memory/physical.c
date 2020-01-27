/*
 * KERNEL.F SOURCE CODE
 * Author: Filipe Chagas Ferraz
 */

#include <memory/physical.h>
#include <hardware/aarch64/rpi3/memorymap.h>
#include <memory/info.h>

#ifdef PHYSICAL_MEM_DEBUG
    #include <log/message.h>
#endif

#define PAGE_LEN 0x10000

physical_mem_info_t physical_mem_info;

void init_physical_memory_manager()
{
    physical_mem_info.physical_end = 0x40000000;
    
    volatile extern char _end;
    physical_mem_info.program_end = &_end;
    physical_mem_info.page_length = PAGE_LEN;
    //physical_mem_info.physical_heap_end = 0x3C000000;
    physical_mem_info.physical_heap_end = (physical_addr_t)get_cpu_memory_size();

    // ---- Step 1: Find 64KiB aligned start addr ----

    ullong_t start_found = -1;
    for( ullong_t multiple_64k = 0;
         multiple_64k < physical_mem_info.physical_heap_end;
         multiple_64k += PAGE_LEN )
    {
        if(multiple_64k >= physical_mem_info.program_end)
        {
            start_found = multiple_64k;
            break;
        }
    }

    if(start_found == -1)
    {
        panic_log("Aligned physical heap start not found","Cannot init memory");
    }
    else
    {
        physical_mem_info.physical_heap_start = start_found;
    }

    // ---- Step 2: Mark all free pages ----

    physical_mem_info.physical_heap_length = physical_mem_info.physical_heap_end - physical_mem_info.physical_heap_start;
    physical_mem_info.page_quantity = physical_mem_info.physical_heap_length / physical_mem_info.page_length;
    physical_mem_info.free_pages_count = 0;

    //First page
    free_page_marker_t *free_page = physical_mem_info.physical_heap_start;
    free_page->magic = FREE_PAGE_MARKER_MAGIC;
    free_page->prev = NULL;
    physical_mem_info.free_pages_list = free_page;
     

    //Next pages
    free_page_marker_t *prev_free_page = NULL;
    while( free_page < physical_mem_info.physical_heap_end )
    {
        physical_mem_info.free_pages_count += 1;

        prev_free_page = free_page;
        free_page = (free_page_marker_t*)((ullong_t)free_page + physical_mem_info.page_length); //Next page
        prev_free_page->next = free_page;
        free_page->prev = prev_free_page;
        free_page->magic = FREE_PAGE_MARKER_MAGIC;

        #ifdef PHYSICAL_MEM_DEBUG
        // debug log
        kernel_log_string("page marked free - ");
        kernel_log_hex(free_page, TRUE);
        kernel_log_string(" - total ");
        kernel_log_natural(physical_mem_info.free_pages_count);
        kernel_log_char('\n');
        #endif
    }
    
    //Last page
    free_page->next = NULL;
}

uint_t physical_page_length()
{
    return physical_mem_info.page_length;
}

ulong_t physical_pages_quantity()
{
    return physical_mem_info.page_quantity;
}

ulong_t free_physical_pages_quantity()
{
    return physical_mem_info.free_pages_count;
}

physical_addr_t alloc_physical_page()
{
    if(physical_mem_info.free_pages_list == NULL) return NULL;
    
    free_page_marker_t *page_addr = physical_mem_info.free_pages_list;
    free_page_marker_t *next = page_addr->next;
    physical_mem_info.free_pages_list = next;

    if(next != NULL)
    {
        next->prev = NULL;
    }

    physical_mem_info.free_pages_count -= 1;

    #ifdef PHYSICAL_MEM_DEBUG
    //debug log
    kernel_log_string("allocated physical page at ");
    kernel_log_hex(page_addr, TRUE);
    kernel_log_string(" - ");
    kernel_log_natural(physical_mem_info.free_pages_count);
    kernel_log_string(" free physical pages available\n");
    #endif

    return page_addr;
}

void free_physical_page(physical_addr_t addr)
{
    if(addr < physical_mem_info.physical_heap_start || addr > physical_mem_info.physical_heap_end) 
        panic_log("Error in free_physical_page", "Memory address out of range");
    
    physical_mem_info.free_pages_count += 1;

    physical_addr_t aligned = (physical_addr_t)((ullong_t)addr & ~((ullong_t)0xFFFF));
    free_page_marker_t *free_page = (free_page_marker_t*)aligned;
    free_page_marker_t *next = physical_mem_info.free_pages_list;

    //Insert in the start of list
    free_page->magic = FREE_PAGE_MARKER_MAGIC;
    free_page->prev = NULL;
    free_page->next = next;
    
    if(next != NULL)
    {
        next->prev = free_page;
    }

    physical_mem_info.free_pages_list = free_page;

    #ifdef PHYSICAL_MEM_DEBUG
    //debug log
    kernel_log_string("free physical page at ");
    kernel_log_hex(addr, TRUE);
    kernel_log_string(" - ");
    kernel_log_natural(physical_mem_info.free_pages_count);
    kernel_log_string(" free physical pages available\n");
    #endif
}