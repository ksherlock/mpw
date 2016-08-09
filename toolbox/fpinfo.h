#ifndef __fpinfo_h__
#define __fpinfo_h__

#include <cstdint>

struct fpinfo {

private:
	/*
	void init(float *);
	void init(double *);
	void init(long double *);
	*/

public:

	bool sign = false;
	bool one = false;
	int exp = 0;
	uint64_t sig = 0;

	bool nan = false;
	bool inf = false;

	fpinfo(const float &f);
	fpinfo(const double &d);
	fpinfo(const long double &ld);
};


#endif
