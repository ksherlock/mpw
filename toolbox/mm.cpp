#include "mm.h"

#include <cpu/defs.h>
#include <cpu/CpuModule.h>
#include <cpu/fmem.h>

#include <string>

#include <mplite/mplite.h>

namespace 
{
	mplite_t pool;
}

namespace MM
{

	uint16_t NewPtr(uint16_t trap)
	{
		/* on entry:
		 * A0 Number of logical bytes requested
		 * on exit:
		 * A0 Address of the new block or NIL
		 * D0 Result code
		 */

		bool clear = trap & (1 << 9);
		bool sys = trap & (1 << 10);

		uint32_t size = cpuGetAReg(0);

		fprintf(stderr, "%04x NewPtr(%08x)\n", trap, size);

		cpuSetAReg(0, 0);
		return memFullErr;
	}


}