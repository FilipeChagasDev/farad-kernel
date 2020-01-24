#include <hardware/aarch64/rpi3/uart.h>
#include <hardware/aarch64/rpi3/multicore.h>
#include <memory/physical.h>
#include <memory/virtual.h>

void test_palloc()
{
    init_physical_memory_manager();
    uart_hex(0);
    uart_puts("\n");
    uart_hex(physical_pages_quantity());
    uart_puts("\n");
    uart_hex(free_physical_pages_quantity());
    uart_puts("\npalloc\n");

    void *ppage[100];

    for(int i = 0; i < 100; i++)
    {
        ppage[i] = alloc_physical_page();
        uart_hex(ppage[i]);
        uart_puts(" -alloc- ");
        uart_hex(free_physical_pages_quantity());
        uart_puts("\n");
    }

    for(int i = 0; i < 100; i++)
    {
        uart_hex(ppage[i]);
        free_physical_page(ppage[i]);
        uart_puts(" -free- ");
        uart_hex(free_physical_pages_quantity());
        uart_puts("\n");
    }

    for(int i = 0; i < 100; i++)
    {
        ppage[i] = alloc_physical_page();
        uart_hex(ppage[i]);
        uart_puts(" -alloc- ");
        uart_hex(free_physical_pages_quantity());
        uart_puts("\n");
    }
}

void test_pagetable_alloc()
{
    init_physical_memory_manager();
    pagetable_addr_t pt = create_pagetable();
    pagetable_map_linear(pt, 0, 0x3F000000, PA_KERNEL);
}

void main()
{
    uart_puts("initializing...\n");
    test_pagetable_alloc();
    uart_puts("done\n");
//    start_other_3_cores();
}

void main_secundary(unsigned int core)
{
    //uart_hex(core);
    //uart_puts("\n");
}