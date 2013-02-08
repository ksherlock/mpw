#include "rm.h"

#include <cpu/defs.h>
#include <cpu/CpuModule.h>
#include <cpu/fmem.h>

#include <string>

namespace
{

	uint32_t PopLong()
	{
		uint32_t sp = cpuGetAReg(7);
		uint32_t value = memoryReadLong(sp);
		cpuSetAReg(7, sp + 4);
		return value;
	}


	uint16_t PopWord()
	{
		uint32_t sp = cpuGetAReg(7);
		uint16_t value = memoryReadWord(sp);
		cpuSetAReg(7, sp + 2);
		return value;
	}

	uint32_t StackFrame(uint32_t &b, uint32_t &a)
	{
		uint32_t sp = cpuGetAReg(7);
		a = memoryReadLong(sp); sp += 4;
		b = memoryReadLong(sp); sp += 4;
		cpuSetAReg(7, sp);

		return sp;
	}

	void ToolReturn(uint32_t sp, uint32_t value)
	{
		memoryWriteLong(sp, value);
	}

	std::string PString(uint32_t address)
	{
		std::string s;

		unsigned length = address == 0 ? 0 : memoryReadByte(address++);
		if (length == 0)
		{
			return s;
		}

		s.reserve(length);
		for (unsigned i = 0; i < length; ++i)
		{
			s.push_back((char)memoryReadByte(address++));
		}

		return s;
	}

}

namespace RM
{
	uint16_t Get1NamedResource(uint16_t trap)
	{
		// Get1NamedResource (theType: ResType; name: Str255) : Handle;

		/*
		 * -----------
		 * +8 outHandle
		 * ------------
		 * +4 theType
		 * ------------
		 * +0 name
		 * ------------
		 *
		 */

		// nb - return address is not on the stack.

		uint32_t sp;
		uint32_t theType;
		uint32_t name;

		sp = StackFrame(theType, name);

		std::string sname = PString(name);

		fprintf(stderr, "%04x Get1NamedResource(%08x, %s)\n", trap, theType, sname.c_str());

		ToolReturn(sp, (uint32_t)0);
		return -192;
	}
}
