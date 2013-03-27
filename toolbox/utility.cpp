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


}
