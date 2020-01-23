#include <hardware/aarch64/rpi3/uart.h>

void main()
{
    uart_puts("Hello World!\n");
}

void main_secundary(unsigned int core)
{
    while (1);
}