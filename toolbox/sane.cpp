#include "qd.h"
#include "toolbox.h"

#include <cpu/defs.h>
#include <cpu/CpuModule.h>
#include <cpu/fmem.h>

#include <stdlib.h>
#include <string>

#include "stackframe.h"

using ToolBox::Log;


namespace SANE
{

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
	float readnum<float>(uint32_t address)
	{
		static_assert(sizeof(float) == 4, "unexpected long double size");

		uint32_t x = memoryReadLong(address);
		return *((float *)&x);
	}

	template<>
	double readnum<double>(uint32_t address)
	{
		static_assert(sizeof(double) == 8, "unexpected long double size");

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



	uint16_t fl2x()
	{
		// long to extended (80-bit fp)
		uint16_t op;
		uint32_t dest;
		uint32_t src;

		StackFrame<10>(src, dest, op);

		Log("     FL2X(%08x, %08x, %04x)\n", src, dest, op);

		int32_t i = readnum<int32_t>(src);

		if (ToolBox::Trace)
		{
			std::string tmp1 = std::to_string(i);
			Log("     %s\n", tmp1.c_str());
		}

		writenum<extended>((extended)i, dest);

		return 0;
	}

#if 0
	uint16_t fdivx()
	{
		// div extended (80-bit fp)
		uint16_t op;
		uint32_t dest;
		uint32_t src;

		StackFrame<10>(src, dest, op);

		Log("     FDIVX(%08x, %08x, %04x)\n", src, dest, op);

		extended s = readnum<extended>(src);
		extended d = readnum<extended>(dest);

		if (ToolBox::Trace)
		{
			std::string tmp1 = std::to_string(d);
			std::string tmp2 = std::to_string(s);
			Log("     %s / %s\n", tmp1.c_str(), tmp2.c_str());
		}

		// dest = dest / src
		d = d / s;

		writenum<extended>((extended)d, dest);

		return 0;
	}

	uint16_t fmulx()
	{
		// multiply extended (80-bit fp)
		uint16_t op;
		uint32_t dest;
		uint32_t src;

		StackFrame<10>(src, dest, op);

		Log("     FMULX(%08x, %08x, %04x)\n", src, dest, op);

		extended s = readnum<extended>(src);
		extended d = readnum<extended>(dest);

		if (ToolBox::Trace)
		{
			std::string tmp1 = std::to_string(d);
			std::string tmp2 = std::to_string(s);
			Log("     %s * %s\n", tmp1.c_str(), tmp2.c_str());
		}

		d = d * s;

		writenum<extended>((extended)d, dest);

		return 0;
		return 0;
	}
#endif

	uint16_t fx2dec()
	{
		// extended (80-bit fp) to decimal
		// convert a to d based on decform f
		uint16_t op;
		uint32_t f_adr;
		uint32_t a_adr;
		uint32_t d_adr;

		StackFrame<14>(f_adr, a_adr, d_adr, op);

		Log("     FX2DEC(%08x, %08x, %08x, %04x)\n", f_adr, a_adr, d_adr, op);

		fprintf(stderr, "warning: FX2DEC not yet implemented\n");

		extended s = readnum<extended>(a_adr);

		if (ToolBox::Trace)
		{
			std::string tmp1 = std::to_string(s);
			Log("     %s\n", tmp1.c_str());
		}

		// ugh, really don't want to write this code right now.
		memoryWriteWord(0, d_adr);
		memoryWriteWord(0, d_adr + 2);
		memoryWriteWord(0, d_adr + 4);

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
			std::string tmp1 = std::to_string(s);
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


	extern "C" void cpuSetFlagsAbs(UWO f);
	uint16_t fp68k(uint16_t trap)
	{
		uint16_t op;
		uint32_t sp;

		sp = cpuGetAReg(7);
		op = memoryReadWord(sp);

		Log("%04x FP68K(%04x)\n", op);

		cpuSetFlagsAbs(0x4);

		if (op == 0x000b) return fx2dec();

		switch(op)
		{
			// addition
			case 0x0000: return fadd<extended>("FADDX");
			case 0x0800: return fadd<double>("FADDD");
			case 0x1000: return fadd<float>("FADDS");
			//case 0x3000: return fadd<complex>("FADDC");
			case 0x2000: return fadd<int16_t>("FADDI");
			case 0x2800: return fadd<int32_t>("FADDL");

			// subtraction
			case 0x0002: return fsub<extended>("FSUBX");
			case 0x0802: return fsub<double>("FSUBD");
			case 0x1002: return fsub<float>("FSUBS");
			//case 0x3002: return fsub<complex>("FSUBC");
			case 0x2002: return fsub<int16_t>("FSUBI");
			case 0x2802: return fsub<int32_t>("FSUBL");			

			// multiplication
			case 0x0004: return fmul<extended>("FMULX");
			case 0x0804: return fmul<double>("FMULD");
			case 0x1004: return fmul<float>("FMULS");
			//case 0x3004: return fmul<complex>("FMUlC");
			case 0x2004: return fmul<int16_t>("FMULI");
			case 0x2804: return fmul<int32_t>("FMULL");

			// division
			case 0x0006: return fdiv<extended>("FDIVX");
			case 0x0806: return fdiv<double>("FDIVD");
			case 0x1006: return fdiv<float>("FDIVS");
			//case 0x3006: return fdiv<complex>("FDIVC");
			case 0x2006: return fdiv<int16_t>("FDIVI");
			case 0x2806: return fdiv<int32_t>("FDIVL");

			// comparison
			case 0x0008: return fcmp<extended>("FCMPX");
			case 0x0808: return fcmp<double>("FCMPD");
			case 0x1008: return fcmp<float>("FCMPS");
			//case 0x3008: return fcmp<complex>("FCMPC");
			case 0x2008: return fcmp<int16_t>("FCMPI");
			case 0x2808: return fcmp<int32_t>("FCMPL");

			case 0x000a: return fcmp<extended>("FCPXX");
			case 0x080a: return fcmp<double>("FCPXD");
			case 0x100a: return fcmp<float>("FCPXS");
			//case 0x300a: return fcmp<complex>("FCPXC");
			case 0x200a: return fcmp<int16_t>("FCPXI");
			case 0x280a: return fcmp<int32_t>("FCPXL");


			// conversion
			case 0x000e: // 2 versions for completeness.
			case 0x0010:
				return fconvert<extended, extended>("FX2X");
				break;

			case 0x0810:
				return fconvert<extended, double>("FX2D");
				break;

			case 0x1010:
				return fconvert<extended, float>("FX2S");
				break;

			// 0x3010 - x to comp
			case 0x2010:
				return fconvert<extended, int16_t>("FX2I");
				break;

			case 0x2810:
				return fconvert<extended, int32_t>("FX2L");
				break;

			case 0x080e:
				return fconvert<double, extended>("FD2X");
				break;
			case 0x100e:
				return fconvert<float, extended>("FS2X");
				break;
			// 0x300e - comp to x
			case 0x200e:
				return fconvert<int16_t, extended>("FI2X");
				break;
			case 0x280e:
				return fconvert<int32_t, extended>("FL2X");
				break; 


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

	uint32_t fpstr2dec()
	{


		printf(stderr, "fpstr2dec is not yet supported.\n");
		exit(1);
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
			return fpstr2dec();
			break;

		default:
			fprintf(stderr, "decstr68k -- op %04x is not yet supported\n", op);
			exit(1);
		}


		return 0;
	}


}