/*
 * KERNEL.F SOURCE CODE
 * Author: Filipe Chagas Ferraz
 */

#include <calc.h>

ullong_t next_multiple(ullong_t factor, ullong_t intermediary)
{
    ullong_t mod = intermediary % factor;
    ullong_t distance = (mod == 0 ? 0 : factor - mod);
    return intermediary + distance;
}

ullong_t prev_multiple(ullong_t factor, ullong_t intermediary)
{
    ullong_t mod = intermediary % factor;
    return intermediary - mod;
}
