/* 
 * --- KERNEL.F SOURCE CODE FILE ---
 * 
 * Author: Filipe Chagas Ferraz
 * Email: filipe.ferraz0@gmail.com
 * GitHub: github.com/filipechagasdev
 * 
 */

#ifndef filesystem_filehandler_h
#define filesystem_filehandler_h

#include <types.h>

typedef struct
{

    ulong_t descriptor;
    ulong_t process_id;
    string_t filename;
    string_t path;
    ulong_t length;

}file_handler_t;

#endif //end module