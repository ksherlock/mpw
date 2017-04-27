/*
 * Copyright (c) 2013, Kelvin W Sherlock
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "qd.h"
#include "toolbox.h"
#include "sane.h"

#include <cpu/defs.h>
#include <cpu/CpuModule.h>
#include <cpu/fmem.h>

#include <stdlib.h>
#include <string>
#include <cstring>
#include <algorithm>
#include <cmath>

#include "stackframe.h"


#include <sane/floating_point.h>
#include <sane/sane.h>
#include <sane/comp.h>

using ToolBox::Log;

enum {
	SIGDIGLEN = 20,
};

namespace SANE
{

using std::to_string;
using std::fpclassify;
using std::signbit;
using std::abs;

namespace fp = floating_point;

	namespace {
		// default environment is:
		// rounding direction: to nearest
		// rounding precision: extended
		// exception flags: clear
		// halts: clear

		/*
		 * Environment: x d d e e e e e x p p h h h h h
		 * x   - reserved
		 * d d - rounding direction
		 * e   - exception flags
		 * p   - rounding precision
		 * h   - halt flags
		 */

		enum {

			kExceptionInexact = 1 << 12,
			kExceptionDivideByZero = 1 << 11,
			kExceptionOverflow = 1 << 10,
			kExceptionUnderflow = 1 << 9,
			kExceptionInvalid = 1 << 8,

			kHaltInexact = 1 << 4,
			kHaltDivideByZero = 1 << 3,
			kHaltOverflow = 1 << 2,
			kHaltUnderflow = 1 << 1,
			kHaltInvalid = 1 << 0,

			kRoundingDirectionMask = 0x6000,
			kRoundingDirectionToNearest = 0x0000,
			kRoundingDirectionUpward = 0x2000,
			kRoundingDirectionDownward = 0x4000,
			kRoundingDirectionTowardZero = 0x6000,

			kRoundingPrecisionMask = 0x0060,
			kRoundingPrecisionExtended = 0x0000,
			kRoundingPrecisionDouble = 0x0020,
			kRoundingPrecisionSingle = 0x0040,
			kRoundingPrecisionUndefined = 0x0060,
		};


		const uint16_t DefaultEnvironment = 0;

		uint16_t Environment = DefaultEnvironment;





	}

	// long double is an 80-bit extended with an extra 48-bits of 0 padding.
	typedef long double extended;



	template <class T>
	T readnum(uint32_t address);

	template<>
	int16_t readnum<int16_t>(uint32_t address)
	{
		return memoryReadWord(address);
	}

	template<>
	int32_t readnum<int32_t>(uint32_t address)
	{
		return memoryReadLong(address);
	}


	template<>
	int64_t readnum<int64_t>(uint32_t address)
	{
		return memoryReadLongLong(address);
	}


	template<>
	comp readnum<comp>(uint32_t address)
	{
		return comp(memoryReadLongLong(address));
	}


	template<>
	float readnum<float>(uint32_t address)
	{
		static_assert(sizeof(float) == 4, "unexpected float size");

		uint32_t x = memoryReadLong(address);
		return *((float *)&x);
	}

	template<>
	double readnum<double>(uint32_t address)
	{
		static_assert(sizeof(double) == 8, "unexpected double size");

		uint64_t x = memoryReadLongLong(address);
		return *((double *)&x);
	}

	template<>
	long double readnum<long double>(uint32_t address)
	{
		char buffer[16];

		static_assert(sizeof(long double) == 16, "unexpected long double size");


		// read and swap 10 bytes
		// this is very much little endian.

		for (unsigned i = 0; i < 10; ++i)
		{
			buffer[9 - i] = memoryReadByte(address + i);
		}
		// remainder are 0-filled.
		for (unsigned i = 10; i < 16; ++i)
			buffer[i] = 0;

		// now cast...

		return *((long double *)buffer);
	}


	template<class T>
	void writenum(T value, uint32_t address);

	template<>
	void writenum<int16_t>(int16_t value, uint32_t address)
	{
		memoryWriteWord(value, address);
	}

	template<>
	void writenum<int32_t>(int32_t value, uint32_t address)
	{
		memoryWriteLong(value, address);
	}

	template<>
	void writenum<int64_t>(int64_t value, uint32_t address)
	{
		memoryWriteLongLong(value, address);
	}

	template<>
	void writenum<comp>(comp value, uint32_t address)
	{
		memoryWriteLongLong((uint64_t)value, address);
	}

	template<>
	void writenum<float>(float value, uint32_t address)
	{
		static_assert(sizeof(value) == 4, "unexpected float size");

		memoryWriteLong(*((uint32_t *)&value), address);
	}

	template<>
	void writenum<double>(double value, uint32_t address)
	{
		static_assert(sizeof(value) == 8, "unexpected double size");

		memoryWriteLongLong(*((uint64_t *)&value), address);
	}

	template<>
	void writenum<long double>(long double value, uint32_t address)
	{
		static_assert(sizeof(value) == 16, "unexpected long double size");

		char buffer[16];

		std::memcpy(buffer, &value, sizeof(value));

		// copy 10 bytes over
		// little-endian specific.
		for(unsigned i = 0; i < 10; ++i)
			memoryWriteByte(buffer[9 - i], address + i);
	}

	decform read_decform(uint32_t address)
	{
		enum {
			_style = 0,
			_unused = 1,
			_digits = 2,
		};

		decform d;
		if (!address) return d;
		d.style = memoryReadByte(address + _style);
		d.digits = memoryReadWord(address + _digits);
		return d;
	}

	void write_decform(const decform &df, uint32_t address)
	{
		enum {
			_style = 0,
			_unused = 1,
			_digits = 2,
		};

		if (!address) return;
		memoryWriteByte(df.style, address + _style);
		memoryWriteByte(0, address + _unused);
		memoryWriteWord(df.digits, address + _digits);
	}

	decimal read_decimal(uint32_t address)
	{
		enum {
			_sgn = 0,
			_exp = 2,
			_sig = 4,
		};

		decimal d;
		if (!address) return d;

		d.sgn = memoryReadByte(address + _sgn);
		d.exp = (int16_t)memoryReadWord(address + _exp);
		//unsigned length = memoryReadByte(address + _sig);
		//length = std::min(length, (unsigned)SIGDIGLEN);
		d.sig = ToolBox::ReadPString(address + _sig, false);
		return d;
	}

	void write_decimal(const decimal &d, uint32_t address)
	{
		enum {
			_sgn = 0,
			_exp = 2,
			_sig = 4,
		};

		memoryWriteByte(d.sgn, address + _sgn);
		memoryWriteByte(0, address + _sgn + 1); // unused.
		memoryWriteWord(d.exp, address + _exp);

		// check if <= SIGDIGLEN?
		ToolBox::WritePString(address + _sig, d.sig);
	}


	uint16_t fx2dec()
	{
		// extended (80-bit fp) to decimal
		// convert a to d based on decform f
		// used by printf %g, %f, %e, etc


		uint16_t op;
		uint32_t f_adr;
		uint32_t a_adr;
		uint32_t d_adr;

		decform df;

		StackFrame<14>(f_adr, a_adr, d_adr, op);


		Log("     FX2DEC(%08x, %08x, %08x, %04x)\n", f_adr, a_adr, d_adr, op);

		extended s = readnum<extended>(a_adr);
		df = read_decform(f_adr);

		if (ToolBox::Trace)
		{
			std::string tmp1 = std::to_string(s);
			Log("     %s (style: %d digits: %d)\n", tmp1.c_str(), df.style, df.digits);
		}

		decimal d = x2dec(s, df);
		truncate(d, SIGDIGLEN);
		write_decimal(d, d_adr);
		return 0;
	}


	template<class SrcType, class DestType = extended>
	uint16_t fadd(const char *name)
	{
		// faddi, etc.
		// destination is always extended.
		uint16_t op;
		uint32_t dest;
		uint32_t src;

		StackFrame<10>(src, dest, op);

		Log("     %s(%08x, %08x, %04x)\n", name, src, dest, op);

		SrcType s = readnum<SrcType>(src);
		DestType d = readnum<DestType>(dest);

		if (ToolBox::Trace)
		{
			std::string tmp1 = std::to_string(d);
			std::string tmp2 = std::to_string(s);
			Log("     %s + %s\n", tmp1.c_str(), tmp2.c_str());
		}

		d = d + s;

		writenum<DestType>(d, dest);
		return 0;
	}

	template<class SrcType, class DestType = extended>
	uint16_t fsub(const char *name)
	{
		// fsub, etc.
		// destination is always extended.
		uint16_t op;
		uint32_t dest;
		uint32_t src;

		StackFrame<10>(src, dest, op);

		Log("     %s(%08x, %08x, %04x)\n", name, src, dest, op);

		SrcType s = readnum<SrcType>(src);
		DestType d = readnum<DestType>(dest);

		if (ToolBox::Trace)
		{
			std::string tmp1 = std::to_string(d);
			std::string tmp2 = std::to_string(s);
			Log("     %s - %s\n", tmp1.c_str(), tmp2.c_str());
		}

		d = d - s;

		writenum<DestType>(d, dest);
		return 0;
	}

	template<class SrcType, class DestType = extended>
	uint16_t fmul(const char *name)
	{
		// multiply extended (80-bit fp)
		uint16_t op;
		uint32_t dest;
		uint32_t src;

		StackFrame<10>(src, dest, op);

		Log("     %s(%08x, %08x, %04x)\n", name, src, dest, op);

		SrcType s = readnum<SrcType>(src);
		DestType d = readnum<DestType>(dest);

		if (ToolBox::Trace)
		{
			std::string tmp1 = std::to_string(d);
			std::string tmp2 = std::to_string(s);
			Log("     %s * %s\n", tmp1.c_str(), tmp2.c_str());
		}

		d = d * s;

		writenum<DestType>((extended)d, dest);

		return 0;
	}

	template<class SrcType, class DestType = extended>
	uint16_t fdiv(const char *name)
	{
		// div extended (80-bit fp)
		uint16_t op;
		uint32_t dest;
		uint32_t src;

		StackFrame<10>(src, dest, op);

		Log("     %s(%08x, %08x, %04x)\n", name, src, dest, op);

		SrcType s = readnum<SrcType>(src);
		DestType d = readnum<DestType>(dest);

		if (ToolBox::Trace)
		{
			std::string tmp1 = std::to_string(d);
			std::string tmp2 = std::to_string(s);
			Log("     %s / %s\n", tmp1.c_str(), tmp2.c_str());
		}

		// dest = dest / src
		d = d / s;

		writenum<DestType>(d, dest);

		return 0;
	}


	template<class SrcType, class DestType>
	uint16_t fconvert(const char *name)
	{
		// type conversion.
		uint16_t op;
		uint32_t dest;
		uint32_t src;

		StackFrame<10>(src, dest, op);

		Log("     %s(%08x, %08x, %04x)\n", name, src, dest, op);

		SrcType s = readnum<SrcType>(src);

		if (ToolBox::Trace)
		{
			std::string tmp1 = to_string(s);
			Log("     %s\n", tmp1.c_str());
		}

		writenum<DestType>((DestType)s, dest);

		return 0;
	}

	extern "C" void cpuSetFlagsShift(BOOLE z, BOOLE n, BOOLE c, BOOLE v);
	template<class SrcType, class DestType = extended>
	uint16_t fcmp(const char *name)
	{
		uint16_t op;
		uint32_t dest;
		uint32_t src;

		// TODO op & 0x0f == 0x08 vs 0x0a
		// for signaling unordered & NaN

		StackFrame<10>(src, dest, op);

		Log("     %s(%08x, %08x, %04x)\n", name, src, dest, op);

		SrcType s = readnum<SrcType>(src);
		DestType d = readnum<DestType>(dest);

		if (ToolBox::Trace)
		{
			std::string tmp1 = std::to_string(d);
			std::string tmp2 = std::to_string(s);
			Log("     %s <> %s\n", tmp1.c_str(), tmp2.c_str());
		}

		// TODO -- verify if src/dest are backwards here
		//

		//
		// check if ordered...

		if (d > s)
		{
			cpuSetFlagsShift(false, false, false, false);
			return 0;
		}
		if (d < s)
		{
			cpuSetFlagsShift(false, true, true, false);
			return 0;
		}
		if (d == s)
		{
			cpuSetFlagsShift(true, false, false, false);
			return 0;
		}

		// unorderable?
		// signal?
		cpuSetFlagsShift(false, false, false, true);
		return 0;
	}

	template <class DestType>
	uint16_t fdecimal(const char *name)
	{
		uint16_t op;
		uint32_t decimalPtr;
		uint32_t dest;


		StackFrame<10>(decimalPtr, dest, op);

		decimal d = read_decimal(decimalPtr);


		Log("     %s({%c %s e%d}, %08x)\n",
			name,
			d.sgn ? '-' : ' ', d.sig.c_str(), d.exp,
			dest
		);

		extended tmp = dec2x(d);

		writenum<DestType>((DestType)tmp, dest);

		return 0;
	}



	template <class SrcType>
	uint16_t fclassify(const char *name)
	{
		/*
		 * The classify operations set the sign of the destination to
		 * the sign of the source and the value of the destination
		 * according to the class of the source, as shown in Table E-18.
		 * The destination is an integer variable. (pg 273)
		 *
		 *
		 * Table E-18
		 * Class Information
		 * ----------------------
		 * Class of SRC     Value
		 * ----------------------
		 * Signaling NaN    1
		 * Quiet NaN        2
		 * Infinity         3
		 * Zero             4
		 * Normalized       5
		 * Denormalized     6
		 *
		 * Table E-19
		 * ---------------------------
		 * Sign of SRC     Sign of DST
		 * ---------------------------
		 * Positive        Positive
		 * Negative        Negative
		 */

		// N.B. - Sane.h uses 0-5, but how can you have integer -0?
		uint16_t op;
		uint32_t dest;
		uint32_t src;

		StackFrame<10>(src, dest, op);
		Log("     %s(%08x, %08x, %04x)\n", name, src, dest, op);

		SrcType s = readnum<SrcType>(src);

		if (ToolBox::Trace)
		{
			std::string tmp1 = to_string(s);
			Log("     %s\n", tmp1.c_str());
		}


		int16_t klass = 0;

		switch(fpclassify(s))
		{
			case FP_INFINITE:
				klass = 3;
				break;
			case FP_NAN:
				// todo -- signaling NaN is indicated
				// by the MSB of the fraction field f
				// 1 is quiet, 0 is signaling.
				klass = 1;
				break;
			case FP_NORMAL:
				klass = 5;
				break;
			case FP_SUBNORMAL:
				klass = 6;
				break;
			case FP_ZERO:
				klass = 4;
				break;

		}
		if (signbit(s)) klass = -klass;

		if (dest) {
			memoryWriteWord(klass, dest);
		}
		return 0;
	}

	#pragma mark - environment

	/*
	 * environment is a uint16_t *.
	 * void setenvironment(environment e);
	 * void getenvironment(environment *e);
	 * void procentry(environment *e);
	 * void procexit(environment e);
	 */

	uint16_t fgetenv(void)
	{
		uint32_t address;
		uint16_t op;

		StackFrame<6>(address, op);
		Log("     FGETENV(%08x)\n", address);

		memoryWriteWord(Environment, address);
		return 0;
	}


	uint16_t fsetenv(void)
	{
		uint32_t address;
		uint16_t value;
		uint16_t op;

		StackFrame<6>(address, op);
		value = address ? memoryReadWord(address) : DefaultEnvironment;
		Log("     FSETENV(%08x (%04x))\n", address, value);

		Environment = value;
		return 0;
	}

	uint16_t fprocentry(void)
	{
		uint32_t address;
		uint16_t op;

		StackFrame<6>(address, op);

		Log("     FPROCENTRY(%08x)\n", address);

		if (address) memoryWriteWord(Environment, address);
		Environment = DefaultEnvironment;

		return 0;
	}

	uint16_t fprocexit(void)
	{
		uint32_t address;
		uint16_t value;
		uint16_t op;


		StackFrame<6>(address, op);
		value = address ? memoryReadWord(address) : DefaultEnvironment;
		Log("     FPROCEXIT(%08x (%04x))\n", address, value);

		// todo -- also should signal exceptions/halts at this point.
		Environment = value;
		return 0;
	}

	uint16_t ftintx()
	{
		// truncate (round towards 0 regardless of rounding settings)

		uint32_t address;
		uint16_t op;

		StackFrame<6>(address, op);

		Log("     FTINTX(%08x)\n", address);

		extended s = readnum<extended>(address);

		if (ToolBox::Trace)
		{
			std::string tmp1 = to_string(s);
			Log("     %s\n", tmp1.c_str());
		}
		s = std::trunc(s);
		writenum<extended>(s, address);
		return 0;
	}

	extern "C" void cpuSetFlagsAbs(UWO f);
	uint16_t fp68k(uint16_t trap)
	{
		uint16_t op;
		uint32_t sp;

		sp = cpuGetAReg(7);
		op = memoryReadWord(sp);

		Log("%04x FP68K(%04x)\n", trap, op);

		cpuSetFlagsAbs(0x4);

		if (op == 0x000b) return fx2dec();

		switch(op)
		{
			// addition
			case 0x0000: return fadd<extended>("FADDX");
			case 0x0800: return fadd<double>("FADDD");
			case 0x1000: return fadd<float>("FADDS");
			//case 0x3000: return fadd<comp>("FADDC");
			case 0x2000: return fadd<int16_t>("FADDI");
			case 0x2800: return fadd<int32_t>("FADDL");

			// subtraction
			case 0x0002: return fsub<extended>("FSUBX");
			case 0x0802: return fsub<double>("FSUBD");
			case 0x1002: return fsub<float>("FSUBS");
			//case 0x3002: return fsub<comp>("FSUBC");
			case 0x2002: return fsub<int16_t>("FSUBI");
			case 0x2802: return fsub<int32_t>("FSUBL");

			// multiplication
			case 0x0004: return fmul<extended>("FMULX");
			case 0x0804: return fmul<double>("FMULD");
			case 0x1004: return fmul<float>("FMULS");
			//case 0x3004: return fmul<comp>("FMUlC");
			case 0x2004: return fmul<int16_t>("FMULI");
			case 0x2804: return fmul<int32_t>("FMULL");

			// division
			case 0x0006: return fdiv<extended>("FDIVX");
			case 0x0806: return fdiv<double>("FDIVD");
			case 0x1006: return fdiv<float>("FDIVS");
			//case 0x3006: return fdiv<comp>("FDIVC");
			case 0x2006: return fdiv<int16_t>("FDIVI");
			case 0x2806: return fdiv<int32_t>("FDIVL");

			// comparison
			case 0x0008: return fcmp<extended>("FCMPX");
			case 0x0808: return fcmp<double>("FCMPD");
			case 0x1008: return fcmp<float>("FCMPS");
			//case 0x3008: return fcmp<comp>("FCMPC");
			case 0x2008: return fcmp<int16_t>("FCMPI");
			case 0x2808: return fcmp<int32_t>("FCMPL");

			case 0x000a: return fcmp<extended>("FCPXX");
			case 0x080a: return fcmp<double>("FCPXD");
			case 0x100a: return fcmp<float>("FCPXS");
			//case 0x300a: return fcmp<comp>("FCPXC");
			case 0x200a: return fcmp<int16_t>("FCPXI");
			case 0x280a: return fcmp<int32_t>("FCPXL");


			// conversion (extended -> ???)
			case 0x0010: return fconvert<extended, extended>("FX2X");
			case 0x0810: return fconvert<extended, double>("FX2D");
			case 0x1010: return fconvert<extended, float>("FX2S");
			case 0x3010: return fconvert<extended, comp>("FX2C");
			case 0x2010: return fconvert<extended, int16_t>("FX2I");
			case 0x2810: return fconvert<extended, int32_t>("FX2L");

			// conversion (??? -> extended)
			case 0x000e: return fconvert<extended, extended>("FX2X");
			case 0x080e: return fconvert<double, extended>("FD2X");
			case 0x100e: return fconvert<float, extended>("FS2X");
			case 0x300e: return fconvert<comp, extended>("FC2X");
			case 0x200e: return fconvert<int16_t, extended>("FI2X");
			case 0x280e: return fconvert<int32_t, extended>("FL2X");


			case 0x001c: return fclassify<extended>("FCLASSX");
			case 0x081c: return fclassify<double>("FCLASSD");
			case 0x101c: return fclassify<float>("FCLASSS");
			case 0x301c: return fclassify<comp>("FCLASSC");

			case 0x0009:
				// fdec2x
				return fdecimal<extended>("FDEC2X");
				break;

			case 0x0016: return ftintx();
			case 0x0017: return fprocentry();
			case 0x0019: return fprocexit();
			case 0x0003: return fgetenv();
			case 0x0001: return fsetenv();
		}


		fprintf(stderr, "fp68k -- op %04x is not yet supported\n", op);
		exit(1);

		return 0;
	}

	uint16_t NumToString(void)
	{
		/*
		 * on entry:
		 * A0 Pointer to pascal string
		 * D0 The number
		 *
		 * on exit:
		 * A0 Pointer to pascal string
		 * D0 Result code
		 *
		 */

		int32_t theNum = cpuGetDReg(0);
		uint32_t theString = cpuGetAReg(0);

		//std::string s = ToolBox::ReadPString(theString, false);
		Log("     NumToString(%08x, %08x)\n", theNum, theString);

		std::string s = std::to_string(theNum);

		ToolBox::WritePString(theString, s);

		return 0;
	}

	uint32_t StringToNum(void)
	{
		/*
		 * on entry:
		 * A0 Pointer to pascal string
		 *
		 * on exit:
		 * D0 the number
		 *
		 */

		uint32_t theString = cpuGetAReg(0);


		std::string s = ToolBox::ReadPString(theString, false);
		Log("     StringToNum(%s)\n", s.c_str());

		bool negative = false;
		uint32_t tmp = 0;

		if (!s.length()) return 0;

		auto iter = s.begin();

		// check for leading +-
		if (*iter == '-')
		{
			negative = true;
			++iter;
		}
		else if (*iter == '+')
		{
			negative = false;
			++iter;
		}

		for ( ; iter != s.end(); ++iter)
		{
			// doesn't actually check if it's a number.
			int value = *iter & 0x0f;
			tmp = tmp * 10 + value;
		}

		if (negative) tmp = -tmp;

		return tmp;
	}

	uint32_t fstr2dec(char type)
	{
		// void str2dec(const char *s,short *ix,decimal *d,short *vp);

		uint32_t stringPtr;
		uint32_t indexPtr;
		uint32_t decimalPtr;
		uint32_t validPtr;

		uint16_t valid;
		uint16_t index;
		decimal d;

		StackFrame<16>(stringPtr, indexPtr, decimalPtr, validPtr);

		index = memoryReadWord(indexPtr);

		std::string str;
		if (type == 'P') str = ToolBox::ReadPString(stringPtr, false);
		if (type == 'C') str = ToolBox::ReadCString(stringPtr, false);

		Log("     F%cSTR2DEC(%s, %04x, %08x, %08x)\n",
			 type, str.c_str(), index, decimalPtr, validPtr);

		if (type == 'P') index--;
		str2dec(str, index, d, valid);
		if (type == 'P') index++;

		truncate(d, SIGDIGLEN);


		memoryWriteWord(index, indexPtr);
		memoryWriteWord(valid, validPtr);

		write_decimal(d, decimalPtr);

		return 0;
	}

	uint16_t fdec2str()
	{
		// void dec2str(const decform *f,const decimal *d,char *s);

		uint32_t f_adr;
		uint32_t d_adr;
		uint32_t s_adr;

		decform df;
		decimal d;

		StackFrame<12>(f_adr, d_adr, s_adr);


		Log("     FDEC2STR(%08x, %08x, %08x)\n", f_adr, d_adr, s_adr);

		df = read_decform(f_adr);
		d = read_decimal(d_adr);

		if (ToolBox::Trace)
		{
			Log("     %d %d %s\n", d.sgn, d.exp, d.sig.c_str());
			Log("     (style: %d digits: %d)\n", df.style, df.digits);
		}

		std::string s;

		dec2str(df, d, s);
		ToolBox::WritePString(s_adr, s);
		return 0;

	}

	uint32_t decstr68k(uint16_t trap)
	{
		// this is a strange one since it may be sane or it may be the binary/decimal package.

		uint16_t op;

		StackFrame<2>(op);

		Log("%04x DECSTR68K(%04x)\n", trap, op);

		switch (op)
		{
		case 0x00:
			return NumToString();
			break;

		case 0x01:
			return StringToNum();
			break;

		case 0x02:
			// fpstr2dec
			return fstr2dec('P');
			break;

		case 0x03:
			// fdec2str
			return fdec2str();
			break;

		case 0x04:
			// fcstr2dec
			return fstr2dec('C');
			break;


		default:
			fprintf(stderr, "decstr68k -- op %04x is not yet supported\n", op);
			exit(1);
		}


		return 0;
	}


}
