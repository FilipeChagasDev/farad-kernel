#include <hardware/aarch64/rpi3/multicore.h>
#include <memory/physical.h>
#include <memory/virtual.h>
#include <log/message.h>

void test_palloc()
{
    init_physical_memory_manager();
    kernel_log_hex(0, TRUE);
    kernel_log_string("\n");
    kernel_log_hex(physical_pages_quantity(), TRUE);
    kernel_log_string("\n");
    kernel_log_hex(free_physical_pages_quantity(), TRUE);
    kernel_log_string("\npalloc\n");

    void *ppage[100];

    for(int i = 0; i < 100; i++)
    {
        ppage[i] = alloc_physical_page();
        kernel_log_hex(ppage[i], TRUE);
        kernel_log_string(" -alloc- ");
        kernel_log_hex(free_physical_pages_quantity(), TRUE);
        kernel_log_string("\n");
    }

    for(int i = 0; i < 100; i++)
    {
        kernel_log_hex(ppage[i], TRUE);
        free_physical_page(ppage[i]);
        kernel_log_string(" -free- ");
        kernel_log_hex(free_physical_pages_quantity(), TRUE);
        kernel_log_string("\n");
    }

    for(int i = 0; i < 100; i++)
    {
        ppage[i] = alloc_physical_page();
        kernel_log_hex(ppage[i], TRUE);
        kernel_log_string(" -alloc- ");
        kernel_log_hex(free_physical_pages_quantity(), TRUE);
        kernel_log_string("\n");
    }
}

void test_pagetable_alloc()
{
    init_physical_memory_manager();
    pagetable_addr_t pt = create_pagetable();
    pagetable_map_linear(pt, 0, 0x3F000000, PA_KERNEL);
}

void test_pagetable_enable()
{
    init_physical_memory_manager();

    pagetable_addr_t pt = create_pagetable();
    pagetable_addr_t pt2 = create_pagetable();

    pagetable_map_linear(pt, 0, physical_mem_info.physical_heap_start, PA_KERNEL);
    pagetable_map_linear(pt, physical_mem_info.physical_heap_end, 0x40000000, PA_KERNEL);
    
    pagetable_map_linear(pt2, 0, physical_mem_info.physical_heap_start, PA_KERNEL);
    pagetable_map_linear(pt2, physical_mem_info.physical_heap_end, 0x40000000, PA_KERNEL);

    physical_addr_t pp = alloc_physical_page();
    physical_addr_t pp2 = alloc_physical_page();
    
    virtual_addr_t vp = physical_mem_info.physical_heap_end - physical_mem_info.page_length;
    virtual_addr_t vp2 = physical_mem_info.physical_heap_end - 2*physical_mem_info.page_length;

    pagetable_map(pt, vp, pp, PA_KERNEL);
    pagetable_map(pt, vp2, pp2, PA_KERNEL);

    pagetable_map(pt2, vp2, pp, PA_KERNEL);
    pagetable_map(pt2, vp, pp2, PA_KERNEL);

    char *str = pp;
    str[0] = '-';
    str[1] = 'H';
    str[2] = 'E';
    str[3] = 'L';
    str[4] = 'L';
    str[5] = 'O';
    str[6] = '\n';
    str[7] = '\0';

    char *str2 = pp2;
    str2[0] = '-';
    str2[1] = 'W';
    str2[2] = 'O';
    str2[3] = 'R';
    str2[4] = 'L';
    str2[5] = 'D';
    str2[6] = '\n';
    str2[7] = '\0';

    char *vstr = vp;
    char *vstr2 = vp2;
    
    enable_pagetable(pt);
    kernel_log_hex(vstr, TRUE);
    kernel_log_string(vstr);
    kernel_log_hex(vstr2, TRUE);
    kernel_log_string(vstr2);

    kernel_log_string("switching\n");
    switch_pagetable(pt2);

    kernel_log_hex(vstr, TRUE);
    kernel_log_string(vstr);

    kernel_log_hex(vstr2, TRUE);
    kernel_log_string(vstr2);
}

void test_linear_mapping()
{
    init_physical_memory_manager();
    pagetable_addr_t pt = create_pagetable();
    pagetable_map_linear(pt, 0, 0x40000000, PA_KERNEL);
    
    char *page = (char*)alloc_physical_page();
    page[0] = 'A';
    page[1] = 'B';
    page[2] = 'C';
    page[3] = 'D';
    page[4] = 'E';
    page[5] = 'F';
    page[6] = 'G';
    page[7] = '\n';
    page[8] = '\0';

    kernel_log_string("before MMU enable\n");
    kernel_log_string(page);
    kernel_log_string("after MMU enable\n");
    enable_pagetable(pt);
    kernel_log_string(page);
}

void test_log()
{
    kernel_log_string("HELLO\n");
    kernel_log_natural(123);
    kernel_log_char('\n');
    kernel_log_integer(-123);
    kernel_log_char('\n');
    kernel_log_hex(0x123ABC, TRUE);
    kernel_log_char('\n');
}

void main()
{
    uart_init();
    kernel_log_string("initializing...\n");
    test_log();
    test_linear_mapping();
    kernel_log_string("done\n");
//    start_other_3_cores();
}

void main_secundary(unsigned int core)
{
    //kernel_log_hex(core, TRUE);
    //kernel_log_string("\n");
}