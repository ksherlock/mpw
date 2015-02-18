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

#include <cpu/defs.h>
#include <cpu/CpuModule.h>
#include <cpu/fmem.h>

#include <macos/sysequ.h>
#include <macos/errors.h>
#include <macos/traps.h>

#include "toolbox.h"
#include "stackframe.h"
#include "os.h"
#include "packages.h"

using ToolBox::Log;
using OS::MacToUnix;

namespace Packages {


	uint16_t IUDateString()
	{
		// void IUDateString(long dateTime,DateForm longFlag,Str255 result)
		// DateForm doesn't seem to do anything.

		char buffer[256];
		int length;
		std::string out;

		uint32_t dateTime;
		uint8_t flag;
		uint32_t result;


		StackFrame<10>(dateTime, flag, result);

		Log("     IUDateString(%08x, %02x, %08x)\n", dateTime, flag, result);

		time_t t = MacToUnix(dateTime);
		struct tm *tm = ::localtime(&t);

		// not strictly correct -- uses %d/%d/%2d form.
		length = std::strftime(buffer, sizeof(buffer), "%m/%d/%y", tm);
		out.assign(buffer, length);

		ToolBox::WritePString(result, out);
		return 0;
	}

	uint16_t IUTimeString()
	{
		// void IUTimeString(long dateTime,Boolean wantSeconds,Str255 result)
		// output: 12:00:00 AM or 12:00 AM

		char buffer[256];
		int length;
		std::string out;

		uint32_t dateTime;
		uint8_t wantSeconds;
		uint32_t result;


		StackFrame<10>(dateTime, wantSeconds, result);
		Log("     IUTimeString(%08x, %02x, %08x)\n", dateTime, wantSeconds, result);

		time_t t = MacToUnix(dateTime);
		struct tm *tm = ::localtime(&t);

		length = std::strftime(buffer, sizeof(buffer), wantSeconds ? "%I:%M:%S %p" : "%I:%M %p", tm);

		out.assign(buffer, length);

		ToolBox::WritePString(result, out);
		return 0;
	}

	uint16_t GetIntlResource()
	{
		// FUNCTION GetIntlResource (theID: Integer) :Handle;

		// todo -- actually load the resource

		/*
		 * theID Contains an integer (0, 1, 2, 4, or 5 respectively for
		 * the 'itl0', 'itl1', 'itl2',  'itl4', and 'itl5'resources) to
		 * identify the type of the desired international resource.
		 */


		uint16_t theID;

		uint32_t sp = StackFrame<2>(theID);

		Log("     GetIntlResource(%04x)\n", theID);

		ToolReturn<4>(sp, 0);
		return 0; // should set res error.
	}

	uint16_t Pack6(uint16_t trap)
	{

		uint16_t selector;
		StackFrame<2>(selector);

		Log("%04x Pack6(%04x)\n", trap, selector);

		switch(selector)
		{
		case 0x0000:
			return IUDateString();
		case 0x0002:
			return IUTimeString();
		case 0x0006:
			return GetIntlResource();

		default:
			fprintf(stderr, "Pack6: selector %04x not supported\n", selector);
			exit(1);
		}
		return 0;
	}


}
