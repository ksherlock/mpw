#ifndef __mpw_sane_h__
#define __mpw_sane_h__

#include <cstdint>
#include <string>

namespace SANE
{
	struct decimal {
		enum {
			SIGDIGLEN = 20,
		};

		int sgn = 0;
		int exp = 0;
		std::string sig;

		static decimal read(uint32_t address);
		void write(uint32_t address);
	};

	struct decform
	{
		enum {
			FLOATDECIMAL = 0,
			FIXEDDECIMAL = 1,
		};

		uint8_t style = 0;
		uint16_t digits = 0;

		static decform read(uint32_t address);
		void write(uint32_t address);
	};


	void str2dec(const std::string &s, uint16_t &index, decimal &d, uint16_t &vp);

	uint32_t decstr68k(uint16_t trap);
	uint16_t fp68k(uint16_t trap);
}

#endif
