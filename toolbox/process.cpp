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

#include <cerrno>
#include <cctype>
#include <ctime>
#include <algorithm>
#include <chrono>
#include <deque>
#include <string>

#include <stdlib.h>
#include <unistd.h>

#include <cpu/defs.h>
#include <cpu/CpuModule.h>
#include <cpu/fmem.h>

#include <macos/sysequ.h>
#include <macos/errors.h>

#include "toolbox.h"
#include "stackframe.h"
#include "process.h"

using ToolBox::Log;

namespace Process {

	const unsigned kProcessID = 1986;

	uint16_t GetCurrentProcess()
	{
		// FUNCTION GetCurrentProcess (VAR PSN: ProcessSerialNumber): OSErr;

		uint32_t psn;
		uint32_t sp;
		sp = StackFrame<4>(psn);
		Log("     GetCurrentProcess(%08x)\n", psn);
		if (psn) memoryWriteLongLong(1, psn);
		ToolReturn<2>(sp, 0);
		return 0;
	}

	uint16_t GetProcessInformation()
	{
		// FUNCTION GetProcessInformation (PSN: ProcessSerialNumber;
		//                                 VAR info: ProcessInfoRec): OSErr;

		enum {
			/* ProcessInfoRec */
			_processInfoLength = 0,
			_processName = 4,
			_processNumber = 8,
			_processType = 16,
			_processSignature = 20,
			_processMode = 24,
			_processLocation = 28,
			_processSize = 32,
			_processFreeMem = 36,
			_processLauncher = 40,
			_processLaunchDate = 48,
			_processActiveTime = 52,
			_processAppSpec = 56,
		};

		uint32_t psn;
		uint32_t info;
		uint32_t sp;
		sp = StackFrame<8>(psn, info);
		Log("     GetProcessInformation(%08x, %08x)\n", psn, info);

		if (!psn || memoryReadLongLong(psn) != kProcessID)
		{
			ToolReturn<2>(sp, MacOS::paramErr);
			return MacOS::paramErr;
		}

		ToolReturn<2>(sp, MacOS::paramErr);
		return MacOS::paramErr;
	}


}
