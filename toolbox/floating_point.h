
#ifndef __floating_point_h__
#define __floating_point_h__

#include <type_traits>
#include <cstdint>
#include <cmath>
#include <cstring>

//#include <inttypes.h>


enum class endian {
		big = 1234,
		little = 3412,
		native = little
};

namespace single_traits {
	constexpr size_t bias = 127;
	constexpr size_t exponent_bits = 8;
	constexpr size_t significand_bits = 23;
	constexpr int max_exp = 127;
	constexpr int min_exp = -127;

	constexpr uint32_t significand_mask = ((1 << significand_bits) - 1);
	constexpr uint32_t sign_bit = UINT32_C(1) << 31;
	constexpr uint32_t nan_exp = UINT32_C(255) << significand_bits;

	constexpr uint32_t quiet_nan = UINT32_C(0x02) << (significand_bits - 2);
	constexpr uint32_t signaling_nan = UINT32_C(0x01) << (significand_bits - 2);
}

namespace double_traits {
	constexpr size_t bias = 1023;
	constexpr size_t exponent_bits = 11;
	constexpr size_t significand_bits = 52;
	constexpr int max_exp = 1023;
	constexpr int min_exp = -1023;

	constexpr uint64_t significand_mask = ((UINT64_C(1) << significand_bits) - 1);
	constexpr uint64_t sign_bit = UINT64_C(1) << 63;
	constexpr uint64_t nan_exp = UINT64_C(2047) << significand_bits;

	constexpr uint64_t quiet_nan = UINT64_C(0x02) << (significand_bits - 2);
	constexpr uint64_t signaling_nan = UINT64_C(0x01) << (significand_bits - 2);
}

namespace extended_traits {

	constexpr size_t bias = 16383;
	constexpr size_t exponent_bits = 15;
	constexpr size_t significand_bits = 63; // does not include explicit 1.
	constexpr int max_exp = 16383;
	constexpr int min_exp = -16383;

	constexpr uint64_t significand_mask = ((UINT64_C(1) << significand_bits) - 1);

	constexpr uint64_t quiet_nan = UINT64_C(0x02) << (significand_bits - 2);
	constexpr uint64_t signaling_nan = UINT64_C(0x01) << (significand_bits - 2);

	constexpr uint64_t one_bit = UINT64_C(0x8000000000000000);


	// stored separately.
	constexpr uint16_t sign_bit = 0x8000;
	constexpr uint16_t nan_exp = 0x7fff;

}

class fpinfo {

public:

	bool sign = false;
	bool one = false;
	int exp = 0;
	uint64_t sig = 0; // includes explicit 1 bit, adjusted to 63 bits of fraction.

	bool nan = false;
	bool inf = false;



	template<size_t size, endian byteorder>
	fpinfo(void *data);

	fpinfo(float f);
	fpinfo(double d);
	fpinfo(long double ld);
	fpinfo() = default;

	template<size_t size, endian byteorder>
	void write(void *vp) const;



	void write(float &x) const;
	void write(double &x) const;
	void write(long double &x) const;


#if 0
	enum {
		fp_zero,
		fp_infinite,
		fp_quiet_nan,
		fp_signaling_nan,
		fp_normal,
		fp_subnormal
	};
#endif

private:

	//static constexpr uint64_t one_bit = UINT64_C(0x8000000000000000);
	template<size_t size, endian byteorder> void init(const void *vp);
};

template<>
void fpinfo::init<4, endian::native>(const void *vp) {

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


template<>
void fpinfo::init<8, endian::native>(const void *vp) {

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


// read a macintosh extended...
template<>
void fpinfo::init<10, endian::big>(const void *vp) {


}


// some c compilers actually generate 128-bit floats....
template<>
void fpinfo::init<16, endian::native>(const void *vp) {

	uint64_t i;
	uint32_t sexp;

	if (endian::native == endian::little) {
		std::memcpy(&i, (const uint8_t *)vp, 8);
		std::memcpy(&sexp, (const uint8_t *)vp + 8, 4);
	} else {
		std::memcpy(&sexp, (const uint8_t *)vp + 4, 4);
		std::memcpy(&i, (const uint8_t *)vp + 8, 8);
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

	if (exp == 0 && sig != 0) {
		// denormalized.
		exp -= 16382;
		return;
	}

	// 

	if (exp) exp -= 16383;
}



fpinfo::fpinfo(float x) {
	init<sizeof(x), endian::native>(&x);
}

fpinfo::fpinfo(double x) {
	init<sizeof(x), endian::native>(&x);
}


fpinfo::fpinfo(long double x) {
	init<sizeof(x), endian::native>(&x);
}

//template<size_t size, endian byteorder> void init(const void *vp);

template<>
void fpinfo::write<4, endian::native>(void *vp) const {

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



template<>
void fpinfo::write<8, endian::native>(void *vp) const {

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

template<>
void fpinfo::write<10, endian::big>(void *vp) const {

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

	std::memcpy(vp, &i, 10);
}

template<>
void fpinfo::write<16, endian::native>(void *vp) const {
}

void fpinfo::write(float &x) const {
	write<sizeof(x), endian::native>(&x);
}
void fpinfo::write(double &x) const {
	write<sizeof(x), endian::native>(&x);		
}
void fpinfo::write(long double &x) const {
	write<sizeof(x), endian::native>(&x);		
}

namespace its_complicated {

/*
	std::string to_string(const fpinfo &fpi)
	{
	}
*/
	inline int fpclassify(const fpinfo &fpi) {
		if (fpi.nan) return FP_NAN;
		if (fpi.inf) return FP_INFINITE;
		if (fpi.sig == 0) return FP_ZERO;
		return fpi.sig >> 63 ? FP_NORMAL : FP_SUBNORMAL;
	}

	inline int signbit(const fpinfo &fpi) {
		return fpi.sign;
	}

	inline int isnan(const fpinfo &fpi) {
		return fpi.nan;
	}

	inline int isinf(const fpinfo &fpi) {
		return fpi.inf;
	}

	inline int isfinite(const fpinfo &fpi) {
		if (fpi.nan || fpi.inf) return false;
		return true;
	}

	inline int isnormal(const fpinfo &fpi) {
		if (fpi.nan || fpi.inf) return false;
		return fpi.sig >> 63;
	}
}


#endif
