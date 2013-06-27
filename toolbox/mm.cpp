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
#include <macos/sysequ.h>
#include <macos/errors.h>

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
		bool locked;
		bool purgeable;

		HandleInfo(uint32_t a = 0, uint32_t s = 0) : 
			address(a), size(s), locked(false), purgeable(false)
		{}
	};

	// map of handle -> size [? just use Ptr map?]
	std::map<uint32_t, HandleInfo> HandleMap;

	inline uint16_t SetMemError(uint16_t error)
	{
		memoryWriteWord(error, MacOS::MemErr);
		return error;
	}

	bool alloc_handle_block()
	{
		const unsigned HandleCount = 128; // 512 bytes of handle blocks.

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

		// allocate a handle master block...

		if (!alloc_handle_block()) return false;

		return true;
	}

	namespace Native {



		void PrintMemoryStats()
		{
			mplite_print_stats(&pool,  std::puts);

			for (const auto & kv : HandleMap)
			{
				const auto h = kv.first;
				const auto & info = kv.second;
				fprintf(stdout, "%08x %08x %08x %c %c\n",
					h, info.address, info.size, 
					info.locked? 'L' : ' ', info.purgeable? 'P' : ' ');
			}

		}


		uint16_t NewPtr(uint32_t size, bool clear, uint32_t &mcptr)
		{
			// native pointers.

			mcptr = 0;
			if (size == 0) return 0;

			uint8_t *ptr = nullptr;
			ptr = (uint8_t *)mplite_malloc(&pool, size);
			if (!ptr)
			{
				return SetMemError(MacOS::memFullErr);
			}

			if (clear)
				std::memset(ptr, 0, size);

			mcptr = ptr - Memory;
			PtrMap.emplace(std::make_pair(mcptr, size));

			return SetMemError(0);
		}
		
		uint16_t DisposePtr(uint32_t mcptr)
		{

			auto iter = PtrMap.find(mcptr);

			if (iter == PtrMap.end()) return SetMemError(MacOS::memWZErr);
			PtrMap.erase(iter);

			uint8_t *ptr = mcptr + Memory;

			mplite_free(&pool, ptr);

			return SetMemError(0);
		}

		uint16_t NewHandle(uint32_t size, bool clear, uint32_t &handle, uint32_t &mcptr)
		{
			uint8_t *ptr;
			uint32_t hh;

			handle = 0;
			mcptr = 0;

			if (!HandleQueue.size())
			{
				if (!alloc_handle_block())
				{
					return SetMemError(MacOS::memFullErr);
				}
			}

			hh = HandleQueue.front();
			HandleQueue.pop_front();

			ptr = nullptr;

			if (size)
			{
				ptr = (uint8_t *)mplite_malloc(&pool, size);
				if (!ptr)
				{
					HandleQueue.push_back(hh);
					return SetMemError(MacOS::memFullErr);
				}
				mcptr = ptr - Memory; 

				if (clear)
					std::memset(ptr, 0, size);
			}

			// need a handle -> ptr map?
			HandleMap.emplace(std::make_pair(hh, HandleInfo(mcptr, size)));

			memoryWriteLong(mcptr, hh);
			handle = hh;
			return SetMemError(0);
		}

		uint16_t NewHandle(uint32_t size, bool clear, uint32_t &handle)
		{
			uint32_t ptr;
			return NewHandle(size, clear, handle, ptr);
		}



		uint16_t DisposeHandle(uint32_t handle)
		{
			auto iter = HandleMap.find(handle);

			if (iter == HandleMap.end()) return SetMemError(MacOS::memWZErr);

			HandleInfo info = iter->second;

			HandleMap.erase(iter);

			if (info.address)
			{
				uint8_t *ptr = info.address + Memory;

				mplite_free(&pool, ptr);
			}
			HandleQueue.push_back(handle);

			return SetMemError(0);
		}


		uint16_t GetHandleSize(uint32_t handle, uint32_t &handleSize)
		{
			handleSize = 0;

			const auto iter = HandleMap.find(handle);

			if (iter == HandleMap.end()) return SetMemError(MacOS::memWZErr);
			handleSize = iter->second.size;
			return SetMemError(0);
		}		


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


		 SetMemError(0);
		 return mplite_maxmem(&pool);
	}

	uint32_t MaxMem(uint16_t trap)
	{
		// return largest contiguous free block size.
		/* 
		 * on entry:
		 * (nothing)
		 *
		 * on exit:
		 * D0: function result (long word)
		 *
		 */

		Log("%04x MaxMem()\n", trap);

		SetMemError(0);
		return mplite_maxmem(&pool);
	}

	uint32_t MaxBlock(uint16_t trap)
	{
		/*
		 * The MaxBlock function returns the maximum contiguous space, in bytes, that you 
		 * could obtain after compacting the current heap zone. MaxBlock does not actually 
		 * do the compaction.
		 */
		 
		/* 
		 * on entry:
		 * (nothing)
		 *
		 * on exit:
		 * D0: function result (long word)
		 *
		 */

		Log("%04x MaxBlock()\n", trap);

		SetMemError(0);
		return mplite_maxmem(&pool);
	}

	uint32_t FreeMem(uint16_t trap)
	{
		// total free memory.
		/* 
		 * on entry:
		 * (nothing)
		 *
		 * on exit:
		 * D0: function result (long word)
		 *
		 */

		Log("%04x FreeMem()\n", trap);

		SetMemError(0);
		return mplite_freemem(&pool);
	}




	uint16_t ReserveMem(uint16_t trap)
	{
		/* 
		 * on entry:
		 * D0: cbNeeded (long word)
		 *
		 * on exit:
		 * D0: Result code.
		 *
		 */

		uint32_t cbNeeded = cpuGetDReg(0);
		uint32_t available;

		Log("%04x ReserveMem($%08x)\n", trap, cbNeeded);

		available = mplite_maxmem(&pool);
		// TODO -- if available < cbNeeded, purge handle and retry?
		if (available < cbNeeded) return SetMemError(MacOS::memFullErr);

		return SetMemError(0);
	}


	uint16_t MoveHHi(uint16_t trap)
	{
		/* 
		 * on entry:
		 * A0: Handle to move
		 *
		 * on exit:
		 * D0: Result code.
		 *
		 */

		uint32_t theHandle = cpuGetAReg(0);

		 Log("%04x MoveHHi(%08x)\n", trap, theHandle);

		// check if it's valid.

		auto iter = HandleMap.find(theHandle);
		if (iter == HandleMap.end()) return SetMemError(MacOS::memWZErr);

		return SetMemError(0);
	}


	uint32_t StackSpace(uint16_t trap)
	{

		/* 
		 * on entry:
		 *
		 * on exit:
		 * D0: Number of bytes between stack and heap
		 *
		 */

		uint32_t sp = cpuGetAReg(7);

		SetMemError(0);

		// find the pointer base...
		for (const auto & iter : PtrMap)
		{
			if (sp >= iter.first && sp < iter.first + iter.second)
			{
				return sp - iter.first;
			}
		}

		return 0;
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
		return error; //SetMemError(error);
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

		return error; //SetMemError(error);
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

		Log("%08x GetPtrSize(%08x)\n", trap, mcptr);

		auto iter = PtrMap.find(mcptr);

		if (iter == PtrMap.end()) return SetMemError(MacOS::memWZErr);

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

		if (iter == PtrMap.end()) return SetMemError(MacOS::memWZErr);

		uint8_t *ptr = mcptr + Memory;

		if (mplite_resize(&pool, ptr, newSize) < 0)
		{
			return SetMemError(MacOS::memFullErr);
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
		uint16_t error;

		bool clear = trap & (1 << 9);
		//bool sys = trap & (1 << 10);

		uint32_t size = cpuGetDReg(0);

		Log("%04x NewHandle(%08x)\n", trap, size);

		error = Native::NewHandle(size, clear, hh);

		cpuSetAReg(0, hh);
		return error;
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

		return Native::DisposeHandle(hh);
	}

	uint16_t EmptyHandle(uint16_t trap)
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
		Log("%04x EmptyHandle(%08x)\n", trap, hh);

		auto iter = HandleMap.find(hh);

		if (iter == HandleMap.end()) return SetMemError(MacOS::memWZErr);

		auto &info = iter->second;
		if (info.address == 0) return SetMemError(0);
		if (info.locked) return SetMemError(MacOS::memLockedErr); // ?

		void *address = Memory + info.address;

		mplite_free(&pool, address);

		info.address = 0;
		info.size = 0;

		memoryWriteLong(0, hh);
		return 0;
	}

	/*
	 * ReallocHandle (h: Handle; logicalSize: Size);
	 * 
	 * ReallocHandle allocates a new relocatable block with a logical
	 * size of logicalSize bytes. It then updates handle h by setting
	 * its master pointer to point to the new block. The main use of
	 * this procedure is to reallocate space for a block that has
	 * been purged. Normally h is an empty handle, but it need not
	 * be: If it points to an existing block, that block is released
	 * before the new block is created.
	 *
	 * In case of an error, no new block is allocated and handle h is 
	 * left unchanged.
	 */
	uint16_t ReallocHandle(uint16_t trap)
	{
		/* 
		 * on entry:
		 * A0 Handle to be disposed of
		 * D0 Logical Size
		 *
		 * on exit:
		 * D0 Result code
		 *
		 */

		uint32_t hh = cpuGetAReg(0);
		uint32_t logicalSize = cpuGetDReg(0);

		Log("%04x ReallocHandle(%08x, %08x)\n", trap, hh, logicalSize);

		auto iter = HandleMap.find(hh);

		if (iter == HandleMap.end()) return SetMemError(MacOS::memWZErr);

		auto& info = iter->second;

		if (info.locked) return SetMemError(MacOS::memLockedErr);

		if (info.address)
		{
			void *address = Memory + info.address;

			mplite_free(&pool, address);

			info.address = 0;
			info.size = 0;
			memoryWriteLong(0, hh);
		}

		// allocate a new block...
		if (logicalSize == 0) return SetMemError(0);

		void *address = mplite_malloc(&pool, logicalSize);
		if (!address) return SetMemError(MacOS::memFullErr);

		uint32_t mcptr = (uint8_t *)address - Memory;

		info.size = logicalSize;
		info.address = mcptr;

		memoryWriteLong(mcptr, hh);

		// lock?  clear purged flag?

		return 0;
	}


	uint32_t GetHandleSize(uint16_t trap)
	{
		/* 
		 * on entry:
		 * A0 handle
		 *
		 * on exit:
		 * D0 size (32-bit) or error code
		 *
		 */

		uint32_t hh = cpuGetAReg(0);

		Log("%08x GetHandleSize(%08x)\n", trap, hh);

		auto iter = HandleMap.find(hh);

		if (iter == HandleMap.end()) return SetMemError(MacOS::memWZErr);

		return iter->second.size;
	}

	uint16_t SetHandleSize(uint16_t trap)
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


		uint32_t hh = cpuGetAReg(0);
		uint32_t newSize = cpuGetDReg(0);

		Log("%04x SetHandleSize(%08x, %08x)\n", trap, hh, newSize);

		auto iter = HandleMap.find(hh);

		if (iter == HandleMap.end()) return SetMemError(MacOS::memWZErr);
		// todo -- if handle ptr is null, other logic?
		// todo -- if locked, can't move.

		auto &info = iter->second;

		// 0 - no change in size.
		if (info.size == newSize) return SetMemError(0);

		uint32_t mcptr = info.address;
		uint8_t *ptr = mcptr + Memory;


		// 1. - resizing to 0.
		if (!newSize)
		{
			if (info.locked) return SetMemError(MacOS::memLockedErr);

			mplite_free(&pool, ptr);
			info.address = 0;
			info.size = 0;
			return SetMemError(0);
		}

		// 2. - resizing from 0.

		if (!ptr)
		{
			ptr = (uint8_t *)mplite_malloc(&pool, newSize);
			if (!ptr) return SetMemError(MacOS::memFullErr);

			mcptr = ptr - Memory;
			info.address = mcptr;
			info.size = newSize;

			return SetMemError(0);
		}

		for (unsigned i = 0; i < 2; ++i)
		{

			// 3. - locked
			if (info.locked)
			{
				if (mplite_resize(&pool, ptr, newSize) == MPLITE_OK)
				{
					info.size = newSize;
					return SetMemError(0);
				}
			}
			else
			{

				// 4. - resize.

				ptr = (uint8_t *)mplite_realloc(&pool, ptr, newSize);

				if (ptr)
				{
					mcptr = ptr - Memory;
					info.address = mcptr;
					info.size = newSize;

					return SetMemError(0);
				}

			}

			fprintf(stderr, "mplite_realloc failed.\n");
			Native::PrintMemoryStats();
			

			if (i > 0) return SetMemError(MacOS::memFullErr);

			// purge...
			for (auto & kv : HandleMap)
			{
				uint32_t handle = kv.first;
				auto &info = kv.second;

				if (handle == hh) continue;
				if (info.size && info.purgeable && !info.locked)
				{
					mplite_free(&pool, Memory + info.address);
					info.size = 0;
					info.address = 0;

					// also need to update memory
					memoryWriteLong(0, handle);
				}
			}

		}

		return SetMemError(MacOS::memFullErr);
	}


	#pragma mark Handle attributes

	// these are all nops for now.

	uint16_t HPurge(uint16_t trap)
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

		Log("%04x HPurge(%08x)\n", trap, hh);

		auto iter = HandleMap.find(hh);

		if (iter == HandleMap.end()) return SetMemError(MacOS::memWZErr);
		iter->second.purgeable = true;

		return 0;
	}

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

		if (iter == HandleMap.end()) return SetMemError(MacOS::memWZErr);

		iter->second.locked = true;
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

		if (iter == HandleMap.end()) return SetMemError(MacOS::memWZErr);

		iter->second.locked = false;
		return 0;
	}

	#pragma mark - OS Utility Routines

	uint16_t PtrToHand(uint16_t trap)
	{
		/* 
		 * on entry:
		 * A0 source Pointer
		 * D0 size
		 *
		 * on exit:
		 * A0 destination pointer
		 * D0 Result code
		 *
		 */	

		uint32_t mcptr = cpuGetAReg(0);
		uint32_t size = cpuGetDReg(0);

		Log("%04x PtrToHand(%08x, %08x)\n", trap, mcptr, size);

		uint32_t destHandle;
		uint32_t destPtr;
		uint32_t d0 = Native::NewHandle(size, false, destHandle, destPtr);
		if (d0 == 0)
		{
			std::memmove(memoryPointer(destPtr), memoryPointer(mcptr), size);
		}
		
		cpuSetAReg(0, destHandle);
		return d0;
	}


	#pragma mark -
	uint32_t StripAddress(uint16_t trap)
	{
		/* 
		 * on entry:
		 * d0 Address to strip
		 *
		 * on exit:
		 * D0 Address that has been stripped.
		 *
		 */

		// TODO -- in 32-bit mode, this is a nop.
		// have a --24 / --32 flag?

		uint32_t address = cpuGetDReg(0);

		Log("%04x StripAddress(%08x)\n", trap, address);

		address &= 0x00ffffff;
		return address;
	}

	uint16_t HandleZone(uint16_t trap)
	{
		// FUNCTION HandleZone (h: Handle): THz;
		/* 
		 * on entry:
		 * A0 Handle whose zone is to be found
		 *
		 * on exit:
		 * A0 Pointer to handle’s heap zone 
		 * D0 Result code
		 *
		 */

		uint32_t h = cpuGetAReg(0);

		Log("%04x HandleZone(%08x)\n", trap, h);


		if (HandleMap.find(h) == HandleMap.end())
		{
			cpuSetAReg(0, 0);
			return SetMemError(MacOS::memWZErr);
		}

		cpuSetAReg(0, 0);
		return SetMemError(0);
	}

}