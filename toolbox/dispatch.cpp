#include <cstdio>
#include <cstdint>

#include <cpu/defs.h>
#include <cpu/CpuModule.h>
#include <cpu/fmem.h>

#include "toolbox.h"

// yuck.  TST.W d0
extern "C" void cpuSetFlagsNZ00NewW(UWO res);



void toolbox(uint16_t trap)
{
	// todo -- remove extra bits for save a0, toolglue

	uint32_t d0 = 0;
	switch (trap)
	{

		_Get1NamedResource
		// Get1NamedResource (theType: ResType; name: Str255) : Handle;
		case 0xa820:
			d0 = RM::Get1NamedResource();
			break;

		default:
			fprintf(stderr, "Unsupported tool trap: %08x\n", trap);
			fprintf(stderr, "pc: %08x\n", cpuGetPC());
			exit(255);
	}

	cpuSetDReg(0, d0);
	cpuSetFlagsNZ00NewW(d0);
}