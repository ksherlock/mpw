#ifndef __mpw_sane_h__
#define __mpw_sane_h__

#include <cstdint>

namespace SANE
{
	uint32_t decstr68k(uint16_t trap);
	uint16_t fp68k(uint16_t trap);
}
#endif
