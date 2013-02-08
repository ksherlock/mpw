#ifndef __mpw_rm_h__
#define __mpw_rm_h__

#include <cstdint>

namespace MM
{
	enum
	{
		memFullErr = -108
	}

	uint16_t NewPtr(uint16_t trap);
}

#endif