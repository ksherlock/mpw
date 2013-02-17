#include "mm.h"
#include "toolbox.h"

#include <cpu/defs.h>
#include <cpu/CpuModule.h>
#include <cpu/fmem.h>

#include <string>
#include <deque>
#include <vector>
#include <map>

#include <mplite/mplite.h>

using ToolBox::Log;

namespace 
{
	mplite_t pool;

	uint8_t *Memory;
	uint32_t MemorySize;

	// queue of free Handles
	std::deque<uint32_t> HandleQueue;

	// map of ptr -> size
	std::map<uint32_t, uint32_t> PtrMap;

	struct HandleInfo
	{
		uint32_t address;
		uint32_t size;

		HandleInfo(uint32_t a = 0, uint32_t s = 0) : 
			address(a), size(s)
		{}
	};

	// map of handle -> size [? just use Ptr map?]
	std::map<uint32_t, HandleInfo> HandleMap;

	inline uint16_t SetMemError(uint16_t error)
	{
		memoryWriteWord(error, 0x0220);
		return error;
	}

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

	namespace Native {

		uint16_t NewPtr(uint32_t size, bool clear, uint32_t &mcptr)
		{
			// native pointers.

			mcptr = 0;
			if (size == 0) return 0;

			uint8_t *ptr = nullptr;
			ptr = (uint8_t *)mplite_malloc(&pool, size);
			if (!ptr)
			{
				return memFullErr;
			}

			if (clear)
				std::memset(ptr, 0, size);

			mcptr = ptr - Memory;
			PtrMap.emplace(std::make_pair(mcptr, size));

			return 0;
		}
		
		uint16_t DisposePtr(uint32_t mcptr)
		{

			auto iter = PtrMap.find(mcptr);

			if (iter == PtrMap.end()) return memWZErr;
			PtrMap.erase(iter);

			uint8_t *ptr = mcptr + Memory;

			mplite_free(&pool, ptr);

			return 0;
		}


	}

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


	uint16_t BlockMove(uint16_t trap)
	{
		/* 
		 * on entry:
		 * A0 Pointer to source
		 * A1 Pointer to destination
		 * D0 Number of bytes to copy
		 *
		 * on exit:
		 * A0 Address of the new block or NIL
		 * D0 Result code
		 *
		 */

		uint32_t source = cpuGetAReg(0);
		uint32_t dest = cpuGetAReg(1);
		uint32_t count = cpuGetDReg(0);

		Log("%04x BlockMove(%08x, %08x, %08x)\n",
			trap, source, dest, count);

		// TODO -- 32-bit clean?
		// TODO -- verify within MemorySize?

		#if 0
		if (source == 0 || dest == 0 || count == 0)
			return 0;
		#endif
		
		std::memmove(Memory + dest, Memory + source, count);

		return 0;
	}


	uint32_t CompactMem(uint16_t trap)
	{
		// todo -- add function to check pool for largest block?

		/* 
		 * on entry:
		 * D0: cbNeeded (long word)
		 *
		 * on exit:
		 * D0: function result (long word)
		 *
		 */
		 uint32_t cbNeeded = cpuGetDReg(0);

		 Log("%04x CompactMem(%08x)\n", trap, cbNeeded);

		 // todo -- add mplite call to return total free mem, largest block.
		 return 0x0f0000;
	}



	#pragma mark Pointers



	uint16_t NewPtr(uint16_t trap)
	{
		/* 
		 * on entry:
		 * D0 Number of logical bytes requested
		 *
		 * on exit:
		 * A0 Address of the new block or NIL
		 * D0 Result code
		 *
		 */


		bool clear = trap & (1 << 9);
		//bool sys = trap & (1 << 10);

		uint32_t size = cpuGetDReg(0);

		Log("%04x NewPtr(%08x)\n", trap, size);

		// todo -- separate pools for sys vs non-sys?
		// todo -- NewPtr(0) -- null or empty ptr?

		uint32_t mcptr;
		uint16_t error;
		error = Native::NewPtr(size, clear, mcptr);

		cpuSetAReg(0, mcptr);
		return SetMemError(error);
	}

	uint16_t DisposePtr(uint16_t trap)
	{
		/* 
		 * on entry:
		 * A0 Pointer to the nonrelocatable block to be disposed of
		 *
		 * on exit:
		 * D0 Result code
		 *
		 */

		uint32_t mcptr = cpuGetAReg(0);

		Log("%04x DisposePtr(%08x)\n", trap, mcptr);


		uint16_t error;
		error = Native::DisposePtr(mcptr);

		return SetMemError(error);
	}

	uint32_t GetPtrSize(uint16_t trap)
	{
		/* 
		 * on entry:
		 * A0 pointer
		 *
		 * on exit:
		 * D0 size (32-bit) or error code
		 *
		 */

		uint32_t mcptr = cpuGetAReg(0);

		Log("%08x GetPtrSize(%08x,)\n", trap, mcptr);

		auto iter = PtrMap.find(mcptr);

		if (iter == PtrMap.end()) return SetMemError(memWZErr);

		return iter->second;
	}

	uint16_t SetPtrSize(uint16_t trap)
	{
		/* 
		 * on entry:
		 * A0 pointer
		 * D0 new size
		 *
		 * on exit:
		 * D0 Result code
		 *
		 */

		uint32_t mcptr = cpuGetAReg(0);
		uint32_t newSize = cpuGetDReg(0);

		Log("%08x SetPtrSize(%08x, %08x)\n", trap, mcptr, newSize);

		auto iter = PtrMap.find(mcptr);

		if (iter == PtrMap.end()) return SetMemError(memWZErr);

		uint8_t *ptr = mcptr + Memory;

		if (mplite_resize(&pool, ptr, newSize) < 0)
		{
			return SetMemError(memFullErr);
		}

		// update the size.
		iter->second = newSize;

		return SetMemError(0);
	}

	#pragma mark Handles

	uint16_t NewHandle(uint16_t trap)
	{
		/* 
		 * on entry:
		 * D0 Number of logical bytes requested
		 *
		 * on exit:
		 * A0 Address of the new handle or NIL
		 * D0 Result code
		 *
		 */

		uint32_t hh = 0;
		uint8_t *ptr;
		uint32_t mcptr;

		bool clear = trap & (1 << 9);
		//bool sys = trap & (1 << 10);

		uint32_t size = cpuGetDReg(0);

		Log("%04x NewHandle(%08x)\n", trap, size);

		if (!HandleQueue.size())
		{
			if (!alloc_handle_block())
			{
				cpuSetAReg(0, 0);
				return SetMemError(memFullErr);
			}
		}

		hh = HandleQueue.front();
		HandleQueue.pop_front();

		ptr = nullptr;
		mcptr = 0;

		if (size)
		{
			ptr = (uint8_t *)mplite_malloc(&pool, size);
			if (!ptr)
			{
				HandleQueue.push_back(hh);
				cpuSetAReg(0, 0);
				return SetMemError(memFullErr);
			}
			mcptr = ptr - Memory; 

			if (clear)
				std::memset(ptr, 0, size);
		}

		// need a handle -> ptr map?
		HandleMap.emplace(std::make_pair(hh, HandleInfo(mcptr, size)));

		memoryWriteLong(mcptr, hh);
		cpuSetAReg(0, hh);
		return SetMemError(0);
	}

	uint16_t DisposeHandle(uint16_t trap)
	{
		/* 
		 * on entry:
		 * A0 Handle to be disposed of
		 *
		 * on exit:
		 * D0 Result code
		 *
		 */

		uint32_t hh = cpuGetAReg(0);

		Log("%04x DisposeHandle(%08x)\n", trap, hh);


		auto iter = HandleMap.find(hh);

		if (iter == HandleMap.end()) return SetMemError(memWZErr);

		HandleInfo info = iter->second;

		HandleMap.erase(iter);

		uint8_t *ptr = info.address + Memory;

		mplite_free(&pool, ptr);

		HandleQueue.push_back(hh);

		return SetMemError(0);
	}

	#pragma mark Handle attributes

	// these are all nops for now.

	uint16_t HLock(uint16_t trap)
	{
		/* 
		 * on entry:
		 * A0 Handle
		 *
		 * on exit:
		 * D0 Result code
		 *
		 */

		uint32_t hh = cpuGetAReg(0);

		Log("%04x HLock(%08x)\n", trap, hh);

		auto iter = HandleMap.find(hh);

		if (iter == HandleMap.end()) return SetMemError(memWZErr);

		return 0;
	}

	uint16_t HUnlock(uint16_t trap)
	{
		/* 
		 * on entry:
		 * A0 Handle
		 *
		 * on exit:
		 * D0 Result code
		 *
		 */

		uint32_t hh = cpuGetAReg(0);

		Log("%04x HUnlock(%08x)\n", trap, hh);

		auto iter = HandleMap.find(hh);

		if (iter == HandleMap.end()) return SetMemError(memWZErr);

		return 0;
	}

}