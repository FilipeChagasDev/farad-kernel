/*
 * KERNEL.F SOURCE CODE
 * Author: Filipe Chagas Ferraz
 */

#ifndef kernelf_types_h
#define kernelf_types_h

#define NULL ((void*)0)

typedef void* virtual_addr_t;
typedef void* physical_addr_t;

typedef unsigned char byte_t;
typedef unsigned long ulong_t;
typedef unsigned long long ullong_t;
typedef unsigned int uint_t;
typedef unsigned short ushort_t;

typedef long long llong_t;

typedef unsigned char boolean_t;
#define TRUE ((boolean_t)1)
#define FALSE ((boolean_t)0)

typedef char* string_t;
typedef string_t* string_array_t;

typedef enum { STATUS_SUCCESS, STATUS_FAILURE } status_t;

#endif //end module