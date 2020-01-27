#include <hardware/aarch64/rpi3/multicore.h>
#include <memory/physical.h>
#include <memory/virtual.h>
#include <log/message.h>
#include <memory/info.h>
#include <memory/linearspace.h>
#include <memory/kernelsegment.h>
#include <memory/kernelheap.h>
#include <calc.h>

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
    
    kernel_log_string("creating pagetable\n");
    pagetable_addr_t pt = create_pagetable();
    kernel_log_string("mapping pagetable\n");
    pagetable_map_linear(pt, 0, 0x60000000, PA_KERNEL);
    
    kernel_log_string("allocating physical page\n");
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

void kernel_log_memory_info()
{
    kernel_log_string("cpu memory base - ");
    kernel_log_hex(get_cpu_memory_base(), TRUE);
    kernel_log_char('\n');

    kernel_log_string("cpu memory size - ");
    kernel_log_hex(get_cpu_memory_size(), TRUE);
    kernel_log_char('\n');

    kernel_log_string("gpu memory base - ");
    kernel_log_hex(get_gpu_memory_base(), TRUE);
    kernel_log_char('\n');

    kernel_log_string("gpu memory size - ");
    kernel_log_hex(get_gpu_memory_size(), TRUE);
    kernel_log_char('\n');
}

void test_linear_space()
{
    init_physical_memory_manager();
    init_linear_space();
    
    kernel_log_hex(physical_pages_quantity(), TRUE);
    kernel_log_string("\n");
    kernel_log_hex(free_physical_pages_quantity(), TRUE);
    kernel_log_string("\npalloc\n");

    void *ppage[100];

    for(int i = 0; i < 10; i++)
    {
        ppage[i] = alloc_physical_page();
    }

    for(int i = 0; i < 10; i++)
    {
        free_physical_page(ppage[i]);
    }

    for(int i = 0; i < 10; i++)
    {
        ppage[i] = alloc_physical_page();
    }
}

void test_kernel_segment()
{
    init_physical_memory_manager();
    init_linear_space();
    init_kernel_segment();
    kernel_log_string("Starting test\n");
    enable_kernel_segment();
    char *num = kernel_segment_info.virtual_heap_base;
    
    for(int i = 0; i < 5; i++)
    {
        num[0] = 'H'; num[1]='E'; num[2]='L'; num[3]='L'; num[4]='O'; num[5]='\0';
        
        kernel_log_string("HELLO written at ");
        kernel_log_hex(num, TRUE);
        num = (char*)((ullong_t)num + physical_mem_info.page_length);
        kernel_log_string(" - next->");
        kernel_log_hex(num, TRUE);
        kernel_log_char('\n');
        extend_kernel_segment();
    }

    kernel_log_string("Switching to linear space\n");

    enter_linear_space();
    char *it = kernel_segment_info.virtual_heap_base;
    while (it < physical_mem_info.physical_heap_end)
    {
        if(it[0]=='H' && it[1]=='E' && it[2]=='L' && it[3]=='L' && it[4]=='O' && it[5]=='\0')
        {
            kernel_log_string("HELLO found at ");
            kernel_log_hex(it, TRUE);
            kernel_log_char('\n');
        }
        it++;
    }
}

void test_malloc()
{
    init_physical_memory_manager();
    init_linear_space();
    init_kernel_segment();
    kernel_log_string("Starting test\n");
    enable_kernel_segment();

    kernel_log_string("Initializing heap\n");
    init_kernel_heap();

    void *spaces[10];

    log_heap_structure();

    for(int i = 0; i < 10; i++)
    {
        spaces[i] = kernel_malloc((i+1)*100);
        kernel_log_string("Allocated at ");
        kernel_log_hex(spaces[i], TRUE);
        kernel_log_string(" - ");
        kernel_log_hex(gap_from_buffer(spaces[i]), TRUE);
        kernel_log_char('\n');
    }

    log_heap_structure();

    for(int i = 0; i < 10; i++)
    {
        kernel_free(spaces[i]);
        kernel_log_string("Free at ");
        kernel_log_hex(spaces[i], TRUE);
        kernel_log_string(" - ");
        kernel_log_hex(gap_from_buffer(spaces[i]), TRUE);
        kernel_log_char('\n');
    } 

    log_heap_structure();

    for(int i = 9; i >= 0; i--)
    {
        spaces[i] = kernel_malloc((i+1)*100);
        kernel_log_string("Allocated at ");
        kernel_log_hex(spaces[i], TRUE);
        kernel_log_string(" - ");
        kernel_log_hex(gap_from_buffer(spaces[i]), TRUE);
        kernel_log_char('\n');
    }

    log_heap_structure();

    for(int i = 9; i >= 0; i--)
    {
        kernel_free(spaces[i]);
        kernel_log_string("Free at ");
        kernel_log_hex(spaces[i], TRUE);
        kernel_log_string(" - ");
        kernel_log_hex(gap_from_buffer(spaces[i]), TRUE);
        kernel_log_char('\n');
    }

    log_heap_structure();
}

void test_calc()
{
    init_physical_memory_manager();
    init_linear_space();
    for(int i = 0; i < 100; i++)
    {
        kernel_log_natural(i);
        kernel_log_string(" - ");
        kernel_log_natural(next_multiple(10,i));
        kernel_log_char('\n');
    }

}

void test_stack_overflow(ullong_t n)
{
    kernel_log_natural(n);
    kernel_log_char('\n');
    test_stack_overflow(n+1);
}

void main()
{
    uart_init();
    kernel_log_string("initializing...\n");

    kernel_log_memory_info();
    //test_stack_overflow(0);
    //test_calc();
    test_malloc();
    //test_kernel_segment();
    //test_linear_space();
    //test_linear_mapping();

    kernel_log_string("done\n");
//    start_other_3_cores();
}

void main_secundary(unsigned int core)
{
    //kernel_log_hex(core, TRUE);
    //kernel_log_string("\n");
}