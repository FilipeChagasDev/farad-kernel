/* 
 * --- KERNEL.F SOURCE CODE FILE ---
 * 
 * Author: Filipe Chagas Ferraz
 * Email: filipe.ferraz0@gmail.com
 * GitHub: github.com/filipechagasdev
 * 
 */

#ifndef filesystem_devicefile_h
#define filesystem_devicefile_h

#include <filesystem/filehandler.h>
#include <types.h>

/*
Pseudofile is a file that is used for kernel streaming. It has no data to write to storage,
but it can be used as a read-write interface by applications to securely communicate with kernel modules.
Event callbacks from these files are handled by a devicefile_interface_t.
*/

typedef struct 
{

    status_t (*on_write)   ( file_handler_t *handler, void *buffer, ulong_t buff_size );
    status_t (*on_read)    ( file_handler_t *handler, void *buffer, ulong_t buff_size );
    status_t (*on_open)    ( file_handler_t *handler );
    status_t (*on_close)   ( file_handler_t *handler );

}devicefile_interface_t;

#endif //end module