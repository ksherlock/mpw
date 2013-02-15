#include "qd.h"
#include "toolbox.h"

#include <cpu/defs.h>
#include <cpu/CpuModule.h>
#include <cpu/fmem.h>


namespace
{

	uint32_t StackFrame(uint32_t &a)
	{
		uint32_t sp = cpuGetAReg(7);
		a = memoryReadLong(sp); sp += 4;
		cpuSetAReg(7, sp);

		return sp;
	}

	uint32_t StackFrame(uint16_t &a)
	{
		uint32_t sp = cpuGetAReg(7);
		a = memoryReadWord(sp); sp += 2;
		cpuSetAReg(7, sp);

		return sp;
	}

	void ToolReturn(uint32_t sp, uint32_t value)
	{
		memoryWriteLong(value, sp);
	}	

}

namespace QD {


	uint16_t ShowCursor(uint16_t trap)
	{
		fprintf(stderr, "%04x ShowCursor()\n", trap);
		return 0;
	}

	uint16_t GetCursor(uint16_t trap)
	{
		uint32_t sp;
		uint16_t cursorID;

		sp = StackFrame(cursorID);

		fprintf(stderr, "%04x GetCursor(%04x)\n", trap, cursorID);


		ToolReturn(sp, 0);
		return 0;
	}

	uint16_t SetCursor(uint16_t trap)
	{
		uint32_t sp;
		uint32_t cursor;

		sp = StackFrame(cursor);

		fprintf(stderr, "%04x SetCursor(%08x)\n", trap, cursor);

		return 0;
	}


}