/*
 * Adapted from Circle https://github.com/rsta2/circle/blob/master/lib/synchronize64.cpp
 */

#include <memory/cache.h>

#define SETWAY_LEVEL_SHIFT		1

#define L1_DATA_CACHE_SETS		128
#define L1_DATA_CACHE_WAYS		4
	#define L1_SETWAY_WAY_SHIFT		30	// 32-Log2(L1_DATA_CACHE_WAYS)
#define L1_DATA_CACHE_LINE_LENGTH	64
	#define L1_SETWAY_SET_SHIFT		6	// Log2(L1_DATA_CACHE_LINE_LENGTH)

#define L2_CACHE_SETS			512
#define L2_CACHE_WAYS			16
	#define L2_SETWAY_WAY_SHIFT		28	// 32-Log2(L2_CACHE_WAYS)
#define L2_CACHE_LINE_LENGTH		64
	#define L2_SETWAY_SET_SHIFT		6	// Log2(L2_CACHE_LINE_LENGTH)

#define DATA_CACHE_LINE_LENGTH_MIN	64		// min(L1_DATA_CACHE_LINE_LENGTH, L2_CACHE_LINE_LENGTH)


void invalidate_data_cache (void)
{
	// invalidate L1 data cache
	for (register unsigned nSet = 0; nSet < L1_DATA_CACHE_SETS; nSet++)
	{
		for (register unsigned nWay = 0; nWay < L1_DATA_CACHE_WAYS; nWay++)
		{
			register ullong_t nSetWayLevel =   nWay << L1_SETWAY_WAY_SHIFT
						    | nSet << L1_SETWAY_SET_SHIFT
						    | 0 << SETWAY_LEVEL_SHIFT;

			asm volatile ("dc isw, %0" : : "r" (nSetWayLevel) : "memory");
		}
	}

	// invalidate L2 unified cache
	for (register unsigned nSet = 0; nSet < L2_CACHE_SETS; nSet++)
	{
		for (register unsigned nWay = 0; nWay < L2_CACHE_WAYS; nWay++)
		{
			register ullong_t nSetWayLevel =   nWay << L2_SETWAY_WAY_SHIFT
						    | nSet << L2_SETWAY_SET_SHIFT
						    | 1 << SETWAY_LEVEL_SHIFT;

			asm volatile ("dc isw, %0" : : "r" (nSetWayLevel) : "memory");
		}
	}

//	DataSyncBarrier ();
}

void clean_data_cache (void)
{
	// clean L1 data cache
	for (register unsigned nSet = 0; nSet < L1_DATA_CACHE_SETS; nSet++)
	{
		for (register unsigned nWay = 0; nWay < L1_DATA_CACHE_WAYS; nWay++)
		{
			register ullong_t nSetWayLevel =   nWay << L1_SETWAY_WAY_SHIFT
						    | nSet << L1_SETWAY_SET_SHIFT
						    | 0 << SETWAY_LEVEL_SHIFT;

			asm volatile ("dc csw, %0" : : "r" (nSetWayLevel) : "memory");
		}
	}

	// clean L2 unified cache
	for (register unsigned nSet = 0; nSet < L2_CACHE_SETS; nSet++)
	{
		for (register unsigned nWay = 0; nWay < L2_CACHE_WAYS; nWay++)
		{
			register ullong_t nSetWayLevel =   nWay << L2_SETWAY_WAY_SHIFT
						    | nSet << L2_SETWAY_SET_SHIFT
						    | 1 << SETWAY_LEVEL_SHIFT;

			asm volatile ("dc csw, %0" : : "r" (nSetWayLevel) : "memory");
		}
	}

//	DataSyncBarrier ();
}
