#include "qd.h"
#include "toolbox.h"

#include <cpu/defs.h>
#include <cpu/CpuModule.h>
#include <cpu/fmem.h>

#include <stdlib.h>

#include "stackframe.h"

using ToolBox::Log;


namespace SANE
{

	double to_double(uint64_t x)
	{
		return *((double *)&x);
	}
	uint64_t to_int64(double d)
	{
		return *((uint64_t *)&d);
	}

	uint16_t fl2x()
	{
		// long to extended (80-bit fp)
		uint16_t op;
		uint32_t dest;
		uint32_t src;

		StackFrame<10>(src, dest, op);
		//80-bit isn't popular anymore, so convert to double (64-bit)

		Log("     FL2X(%08x, %08x, %084x)\n", src, dest, op);

		int32_t i = memoryReadLong(src);
		double d = (double)i;

		int64_t i64 = to_int64(d);

		memoryWriteLongLong(i64, dest); 
		memoryWriteWord(0, dest + 8);

		return 0;
	}

	uint16_t fdivx()
	{
		// div extended (80-bit fp)
		uint16_t op;
		uint32_t dest;
		uint32_t src;

		StackFrame<10>(src, dest, op);
		//80-bit isn't popular anymore, so convert to double (64-bit)

		Log("     FDIVX(%08x, %08x, %084x)\n", src, dest, op);

		uint64_t si = memoryReadLongLong(src);
		uint64_t di = memoryReadLongLong(dest);


		double sd = to_double(si);
		double dd = to_double(di);

		// dest = dest / src
		dd = dd / sd;

		di = to_int64(dd);
		memoryWriteLongLong(di, dest);
		memoryWriteWord(0, dest + 8);

		return 0;
	}

	uint16_t fx2dec()
	{
		// extended (80-bit fp) to decimal
		// convert a to d based on decform f
		uint16_t op;
		uint32_t f_adr;
		uint32_t a_adr;
		uint32_t d_adr;

		StackFrame<14>(f_adr, a_adr, d_adr, op);

		uint64_t si = memoryReadLongLong(a_adr);
		double sd = to_double(si);

		// ugh, really don't want to write this code right now.
		memoryWriteWord(0, d_adr);
		memoryWriteWord(0, d_adr + 2);
		memoryWriteWord(0, d_adr + 4);

		return 0;
	}

	uint16_t fp68k(uint16_t trap)
	{
		uint16_t op;
		uint32_t sp;

		sp = cpuGetAReg(7);
		op = memoryReadWord(sp);

		Log("%04x FP68K(%04x)\n", op);

		if (op == 0x0006) return fdivx();
		if (op == 0x000b) return fx2dec();
		if (op == 0x280e) return fl2x();


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

	uint32_t decstr68k(uint16_t trap)
	{
		// this is a strange one since it may be sane or it may be the binary/decimal package.

		uint16_t op;

		StackFrame<2>(op);

		Log("%04x DECSTR68K(%04x)\n", op);

		switch (op)
		{
		case 0x00:
			return NumToString();
			break;
			
		case 0x01:
			return StringToNum();
			break;

		default:
			fprintf(stderr, "decstr68k -- op %04x is not yet supported\n", op);
			exit(1);
		}


		return 0;
	}


}