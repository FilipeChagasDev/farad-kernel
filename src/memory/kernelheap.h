/*
 * KERNEL.F SOURCE CODE
 * Author: Filipe Chagas Ferraz
 */

#ifndef kernelf_memory_kernelheap_h
#define kernelf_memory_kernelheap_h

#include <types.h>
#include <panic.h>
#include <memory/virtual.h>
#include <memory/physical.h>
#include <memory/kernelsegment.h>

#define KERNEL_HEAP_GAP_MAGIC 0xA0
#define KERNEL_HEAP_BUFFER_MAGIC 0xA1

typedef struct 
{
    byte_t magic;
    ullong_t length;
}kernel_buffer_side_t;

typedef enum {GAP_NONE_BRANCH, GAP_BIGGER_BRANCH, GAP_EQUAL_BRANCH, GAP_SMALLER_BRANCH} gap_brach_t;

typedef struct kernel_heap_gap
{
    byte_t magic;
    virtual_addr_t end;
    //struct kernel_heap_gap *next;
    gap_brach_t upper_length_branch;
    struct kernel_heap_gap *upper_length;
    struct kernel_heap_gap *bigger_length;
    struct kernel_heap_gap *equal_length;
    struct kernel_heap_gap *smaller_length;
    
    gap_brach_t upper_addr_branch;
    struct kernel_heap_gap *upper_addr;
    struct kernel_heap_gap *bigger_addr;
    struct kernel_heap_gap *smaller_addr;
}kernel_heap_gap_t;

typedef struct
{
    //kernel_heap_gap_t *list;
    kernel_heap_gap_t *length_tree;
    kernel_heap_gap_t *addr_tree;
    ullong_t gap_quantity;
}kernel_heap_info_t;

extern kernel_heap_info_t kernel_heap_info;

void init_gap(kernel_heap_gap_t *gap, virtual_addr_t end);
void init_kernel_heap();
kernel_heap_gap_t *find_bigger_length_gap();
kernel_heap_gap_t *find_bigger_addr_gap();
ullong_t gap_length(kernel_heap_gap_t *gap);
void insert_gap_by_length(kernel_heap_gap_t *gap);
void insert_gap_by_addr(kernel_heap_gap_t *gap);
void remove_gap_by_length(kernel_heap_gap_t *gap);
void remove_gap_by_addr(kernel_heap_gap_t *gap);
void insert_gap(kernel_heap_gap_t *gap);
void remove_gap(kernel_heap_gap_t *gap);
boolean_t is_buffer(virtual_addr_t buffer);
ullong_t buffer_length(virtual_addr_t buffer);
ullong_t buffer_length_without_side(virtual_addr_t buffer);
kernel_heap_gap_t *gap_from_buffer(virtual_addr_t buffer);
void list_gaps_by_addr(kernel_heap_gap_t **array);
boolean_t try_merge_gaps(kernel_heap_gap_t *higger_gap, kernel_heap_gap_t *lower_gap);

virtual_addr_t kernel_malloc(ullong_t length);
void kernel_free(virtual_addr_t addr);
void log_heap_structure();

// --- Unnecessary functions ---
//void for_each_gap_by_length(kernel_heap_gap_t *root, void (*callback)(kernel_heap_gap_t*));
//void for_each_gap_by_addr(kernel_heap_gap_t *root, void (*callback)(kernel_heap_gap_t*));
//void for_each_gap_by_addr_arg2(kernel_heap_gap_t *root, void (*callback)(kernel_heap_gap_t*, kernel_heap_gap_t*), kernel_heap_gap_t* arg2);

#endif //end module