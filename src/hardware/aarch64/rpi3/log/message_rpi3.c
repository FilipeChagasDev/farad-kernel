/*
 * KERNEL.F SOURCE CODE
 * Author: Filipe Chagas Ferraz
 */

#include <log/message.h>
#include <hardware/aarch64/rpi3/uart.h>

/*
Other implementations of message.h functions are in src/log/message.c file
*/

void kernel_log_string(string_t str)
{
    uart_puts(str);
}

void kernel_log_char(char c)
{
    uart_send(c);
}
