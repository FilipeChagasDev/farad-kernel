/*
 * KERNEL.F SOURCE CODE
 * Author: Filipe Chagas Ferraz
 */

#include <memory/kernelheap.h>
#include <calc.h>

#undef HEAP_DEBUG //Comment this line to see the debug heap logs

#ifdef HEAP_DEBUG
    #include <log/message.h>
#endif

kernel_heap_info_t kernel_heap_info;

void init_gap(kernel_heap_gap_t *gap, virtual_addr_t end)
{
    gap->magic = KERNEL_HEAP_GAP_MAGIC;
    gap->end = end;
    
    gap->upper_length_branch = GAP_NONE_BRANCH;
    gap->upper_length = NULL;
    gap->bigger_length = NULL;
    gap->smaller_length = NULL;

    gap->upper_addr_branch = GAP_NONE_BRANCH;
    gap->upper_addr = NULL;
    gap->bigger_addr = NULL;
    gap->smaller_addr = NULL;
}

void init_kernel_heap()
{
    kernel_heap_gap_t *first_gap = kernel_segment_heap_base();
    ullong_t gap_len = prev_multiple(sizeof(kernel_heap_gap_t), kernel_segment_heap_length());
    init_gap(first_gap, (virtual_addr_t)((ullong_t)first_gap + gap_len));

    kernel_heap_info.length_tree = first_gap;
    kernel_heap_info.addr_tree = first_gap;
    kernel_heap_info.gap_quantity = 1;
}


kernel_heap_gap_t *find_bigger_length_gap()
{
    kernel_heap_gap_t *root = kernel_heap_info.length_tree;
    if(root == NULL) return NULL; //Empty tree
    //if(root->magic != KERNEL_HEAP_GAP_MAGIC) panic_log("Failure in find_bigger_length_gap", "Invalid root");

    kernel_heap_gap_t *it = root;
    while (it->bigger_length != NULL)
    {
        it = it->bigger_length;
    }

    #ifdef HEAP_DEBUG
    kernel_log_string("Bigger gap found - ");
    kernel_log_natural(gap_length(it));
    kernel_log_string(" bytes at ");
    kernel_log_hex(it, TRUE);
    kernel_log_char('\n');
    #endif

    return it;
}

kernel_heap_gap_t *find_bigger_addr_gap()
{
    kernel_heap_gap_t *root = kernel_heap_info.addr_tree; 
    if(root == NULL) return NULL; //Empty tree
    //if(root->magic != KERNEL_HEAP_GAP_MAGIC) panic_log("Failure in find_bigger_addr_gap", "Invalid root");

    kernel_heap_gap_t *it = root;
    while (it->bigger_addr != NULL)
    {
        it = it->bigger_addr;
    }

    #ifdef HEAP_DEBUG
    kernel_log_string("Higher gap found - ");
    kernel_log_natural(gap_length(it));
    kernel_log_string(" bytes at ");
    kernel_log_hex(it, TRUE);
    kernel_log_char('\n');
    #endif

    return it;
}

ullong_t gap_length(kernel_heap_gap_t *gap)
{
    return (ullong_t)gap->end - (ullong_t)gap;
}

void insert_gap_by_length(kernel_heap_gap_t *gap)
{
    #ifdef HEAP_DEBUG
    kernel_log_string("Inserting gap by len - ");
    kernel_log_hex(gap, TRUE);
    kernel_log_char('\n');
    #endif

    kernel_heap_gap_t *root = kernel_heap_info.length_tree;
    if(gap == NULL) panic_log("Failure in insert_gap_by_length", "NULL arg");
    if(gap->magic != KERNEL_HEAP_GAP_MAGIC) panic_log("Failure in insert_gap_by_length", "Invalid arg");

    if(root == NULL) //Empty tree
    {
        kernel_heap_info.length_tree = gap;
        gap->upper_length = NULL;
        gap->upper_length_branch = GAP_NONE_BRANCH;
        return;
    }

    kernel_heap_gap_t *it = root;
    while (1)
    {
        ullong_t gap_len = gap_length(gap);
        ullong_t it_len = gap_length(it);
        
        if(gap_len <= it_len)
        {
            if(it->smaller_length == NULL)
            {
                it->smaller_length = gap;
                gap->upper_length = it;
                gap->upper_length_branch = GAP_SMALLER_BRANCH;
                break;
            }
            else
            {
                it = it->smaller_length;
            }
        }
        else //if(gap_len > it_len)
        {
            if(it->bigger_length == NULL)
            {
                it->bigger_length = gap;
                gap->upper_length = it;
                gap->upper_length_branch = GAP_BIGGER_BRANCH;
                break;
            }
            else
            {
                it = it->bigger_length;
            }
        }
    }
}

void insert_gap_by_addr(kernel_heap_gap_t *gap)
{
    #ifdef HEAP_DEBUG
    kernel_log_string("Inserting gap by addr - ");
    kernel_log_hex(gap, TRUE);
    kernel_log_char('\n');
    #endif

    kernel_heap_gap_t *root = kernel_heap_info.addr_tree; 
    if(gap == NULL) panic_log("Failure in insert_gap_by_addr", "NULL arg");
    if(gap->magic != KERNEL_HEAP_GAP_MAGIC) panic_log("Failure in insert_gap_by_addr", "Invalid arg");
    
    if(root == NULL) // Empty tree
    {
        kernel_heap_info.addr_tree = gap;
        gap->upper_addr = NULL;
        gap->upper_addr_branch = GAP_NONE_BRANCH;
        return;
    }

    kernel_heap_gap_t *it = root;
    while (1)
    {
        ullong_t gap_addr = (ullong_t)gap;
        ullong_t it_addr = (ullong_t)it;
        
        if(gap_addr > it_addr)
        {
            if(it->bigger_addr == NULL)
            {
                it->bigger_addr = gap;
                gap->upper_addr = it;
                gap->upper_addr_branch = GAP_BIGGER_BRANCH;
                break;
            }    
            else
            {
                it = it->bigger_addr;
            }
        }
        else //if(gap_addr < it_addr)
        {
            if(it->smaller_addr == NULL)
            {
                it->smaller_addr = gap;
                gap->upper_addr = it;
                gap->upper_addr_branch = GAP_SMALLER_BRANCH;
                break;
            }
            else
            {
                it = it->smaller_addr;
            }
        }
    }
}

void remove_gap_by_length(kernel_heap_gap_t *gap)
{
    #ifdef HEAP_DEBUG
    kernel_log_string("Removing gap by len - ");
    kernel_log_hex(gap, TRUE);
    kernel_log_char('\n');
    #endif

    kernel_heap_gap_t *root = kernel_heap_info.length_tree; 
    if(root == NULL) panic_log("Failure in remove_gap_by_length", "NULL root");
    if(gap == NULL) panic_log("Failure in remove_gap_by_length", "NULL arg");
    if(gap->magic != KERNEL_HEAP_GAP_MAGIC) panic_log("Failure in remove_gap_by_length", "Invalid arg");    

    if(gap->upper_length == NULL) //Root
    {
        kernel_heap_gap_t *smaller = gap->smaller_length;
        kernel_heap_gap_t *bigger = gap->bigger_length;

        if(smaller == NULL && bigger != NULL)
        {
            root = bigger;
            root->upper_length = NULL;
            root->upper_length_branch = GAP_NONE_BRANCH;
            kernel_heap_info.length_tree = root;
        }
        else if(smaller != NULL && bigger == NULL)
        {
            root = smaller;
            root->upper_length = NULL;
            root->upper_length_branch = GAP_NONE_BRANCH;
            kernel_heap_info.length_tree = root;
        }
        else if(smaller != NULL && bigger != NULL)
        {
            root = bigger;
            root->upper_length = NULL;
            root->upper_length_branch = GAP_NONE_BRANCH;
            kernel_heap_info.length_tree = root;
            insert_gap_by_length(smaller);
        }
        else
        {
            kernel_heap_info.length_tree = NULL;
        }
    }
    else
    {
        kernel_heap_gap_t *smaller = gap->smaller_length;
        kernel_heap_gap_t *bigger = gap->bigger_length;
        kernel_heap_gap_t *upper = gap->upper_length;

        if(gap->upper_length_branch == GAP_BIGGER_BRANCH)
        {
            upper->bigger_length = NULL;
        }
        else //if(gap->upper_length_branch == GAP_SMALLER_BRANCH)
        {
            upper->smaller_length = NULL;
        }

        if(smaller != NULL) insert_gap_by_length(smaller);
        if(bigger != NULL) insert_gap_by_length(bigger);
    }
}


void remove_gap_by_addr(kernel_heap_gap_t *gap)
{
    #ifdef HEAP_DEBUG
    kernel_log_string("Removing gap by addr - ");
    kernel_log_hex(gap, TRUE);
    kernel_log_char('\n');
    #endif

    kernel_heap_gap_t *root = kernel_heap_info.addr_tree; 
    if(root == NULL) panic_log("Failure in remove_gap_by_addr", "NULL root");
    if(gap == NULL) panic_log("Failure in remove_gap_by_addr", "NULL arg");
    if(gap->magic != KERNEL_HEAP_GAP_MAGIC) panic_log("Failure in remove_gap_by_addr", "Invalid arg");

    if(gap->upper_addr == NULL) //Root
    {
        kernel_heap_gap_t *smaller = gap->smaller_addr;
        kernel_heap_gap_t *bigger = gap->bigger_addr;
        
        if(smaller == NULL && bigger != NULL)
        {
            root = bigger;
            root->upper_addr = NULL;
            root->upper_addr_branch = GAP_NONE_BRANCH;
            kernel_heap_info.addr_tree = root;
        }
        else if(smaller != NULL && bigger == NULL)
        {
            root = smaller;
            root->upper_addr = NULL;
            root->upper_addr_branch = GAP_NONE_BRANCH;
            kernel_heap_info.addr_tree = root;
        }
        else if(smaller != NULL && bigger != NULL)
        {
            root = bigger;
            root->upper_addr = NULL;
            root->upper_addr_branch = GAP_NONE_BRANCH;
            kernel_heap_info.addr_tree = root;
            insert_gap_by_addr(smaller);
        }
        else
        {
            kernel_heap_info.addr_tree = NULL;
        }
        
    }
    else
    {
        kernel_heap_gap_t *smaller = gap->smaller_addr;
        kernel_heap_gap_t *bigger = gap->bigger_addr;
        kernel_heap_gap_t *upper = gap->upper_addr;

        if(gap->upper_addr_branch == GAP_BIGGER_BRANCH)
        {
            upper->bigger_addr = NULL;
        }
        else //if(gap->upper_addr_branch == GAP_SMALLER_BRANCH)
        {
            upper->smaller_addr = NULL;
        }

        if(smaller != NULL) insert_gap_by_addr(smaller);
        if(smaller != NULL) insert_gap_by_addr(bigger);
    }
}

void insert_gap(kernel_heap_gap_t *gap)
{
    insert_gap_by_addr(gap);
    insert_gap_by_length(gap);

    kernel_heap_info.gap_quantity += 1;
}

void remove_gap(kernel_heap_gap_t *gap)
{
    remove_gap_by_addr(gap);
    remove_gap_by_length(gap);

    kernel_heap_info.gap_quantity -= 1;
}

boolean_t is_buffer(virtual_addr_t buffer)
{
    kernel_buffer_side_t *side = (kernel_buffer_side_t*)((ullong_t)buffer - sizeof(kernel_buffer_side_t));
    return side->magic == KERNEL_HEAP_BUFFER_MAGIC;
}

ullong_t buffer_length(virtual_addr_t buffer)
{
    if(is_buffer(buffer) == FALSE) panic_log("Failure in buffer_length", "Invalid arg");
    kernel_buffer_side_t *side = (kernel_buffer_side_t*)((ullong_t)buffer - sizeof(kernel_buffer_side_t));
    return side->length;
}

ullong_t buffer_length_without_side(virtual_addr_t buffer)
{
    if(is_buffer(buffer) == FALSE) panic_log("Failure in buffer_length", "Invalid arg");
    kernel_buffer_side_t *side = (kernel_buffer_side_t*)((ullong_t)buffer - sizeof(kernel_buffer_side_t));
    return side->length - sizeof(kernel_buffer_side_t);
}

kernel_heap_gap_t *gap_from_buffer(virtual_addr_t buffer)
{
    return (kernel_heap_gap_t*)((ullong_t)buffer - sizeof(kernel_buffer_side_t));
}

boolean_t try_merge_gaps(kernel_heap_gap_t *higher_gap, kernel_heap_gap_t *lower_gap)
{
    #ifdef HEAP_DEBUG
    kernel_log_string("Try merge gaps l");
    kernel_log_hex(lower_gap, TRUE);
    kernel_log_string(" and h");
    kernel_log_hex(higher_gap, TRUE);
    #endif

    if((ullong_t)higher_gap == (ullong_t)lower_gap)
    {
         return FALSE;
    }

    if((ullong_t)higher_gap < (ullong_t)lower_gap)
    {
        //Swap
        kernel_heap_gap_t *aux = higher_gap;
        higher_gap = lower_gap;
        lower_gap = aux;
    }

    ullong_t distance = (ullong_t)higher_gap - (ullong_t)lower_gap->end;

    #ifdef HEAP_DEBUG
    kernel_log_string(" [");
    kernel_log_natural(distance);
    kernel_log_string(", ");
    kernel_log_natural(sizeof(kernel_heap_gap_t));
    kernel_log_string("]\n");
    #endif

    if(distance < sizeof(kernel_heap_gap_t)) //Neighboring gaps, merge it
    {
        #ifdef HEAP_DEBUG
        kernel_log_string("Merging gaps - ");
        kernel_log_hex(lower_gap, TRUE);
        kernel_log_string(" and ");
        kernel_log_hex(higher_gap, TRUE);
        kernel_log_char('\n');
        #endif

        ullong_t space_len = (ullong_t)higher_gap->end - (ullong_t)lower_gap;
        ullong_t new_gap_len = prev_multiple(sizeof(kernel_heap_gap_t), space_len);
        remove_gap(higher_gap);
        lower_gap->end = (virtual_addr_t)((ullong_t)lower_gap + new_gap_len);
        return TRUE; //Success
    }

    return FALSE; //Cannot merge gaps
}

void __recursive_list_gaps_by_addr__(kernel_heap_gap_t *addr_root, kernel_heap_gap_t **array, ullong_t *index)
{
    if(addr_root == NULL) return;
    __recursive_list_gaps_by_addr__(addr_root->smaller_addr, array, index);
    array[*index] = addr_root;
    *index += 1;
    __recursive_list_gaps_by_addr__(addr_root->bigger_addr, array, index);
}

void list_gaps_by_addr(kernel_heap_gap_t **array)
{
    ullong_t index = 0;
    __recursive_list_gaps_by_addr__(kernel_heap_info.addr_tree, array, &index);
}

void merge_all_gaps()
{
    kernel_heap_gap_t *gaps[kernel_heap_info.gap_quantity];
    ullong_t index = 0;
    list_gaps_by_addr(gaps);

    ullong_t no_merge_occurred = TRUE;

    for(ullong_t i = 0; i < kernel_heap_info.gap_quantity-1; i++)
    {
        ullong_t j = i+1;
        boolean_t success = try_merge_gaps(gaps[j], gaps[i]);
        if(success == TRUE)
        { 
            i++; //skip the next index
            no_merge_occurred = FALSE;
        }
    }

    if(no_merge_occurred == FALSE) merge_all_gaps(); //try again
}

boolean_t extend_kernel_heap()
{
    virtual_addr_t new_space_base = extend_kernel_segment();
    if(new_space_base == NULL)
    {   
        #ifdef HEAP_DEBUG
        kernel_log_string("Cannot extend kernel heap\n");
        #endif

        return FALSE; //Cannot extend kernel heap
    } 

    kernel_heap_gap_t *new_gap = (kernel_heap_gap_t*)new_space_base;
    
    ullong_t new_space_len = (ullong_t)kernel_segment_heap_end() - (ullong_t)new_gap;
    ullong_t new_gap_len = prev_multiple(sizeof(kernel_heap_gap_t), new_space_len);
    virtual_addr_t new_gap_end = (virtual_addr_t)((ullong_t)new_gap + new_gap_len);

    init_gap(new_gap, new_gap_end);
    insert_gap(new_gap);
    merge_all_gaps();

    return TRUE; //Success
}

virtual_addr_t kernel_malloc(ullong_t length)
{
    #ifdef HEAP_DEBUG
    kernel_log_string("allocating ");
    kernel_log_natural(length);
    kernel_log_string("bytes at kernel heap\n");
    #endif

    if(length == 0) return NULL;

    //Allocated space most be multiple of sizeof(kernel_heap_gap_t)
    ullong_t required_len = next_multiple(sizeof(kernel_heap_gap_t), length + sizeof(kernel_buffer_side_t));

    #ifdef HEAP_DEBUG
    kernel_log_natural(required_len);
    kernel_log_string(" bytes required\n");
    #endif

    //Use the larger gap available
    kernel_heap_gap_t *gap = find_bigger_length_gap();

    while (gap_length(gap) < required_len || gap == NULL)
    {
        boolean_t success = extend_kernel_heap();
        
        if(success == FALSE)
        {
            #ifdef HEAP_DEBUG
            kernel_log_string("kernel_malloc returning null\n");
            #endif

            return NULL; //Failure
        }

        gap = find_bigger_length_gap();
    }
    
    //Calculate buffer limits
    virtual_addr_t buffer_base = (virtual_addr_t)((ullong_t)gap + sizeof(kernel_buffer_side_t));
    virtual_addr_t buffer_end = (virtual_addr_t)((ullong_t)gap + required_len);

    //Place a new gap in the remaining space
    ullong_t remaining = (ullong_t)gap->end - (ullong_t)buffer_end;
    if(remaining >= sizeof(kernel_heap_gap_t))
    {
        kernel_heap_gap_t *new_gap = (kernel_heap_gap_t*)buffer_end;
        virtual_addr_t new_gap_end = gap->end;
        init_gap(new_gap, new_gap_end);

        #ifdef HEAP_DEBUG
        kernel_log_string("new_gap at - ");
        kernel_log_hex(new_gap, TRUE);
        kernel_log_char('\n');
        #endif

        insert_gap(new_gap);
    }

    remove_gap(gap);

    //Set up buffer side
    kernel_buffer_side_t *buffer_side = (kernel_buffer_side_t*)gap;
    buffer_side->magic = KERNEL_HEAP_BUFFER_MAGIC;
    buffer_side->length = required_len;

    return buffer_base;
}

void kernel_free(virtual_addr_t addr)
{
    if(is_buffer(addr) == FALSE) panic_log("Failure in kernel_free", "addr is not a valid buffer");
    
    virtual_addr_t buffer_end = (virtual_addr_t)((ullong_t)addr + buffer_length_without_side(addr));
    kernel_heap_gap_t *new_gap = gap_from_buffer(addr);
    init_gap(new_gap, buffer_end);
    insert_gap(new_gap);
    merge_all_gaps();
}


void __recursive_log_heap_structure__(kernel_heap_gap_t *addr_root)
{
    if(addr_root == NULL) return;
    __recursive_log_heap_structure__(addr_root->smaller_addr);
    
    kernel_log_string("gap [");
    kernel_log_hex(addr_root, TRUE);
    kernel_log_string(" , ");
    kernel_log_hex(addr_root->end, TRUE);
    kernel_log_string("] ");
    kernel_log_natural(gap_length(addr_root));
    kernel_log_string(" bytes\n");

    __recursive_log_heap_structure__(addr_root->bigger_addr);
}

void log_heap_structure()
{
    kernel_log_string("---heap structure---\n");
    __recursive_log_heap_structure__(kernel_heap_info.addr_tree);
    kernel_log_string("---\n");
}

#if 0 //Unnecessary functions

void for_each_gap_by_length(kernel_heap_gap_t *root, void (*callback)(kernel_heap_gap_t*))
{
    if(root == NULL) return;
    for_each_gap_by_length(root->smaller_length, callback);
    callback(root);
    for_each_gap_by_length(root->bigger_length, callback);
}

void for_each_gap_by_addr(kernel_heap_gap_t *root, void (*callback)(kernel_heap_gap_t*))
{
    if(root == NULL) return;
    for_each_gap_by_addr(root->smaller_addr, callback);
    callback(root);
    for_each_gap_by_addr(root->bigger_addr, callback);
}

void for_each_gap_by_addr_arg2(kernel_heap_gap_t *root, void (*callback)(kernel_heap_gap_t*, kernel_heap_gap_t*), kernel_heap_gap_t* arg2)
{
    if(root == NULL) return;
    for_each_gap_by_addr(root->smaller_addr, callback);
    callback(root, arg2);
    for_each_gap_by_addr(root->bigger_addr, callback);
}

#endif