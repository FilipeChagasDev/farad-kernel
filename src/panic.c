/*
 * KERNEL.F SOURCE CODE
 * Author: Filipe Chagas Ferraz
 */

#include <panic.h>

void panic_log(char *title, char *description)
{
    kernel_log_string("\n[PANIC]\n");
    kernel_log_string(title);
    kernel_log_char('\n');
    kernel_log_string(description);
    kernel_log_char('\n');
    while (1);
}