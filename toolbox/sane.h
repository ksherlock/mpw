#ifndef __mpw_sane_h__
#define __mpw_sane_h__

#include <cstdint>
#include <string>
#include <sane/sane.h>

namespace SANE {

	void str2dec(const std::string &s, uint16_t &index, decimal &d, uint16_t &vp);

	uint32_t decstr68k(uint16_t trap);
	uint16_t fp68k(uint16_t trap);
}

#endif
