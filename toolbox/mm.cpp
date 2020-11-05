/*
 * Copyright (c) 2013, Kelvin W Sherlock
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "mm.h"
#include "toolbox.h"

#include <cpu/defs.h>
#include <cpu/CpuModule.h>
#include <cpu/fmem.h>

#include <string>
#include <cstring>
#include <deque>
#include <vector>
#include <map>

#include <mplite/mplite.h>
#include <macos/sysequ.h>
#include <macos/errors.h>

#include "stackframe.h"

using ToolBox::Log;


namespace
{
	mplite_t pool;

	uint8_t *Memory;
	uint32_t MemorySize;
	uint32_t HeapSize;

	// queue of free Handles
	std::deque<uint32_t> HandleQueue;

	// map of ptr -> size
	std::map<uint32_t, uint32_t> PtrMap;



	// map of handle -> size [? just use Ptr map?]
	std::map<uint32_t, MM::HandleInfo> HandleMap;

	inline MacOS::macos_error SetMemError(MacOS::macos_error error)
	{
		memoryWriteWord(error, MacOS::MemErr);
		return error;
	}

	inline MacOS::macos_error SetMemError(int16_t error)
	{
		return SetMemError((MacOS::macos_error)error);
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


	template<class Fx>
	int16_t with_handle(uint32_t handle, Fx fx)
	{
		auto iter = HandleMap.find(handle);
		if (iter == HandleMap.end()) return MacOS::memWZErr;
		return fx(iter->second);
	}
}

namespace MM
{

	bool Init(uint8_t *memory, uint32_t memorySize, uint32_t globals, uint32_t stack)
	{
		int ok;

		Memory = memory;
		MemorySize = memorySize;
		HeapSize = memorySize - stack;

		ok = mplite_init(&pool,
			memory + globals,
			memorySize - globals - stack,
			32,
			NULL);

		if (ok != MPLITE_OK) return false;

		// allocate a handle master block...

		if (!alloc_handle_block()) return false;


		// create system handles for the stack and global space?

		return true;
	}

	namespace Native {


		// debugger support.
		// print info on an address.
		void MemoryInfo(uint32_t address)
		{
			// 1. check if it's a pointer.
			{
				auto iter = PtrMap.find(address);
				if (iter != PtrMap.end())
				{
					printf("Pointer $%08x Size: $%08x\n", iter->first, iter->second);
					return;
				}
			}

			// 2. check if it's contained in a pointer
			for (const auto &kv : PtrMap)
			{
				if (address < kv.first) continue;
				if (address >= kv.first + kv.second) continue;

				printf("Pointer $%08x Size: $%08x\n", kv.first, kv.second);

				return;
			}

			// 2. check if it's a handle.
			{
				auto iter = HandleMap.find(address);
				if (iter != HandleMap.end())
				{
					const HandleInfo &info = iter->second;
					printf("Handle $%08x Pointer: $%08x Size: $%08x Flags: %c %c %c\n",
						iter->first,
						info.address,
						info.size,
						info.locked ? 'L' : ' ',
						info.purgeable ? 'P' : ' ',
						info.resource ? 'R' : ' '
					);
					return;
				}
			}

			// 3. check if the address is within a handle.
			{
				for (const auto &kv : HandleMap)
				{
					const HandleInfo &info = kv.second;

					if (!info.address) continue;

					uint32_t begin = info.address;
					uint32_t end = info.address + info.size;
					if (!info.size) end++;
					if (address >= begin && address < end)
					{
						printf("Handle $%08x Pointer: $%08x Size: $%08x Flags: %c %c %c\n",
							kv.first,
							info.address,
							info.size,
							info.locked ? 'L' : ' ',
							info.purgeable ? 'P' : ' ',
							info.resource ? 'R' : ' '
						);
						return;
					}
				}


			}

		}

		void PrintMemoryStats()
		{
			mplite_print_stats(&pool,  std::puts);

			for (const auto & kv : HandleMap)
			{
				const auto h = kv.first;
				const auto & info = kv.second;
				fprintf(stdout, "%08x %08x %08x %c %c %c\n",
					h,
					info.address,
					info.size,
					info.locked? 'L' : ' ',
					info.purgeable? 'P' : ' ',
					info.resource ? 'R' : ' '
					);
			}

		}


		uint16_t NewPtr(uint32_t size, bool clear, uint32_t &mcptr)
		{
			// native pointers.

			mcptr = 0;
			//if (size == 0) return 0;

			uint8_t *ptr = nullptr;
			ptr = (uint8_t *)mplite_malloc(&pool, size ? size : 1);
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

			// todo -- size 0 should have a ptr to differentiate
			// from purged.

			// PPCLink calls NewHandle(0) but expects a valid pointer
			// Assertion failed: *fHandle != NULL
			//if (size)
			//{
				ptr = (uint8_t *)mplite_malloc(&pool, size ? size : 1);
				if (!ptr)
				{
					HandleQueue.push_back(hh);
					return SetMemError(MacOS::memFullErr);
				}
				mcptr = ptr - Memory;

				if (clear)
					std::memset(ptr, 0, size);
			//}

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


		uint16_t ReallocHandle(uint32_t handle, uint32_t logicalSize)
		{

			auto iter = HandleMap.find(handle);

			if (iter == HandleMap.end()) return SetMemError(MacOS::memWZErr);

			auto& info = iter->second;

			if (info.locked) return SetMemError(MacOS::memLockedErr);


			uint32_t mcptr = 0;

			if (logicalSize)
			{
				// todo -- purge & retry on failure.

				void *address = mplite_malloc(&pool, logicalSize);
				if (!address) return SetMemError(MacOS::memFullErr);

				mcptr = (uint8_t *)address - Memory;
			}

			// the handle is not altered in the event of an error.
			if (info.address)
			{
				void *address = Memory + info.address;

				mplite_free(&pool, address);
			}

			info.address = mcptr;
			info.size = logicalSize;

			memoryWriteLong(mcptr, handle);

			// lock?  clear purged flag?

			return 0;

		}


		uint16_t SetHandleSize(uint32_t handle, uint32_t newSize)
		{
			if (handle == 0) return SetMemError(MacOS::nilHandleErr);

			const auto iter = HandleMap.find(handle);

			if (iter == HandleMap.end()) return SetMemError(MacOS::memWZErr);

			auto &info = iter->second;

			// 0 - no change in size.
			if (info.size == newSize) return SetMemError(0);

			uint32_t mcptr = info.address;
			uint8_t *ptr = mcptr + Memory;

			// 1. - resizing to 0.
			if (!newSize)
			{
				if (info.locked)
				{
					//return SetMemError(MacOS::memLockedErr);

					// ppclink resizes locked handles.
					info.size = 0;
					return SetMemError(0);
				}

				// todo -- size 0 should have a ptr to differentiate
				// from purged.

				mplite_free(&pool, ptr);
				info.address = 0;
				info.size = 0;

				memoryWriteLong(info.address, handle);
				return SetMemError(0);
			}

			// 2. - resizing from 0.

			if (!mcptr)
			{
				if (info.locked) return SetMemError(MacOS::memLockedErr);

				ptr = (uint8_t *)mplite_malloc(&pool, newSize);
				if (!ptr) return SetMemError(MacOS::memFullErr);

				mcptr = ptr - Memory;
				info.address = mcptr;
				info.size = newSize;

				memoryWriteLong(info.address, handle);
				return SetMemError(0);
			}

			for (unsigned i = 0; i < 2; ++i)
			{

				// 3. - locked
				if (info.locked)
				{
					if (mplite_resize(&pool, ptr, mplite_roundup(&pool, newSize)) == MPLITE_OK)
					{
						info.size = newSize;
						return SetMemError(0);
					}
				}
				else
				{

					// 4. - resize.

					ptr = (uint8_t *)mplite_realloc(&pool, ptr, mplite_roundup(&pool, newSize));

					if (ptr)
					{
						mcptr = ptr - Memory;
						info.address = mcptr;
						info.size = newSize;

						memoryWriteLong(info.address, handle);
						return SetMemError(0);
					}

				}

				fprintf(stderr, "mplite_realloc failed.\n");
				Native::PrintMemoryStats();

				if (i > 0) return SetMemError(MacOS::memFullErr);

				// purge...
				for (auto & kv : HandleMap)
				{
					uint32_t ph = kv.first;
					auto &info = kv.second;

					if (ph == handle) continue;
					if (info.size && info.purgeable && !info.locked)
					{
						mplite_free(&pool, Memory + info.address);
						info.size = 0;
						info.address = 0;

						// also need to update memory
						memoryWriteLong(0, ph);
					}
				}
			}
			return SetMemError(MacOS::memFullErr);

		}

		// template class to validate handle and work on it.
		template<class FX>
		uint16_t HandleIt(uint32_t handle, FX fx)
		{
			const auto iter = HandleMap.find(handle);

			if (iter == HandleMap.end()) return SetMemError(MacOS::memWZErr);

			auto &info = iter->second;
			fx(info);
			return SetMemError(0);
		}

		uint16_t HSetRBit(uint32_t handle)
		{
			const auto iter = HandleMap.find(handle);

			if (iter == HandleMap.end()) return SetMemError(MacOS::memWZErr);

			auto &info = iter->second;
			info.resource = true;
			return SetMemError(0);
		}

		uint16_t HClrRBit(uint32_t handle)
		{
			const auto iter = HandleMap.find(handle);

			if (iter == HandleMap.end()) return SetMemError(MacOS::memWZErr);

			auto &info = iter->second;
			info.resource = false;
			return SetMemError(0);
		}

		uint16_t HLock(uint32_t handle)
		{
			const auto iter = HandleMap.find(handle);

			if (iter == HandleMap.end()) return SetMemError(MacOS::memWZErr);

			auto &info = iter->second;
			info.locked = true;
			return SetMemError(0);
		}

		uint16_t HUnlock(uint32_t handle)
		{
			const auto iter = HandleMap.find(handle);

			if (iter == HandleMap.end()) return SetMemError(MacOS::memWZErr);

			auto &info = iter->second;
			info.locked = false;
			return SetMemError(0);
		}



	}


	#pragma mark --

	tool_return<uint32_t> GetHandleSize(uint32_t handle)
	{

		const auto iter = HandleMap.find(handle);

		if (iter == HandleMap.end()) return SetMemError(MacOS::memWZErr);

		SetMemError(0);
		return iter->second.size;
	}

	tool_return<HandleInfo> GetHandleInfo(uint32_t handle)
	{
		const auto iter = HandleMap.find(handle);

		if (iter == HandleMap.end()) return SetMemError(MacOS::memWZErr);
		SetMemError(0);
		return iter->second;
	}


	#pragma mark --


	uint16_t BlockMove(uint16_t trap)
	{
		// also implements BlockMoveData.
		// BlockMove will flush caches, BlockMoveData will not.

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

		Log("%04x StackSpace(%08x)\n", trap);


		SetMemError(0);

		// MemorySize is the top of the heap. stack is after it.

		return sp - HeapSize;
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

		if (mplite_resize(&pool, ptr, mplite_roundup(&pool, newSize)) < 0)
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

		return Native::ReallocHandle(hh, logicalSize);

#if 0
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
#endif
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

		/*
		 * The trap dispatcher sets the condition codes before returning
		 * from a trap by testing the low-order word of register D0 with
		 * a TST.W instruction. Because the block size returned in D0 by
		 * _GetHandleSize is a full 32-bit long word, the word-length
		 * test sets the condition codes incorrectly in this case. To
		 * branch on the contents of D0, use your own TST.L instruction
		 * on return from the trap to test the full 32 bits of the register.
		*/

		uint32_t hh = cpuGetAReg(0);

		Log("%04x GetHandleSize(%08x)\n", trap, hh);

		if (hh == 0) return SetMemError(MacOS::nilHandleErr); // ????

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

		return Native::SetHandleSize(hh, newSize);
	}

	uint32_t RecoverHandle(uint16_t trap)
	{
		// FUNCTION RecoverHandle (p: Ptr): Handle;

		/*
		 * on entry:
		 * A0 Master pointer
		 *
		 * on exit:
		 * A0 Handle to master pointer’s relocatable block
		 * D0 Unchanged
		 *
		 */


		uint32_t p = cpuGetAReg(0);
		uint32_t hh = 0;

		Log("%04x RecoverHandle(%08x)\n", trap, p);

		uint16_t error = MacOS::memBCErr;
		for (const auto &kv : HandleMap)
		{
			const HandleInfo &info = kv.second;

			if (!info.address) continue;

			uint32_t begin = info.address;
			uint32_t end = info.address + info.size;
			if (!info.size) end++;
			if (p >= begin && p < end)
			{
				hh = kv.first;
				error = MacOS::noErr;
				break;
			}
		}

		SetMemError(error);
		cpuSetAReg(0, hh);

		// return d0 register unchanged.
		return cpuGetDReg(0);
	}


	#pragma mark Handle attributes

	uint16_t HGetState(uint16_t trap)
	{
		/*
		 * on entry:
		 * A0 Handle
		 *
		 * on exit:
		 * D0 flag byte
		 *
		 */

		unsigned flags = 0;
		uint32_t hh = cpuGetAReg(0);

		Log("%04x HGetState(%08x)\n", trap, hh);


		auto iter = HandleMap.find(hh);

		if (iter == HandleMap.end()) return SetMemError(MacOS::memWZErr);

		/*
		 * flag bits:
		 * 0-4: reserved
		 * 5: is a resource
		 * 6: set if purgeable
		 * 7: set if locked
		 */

		const auto &info = iter->second;

		// resouce not yet supported...
		// would need extra field and support in RM:: when
		// creating.
		// see HSetRBit, HClrRBit
		if (info.resource) flags |= (1 << 5);
		if (info.purgeable) flags |= (1 << 6);
		if (info.locked) flags |= (1 << 7);

		SetMemError(0);
		return flags;
	}

	uint16_t HSetState(uint16_t trap)
	{
		/*
		 * on entry:
		 * A0 Handle
		 * D0 flags
		 *
		 * on exit:
		 * D0 flag byte
		 *
		 */

		uint32_t hh = cpuGetAReg(0);
		uint16_t flags = cpuGetDReg(0);

		Log("%04x HSetState(%08x, %04x)\n", trap, hh, flags);

		auto iter = HandleMap.find(hh);

		if (iter == HandleMap.end()) return SetMemError(MacOS::memWZErr);

		auto &info = iter->second;

		info.resource = (flags & (1 << 5));
		info.purgeable = (flags & (1 << 6));
		info.locked = (flags & (1 << 7));


		return SetMemError(0);
	}

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

		return SetMemError(0);
	}


	uint16_t HNoPurge(uint16_t trap)
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

		Log("%04x HNoPurge(%08x)\n", trap, hh);

		auto iter = HandleMap.find(hh);

		if (iter == HandleMap.end()) return SetMemError(MacOS::memWZErr);
		iter->second.purgeable = false;

		return SetMemError(0);
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
		return SetMemError(0);
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
		return SetMemError(0);
	}

	#pragma mark - OS Utility Routines


	uint16_t HandToHand(uint16_t trap)
	{
		/*
		 * on entry:
		 * A0 source Handle
		 *
		 * on exit:
		 * A0 destination Handle
		 * D0 Result code
		 *
		 */

		uint32_t srcHandle = cpuGetAReg(0);

		Log("%04x HandToHand(%08x)\n", trap, srcHandle);

		auto iter = HandleMap.find(srcHandle);
		if (iter == HandleMap.end())
			return SetMemError(MacOS::memWZErr);


		auto const info = iter->second;


		uint32_t destHandle;
		uint32_t destPtr;
		uint32_t d0 = Native::NewHandle(info.size, false, destHandle, destPtr);
		if (d0 == 0)
		{
			std::memmove(memoryPointer(destPtr), memoryPointer(info.address), info.size);
		}

		cpuSetAReg(0, destHandle);
		return d0; // SetMemError called by Native::NewHandle.
	}


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
		return d0; // SetMemError called by Native::NewHandle.
	}

	uint16_t PtrAndHand(uint16_t trap)
	{
		// FUNCTION PtrAndHand (pntr: Ptr; hndl: Handle; size: LongInt): OSErr;

		/*
		 * on entry:
		 * A0 source Pointer
		 * A1 dest Handle
		 * D0 number of bytes to concatenate
		 *
		 * on exit:
		 * A0 destination Handle
		 * D0 Result code
		 *
		 */

		uint32_t ptr = cpuGetAReg(0);
		uint32_t handle = cpuGetAReg(1);
		uint32_t size = cpuGetDReg(0);

		Log("%04x PtrAndHand(%08x, %08x, %08x)\n", trap, ptr, handle, size);

		cpuSetAReg(0, handle);

		uint32_t oldSize = 0;
		uint32_t d0;

		d0 = Native::GetHandleSize(handle, oldSize);
		if (d0) return d0;

		if ((uint64_t)oldSize + (uint64_t)size > UINT32_MAX)
			return SetMemError(MacOS::memFullErr);


		d0 = Native::SetHandleSize(handle, oldSize + size);
		if (d0) return d0;

		auto iter = HandleMap.find(handle);
		if (iter == HandleMap.end())
			return SetMemError(MacOS::memWZErr);

		auto const info = iter->second;

		std::memmove(memoryPointer(info.address + oldSize), memoryPointer(ptr), size);

		return SetMemError(0);
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

		if (MemorySize <= 0x00ffffff)
			address &= 0x00ffffff;

		return address;
	}

	#pragma mark - zone
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


	uint16_t GetZone(uint16_t trap)
	{
		// FUNCTION GetZone: THz;

		/*
		 * on entry:
		 *
		 * on exit:
		 * A0 Pointer to current heap zone
		 * D0 Result code
		 */

		 Log("%04x GetZone()\n", trap);

		 cpuSetAReg(0, 0);
		 return 0;
	}

	uint16_t SetZone(uint16_t trap)
	{
		// PROCEDURE SetZone (hz: THz);

		/*
		 * on entry:
		 * A0 Pointer to new current heap zone
		 *
		 * on exit:
		 * D0 Result code
		 */

		uint32_t THz = cpuGetAReg(0);
		Log("%04x SetZone(%08x)\n", trap, THz);

		return 0;
	}

	uint16_t MaxApplZone(uint16_t trap)
	{
		// PROCEDURE MaxApplZone;

		/*
		 * on exit:
		 * D0 Result code
		 */

		Log("%04x MaxApplZone\n", trap);

		return 0;
	}

	uint16_t SetApplLimit(uint16_t trap)
	{
		// PROCEDURE SetApplLimit (zoneLimit: Ptr);

		/*
		 * on entry
		 * A0 Pointer to desired new zone limit
		 *
		 * on exit
		 * D0 Result code
		 */

		uint32_t zoneLimit = cpuGetAReg(0);

		Log("%04x SetApplLimit(%08x)\n", trap, zoneLimit);
		return 0;
	}



	uint32_t PurgeSpace(uint16_t trap)
	{
		// PROCEDURE PurgeSpace (VAR total: LongInt; VAR contig: LongInt);

		/*
		 * Registers on exit:
		 * A0 Maximum number of contiguous bytes after purge
		 * D0 Total free memory after purge
		 */

		Log("%04x PurgeSpace()\n", trap);

		 SetMemError(0);
		 cpuSetAReg(0, mplite_maxmem(&pool));
		 return mplite_freemem(&pool);
	}

	uint16_t TempMaxMem(void)
	{
		// FUNCTION TempMaxMem (VAR grow: Size): Size;

		uint32_t address;

		uint32_t sp = StackFrame<4>(address);

		Log("     TempMaxMem(%08x)\n", address);

		if (address) memoryWriteLong(0, address);

		ToolReturn<4>(sp, mplite_maxmem(&pool));

		return SetMemError(0);
	}

	uint16_t TempFreeMem(void)
	{

		// FUNCTION TempFreeMem: LongInt;

		Log("     TempFreeMem()\n");

		ToolReturn<4>(-1, mplite_freemem(&pool));

		return SetMemError(0);
	}

	uint16_t TempNewHandle(void)
	{
		// FUNCTION TempNewHandle (logicalSize: Size;
		//                         VAR resultCode: OSErr): Handle;
		uint16_t rv;
		uint32_t logicalSize;
		uint32_t resultCode;
		uint32_t theHandle;

		uint32_t sp = StackFrame<8>(logicalSize, resultCode);

		Log("     TempNewHandle(%08x, %08x)\n", logicalSize, resultCode);

		rv = Native::NewHandle(logicalSize, true, theHandle);

		if (resultCode) memoryWriteWord(rv, resultCode);
		ToolReturn<4>(sp, theHandle);
		return rv;
	}

	uint16_t TempHLock(void)
	{
		// PROCEDURE TempHLock (theHandle: Handle; VAR resultCode: OSErr);
		uint32_t theHandle;
		uint32_t resultCode;

		StackFrame<8>(theHandle, resultCode);

		Log("     TempHLock(%08x, %08x)\n", theHandle, resultCode);

		uint16_t rv = Native::HLock(theHandle);

		if (resultCode) memoryWriteWord(rv, resultCode);
		return rv;
	}

	uint16_t TempHUnlock(void)
	{
		// PROCEDURE TempHUnlock (theHandle: Handle; VAR resultCode: OSErr);
		uint32_t theHandle;
		uint32_t resultCode;

		StackFrame<8>(theHandle, resultCode);

		Log("     TempHUnlock(%08x, %08x)\n", theHandle, resultCode);

		uint16_t rv = Native::HUnlock(theHandle);

		if (resultCode) memoryWriteWord(rv, resultCode);
		return rv;
	}


	uint16_t TempDisposeHandle(void)
	{
		// PROCEDURE TempDisposeHandle (theHandle: Handle; VAR resultCode: OSErr);
		uint32_t theHandle;
		uint32_t resultCode;

		StackFrame<8>(theHandle, resultCode);

		Log("     TempDisposeHandle(%08x, %08x)\n", theHandle, resultCode);

		uint16_t rv = Native::DisposeHandle(theHandle);

		if (resultCode) memoryWriteWord(rv, resultCode);
		return rv;
	}
}
