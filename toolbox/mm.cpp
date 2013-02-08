#include "mm.h"

#include <cpu/defs.h>
#include <cpu/CpuModule.h>
#include <cpu/fmem.h>

#include <string>
#include <deque>
#include <vector>
#include <map>

#include <mplite/mplite.h>

namespace 
{
	mplite_t pool;

	uint8_t *Memory;
	uint32_t MemorySize;

	// queue of free Handles
	std::deque<uint32_t> HandleQueue;

	// map of ptr -> size
	std::map<uint32_t, uint32_t> PtrMap;

	// map of handle -> size [? just use Ptr map?]
	std::map<uint32_t, uint32_t> HandleMap;

	bool alloc_handle_block()
	{
		const unsigned HandleCount = 128;
		uint8_t *block = (uint8_t *)mplite_malloc(&pool, 
			sizeof(uint32_t) * HandleCount);

		if (!block) return false;

		uint32_t hh = block - Memory;
		uint32_t end = hh + 128 * sizeof(uint32_t);

		for ( ; hh < end; hh += sizeof(uint32_t))
		{
			HandleQueue.push_back(hh);
		} 

		return true;
	}

}

namespace MM
{

	bool Init(uint8_t *memory, uint32_t memorySize, uint32_t reserved)
	{
		int ok;

		Memory = memory;
		MemorySize = memorySize;

		ok = mplite_init(&pool, 
			memory + reserved, 
			memorySize - reserved, 
			32, 
			NULL);

		if (ok != MPLITE_OK) return false;

		// allocate a handle block...

		if (!alloc_handle_block()) return false;

		return true;
	}

	uint16_t NewPtr(uint16_t trap)
	{
		/* on entry:
		 * A0 Number of logical bytes requested
		 * on exit:
		 * A0 Address of the new block or NIL
		 * D0 Result code
		 */

		bool clear = trap & (1 << 9);
		bool sys = trap & (1 << 10);

		uint32_t size = cpuGetAReg(0);

		fprintf(stderr, "%04x NewPtr(%08x)\n", trap, size);


		// todo -- separate pools for sys vs non-sys?
		// todo -- NewPtr(0) -- null or empty ptr?

		if (size == 0)
		{
			cpuSetAReg(0, 0);
			return 0;
		}

		uint8_t *ptr = NULL;
		ptr = (uint8_t *)mplite_malloc(&pool, size);
		if (!ptr)
		{
			cpuSetAReg(0, 0);
			return memFullErr;
		}

		if (clear)
		{
			std::memset(ptr, 0, size);
		}

		uint32_t mcptr = ptr - Memory;
		PtrMap.emplace(std::make_pair(mcptr, size));
		cpuSetAReg(0, mcptr);
		return 0;
	}


}