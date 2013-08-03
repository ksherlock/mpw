#ifndef __mpw_mm_h__
#define __mpw_mm_h__

#include <cstdint>

namespace MM
{
	// native functions.
	namespace Native
	{

		void MemoryInfo(uint32_t address);
		void PrintMemoryStats();

		uint16_t NewHandle(uint32_t size, bool clear, uint32_t &handle);
		uint16_t NewHandle(uint32_t size, bool clear, uint32_t &handle, uint32_t &ptr);

		uint16_t NewPtr(uint32_t size, bool clear, uint32_t &pointer);

		uint16_t DisposeHandle(uint32_t handle);
		uint16_t DisposePtr(uint32_t pointer);

		uint16_t GetHandleSize(uint32_t handle, uint32_t &handleSize);
		uint16_t HSetRBit(uint32_t handle);
		uint16_t HClrRBit(uint32_t handle);

	}

	bool Init(uint8_t *memory, uint32_t memorySize, uint32_t reserved);


	uint16_t BlockMove(uint16_t trap);
	uint32_t CompactMem(uint16_t trap);
	uint32_t MaxMem(uint16_t trap);
	uint32_t MaxBlock(uint16_t trap);
	uint32_t FreeMem(uint16_t trap);
	uint16_t ReserveMem(uint16_t trap);
	

	uint16_t DisposeHandle(uint16_t trap);
	uint16_t DisposePtr(uint16_t trap);

	uint16_t EmptyHandle(uint16_t trap);
	uint16_t ReallocHandle(uint16_t trap);

	uint32_t GetHandleSize(uint16_t);
	uint32_t GetPtrSize(uint16_t);

	uint16_t NewHandle(uint16_t trap);
	uint16_t NewPtr(uint16_t trap);

	uint16_t SetHandleSize(uint16_t);
	uint16_t SetPtrSize(uint16_t);

	uint16_t HGetState(uint16_t trap);

	uint16_t HLock(uint16_t trap);
	uint16_t HUnlock(uint16_t trap);

	uint16_t HPurge(uint16_t trap);
	
	uint16_t MoveHHi(uint16_t trap);

	uint32_t StripAddress(uint16_t trap);

	// operating system utility routines
	uint16_t HandAndHand(uint16_t trap);
	uint16_t HandToHand(uint16_t trap);
	uint16_t PtrAndHand(uint16_t trap);
	uint16_t PtrToHand(uint16_t trap);
	uint16_t PtrToXHand(uint16_t trap);

	uint32_t StackSpace(uint16_t trap);

	uint16_t HandleZone(uint16_t trap);
	uint16_t GetZone(uint16_t trap);
	uint16_t SetZone(uint16_t trap);
}



#endif