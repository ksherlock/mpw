
#include <string>
//#include <unordered_map>
#include <list>
#include <map>

#include <CoreServices/CoreServices.h>


#include "rm.h"
#include "toolbox.h"
#include "mm.h"
#include "os_internal.h"

#include <cpu/defs.h>
#include <cpu/CpuModule.h>
#include <cpu/fmem.h>

#include <macos/sysequ.h>
#include <macos/errors.h>

#include "stackframe.h"

using ToolBox::Log;

using namespace OS::Internal;

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


	std::map<uint32_t, Handle> rhandle_map;


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

	bool LoadResType(uint32_t type)
	{

		switch (type)
		{
			case 0x76657273: // 'vers';
			case 0x48455841: // 'HEXA'
			case 0x53545220: // 'STR '
			case 0x53545223: // 'STR#' (reziigs)
			case 0x59414343: // 'YACC' (pascaliigs)
			case 0x72547970: // 'rTyp' (rezIIgs)
			case 0x756e6974: // 'unit' (Pascal)
			case 0x434f4445: // 'CODE' (Link)
				return true;
			default:
				return false;
		}

	}



}
namespace RM
{



	namespace Native 
	{


		template<class FX>
		uint16_t LoadResource(uint32_t type, uint32_t &theHandle, FX fx)
		{
			uint32_t ptr;
			uint16_t error;

			Handle nativeHandle;
			uint32_t size;

			theHandle = 0;
			if (!LoadResType(type))
				return SetResError(resNotFound);

			nativeHandle = fx();

			if (!nativeHandle) return SetResError(resNotFound);

			size = ::GetHandleSize(nativeHandle);
			error = MM::Native::NewHandle(size, false, theHandle, ptr);

			if (!theHandle)
			{
				::ReleaseResource(nativeHandle);
				return SetResError(error);
			}

			if (size)
				std::memcpy(memoryPointer(ptr), *(void **)nativeHandle, size);

			//::ReleaseResource(nativeHandle);

			rhandle_map.insert({theHandle, nativeHandle});

			return SetResError(0);
		}


		// used by GetString (utility.h)
		uint16_t GetResource(uint32_t type, uint16_t id, uint32_t &theHandle)
		{
			return LoadResource(type, theHandle,
				[type, id](){
					return ::GetResource(type, id);
				});
		}

	}

	uint16_t CloseResFile(uint16_t trap)
	{
		uint16_t refNum;

		StackFrame<2>(refNum);

		Log("%04x CloseResFile(%04x)\n", trap, refNum);
		::CloseResFile(refNum);
		return SetResError(::ResError());
		
		//return SetResError(resFNotFound);
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

		sp = StackFrame<8>(theType, name);

		std::string sname = ToolBox::ReadPString(name);

		Log("%04x Get1NamedResource(%08x, %s)\n", trap, theType, sname.c_str());

		uint32_t resourceHandle;
		uint32_t d0;
		d0 = Native::LoadResource(theType, resourceHandle, 
			[theType, name](){
				return ::Get1NamedResource(theType, memoryPointer(name));
			}
		);

		ToolReturn<4>(sp, resourceHandle);
		return SetResError(d0);
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

		sp = StackFrame<6>(theType, theID);

		Log("%04x GetResource(%08x ('%s'), %04x)\n", trap, theType, TypeToString(theType).c_str(), theID);


		uint32_t resourceHandle;
		uint32_t d0;
		d0 = Native::LoadResource(theType, resourceHandle, 
			[theType, theID](){
				return ::GetResource(theType, theID);
			}
		);

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

		sp = StackFrame<6>(theType, theID);

		Log("%04x Get1Resource(%08x ('%s'), %04x)\n", trap, theType, TypeToString(theType).c_str(), theID);


		uint32_t resourceHandle;
		uint32_t d0;
		d0 = Native::LoadResource(theType, resourceHandle, 
			[theType, theID](){
				return ::Get1Resource(theType, theID);
			}
		);


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

		::SetResLoad(load);

		memoryWriteByte(load ? 0xff : 0x00, MacOS::ResLoad); // word or byte?
		return SetResError(0);
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

	uint16_t CreateResFile(uint16_t trap)
	{
		// PROCEDURE CreateResFile (fileName: Str255);

		/*
		 * CreateResFile creates a resource file containing no resource 
		 * data. If there's no file at all with the given name, it also 
		 * creates an empty data fork for the file. If there's already a 
		 * resource file with the given name (that is, a resource fork 
		 * that isn't empty), CreateResFile will do nothing and the 
		 * ResError function will return an appropriate Operating System 
		 * result code.
		 */

		// FSCreateResourceFile uses the parent FSRef + a file name.


		uint32_t fileName;
		FSRef ref;
		OSErr error;
		int fd;

		StackFrame<4>(fileName);

		std::string sname = ToolBox::ReadPString(fileName, true);
		Log("%04x CreateResFile(%s)\n", trap, sname.c_str());

		if (!sname.length()) return SetResError(MacOS::paramErr);

		// 1. if the file exists, call FSCreateResourceFork on the file.
		// 2. if the file does not exist, create the file then call 
		//    FSCreateResourceFork

		error = ::FSPathMakeRef((const UInt8 *)sname.c_str(), &ref, NULL);
		if (error != noErr)
		{
			return SetResError(error);
		}

		fd = ::open(sname.c_str(), O_CREAT | O_EXCL | O_RDWR, 0666);
		if (fd < 0)
		{
			if (errno != EEXIST) return SetResError(errno_to_oserr(errno));
		}
		if (fd >= 0) close(fd);

        HFSUniStr255 fork = {0,{0}};
        ::FSGetResourceForkName(&fork);

		error = ::FSCreateResourceFork(&ref, fork.length, fork.unicode, 0);

		return SetResError(error);
	}


	uint16_t OpenResFile(uint16_t trap)
	{
		// OpenResFile (fileName: Str255) : INTEGER;
		
		ResFileRefNum refNum;
		FSRef ref;

		uint32_t sp;
		uint32_t fileName;
		uint16_t permission;
		OSErr error;

		sp = StackFrame<4>(fileName);

		std::string sname = ToolBox::ReadPString(fileName, true);

		Log("%04x OpenResFile(%s)\n", trap, sname.c_str());

		error = ::FSPathMakeRef( (const UInt8 *)sname.c_str(), &ref, NULL);
		if (error != noErr)
		{
			ToolReturn<2>(sp, (uint16_t)-1);
			return SetResError(error);
		}

        HFSUniStr255 fork = {0,{0}};
        ::FSGetResourceForkName(&fork);

        refNum = -1;
        permission = 0; // whatever is allowed.
    	error = ::FSOpenResourceFile(&ref, 
    		fork.length, 
    		fork.unicode, 
    		permission, 
    		&refNum);

		ToolReturn<2>(sp, (uint16_t)refNum);

		return SetResError(error);
	}



	uint16_t OpenRFPerm(uint16_t trap)
	{
		// FUNCTION OpenRFPerm (fileName: Str255; vRefNum: Integer;
        //           permission: SignedByte): Integer;
		ResFileRefNum refNum;
		FSRef ref;

		uint32_t sp;
		uint32_t fileName;
		uint16_t vRefNum;
		uint16_t permission;
		OSErr error;

		sp = StackFrame<8>(fileName, vRefNum, permission);

		std::string sname = ToolBox::ReadPString(fileName, true);
		Log("%04x OpenRFPerm(%s, %04x, %04x)\n",
			trap, sname.c_str(), vRefNum, permission);

		error = ::FSPathMakeRef( (const UInt8 *)sname.c_str(), &ref, NULL);
		if (error != noErr)
		{
			ToolReturn<2>(sp, (uint16_t)-1);
			return SetResError(error);
		}

        HFSUniStr255 fork = {0,{0}};
        ::FSGetResourceForkName(&fork);

        refNum = -1;
    	error = ::FSOpenResourceFile(&ref, 
    		fork.length, 
    		fork.unicode, 
    		permission, 
    		&refNum);

		ToolReturn<2>(sp, (uint16_t)refNum);

		return SetResError(error);
	}

	uint16_t Count1Resources(uint16_t trap)
	{
		// FUNCTION Count1Resources (theType: ResType): Integer;

		uint32_t sp;
		uint32_t theType;
		uint16_t count;

		sp = StackFrame<4>(theType);

		Log("%04x Count1Resources(%08x ('%s'))\n", 
			trap, theType, TypeToString(theType).c_str());

		count = ::Count1Resources(theType);

		ToolReturn<2>(sp, count);
		return SetResError(0);
	}


	uint16_t UpdateResFile(uint16_t trap)
	{
		// PROCEDURE UpdateResFile (refNum: Integer);

		uint16_t refNum;

		StackFrame<2>(refNum);

		Log("%04x UpdateResFile(%04x)\n", trap, refNum);

		::UpdateResFile(refNum);

		return SetResError(::ResError());
	}

	uint16_t GetResFileAttrs(uint16_t trap)
	{
		// FUNCTION GetResFileAttrs (refNum: Integer): Integer;
		uint32_t sp;
		uint16_t attrs;
		uint16_t refNum;

		sp = StackFrame<2>(refNum);
		Log("%04x GetResFileAttrs(%04x)\n", trap, refNum);

		attrs = ::GetResFileAttrs(refNum);
		ToolReturn<2>(sp, attrs);

		return SetResError(::ResError());
	}


	uint16_t SetResFileAttrs(uint16_t trap)
	{
		// PROCEDURE SetResFileAttrs  (refNum: Integer; attrs: Integer);
		uint32_t sp;
		uint16_t attrs;
		uint16_t refNum;

		sp = StackFrame<4>(refNum, attrs);
		Log("%04x GetResFileAttrs(%04x, %04x)\n", trap, refNum, attrs);

		::SetResFileAttrs(refNum, attrs);

		return SetResError(::ResError());
	}

	// TODO -- update rhandle_map when loading resources
	// TODO -- update rhandle_map when disposing resources.

	uint16_t AddResource(uint16_t trap)
	{
		// PROCEDURE AddResource (theData: Handle; theType: ResType;
		// theID: Integer; name: Str255);

		uint32_t theData;
		uint32_t theType;
		uint16_t theID;
		uint32_t namePtr;


		StackFrame<14>(theData, theType, theID, namePtr);

		std::string sname = ToolBox::ReadPString(namePtr, false);

		Log("%04x AddResource(%08x, %08x ('%s'), %04x, %s)\n",
			trap, theData, theType, TypeToString(theType).c_str(), sname.c_str()
		);


		Handle nativeHandle = NULL;
		if (theData)
		{
			uint32_t handleSize;
			MM::Native::GetHandleSize(theData, handleSize);

			if (handleSize)
			{

				nativeHandle = ::NewHandle(handleSize);
				if (!nativeHandle) return SetResError(MacOS::addResFailed);

				uint32_t src = memoryReadLong(theData);

				std::memcpy(*(uint8_t **)nativeHandle, memoryPointer(src), handleSize);  

				rhandle_map.insert({theData, nativeHandle});
			}
		}
		// AddResource assumes ownership of the handle.
		::AddResource(nativeHandle, theType, theID, memoryPointer(namePtr));
		return SetResError(::ResError());
	}

	uint16_t SetResAttrs(uint16_t trap)
	{
		// PROCEDURE SetResAttrs (theResource: Handle; attrs: Integer);

		uint32_t theResource;
		uint16_t attrs;

		StackFrame<6>(theResource, attrs);

		Log("%04x SetResAttrs(%08x, %04x)\n", trap, theResource, attrs);

		// find the native handle..

		auto iter = rhandle_map.find(theResource);
		if (iter == rhandle_map.end()) return SetResError(MacOS::resNotFound);

		Handle nativeHandle = iter->second;

		::SetResAttrs(nativeHandle, attrs);

		return SetResError(::ResError());
	}

	uint16_t GetResAttrs(uint16_t trap)
	{
		// FUNCTION GetResAttrs (theResource: Handle): Integer;

		uint32_t sp;
		uint32_t theResource;
		uint16_t attrs;

		sp = StackFrame<4>(theResource);

		Log("%04x GetResAttrs(%08x)\n", trap, theResource);

		auto iter = rhandle_map.find(theResource);
		if (iter == rhandle_map.end())
		{
			ToolReturn<2>(sp, 0);
			return SetResError(MacOS::resNotFound);
		}

		Handle nativeHandle = iter->second;

		attrs = ::GetResAttrs(nativeHandle);

		ToolReturn<2>(sp, attrs);

		return SetResError(::ResError());
	}


	uint16_t WriteResource(uint16_t trap)
	{
		// PROCEDURE WriteResource (theResource: Handle);

		uint32_t theResource;
		StackFrame<4>(theResource);

		Log("%04x WriteResource(%08x)\n", trap, theResource);


		auto iter = rhandle_map.find(theResource);
		if (iter == rhandle_map.end()) return SetResError(MacOS::resNotFound);

		Handle nativeHandle = iter->second;

		// todo -- need to verify handle size, re-copy handle memory?
		::WriteResource(nativeHandle);

		return SetResError(::ResError());
	}



	uint16_t DetachResource(uint16_t trap)
	{
		// PROCEDURE DetachResource (theResource: Handle);

		uint32_t theResource;
		StackFrame<4>(theResource);

		Log("%04x DetachResource(%08x)\n", trap, theResource);


		auto iter = rhandle_map.find(theResource);
		if (iter == rhandle_map.end()) return SetResError(MacOS::resNotFound);

		Handle nativeHandle = iter->second;

		rhandle_map.erase(iter);

		::ReleaseResource(nativeHandle);

		return SetResError(::ResError());
	}


	uint16_t Get1IndResource(uint16_t trap)
	{
		// FUNCTION Get1IndResource (theType: ResType; index: Integer): Handle;

		uint32_t sp;
		uint32_t theType;
		uint16_t index;

		sp = StackFrame<6>(theType, index);
		Log("%04x Get1IndResource(%08x ('%s'), %04x)\n", 
			trap, theType, TypeToString(theType).c_str(), index);

		uint32_t resourceHandle = 0;
		uint16_t d0;
		d0 = Native::LoadResource(theType, resourceHandle, 
			[theType, index](){
				return ::Get1IndResource(theType, index);
			}
		);

		ToolReturn<4>(sp, resourceHandle);
		return SetResError(d0);
	}


	uint16_t RemoveResource(uint16_t trap)
	{
		// PROCEDURE RemoveResource (theResource: Handle);

		uint32_t theResource;

		StackFrame<4>(theResource);

		Log("%04x RemoveResource(%08x)\n", trap, theResource);


		auto iter = rhandle_map.find(theResource);
		if (iter == rhandle_map.end()) return SetResError(MacOS::resNotFound);

		Handle nativeHandle = iter->second;
		rhandle_map.erase(iter);

		::RemoveResource(nativeHandle);
		::DisposeHandle(nativeHandle);

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
