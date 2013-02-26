
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

	uint32_t GetXResource(uint32_t type, uint16_t id)
	{
		uint32_t handle;
		uint32_t ptr;
		uint16_t error;

		Handle nativeHandle;
		uint32_t size;

		switch (type)
		{
			case 0x76657273: // 'vers';
			case 0x48455841: // 'HEXA'
			case 0x53545220: // 'STR '
			case 0x59414343: // 'YACC'
				break;
			default:
				return 0;
		}

		nativeHandle = ::Get1Resource(type, id);
		if (!nativeHandle) return 0;

		size = ::GetHandleSize(nativeHandle);
		error = MM::Native::NewHandle(size, false, handle, ptr);

		if (!handle) return 0;

		std::memcpy(memoryPointer(ptr), *(void **)nativeHandle, size);
		::ReleaseResource(nativeHandle);

		return handle;
	}


}
namespace RM
{
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

		uint32_t resourceHandle;

		sp = StackFrame<6>(theType, theID);

		Log("%04x GetResource(%08x ('%s'), %04x)\n", trap, theType, TypeToString(theType).c_str(), theID);


		resourceHandle = GetXResource(theType, theID);

		ToolReturn<4>(sp, resourceHandle);
		return SetResError(resourceHandle ? 0 : resNotFound);
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


		sp = StackFrame<6>(theType, theID);

		Log("%04x Get1Resource(%08x ('%s'), %04x)\n", trap, theType, TypeToString(theType).c_str(), theID);

		resourceHandle = GetXResource(theType, theID);

		ToolReturn<4>(sp, resourceHandle);
		return SetResError(resourceHandle ? 0 : resNotFound);
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

		return 0;
	}


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
