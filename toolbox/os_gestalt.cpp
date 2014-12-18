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

#include "os.h"
#include "os_internal.h"
#include "toolbox.h"
#include "stackframe.h"

using ToolBox::Log;

namespace OS {

	enum
	{
		gestaltUndefSelectorErr = -5551,
		gestaltUnknownErr = -5550,
		gestaltDupSelectorErr = -5552,
		gestaltLocationErr = -5553,
	};

	std::map<uint32_t, uint32_t> GestaltMap = {
		{'alis', 1}, // Alias Manager
		{'tmgr', 2}, // Time Manager (2 = revised, 3 = extended.)
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

}

