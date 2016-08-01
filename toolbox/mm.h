#ifndef __mpw_mm_h__
#define __mpw_mm_h__

#include <cstdint>

#include <macos/tool_return.h>


namespace MM
{

	struct HandleInfo
	{
		uint32_t address = 0;
		uint32_t size = 0;
		bool locked = false;
		bool purgeable = false;
		bool resource = false;

		HandleInfo(uint32_t a = 0, uint32_t s = 0) :
			address(a), size(s)
		{}
	};


	// native functions.
	namespace Native
	{

		using MacOS::tool_return;

		struct hp {
			uint32_t handle = 0;
			uint32_t pointer = 0; 
			hp() = default;
			hp(uint32_t a, uint32_t b) : handle(a), pointer(b) {};
		};

		void MemoryInfo(uint32_t address);
		void PrintMemoryStats();

		tool_return<hp> NewHandle(uint32_t size, bool clear);

		tool_return<uint32_t> NewPtr(uint32_t size, bool clear);

		tool_return<void> DisposeHandle(uint32_t handle);
		tool_return<void> DisposePtr(uint32_t pointer);

		tool_return<uint32_t> GetHandleSize(uint32_t handle);

		tool_return<void> SetHandleSize(uint32_t handle, uint32_t newSize);
		tool_return<void> ReallocHandle(uint32_t handle, uint32_t newSize);

		tool_return<void> HSetRBit(uint32_t handle);
		tool_return<void> HClrRBit(uint32_t handle);

		tool_return<void> HLock(uint32_t handle);
		tool_return<void> HUnlock(uint32_t handle);


		tool_return<HandleInfo> GetHandleInfo(uint32_t handle);

		tool_return<uint32_t> GetHandleSize(uint32_t handle);
	}

	bool Init(uint8_t *memory, uint32_t memorySize, uint32_t globals, uint32_t stack);



	using MacOS::tool_return;




	uint16_t BlockMove(uint16_t trap);
	uint32_t CompactMem(uint16_t trap);
	uint32_t MaxMem(uint16_t trap);
	uint32_t MaxBlock(uint16_t trap);
	uint32_t FreeMem(uint16_t trap);
	uint16_t ReserveMem(uint16_t trap);
	uint32_t PurgeSpace(uint16_t trap);


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

	uint32_t RecoverHandle(uint16_t);

	uint16_t HGetState(uint16_t trap);
	uint16_t HSetState(uint16_t trap);

	uint16_t HLock(uint16_t trap);
	uint16_t HUnlock(uint16_t trap);

	uint16_t HPurge(uint16_t trap);
	uint16_t HNoPurge(uint16_t trap);

	uint16_t HSetRBit(uint16_t trap);
	uint16_t HClrRBit(uint16_t trap);


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

	uint16_t MaxApplZone(uint16_t trap);

	uint16_t SetApplLimit(uint16_t trap);

	// OS Dispatch

	uint16_t TempMaxMem(void);
	uint16_t TempFreeMem(void);
	uint16_t TempNewHandle(void);

	uint16_t TempHLock(void);
	uint16_t TempHUnlock(void);
	uint16_t TempDisposeHandle(void);
}



#endif
