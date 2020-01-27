/*
 * KERNEL.F SOURCE CODE
 * Author: Filipe Chagas Ferraz
 */

#include <memory/kernelsegment.h>
#include <memory/physical.h>
#include <memory/virtual.h>

kernel_segment_info_t kernel_segment_info;

void init_kernel_segment()
{
    kernel_segment_info.pagetable = create_pagetable();
    pagetable_map_linear(kernel_segment_info.pagetable, 0, physical_mem_info.physical_heap_start, PA_KERNEL);
    pagetable_map_linear(kernel_segment_info.pagetable, physical_mem_info.physical_heap_end, physical_mem_info.physical_end, PA_KERNEL);

    physical_addr_t first_physical_page = alloc_physical_page();

    kernel_segment_info.virtual_heap_base = physical_mem_info.physical_heap_start;
    
    pagetable_map(kernel_segment_info.pagetable, kernel_segment_info.virtual_heap_base, first_physical_page, PA_KERNEL);

    kernel_segment_info.virtual_heap_end = (virtual_addr_t)((ullong_t)kernel_segment_info.virtual_heap_base + physical_mem_info.page_length);
}

void enable_kernel_segment()
{
    linear_space_drop();
    switch_pagetable(kernel_segment_info.pagetable);
}

virtual_addr_t extend_kernel_segment()
{
    enter_linear_space();

    physical_addr_t physical_page = alloc_physical_page();
    //if(physical_page == NULL) panic_log("Failure in extend_kernel_segment", "No memory available");
    
    if(physical_page == NULL) //Failure
    {
        leave_linear_space();
        return NULL; 
    }

    virtual_addr_t old_end = kernel_segment_info.virtual_heap_end;
    pagetable_map(kernel_segment_info.pagetable, kernel_segment_info.virtual_heap_end, physical_page, PA_KERNEL);
    
    kernel_segment_info.virtual_heap_end = (virtual_addr_t)((ullong_t)kernel_segment_info.virtual_heap_end + physical_mem_info.page_length);
    
    leave_linear_space();
    return old_end;
}

virtual_addr_t kernel_segment_heap_base()
{
    return kernel_segment_info.virtual_heap_base;
}

virtual_addr_t kernel_segment_heap_end()
{
    return kernel_segment_info.virtual_heap_end;
}

ullong_t kernel_segment_heap_length()
{
    return ((ullong_t)kernel_segment_info.virtual_heap_end - (ullong_t)kernel_segment_info.virtual_heap_base);
}
