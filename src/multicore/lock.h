/*
 * KERNEL.F SOURCE CODE
 * Author: Filipe Chagas Ferraz
 */

#ifndef kernelf_multicore_lock_h
#define kernelf_multicore_lock_h

#include <types.h>

typedef struct kernel_lock_request
{
    ulong_t cpu_id;
    struct kernel_lock_request *next;
}kernel_lock_request_t;

typedef struct
{
    kernel_lock_request_t *queue_start;
    kernel_lock_request *queue_end;
}kernel_lock_t;

void kernel_lock_init(kernel_lock_t *lock);
void kernel_lock_acquire(kernel_lock_t *lock);
void kernel_lock_release(kernel_lock_t *lock);

#endif //end module