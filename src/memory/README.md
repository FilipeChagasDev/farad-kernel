# Memory Management

## Physical

**The Physical module is responsible for managing the allocation and deallocation of physical memory pages.**

**physical_mem_info** registry have some informations about the physical memory usage. 

The physical memory space where pages that can be dynamically allocated are called physical_heap. The **physical_heap_start** and **physical_heap_end** pointers (from the **physical_mem_info** registry) enclose this space.

Each page of unused physical memory page starts with a **free_page_marker_t**. This struct contains two pointers, one next and one previous. The set of **free_page_marker_t** forms a linked list of free pages. This linked list is used by the physical page **alloc_physical_page** and **free_physical_page** functions.

The functions of the **Physical** module files can only be used when the MMU is disabled or when a linear virtual address space is being used in the kernel.

**physical.h** have one **physical.c** implementation for each hardware.

## Virtual

**The Virtual module is responsible for creating, handling and enabling pagetables.**

The **create_pagetable** function creates a blank pagetable and return it's base physical address. The **delete_pagetable** delete it's from the physical memory.

The **pagetable_map** function sets the pagetable translation between a virtual addres and a physical address. The **pagetable_unmap** undo it.

The **pagetable_map_linear** function sets a range of virtual addresses that are translated to equal physical addresses.

The **enable_pagetable** and **switch_pagetable** set up the MMU for the working with a pagetable. The **enable_pagetable** must be used only at the first MMU setting up. Switching between different pagetables must be done with the **switch_pagetable**.

By default, all the successfully translated virtual addresses have execution allowed. Only protected or unmapped virtual addresses have execution forbided.

**virtual.h** have one **virtual.c** implementation for each hardware.

## Linear Space

**The linear space is a virtual addressing space with all the virtual addresses are mapped to equal physical addresses.  Kernel eventually uses this addressing space when the physical memory manager need to be used.**

The **init_linear_space** function creates the pagetable that maps the virtual linear space.

The **enable_linear_space** function set up the MMU for working with the linear space. It should only be used once.

The **enter_linear_space** function switch from any virtual addressing space to the linear space. The **leave_linear_space** function switch from the linear space for the virtual addressing space that was being used before the **enter_linear_space** call.

The **linear_space_drop** reset the internal variables of the enter-leave operation. It must be used only when a pagetable switching from the linear space to another addressing space happens without a **enter_linear_space** call.

## Kernel Segment

**The kernel segment is a virtual addressing space with the kernel code and the peripherals i/o addressing spaces linearly mapped. The physical heap space is not linearly mapped. Only the physical pages used by the kernel are mapped as a contiguous virtual addressing segment. This segment can be extended.

## Kernel Heap

**The Kernel Heap module is responsible for allocation and deallocation of buffers with variable lengths for the kernel usage. The kmalloc and kfree functions are used for this purpose.**

The Kernel Heap uses the kernel segment to allocate and deallocate buffers. Basically, it's module organize all the free spaces in the segment using tree nodes, ordering these free spaces by length and by address.

The kernel heap organizes the segment with the following elements:

* Superbuffers

A Superbuffer is a non free allocated space in the segment that starts with a **kernel_superbuffer_t** registry.

* Buffers

The Buffer is the superbuffer inner space that is used by the other modules of kernel after the **kmalloc** calling. This space starts right after the **kernel_superbuffer_t** registry.

* Gaps

A gap is a free space in the segment that starts with a **kernel_heap_gap_t** registry. The set of all the gaps in the segment composes two binary trees: The **length tree** and the **addr tree**. **Length tree** sorts the gaps by length, and **addr tree** sorts the gaps by address.

* Void

A void is a free space in the segment that is too small to contain a **kernel_heap_gap_t**. This space is ignored, becoming useless. However, if the void is next to two gaps, it can be merged with these two during a merge operation. The formation of voids in the heap is not desirable and characterizes an increase in external fragmentation in the segment.

### NO_VOID

When the **NO_VOID** macro is defined in the kernel heap module, all the superbuffers are allocated with lengths multiple of  *sizeof(kernel_heap_gap_t)* (60 bytes) to avoid the formation of voids.