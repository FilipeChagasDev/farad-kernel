/*
 * KERNEL.F SOURCE CODE
 * Author: Filipe Chagas Ferraz
 */

#ifndef kernelf_log_message_h
#define kernelf_log_message_h

#include <types.h>

void kernel_log_char(char c);
void kernel_log_string(string_t str);
void kernel_log_natural(ullong_t number);
void kernel_log_integer(llong_t number);
void kernel_log_hex(ullong_t value, boolean_t caps);

#endif