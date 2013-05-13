
#include <string>
//#include <unordered_map>
#include <list>


#include <CoreServices/CoreServices.h>


#include "rm.h"
#include "toolbox.h"
#include "mm.h"

#include <cpu/defs.h>
#include <cpu/CpuModule.h>
#include <cpu/fmem.h>

#include <macos/sysequ.h>


#include "stackframe.h"

using ToolBox::Log;

namespace
{

#if 0
	struct ResEntry
	{
		ResEntry(uint32_t type = 0, uint32_t id = 0) : 
			resType(type), resID(id)
		{}
		uint32_t resType;
		uint32_t resID;
		uint32_t handle;
	};

	std::list<ResEntry> ResourceMapList;
#endif
	// sigh... really need to create a resmap for every open
	// resource file and update that with the pointer.


	inline uint16_t SetResError(uint16_t error)
	{
		memoryWriteWord(error, MacOS::ResErr);
		return error;
	}


	std::string TypeToString(uint32_t type)
	{
		char tmp[5] = { 0, 0, 0, 0, 0};

		for (unsigned i = 0; i < 4; ++i)
		{
			char c = type & 0xff;
			type >>= 8;

			c = isprint(c) ? c : '.';

			tmp[3 - i] = c;
		}

		return std::string(tmp);
	}




}
namespace RM
{

	namespace Native 
	{

		uint16_t GetResource(uint32_t type, uint16_t id, uint32_t &theHandle)
		{
			uint32_t ptr;
			uint16_t error;

			Handle nativeHandle;
			uint32_t size;

			theHandle = 0;
			switch (type)
			{
				case 0x76657273: // 'vers';
				case 0x48455841: // 'HEXA'
				case 0x53545220: // 'STR '
				case 0x53545223: // 'STR#' (reziigs)
				case 0x59414343: // 'YACC' (pascaliigs)
				case 0x72547970: // 'rTyp' (rezIIgs)
				case 0x756e6974: // 'unit' (Pascal)
					break;
				default:
					return SetResError(resNotFound);
			}

			nativeHandle = ::Get1Resource(type, id);
			if (!nativeHandle) return SetResError(resNotFound);

			size = ::GetHandleSize(nativeHandle);
			error = MM::Native::NewHandle(size, false, theHandle, ptr);

			if (!theHandle)
			{
				::ReleaseResource(nativeHandle);
				return error;
			}

			std::memcpy(memoryPointer(ptr), *(void **)nativeHandle, size);
			::ReleaseResource(nativeHandle);

			return 0;
		}

	}

	uint16_t CloseResFile(uint16_t trap)
	{
		uint16_t refNum;

		StackFrame<2>(refNum);

		Log("%04x CloseResFile(%04x)\n", trap, refNum);

		return SetResError(resFNotFound);
	}


	uint16_t Get1NamedResource(uint16_t trap)
	{
		// Get1NamedResource (theType: ResType; name: Str255) : Handle;

		/*
		 * -----------
		 * +8 outHandle
		 * ------------
		 * +4 theType
		 * ------------
		 * +0 name
		 * ------------
		 *
		 */

		// nb - return address is not on the stack.

		uint32_t sp;
		uint32_t theType;
		uint32_t name;

		uint32_t resourceHandle = 0;

		sp = StackFrame<8>(theType, name);

		std::string sname = ToolBox::ReadPString(name);

		Log("%04x Get1NamedResource(%08x, %s)\n", trap, theType, sname.c_str());

		ToolReturn<4>(sp, resourceHandle);
		return SetResError(resourceHandle ? 0 : resNotFound);
	}



	uint16_t GetResource(uint16_t trap)
	{
		// GetResource (theType: ResType; theID: Integer): Handle;

		/*
		 * -----------
		 * +6 outHandle
		 * ------------
		 * +2 theType
		 * ------------
		 * +0 theID
		 * ------------
		 *
		 */

		// nb - return address is not on the stack.

		uint32_t sp;
		uint32_t theType;
		uint16_t theID;
		uint32_t d0;

		uint32_t resourceHandle;

		sp = StackFrame<6>(theType, theID);

		Log("%04x GetResource(%08x ('%s'), %04x)\n", trap, theType, TypeToString(theType).c_str(), theID);


		d0 = Native::GetResource(theType, theID, resourceHandle);

		ToolReturn<4>(sp, resourceHandle);
		return d0;
	}


	uint16_t Get1Resource(uint16_t trap)
	{
		// Get1Resource (theType: ResType; theID: Integer): Handle;

		/*
		 * -----------
		 * +6 outHandle
		 * ------------
		 * +2 theType
		 * ------------
		 * +0 theID
		 * ------------
		 *
		 */

		// nb - return address is not on the stack.

		uint32_t sp;
		uint32_t theType;
		uint16_t theID;

		uint32_t resourceHandle;
		uint32_t d0;


		sp = StackFrame<6>(theType, theID);

		Log("%04x Get1Resource(%08x ('%s'), %04x)\n", trap, theType, TypeToString(theType).c_str(), theID);

		d0 = Native::GetResource(theType, theID, resourceHandle);

		ToolReturn<4>(sp, resourceHandle);
		return d0;
	}


	uint16_t ReleaseResource(uint16_t trap)
	{
		// ReleaseResource (theResource: Handle);	

		/*
		 * ------------
		 * +0 theResource
		 * ------------
		 *
		 */

		uint32_t sp;
		uint32_t theResource;

		sp = StackFrame<4>(theResource);

		Log("%04x ReleaseResource(%08x)\n", trap, theResource);

		return SetResError(0);
	}

	uint16_t ResError(uint16_t trap)
	{
		Log("%04x ResError()\n", trap);

		return memoryReadWord(MacOS::ResErr);
	}

	// SetResLoad (load: BOOLEAN);
	uint16_t SetResLoad(uint16_t trap)
	{
		uint16_t load;

		StackFrame<2>(load);

		Log("%04x SetResLoad(%04x)\n", trap, load);

		memoryWriteByte(load ? 0xff : 0x00, MacOS::ResLoad); // word or byte?
		return 0;
	}



	uint16_t OpenResFile(uint16_t trap)
	{
		// OpenResFile (fileName: Str255) : INTEGER;

		uint32_t sp;
		uint32_t fileName;

		sp = StackFrame<4>(fileName);

		std::string sname = ToolBox::ReadPString(fileName);

		Log("%04x OpenResFile(%s)\n", trap, sname.c_str());

		ToolReturn<2>(sp, (uint16_t)-1);
		return SetResError(resFNotFound);
	}


	uint16_t CurResFile(uint16_t trap)
	{

		Log("%04x CurResFile()\n", trap);

		ResFileRefNum refNum = ::CurResFile();

		ToolReturn<2>(-1, refNum);
		return SetResError(::ResError());
	}

	uint16_t UseResFile(uint16_t trap)
	{
		uint16_t resFile;

		StackFrame<2>(resFile);

		Log("%04x UseResFile(%04x)\n", trap, resFile);

		::UseResFile(resFile);
		return SetResError(::ResError());
	}


	// todo -- move since it's not RM related.
	uint16_t UnloadSeg(uint16_t trap)
	{
		// UnloadSeg (routineAddr: Ptr);	

		/*
		 * ------------
		 * +0 routineAddr
		 * ------------
		 *
		 */

		 uint32_t sp;
		 uint32_t routineAddr;

		 sp = StackFrame<4>(routineAddr);

		 Log("%04x UnloadSeg(%08x)\n", trap, routineAddr);

		 return 0;
	}

}
