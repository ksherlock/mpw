/*
 * Copyright (c) 2015, Kelvin W Sherlock
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

#include <stdlib.h>

#include <cpu/defs.h>
#include <cpu/CpuModule.h>

#include "os.h"
#include "toolbox.h"

using ToolBox::Log;

namespace OS {

	uint16_t SwapInstructionCache()
	{
		// FUNCTION SwapInstructionCache (cacheEnable: Boolean): Boolean;
		// boolean value sent/returned via a0.
		// c.f. mpw DumpObj -m SWAPINSTRUCTIONCACHE Interface.o

		uint16_t cacheEnable = cpuGetAReg(0) & 0xff;

		Log("     SwapInstructionCache(%02x)\n", cacheEnable);
		cpuSetAReg(0, 0);
		return 0;
	}

	uint16_t FlushInstructionCache()
	{
		// PROCEDURE FlushInstructionCache;

		Log("     FlushInstructionCache()\n");
		return 0;
	}

	uint16_t SwapDataCache()
	{
		// FUNCTION SwapDataCache (cacheEnable: Boolean): Boolean;
		// boolean value sent/returned via a0.
		// c.f. mpw DumpObj -m SWAPDATACACHE Interface.o

		uint16_t cacheEnable = cpuGetAReg(0) & 0xff;

		Log("     SwapDataCache(%02x)\n", cacheEnable);
		cpuSetAReg(0, 0);
		return 0;
	}

	uint16_t FlushDataCache()
	{
		// PROCEDURE FlushDataCache;

		Log("     FlushDataCache()\n");
		return 0;
	}


	uint16_t FlushCodeCacheRange()
	{
		// FUNCTION FlushCodeCacheRange (address: UNIV Ptr; count: LongInt): OSErr;

		uint32_t address = cpuGetAReg(0);
		uint32_t count = cpuGetAReg(1); 

		Log("     FlushCodeCacheRange(%08x, %08x)\n", address, count);
		return 0;
	}

	uint16_t FlushCodeCache(uint16_t trap)
	{
		// PROCEDURE FlushCodeCache;
		Log("%04x FlushCodeCache()\n", trap);
		return 0;
	}

	uint16_t HWPriv(uint16_t trap)
	{

		uint16_t selector;
		uint16_t d0 = 0;

		selector = cpuGetDReg(0) & 0xffff;
		Log("%04x HWTrap(%04x)\n", trap, selector);


		switch(selector)
		{
			case 0x0000:
				d0 = SwapInstructionCache();
				break;

			case 0x0001:
				d0 = FlushInstructionCache();
				break;

			case 0x0002:
				d0 = SwapDataCache();
				break;

			case 0x0003:
				d0 = FlushDataCache();
				break;

			case 0x0009:
				d0 = FlushCodeCacheRange();
				break;

			default:
				fprintf(stderr, "HWTrap selector %04x not yet supported\n", selector);
				exit(1);
		}

		return d0;

	}

}