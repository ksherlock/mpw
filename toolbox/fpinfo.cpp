#include "fpinfo.h"

#include <config.h>
#include <include/endian.h>

static_assert(sizeof(float) == 4, "Unexpected float size");
static_assert(sizeof(double) == 8, "Unexpected double size");
static_assert(sizeof(long double) == 8 || sizeof(long double) == 12 || sizeof(long double) == 16, "Unexpected long double size");


namespace {


	template<int size>
	void init(const void *vp, fpinfo &info);

	// 32-bit float
	template<>
	void init<4>(const void *vp, fpinfo &info) {


		uint32_t i = *(uint32_t *)vp;

		info.sign = i >> 31;
		info.one = 1;
		info.exp = (i >> 23) & ((1 << 8) - 1);
		info.sig = i & ((1 << 24) - 1);

		if (info.exp == 255) {
			if (info.sig == 0) info.inf = true;
			else info.nan = true;
			return;
		}

		if (info.exp == 0) {
			// 0 or denormalized.
			info.one = 0;
			info.exp = -126;
			return;
		}

		info.exp -= 127; // bias
	}

	// 64-bit double or long double.
	template<>
	void init<8>(const void *vp, fpinfo &info) {

		uint64_t i = *(uint64_t *)vp;

		info.sign = i >> 63;
		info.one = 1;
		info.exp = (i >> 52) & ((1 << 11) - 1);
		info.sig = i & ((UINT64_C(1) << 53) - 1);


		if (info.exp == 2047) {
			if (info.sig == 0) info.inf = true;
			else info.nan = true;
			return;
		}

		if (info.exp == 0) {
			// 0 or denormalized.
			info.one = 0;
			info.exp = -1022;
			return;
		}

		info.exp -= 1023; // bias
	}

	/* solaris - 96-bit long double. */
	template<>
	void init<12>(const void *vp, fpinfo &info) {


		uint64_t i;
		uint32_t sexp;


		// this needs to be verified.
		#if BYTE_ORDER == LITTLE_ENDIAN
		i = ((uint64_t *)vp)[0];
		sexp = ((uint32_t *)vp)[2] & 0xffff;
		#else
		#error "please verify big-endian long double format."
		sexp = *((uint32_t *)vp) & 0xffff;
		i = *((uint64_t *)((uint8_t *)vp+4);
		#endif

		info.sign = (sexp >> 15) & 0x01;
		info.exp = sexp & ((1 << 15) - 1);

		info.one = i >> 63;
		info.sig = i & ((UINT64_C(1) << 63) - 1);

		if (info.exp == 32767) {
			if (info.sig == 0) info.inf = true;
			else info.nan = true;
			return;
		}
		// 

		info.exp -= 16383;

	}

	/* 128-bit long double. same format as 96 but extra padding */
	template<>
	void init<16>(const void *vp, fpinfo &info) {

		uint64_t i;
		uint32_t sexp;


		// this needs to be verified.
		#if BYTE_ORDER == LITTLE_ENDIAN
		i = ((uint64_t *)vp)[0];
		sexp = ((uint32_t *)vp)[2] & 0xffff;
		#else
		#error "please verify big-endian long double format."
		i = ((uint64_t *)vp)[1];
		sexp = ((uint64_t *)vp)[0] & 0xffff;
		#endif

		info.sign = (sexp >> 15) & 0x01;
		info.exp = sexp & ((1 << 15) - 1);

		info.one = i >> 63;
		info.sig = i & ((UINT64_C(1) << 63) - 1);

		if (info.exp == 32767) {
			if (info.sig == 0) info.inf = true;
			else info.nan = true;
			return;
		}
		// 

		info.exp -= 16383;
	}


}

fpinfo::fpinfo(const float &f) {
	init<sizeof(f)>(&f, *this);
}
fpinfo::fpinfo(const double &d) {
	init<sizeof(d)>(&d, *this);

}
fpinfo::fpinfo(const long double &ld) {
	init<sizeof(ld)>(&ld, *this);
}

#if 0
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

#endif