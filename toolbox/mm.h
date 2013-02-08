#ifndef __mpw_mm_h__
#define __mpw_mm_h__

#include <cstdint>

namespace MM
{
	enum
	{
		memFullErr = -108
	};

	bool Init(uint8_t *memory, uint32_t memorySize, uint32_t reserved);

	uint16_t NewPtr(uint16_t trap);
}

#endif