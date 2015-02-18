/*
 * Copyright (c) 2013, Kelvin W Sherlock
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

#include "qd.h"
#include "toolbox.h"

#include <cpu/defs.h>
#include <cpu/CpuModule.h>
#include <cpu/fmem.h>

#include <macos/sysequ.h>

#include "stackframe.h"

using ToolBox::Log;


namespace QD {


	uint16_t ShowCursor(uint16_t trap)
	{
		Log("%04x ShowCursor()\n", trap);
		return 0;
	}

	uint16_t GetCursor(uint16_t trap)
	{
		uint32_t sp;
		uint16_t cursorID;

		sp = StackFrame<2>(cursorID);

		Log("%04x GetCursor(%04x)\n", trap, cursorID);


		ToolReturn<4>(sp, 0);
		return 0;
	}

	uint16_t SetCursor(uint16_t trap)
	{
		uint32_t sp;
		uint32_t cursor;

		sp = StackFrame<4>(cursor);

		Log("%04x SetCursor(%08x)\n", trap, cursor);

		return 0;
	}


	uint16_t GetFNum(uint16_t trap)
	{
		uint32_t sp;
		uint32_t fontName;
		uint32_t theNum;


		sp = StackFrame<8>(fontName, theNum);
		std::string sname = ToolBox::ReadPString(fontName);

		Log("%04x GetFNum(%s, %08x)\n", trap, sname.c_str(), theNum);

		if (theNum) memoryWriteWord(0, theNum);
		return 0;
	}

	uint16_t InitGraf(uint16_t trap)
	{
		// PROCEDURE InitGraf (globalPtr: Ptr);

		uint32_t globalPtr;

		StackFrame<4>(globalPtr);

		Log("%04x InitGraf($%08x)\n", trap, globalPtr);


		return 0;
	}

	uint16_t SetFScaleDisable(uint16_t trap)
	{
		uint16_t value;
		StackFrame<2>(value);

		Log("%04x SetFScaleDisable($%04x)\n", trap, value);
		// sets FScaleDisable global variable

		return 0;
	}


}
