#include <memory/physical.h>
#include <hardware/aarch64/rpi3/memorymap.h>
#include <hardware/aarch64/rpi3/uart.h>

#define PAGE_LEN 0x10000

physical_mem_info_t physical_mem_info;

void init_physical_memory_manager()
{
    volatile extern char _end;
    physical_mem_info.program_end = &_end;
    physical_mem_info.page_length = PAGE_LEN;
    //physical_mem_info.physical_heap_end = MMIO_BASE;
    physical_mem_info.physical_heap_end = 0x3C000000;

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
    free_page_marker_t *prev_free_page = NULL;
    physical_mem_info.free_pages_list = free_page;
    free_page->prev = NULL; 

    //Next pages
    while( free_page < physical_mem_info.physical_heap_end )
    {
        /*
        // ==== debug log ====
        uart_puts("page");
        uart_hex(free_page);
        uart_puts(" ");
        uart_hex(physical_mem_info.page_length);
        uart_puts("\n");
        // ====================
        */
       
        physical_mem_info.free_pages_count += 1;
        prev_free_page = free_page; 
        free_page += physical_mem_info.page_length>>4;
        prev_free_page->next = free_page;
        free_page->prev = prev_free_page;
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
    else
    {
        physical_mem_info.free_pages_list = NULL;
    }

    physical_mem_info.free_pages_count -= 1;

    // ==== debug log =====
    uart_puts("allocated page at ");
    uart_hex(page_addr);
    uart_send('\n');
    // =====================

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
    free_page->prev = NULL;
    free_page->next = next;
    
    if(next != NULL)
    {
        next->prev = free_page;
    }

    physical_mem_info.free_pages_list = free_page;
}