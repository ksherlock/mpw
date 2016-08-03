#include "fpinfo.h"

#include <machine/endian.h>


static_assert(sizeof(float) == 4, "Unexpected float size");
static_assert(sizeof(double) == 8, "Unexpected double size");
static_assert(sizeof(long double) == 8 || sizeof(long double) == 12 || sizeof(long double) == 16, "Unexpected long double size");

void fpinfo::init(float *f)
{
	union split {
		float f;
		uint32_t i;
	};

	uint32_t i = ((split *)f)->i;

	sign = i >> 31;
	one = 1;
	exp = (i >> 23) & ((1 << 8) - 1);
	sig = i & ((1 << 24) - 1);

	if (exp == 255) {
		if (sig == 0) inf = true;
		else nan = true;
		return;
	}

	if (exp == 0) {
		// 0 or denormalized.
		one = 0;
		exp = -126;
		return;
	}

	exp -= 127; // bias
}

void fpinfo::init(double *d)
{

	union split {
		double d;
		uint64_t i;
	};

	uint64_t i = ((split *)d)->i;

	sign = i >> 63;
	one = 1;
	exp = (i >> 52) & ((1 << 11) - 1);
	sig = i & ((UINT64_C(1) << 53) - 1);


	if (exp == 2047) {
		if (sig == 0) inf = true;
		else nan = true;
		return;
	}
	if (exp == 0) {
		// 0 or denormalized.
		one = 0;
		exp = -1022;
		return;
	}

	exp -= 1023; // bias
}

void fpinfo::init(long double *ld)
{

	union split {
		long double ld;
		uint64_t i[2];
	};

	uint64_t i;
	uint32_t sexp;


	// this needs to be verified.
	#if BYTE_ORDER == LITTLE_ENDIAN
	i = ((split *)ld)->i[0];
	sexp = ((split *)ld)->i[1];
	#else
	i = ((split *)ld)->i[1];
	sexp = ((split *)ld)->i[0] & 0xffff;
	#endif

	sign = (sexp >> 15) & 0x01;
	exp = sexp & ((1 << 15) - 1);

	one = i >> 63;
	sig = i & ((UINT64_C(1) << 63) - 1);

	if (exp == 32767) {
		if (sig == 0) inf = true;
		else nan = true;
		return;
	}
	// 

	exp -= 16383;
}