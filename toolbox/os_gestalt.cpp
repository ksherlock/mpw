/*
 * Copyright (c) 2014, Charles Lehner
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

#include <cerrno>
#include <cctype>
#include <ctime>
#include <algorithm>
#include <chrono>
#include <deque>
#include <string>
#include <map>
#include <type_traits>

#include <sys/xattr.h>
#include <sys/stat.h>
#include <sys/paths.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <strings.h>

#include <cpu/defs.h>
#include <cpu/CpuModule.h>
#include <cpu/fmem.h>

#include <macos/errors.h>

#include "os.h"
#include "os_internal.h"
#include "toolbox.h"
#include "stackframe.h"

using ToolBox::Log;

#define FOUR_CHAR_CODE(x) x

namespace OS {

	enum
	{
		gestaltUndefSelectorErr = -5551,
		gestaltUnknownErr = -5550,
		gestaltDupSelectorErr = -5552,
		gestaltLocationErr = -5553,
	};


	enum {
		gestaltOSAttr                 = FOUR_CHAR_CODE('os  '), /* o/s attributes */
		gestaltSysZoneGrowable        = 0,    /* system heap is growable */
		gestaltLaunchCanReturn        = 1,    /* can return from launch */
		gestaltLaunchFullFileSpec     = 2,    /* can launch from full file spec */
		gestaltLaunchControl          = 3,    /* launch control support available */
		gestaltTempMemSupport         = 4,    /* temp memory support */
		gestaltRealTempMemory         = 5,    /* temp memory handles are real */
		gestaltTempMemTracked         = 6,    /* temporary memory handles are tracked */
		gestaltIPCSupport             = 7,    /* IPC support is present */
		gestaltSysDebuggerSupport     = 8,    /* system debugger support is present */
		gestaltNativeProcessMgrBit    = 19    /* the process manager itself is native */
	};


	template<unsigned...>
	struct make_bitmask;

	template<>
	struct make_bitmask<> : public std::integral_constant<uint32_t, 0>
	{};

	template<unsigned First, unsigned... Rest>
	struct make_bitmask<First, Rest...> : 
	public std::integral_constant<uint32_t, (1 << First) | make_bitmask<Rest...>::value > 
	{};




	std::map<uint32_t, uint32_t> GestaltMap = {
		{'alis', 1}, // Alias Manager
		{'tmgr', 2}, // Time Manager (2 = revised, 3 = extended.)
		{
			// enable temp memory for codewarrior
			gestaltOSAttr, 
			make_bitmask<gestaltTempMemSupport, gestaltRealTempMemory, gestaltTempMemTracked>::value
		},
	};

	uint16_t Gestalt(uint16_t trap)
	{
		// FUNCTION Gestalt (selector: OSType; VAR response: LongInt): OSErr;

		/*
		 * on entry:
		 * D0 Selector code
		 *
		 * on exit:
		 * A0 Response
		 * D0 Result code
		 *
		 */

		uint32_t selector = cpuGetDReg(0);
		uint32_t response;

		Log("%04x Gestalt('%s')\n", trap, ToolBox::TypeToString(selector).c_str());

		auto iter = GestaltMap.find(selector);

		if (iter == GestaltMap.end()) return gestaltUndefSelectorErr;
		response = iter->second;
		cpuSetAReg(0, response);

		return 0;
	}


	uint16_t SysEnvirons(uint16_t trap)
	{
		//  FUNCTION SysEnvirons (versionRequested: Integer;
		//                        VAR theWorld: SysEnvRec): OSErr;

		/*
		 * on entry:
		 * D0 Version requested
		 * A0 SysEnvRec pointer
		 *
		 * on exit:
		 * D0 Result code
		 *
		 */

		enum {
			/* SysEnvRec */
			_environsVersion = 0,
			_machineType = 2,
			_systemVersion = 4,
			_processor = 6,
			_hasFPU = 8,
			_hasColorQD = 9,
			_keyBoardType = 10,
			_atDrvrVersNum = 12,
			_sysVRefNum = 14,
		};

		uint16_t versionRequested = cpuGetDReg(0);
		uint32_t theWorld = cpuGetAReg(0);

		Log("%04x SysEnvirons(%04x, %08x)\n", trap, versionRequested, theWorld);

		memoryWriteWord(2, theWorld + _environsVersion);

		// negative version.
		if (versionRequested >= 0x8000)
			return MacOS::envBadVers;

		if (versionRequested > 2)
			return MacOS::envVersTooBig;

		memoryWriteWord(0, theWorld + _machineType); // 0 = unknown model newer than the IIci (v1) or IIfx (v2)
		memoryWriteWord(1 + cpuGetModelMajor(), theWorld + _processor);
		memoryWriteWord(0x0700, theWorld + _systemVersion); // system 7
		memoryWriteWord(0, theWorld + _hasFPU);
		memoryWriteWord(0, theWorld + _hasColorQD);
		memoryWriteWord(5, theWorld + _keyBoardType); // standard adb I guess
		memoryWriteWord(0, theWorld + _atDrvrVersNum); // no appletalk
		memoryWriteWord(-1, theWorld + _sysVRefNum); // System folder #

		return 0;
	}

}
