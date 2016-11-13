
#include "floating_point.h"

#include <cstdint>

namespace floating_point {

	void info::read_single(const void *vp) {

		uint32_t i;
		std::memcpy(&i, vp, 4);

		sign = i >> 31;
		one = 1;
		exp = (i >> 23) & ((1 << 8) - 1);
		sig = i & ((1 << 23) - 1);

		if (exp == 255) {
			exp = 0;
			if (sig == 0) inf = true;
			else {
				nan = true;
				sig &= ~(UINT64_C(1) << 22);
			}
			return;
		}

		if (exp == 0 && sig != 0) {
			// denormalized.
			one = 0;
			sig <<= 40; //?
			exp = -126;
			return;
		}

		if (exp) exp -= 127; // bias

		// adjust to 64 bit significand.
		sig <<= 40;
		if (one) sig |= (UINT64_C(1) << 63);
	}




	void info::read_double(const void *vp) {

		uint64_t i;
		std::memcpy(&i, vp, 8);

		sign = i >> 63;
		one = 1;
		exp = (i >> 52) & ((1 << 11) - 1);
		sig = i & ((UINT64_C(1) << 52) - 1);


		if (exp == 2047) {
			exp = 0;
			if (sig == 0) inf = true;
			else {
				nan = true;
				sig &= ~(UINT64_C(1) << 51);
			}
			return;
		}
		if (exp == 0 && sig != 0) {
			// denormalized.
			one = 0;
			sig <<= 10; //?
			exp = -1022;
			return;
		}

		if (exp) exp -= 1023; // bias
		sig <<= 11;
		if (one) sig |= (UINT64_C(1) << 63);
	}


	void info::read_extended(const void *vp) {

		uint64_t i;
		uint16_t sexp;

		if (endian::native == endian::little) {
			std::memcpy(&i, (const uint8_t *)vp, 8);
			std::memcpy(&sexp, (const uint8_t *)vp + 8, 2);
		} else {
			std::memcpy(&sexp, (const uint8_t *)vp, 2);
			std::memcpy(&i, (const uint8_t *)vp + 2, 8);
		}

		sign = (sexp >> 15) & 0x01;
		exp = sexp & ((1 << 15) - 1);

		one = i >> 63;
		sig = i; // includes 1. i & ((UINT64_C(1) << 63) - 1);

		if (exp == 32767) {
			exp = 0;
			sig &= ((UINT64_C(1) << 63) - 1);
			if (sig == 0) inf = true;
			else {
				nan = true;
				sig &= ((UINT64_C(1) << 62) - 1);
			}
			return;
		}

	#if 0
		if (exp == 0 && sig != 0) {
			// denormalized.
			exp -= 16382;
			return;
		}
	#endif
		// 

		if (exp) exp -= 16383;
	}



	void info::write_single(void *vp) const {

		using namespace single_traits;

		uint32_t i = 0;

		if (sign) i |= sign_bit;

		if (nan) {
			// todo -- better signalling vs quiet...
			i |= nan_exp;
			i |= quiet_nan; // nan bit.

			unsigned tmp = sig & 0xff;
			if (!tmp) tmp = 1;
			i |= tmp;
		}
		else if (inf || exp > max_exp ) {
			i |= nan_exp; // also infinite.
		}
		else if (exp < min_exp || !one) {
			// todo -- could de-normalize here...
			// if too small ->  0.
			// no need to modify i!
		}
		else {
			// de-normalized numbers handled above (as 0)
			uint32_t e = exp + bias;
			e <<= significand_bits;
			i |= e;

			uint32_t s = sig >> (63 - significand_bits);
			// and clear 1-bit
			s &= significand_mask;		
			i |= s;
		}

		std::memcpy(vp, &i, 4);
	}

	void info::write_double(void *vp) const {

		using namespace double_traits;

		uint64_t i = 0;

		if (sign) i |= sign_bit;

		if (nan) {
			// todo -- better signalling vs quiet...
			i |= nan_exp;
			i |= quiet_nan; // nan bit.

			unsigned tmp = sig & 0xff;
			if (!tmp) tmp = 1;
			i |= tmp;
		}
		else if (inf || exp > max_exp ) {
			i |= nan_exp;
		}
		else if (exp < min_exp || !one) {
			// if too small ->  0.
			// no need to modify i!
		}
		else {
			// de-normalized numbers handled above (as 0)
			uint64_t e = exp + bias;
			e <<= significand_bits;
			i |= e;

			uint64_t s = sig >> (63 - significand_bits);
			// and clear 1-bit
			s &= significand_mask;		
			i |= s;
		}

		std::memcpy(vp, &i, 8);
	}

	void info::write_extended(void *vp) const {
		//...

		using namespace extended_traits;

		uint64_t i = 0;
		uint16_t sexp = 0;

		if (sign) sexp |= sign_bit;

		if (nan) {
			// todo -- better signalling vs quiet...
			sexp |= nan_exp;
			i |= quiet_nan; // nan bit.
			i |= one_bit;

			unsigned tmp = sig & 0xff;
			if (!tmp) tmp = 1;
			i |= tmp;
		}
		else if (inf || exp > max_exp ) {
			sexp |= nan_exp;
			i |= one_bit;
		}
		else if (exp < min_exp || !one) {
			// if too small ->  0.
			// no need to modify i!
		}
		else {
			// de-normalized numbers handled above (as 0)
			uint64_t e = exp + bias;
			sexp |= e;

			i = sig; // 1-bit already set.
		}

		uint8_t *cp = (uint8_t *)vp;
		if (endian::native == endian::little) {
			std::memcpy(cp + 0, &i, 8);
			std::memcpy(cp + 8, &sexp, 2);
		} else {
			std::memcpy(cp + 0, &sexp, 2);
			std::memcpy(cp + 2, &i, 8);
		}
	}

}
