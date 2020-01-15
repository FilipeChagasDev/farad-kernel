/*
 * KERNEL.F SOURCE CODE
 * Author: Filipe Chagas Ferraz
 */

#ifndef kernelf_multicore_cpu_h
#define kernelf_multicore_cpu_h

#include <types.h>

ulong_t get_cpu_id();

ullong_t cpus_quantity();

void sleep();

void send_to_sleep(ullong_t cpu_id);

void send_to_wakeup(ullong_t cpu_id);

#endif //end module