# Memory Management

physical_mem_info struct have some informations about the physical memory usage. 

The physical memory space where pages that can be dynamically allocated are called physical_heap. The physical_heap_start and physical_heap_end pointers (from the physical_mem_info struct) enclose this space.

Each page of unused physical memory starts with a free_page_marker_t. This struct contains two pointers, one next and one previous. The set of free_page_marker_t forms a linked list of free pages. This linked list is used by physical page allocators and releasers.

The functions of the physical.h and virtual.h files can only be used when the MMU is disabled or when a linear virtual address space is being used in the kernel.

physical.h and virtual.h have an implementation for each hardware
