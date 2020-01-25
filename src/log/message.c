/*
 * KERNEL.F SOURCE CODE
 * Author: Filipe Chagas Ferraz
 */

#include <log/message.h>

/*
The kernel_log_string and kernel_log_char functions have specific hardware implementation.
These implementations are in other .c files in the src/hardware directory.
*/

void kernel_log_natural(ullong_t number)
{
    ullong_t next_number = number / 10;
    if(next_number > 0) kernel_log_natural(next_number);
    
    char out = '0' + (number % 10);
    kernel_log_char(out);
}

void kernel_log_integer(llong_t number)
{
    ullong_t abs;
    if(number < 0)
    {
        kernel_log_char('-');
        abs = -number;
    }
    else
    {
        abs = number;
    }

    kernel_log_natural(abs);
}

void kernel_log_hex(ullong_t value, boolean_t caps)
{
    ullong_t next_value = value / 16;
    if(next_value > 0) kernel_log_hex(next_value, caps);
    
    ullong_t digit = value % 16;

    char out;

    if(digit < 10) out = (char)((ullong_t)'0' + (ullong_t)digit);
    else out = (char)((ullong_t)(caps ? 'A' : 'a') + (ullong_t)(digit-10));
    
    kernel_log_char(out);
}