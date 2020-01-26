/*
 * KERNEL.F SOURCE CODE
 * Author: Filipe Chagas Ferraz
 */

#include <memory/info.h>
#include <hardware/aarch64/rpi3/mbox.h>

physical_addr_t get_cpu_memory_base()
{
    mailbox[0] = 8*4;
	mailbox[1] = MBOX_REQUEST;
	mailbox[2] = MBOX_TAG_GET_ARM_MEM;
	mailbox[3] = 8;
	mailbox[4] = 0;
	mailbox[5] = 0;
	mailbox[6] = 0;
	mailbox[7] = 0;
	
    if(!mailbox_call(MBOX_CH_PROP))
    {
        panic_log("Failure in get_cpu_memory_base", "Mailbox did not respond");
    }

    return (physical_addr_t)mailbox[5];
}

ullong_t get_cpu_memory_size()
{
    mailbox[0] = 8*4;
	mailbox[1] = MBOX_REQUEST;
	mailbox[2] = MBOX_TAG_GET_ARM_MEM;
	mailbox[3] = 8;
	mailbox[4] = 0;
	mailbox[5] = 0;
	mailbox[6] = 0;
	mailbox[7] = 0;
	
    if(!mailbox_call(MBOX_CH_PROP))
    {
        panic_log("Failure in get_cpu_memory_size", "Mailbox did not respond");
    }

    return mailbox[6];
}

physical_addr_t get_gpu_memory_base()
{
    mailbox[0] = 8 * 4;
	mailbox[1] = 0;
	mailbox[2] = MBOX_TAG_GET_VC_MEM;
	mailbox[3] = 8;
	mailbox[4] = 0;
	mailbox[5] = 0;
	mailbox[6] = 0;
	mailbox[7] = 0;

    if(!mailbox_call(MBOX_CH_PROP))
    {
        panic_log("Failure in get_gpu_memory_size", "Mailbox did not respond");
    }

    return (physical_addr_t)mailbox[5];
}

ullong_t get_gpu_memory_size()
{
    mailbox[0] = 8 * 4;
	mailbox[1] = 0;
	mailbox[2] = MBOX_TAG_GET_VC_MEM;
	mailbox[3] = 8;
	mailbox[4] = 0;
	mailbox[5] = 0;
	mailbox[6] = 0;
	mailbox[7] = 0;

    if(!mailbox_call(MBOX_CH_PROP))
    {
        //return 0x4000000; //64MiB
        panic_log("Failure in get_gpu_memory_size", "Mailbox did not respond");
    }

    return mailbox[6];
}