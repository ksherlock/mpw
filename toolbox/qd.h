#ifndef __mpw_qd_h__
#define __mpw_qd_h__

#include <cstdint>

namespace QD
{

	uint16_t ShowCursor(uint16_t trap);
	uint16_t GetCursor(uint16_t trap);
	uint16_t SetCursor(uint16_t trap);

	uint16_t GetFNum(uint16_t trap);
}

#endif
