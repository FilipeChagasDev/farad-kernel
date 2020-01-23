/*
 * KERNEL.F SOURCE CODE
 * Author: Filipe Chagas Ferraz
 */

#ifndef kernelf_hardware_rpi3_memorymap_h
#define kernelf_hardware_rpi3_memorymap_h

#ifndef MEGABYTE
    #define MEGABYTE	0x100000
#endif
    
#ifndef GIGABYTE
    #define GIGABYTE	0x40000000UL
#endif

#define MMIO_BASE       0x3F000000

#endif //end module
