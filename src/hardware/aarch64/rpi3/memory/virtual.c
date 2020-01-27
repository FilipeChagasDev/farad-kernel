/*
 * KERNEL.F SOURCE CODE
 * Author: Filipe Chagas Ferraz
 */

#include <memory/virtual.h>
#include <hardware/aarch64/memory/armv8mmu.h>
#include <hardware/aarch64/rpi3/memorymap.h>
#include <memory/cache.h>

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
        l2[i].APTable = 0; //No access
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
        if(l3 == NULL) panic_log("Failure in pagetable_map", "alloc_physical_page returned null");

        for(ulong_t i = 0; i < ARMV8MMU_TABLE_ENTRIES; i++)
        {
            l3[i].Value11 = UNDEFINED_BLOCK;
            l3[i].AP = 0; //No access
        }

        l2[l2_index].TableAddress = ARMV8MMUL2TABLEADDR((ullong_t)l3);
        l2[l2_index].Value11 = DEFINED_BLOCK;
        l2[l2_index].APTable = ATTRIB_AP_RW_ALL;
        l2[l2_index].Ignored1 = 0;
        l2[l2_index].Reserved0 = 0;
        l2[l2_index].Ignored2 = 0;
        l2[l2_index].PXNTable = ATTRIB_PXN_ALLOW_EXECUTE;
        l2[l2_index].UXNTable = ATTRIB_UXN_ALLOW_EXECUTE;
        l2[l2_index].NSTable = 0;
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
	l3[l3_index].Ignored = 0;

    switch (page_access)
    {
        case PA_KERNEL: 
            l3[l3_index].AP = ATTRIB_AP_RW_EL1;
            l3[l3_index].UXN = ATTRIB_UXN_NEVER_EXECUTE; 
            break;
        case PA_USER_READ_ONLY: 
            l3[l3_index].AP = ATTRIB_AP_RO_ALL; 
            l3[l3_index].UXN = ATTRIB_UXN_ALLOW_EXECUTE;
            break;
        case PA_USER_READ_WRITE: 
            l3[l3_index].AP = ATTRIB_AP_RW_ALL; 
            l3[l3_index].UXN = ATTRIB_UXN_ALLOW_EXECUTE;
            break;
        default: 
            l3[l3_index].AP = ATTRIB_AP_RW_EL1; 
            l3[l3_index].UXN = ATTRIB_UXN_NEVER_EXECUTE;
            break;
    }

    if(physical_addr >= physical_mem_info.physical_heap_end ) l3[l3_index].PXN = 1;
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
    /*FIXME: Address translation does not work when this function is used */

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
            if(l3[l3_index].Value11 == DEFINED_BLOCK) return; //l3 is not empty
        }

        //if l3 is empty, free the containing page
        free_physical_page(l3);
        l2[l2_index].Value11 = UNDEFINED_BLOCK;
    }
}

void pagetable_map_linear(pagetable_addr_t pagetable_base, physical_addr_t start_addr, physical_addr_t end_addr, page_access_enum_t page_access)
{
    /* FIXME: Do not works with real hardware */
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

void enable_pagetable(pagetable_addr_t pagetable_base)
{
    current_pagetable = pagetable_base;

	ullong_t nMAIR_EL1 =   0xFF << ATTRINDX_NORMAL*8	// inner/outer write-back non-transient, allocating
	                | 0x04 << ATTRINDX_DEVICE*8	        // Device-nGnRE
	                | 0x00 << ATTRINDX_COHERENT*8;	    // Device-nGnRnE
	
    asm volatile ("msr mair_el1, %0" : : "r" (nMAIR_EL1));

	asm volatile ("msr ttbr0_el1, %0" : : "r" (pagetable_base));

	ullong_t nTCR_EL1 = 0;
	asm volatile ("mrs %0, tcr_el1" : "=r" (nTCR_EL1));

	nTCR_EL1 &= ~(  TCR_EL1_IPS__MASK
		      | TCR_EL1_A1
		      | TCR_EL1_TG0__MASK
		      | TCR_EL1_SH0__MASK
		      | TCR_EL1_ORGN0__MASK
		      | TCR_EL1_IRGN0__MASK
		      | TCR_EL1_EPD0
		      | TCR_EL1_T0SZ__MASK);

	nTCR_EL1 |=   TCR_EL1_EPD1
		     | TCR_EL1_TG0_64KB	    << TCR_EL1_TG0__SHIFT
		     | TCR_EL1_SH0_INNER     << TCR_EL1_SH0__SHIFT
		     | TCR_EL1_ORGN0_WR_BACK_ALLOCATE << TCR_EL1_ORGN0__SHIFT
		     | TCR_EL1_IRGN0_WR_BACK_ALLOCATE << TCR_EL1_IRGN0__SHIFT
		     | TCR_EL1_IPS_4GB	    << TCR_EL1_IPS__SHIFT
		     | TCR_EL1_T0SZ_4GB	    << TCR_EL1_T0SZ__SHIFT;

	asm volatile ("msr tcr_el1, %0" : : "r" (nTCR_EL1));

	uint_t nSCTLR_EL1 = 0;
	asm volatile ("mrs %0, sctlr_el1" : "=r" (nSCTLR_EL1));
	
    nSCTLR_EL1 &= ~(  SCTLR_EL1_WXN | SCTLR_EL1_A);
	
    nSCTLR_EL1 |=   SCTLR_EL1_I
		       | SCTLR_EL1_C
		       | SCTLR_EL1_M;

	asm volatile ("msr sctlr_el1, %0" : : "r" (nSCTLR_EL1) : "memory");
}

void switch_pagetable(pagetable_addr_t next_pagetable_base)
{
    invalidate_data_cache();
    enable_pagetable(next_pagetable_base);
}

/*
void set_ttbr0_el1(ullong_t v)
{
    asm volatile("msr ttbr0_el1, %0" : : "r" (v) : "memory");
}

void set_ttbr0_el1(ullong_t v)
{
    asm volatile("msr ttbr1_el1, %0" : : "r" (v) : "memory");
}

void set_tcr_el1(ullong_t v)
{
    asm volatile("msr tcr_el1, %0" : : "r" (v) : "memory");
}

void set_mair_el1(ullong_t v)
{
    asm volatile("msr mair_el1, %0" : : "r" (v) : "memory");
}
*/


