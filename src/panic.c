#include <panic.h>

void panic_log(char *title, char *description)
{
    uart_puts("\n[PANIC]\n");
    uart_puts(title);
    uart_puts("\n");
    uart_puts(description);
    uart_puts("\n");
    while (1) asm("nop");
}