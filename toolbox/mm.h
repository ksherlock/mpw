#ifndef __mpw_mm_h__
#define __mpw_mm_h__

#include <cstdint>

namespace MM
{
	enum
	{
		memFullErr = -108,
		memWZErr = -111,
	};

	bool Init(uint8_t *memory, uint32_t memorySize, uint32_t reserved);

	// native functions.
	namespace Native
	{
		uint16_t NewHandle(uint32_t size, uint32_t &handle);
		uint16_t NewPtr(uint32_t size, bool clear, uint32_t &pointer);

		uint16_t DisposeHandle(uint32_t handle);
		uint16_t DisposePtr(uint32_t pointer);
	}


	uint16_t BlockMove(uint16_t trap);
	uint32_t CompactMem(uint16_t trap);

	uint16_t DisposeHandle(uint16_t trap);
	uint16_t DisposePtr(uint16_t trap);

	uint32_t GetHandleSize(uint16_t);
	uint32_t GetPtrSize(uint16_t);

	uint16_t NewHandle(uint16_t trap);
	uint16_t NewPtr(uint16_t trap);

	uint16_t SetHandleSize(uint16_t);
	uint16_t SetPtrSize(uint16_t);

	uint16_t HLock(uint16_t trap);
	uint16_t HUnlock(uint16_t trap);
}



#endif