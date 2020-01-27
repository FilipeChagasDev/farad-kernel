/*
 * KERNEL.F SOURCE CODE
 * Author: Filipe Chagas Ferraz
 */

#include <memory/linearspace.h>
#include <log/message.h>

boolean_t linear_space_enabled;
pagetable_addr_t linear_pagetable;

pagetable_addr_t saved_nonlinear_pagetable;
ulong_t enter_linear_space_count;

void init_linear_space()
{
    enter_linear_space_count = 0;
    saved_nonlinear_pagetable = NULL;
    linear_pagetable = create_pagetable();
    pagetable_map_linear(linear_pagetable, 0, physical_mem_info.physical_end, PA_KERNEL);
    linear_space_enabled = TRUE;
    enable_pagetable(linear_pagetable);
}

void enable_linear_space()
{
    linear_space_enabled = TRUE;
    switch_pagetable(linear_pagetable);
}

void enter_linear_space()
{
    if(enter_linear_space_count == 0)
    {
        saved_nonlinear_pagetable = current_pagetable;
        enable_linear_space();
    }

    enter_linear_space_count += 1;
}

void leave_linear_space()
{
    if(enter_linear_space_count > 0)
    {
        if(enter_linear_space_count == 1)
        {
            switch_pagetable(saved_nonlinear_pagetable);
            linear_space_enabled = FALSE;
        }

        enter_linear_space_count -= 1;
    } 
}

void linear_space_drop()
{
    enter_linear_space_count = 0;
    linear_space_enabled = FALSE;
}