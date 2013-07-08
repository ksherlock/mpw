#ifndef __mpw_sane_h__
#define __mpw_sane_h__

#include <cstdint>
#include <string>

namespace SANE
{
	struct decimal {
		decimal() : sgn(0), exp(0) {}
		int sgn;
		int exp;
		std::string sig;
	};

	void str2dec(const std::string &s, uint16_t &index, decimal &d, uint16_t &vp);

	uint32_t decstr68k(uint16_t trap);
	uint16_t fp68k(uint16_t trap);
}
#endif
