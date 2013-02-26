#ifndef __mpw_rm_h__
#define __mpw_rm_h__

#include <cstdint>

namespace RM
{
	uint16_t CloseResFile(uint16_t trap);
	uint16_t GetNamedResource(uint16_t trap);
	uint16_t Get1NamedResource(uint16_t trap);
	
	uint16_t GetResource(uint16_t trap);
	uint16_t Get1Resource(uint16_t trap);

	uint16_t UnloadSeg(uint16_t trap);

	uint16_t ReleaseResource(uint16_t trap);
	uint16_t ResError(uint16_t trap);

	uint16_t OpenResFile(uint16_t trap);

	uint16_t SetResLoad(uint16_t trap);
}


#endif