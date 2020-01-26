/*
 * KERNEL.F SOURCE CODE
 * Author: Filipe Chagas Ferraz
 */

#ifndef kernelf_memory_info_h
#define kernelf_memory_info_h

#include <types.h>
#include <panic.h>

physical_addr_t get_cpu_memory_base();
ullong_t get_cpu_memory_size();

physical_addr_t get_gpu_memory_base();
ullong_t get_gpu_memory_size();

#endif //end module