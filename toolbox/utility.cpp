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

#include "utility.h"
#include "toolbox.h"
#include "mm.h"
#include "rm.h"

#include <cpu/defs.h>
#include <cpu/CpuModule.h>
#include <cpu/fmem.h>

#include "stackframe.h"

using ToolBox::Log;

namespace Utility {


	// FUNCTION NewString (theString: Str255): StringHandle;
	uint16_t NewString(uint16_t trap)
	{
		/*
		 * NewString allocates the specified string as a relocatable object 
		 * in the heap and returns a handle to it.
		 */

		uint32_t theString;
		uint32_t theHandle;
		uint32_t sp;
		uint32_t length = 0;
		uint32_t d0;

		std::string s;

		sp = StackFrame<4>(theString);

		s = ToolBox::ReadPString(theString);

		Log("%04x NewString(%s)\n", trap, s.c_str());

		length = s.length() + 1;

		// if (length) // always true...
		{
			uint32_t ptr;
			d0 = MM::Native::NewHandle(s.length(), false, theHandle, ptr);

			if (!d0)
			{
				ToolBox::WritePString(ptr, s);
			}
		}

		ToolReturn<4>(sp, theHandle);
		return d0;
	}

	// FUNCTION GetString (stringID: Integer): StringHandle;
	uint16_t GetString(uint16_t trap)
	{
		/*
		 * The GetString function loads a string from a string ('STR ') 
		 * resource into memory. It returns a handle to the string with the 
		 * specified resource ID, reading it from the resource file if 
		 * necessary.
		 */

		uint16_t stringID;
		uint32_t sp;
		uint32_t d0;
		uint32_t theHandle;

		sp = StackFrame<2>(stringID);

		Log("%04x GetString($%04x)\n", trap, stringID);

		d0 = RM::Native::GetResource(0x53545220, stringID, theHandle);

		ToolReturn<4>(sp, theHandle);
		return d0;
	}

	// FUNCTION BitTst (bytePtr: Ptr; bitNum: LONGINT) : BOOLEAN;
	uint16_t BitTst(uint16_t trap)
	{
		/* 
		 * BitTst tests whether a given bit is set and returns TRUE if so or 
		 * FALSE if not. The bit is specified by bitNum, an offset from the 
		 * high-order bit of the byte pointed to by bytePtr.
		 */

		uint32_t bytePtr;
		uint32_t bitNum;
		uint16_t rv;
		uint32_t sp;

		sp = StackFrame<8>(bytePtr, bitNum);
		Log("%04x BitTst($%08x, $%08x)\n", trap, bytePtr, bitNum);


		 uint32_t offset = bitNum >> 3;
		 int mask = 0x80 >> (bitNum & 0x07);

		 uint32_t value = memoryReadByte(bytePtr + offset);

		 rv = value & mask ? 1 : 0;
		 ToolReturn<2>(sp, rv);

		 return 0;
	}


}
