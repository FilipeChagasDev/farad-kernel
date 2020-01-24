#include <memory/virtual.h>
#include <hardware/aarch64/memory/armv8mmu.h>

pagetable_addr_t current_pagetable;

#define DESC_L2 struct TARMV8MMU_LEVEL2_TABLE_DESCRIPTOR
#define DESC_L3 struct TARMV8MMU_LEVEL3_PAGE_DESCRIPTOR

#define DEFINED_BLOCK 3
#define UNDEFINED_BLOCK 0

// index into MAIR_EL1 register
#define ATTRINDX_NORMAL		0
#define ATTRINDX_DEVICE		1
#define ATTRINDX_COHERENT	2

ulong_t pagetable_length()
{
    return 0x10000; //64KiB
}

pagetable_addr_t create_pagetable()
{
    DESC_L2 *l2 = (DESC_L2*)alloc_physical_page();
    if(l2 == NULL) return NULL;

    for(ulong_t i = 0; i < ARMV8MMU_TABLE_ENTRIES; i++)
    {
        l2[i].Value11 = UNDEFINED_BLOCK;
    }

    return (pagetable_addr_t)l2;
}

void delete_pagetable(pagetable_addr_t pagetable_base)
{
    DESC_L2 *l2 = (DESC_L2*)pagetable_base;
    
    for(ulong_t i = 0; i < ARMV8MMU_TABLE_ENTRIES; i++)
    {
        if(l2[i].Value11 == DEFINED_BLOCK)
        {
            DESC_L3 *l3 = (DESC_L3*)ARMV8MMUL2TABLEPTR(l2[i].TableAddress);
            free_physical_page(l3);
        }
    }

    free_physical_page(l2);
}

void pagetable_map(pagetable_addr_t pagetable_base, virtual_addr_t virtual_addr, physical_addr_t physical_addr, page_access_enum_t page_access)
{
    DESC_L2 *l2 = (DESC_L2*)pagetable_base;
    ulong_t l2_index = ((ullong_t)virtual_addr)>>29 & 0x1FFF;
    ulong_t l3_index = ((ullong_t)virtual_addr)>>16 & 0x1FFF;
    
    DESC_L3 *l3;
    if(l2[l2_index].Value11 == UNDEFINED_BLOCK)
    {
        l3 = (DESC_L3*)alloc_physical_page();
        
        for(ulong_t i = 0; i < ARMV8MMU_TABLE_ENTRIES; i++)
        {
            l3[i].Value11 = UNDEFINED_BLOCK;
        }

        l2[l2_index].TableAddress = ARMV8MMUL2TABLEADDR((ullong_t)l3);
        l2[l2_index].Value11 = DEFINED_BLOCK;    
    }
    else
    {
        l3 = ARMV8MMUL2TABLEPTR(l2[l2_index].TableAddress);
    }

    l3[l3_index].Value11 = DEFINED_BLOCK;
	l3[l3_index].NS = 0;
	l3[l3_index].AF = 1;
	l3[l3_index].nG = 0;
	l3[l3_index].Reserved0_1 = 0;
	l3[l3_index].OutputAddress = ARMV8MMUL3PAGEADDR ((ullong_t)physical_addr);
	l3[l3_index].Reserved0_2 = 0;
	l3[l3_index].Continous = 0;
	l3[l3_index].UXN = 1;
	l3[l3_index].Ignored = 0;

    switch (page_access)
    {
        case PA_KERNEL: l3[l3_index].AP = ATTRIB_AP_RW_EL1; break;
        case PA_USER_READ_ONLY: l3[l3_index].AP = ATTRIB_AP_RO_ALL; break;
        case PA_USER_READ_WRITE: l3[l3_index].AP = ATTRIB_AP_RW_ALL; break;
        default: PA_KERNEL: l3[l3_index].AP = ATTRIB_AP_RW_EL1; break;
    }

    if(physical_addr >= physical_mem_info.program_end ) l3[l3_index].PXN = 1;
    else l3[l3_index].PXN = 0;

    if(physical_addr >= MMIO_BASE) //physical_addr is a device address
    {
        l3[l3_index].AttrIndx = ATTRINDX_DEVICE;
        l3[l3_index].SH = ATTRIB_SH_OUTER_SHAREABLE;
    }
    else
    {
        l3[l3_index].AttrIndx = ATTRINDX_NORMAL;
        l3[l3_index].SH = ATTRIB_SH_INNER_SHAREABLE;
    }
}

void pagetable_unmap(pagetable_addr_t pagetable_base, virtual_addr_t virtual_addr)
{
    DESC_L2 *l2 = (DESC_L2*)pagetable_base;
    ulong_t l2_index = ((ullong_t)virtual_addr)>>29 & 0x1FFF;
    ulong_t l3_index = ((ullong_t)virtual_addr)>>16 & 0x1FFF;
    
    DESC_L3 *l3;
    if(l2[l2_index].Value11 == DEFINED_BLOCK)
    {
        l3 = ARMV8MMUL2TABLEPTR(l2[l2_index].TableAddress);
        l3[l3_index].Value11 = UNDEFINED_BLOCK;

        //check if the l3 is empty
        for(ulong_t i = 0; i < ARMV8MMU_TABLE_ENTRIES; i++)
        {
            if(l3[l3_index].Value11 == DEFINED_BLOCK) return;
        }
        //if true, free the containing page
        free_physical_page(l3);
        l2[l2_index].Value11 = UNDEFINED_BLOCK;
    }
}

void pagetable_map_linear(pagetable_addr_t pagetable_base, physical_addr_t start_addr, physical_addr_t end_addr, page_access_enum_t page_access)
{
    ullong_t addr = ((ullong_t)start_addr) & ~((ullong_t)0xFFFF);
    while (addr < end_addr)
    {
        pagetable_map(pagetable_base, (virtual_addr_t)addr, (physical_addr_t)addr, page_access);
        addr += physical_mem_info.page_length;
    }
}

physical_addr_t pagetable_get_translation(pagetable_addr_t pagetable_base, virtual_addr_t virtual_addr)
{
    DESC_L2 *l2 = (DESC_L2*)pagetable_base;
    ulong_t l2_index = ((ullong_t)virtual_addr)>>29 & 0x1FFF;
    ulong_t l3_index = ((ullong_t)virtual_addr)>>16 & 0x1FFF;
    
    if(l2[l2_index].Value11 == DEFINED_BLOCK)
    {
        DESC_L3 *l3 = (DESC_L3*)ARMV8MMUL2TABLEPTR(l2[l2_index].TableAddress);
        
        if(l3[l3_index].Value11 == DEFINED_BLOCK) 
            return (physical_addr_t)ARMV8MMUL3PAGEPTR(l3[l3_index].OutputAddress);
        else
            return NO_TRANSLATION;
    }
    else
        return NO_TRANSLATION;
}

