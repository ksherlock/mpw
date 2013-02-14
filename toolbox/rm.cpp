#include "rm.h"
#include "toolbox.h"

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

	uint32_t StackFrame(uint32_t &b, uint16_t &a)
	{
		uint32_t sp = cpuGetAReg(7);
		a = memoryReadWord(sp); sp += 2;
		b = memoryReadLong(sp); sp += 4;
		cpuSetAReg(7, sp);

		return sp;
	}

	void ToolReturn(uint32_t sp, uint32_t value)
	{
		memoryWriteLong(value, sp);
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

		std::string sname = ToolBox::ReadPString(name);

		fprintf(stderr, "%04x Get1NamedResource(%08x, %s)\n", trap, theType, sname.c_str());

		ToolReturn(sp, (uint32_t)0);
		return -192;
	}

	uint16_t GetResource(uint16_t trap)
	{
		// GetResource (theType: ResType; theID: Integer): Handle;

		/*
		 * -----------
		 * +6 outHandle
		 * ------------
		 * +2 theType
		 * ------------
		 * +0 theID
		 * ------------
		 *
		 */

		// nb - return address is not on the stack.

		uint32_t sp;
		uint32_t theType;
		uint16_t theID;

		sp = StackFrame(theType, theID);

		fprintf(stderr, "%04x GetResource(%08x, %04x)\n", trap, theType, theID);

		ToolReturn(sp, (uint32_t)0);
		return -192;
	}

}
