/*
 * KERNEL.F SOURCE CODE
 * Author: Filipe Chagas Ferraz
 */

#ifndef kernelf_scheduler_context_h
#define kernelf_scheduler_context_h

typedef void* context_ptr_t;

typedef struct context_node
{
    context_ptr_t context;
    struct context_node *next;
}context_node_t;

typedef struct
{
    context_node_t *start;
}context_stack_t;

#endif //end module