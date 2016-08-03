#ifndef __fpinfo_h__
#define __fpinfo_h__

#include <cstdint>

struct fpinfo {
	bool sign = false;
	bool one = false;
	int exp = 0;
	uint64_t sig = 0;

	bool nan = false;
	bool inf = false;

	fpinfo(float f) { init(&f); }
	fpinfo(double d) { init(&d); }
	fpinfo(long double ld) {
		if (sizeof(long double) == 16 || sizeof(long double) == 12) init(&ld);
		if (sizeof(long double) == 8) init((double *)&ld);
	}

private:
	void init(float *);
	void init(double *);
	void init(long double *);
};


#endif
