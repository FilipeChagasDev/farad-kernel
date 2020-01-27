/*
 * KERNEL.F SOURCE CODE
 * Author: Filipe Chagas Ferraz
 */

#ifndef kernelf_memory_cache_h
#define kernelf_memory_cache_h

#include <types.h>

void invalidate_data_cache();
void clean_data_cache();

#endif //end module