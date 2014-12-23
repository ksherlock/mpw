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
#include <macos/tool_return.h>

#include "stackframe.h"
#include "fs_spec.h"
using ToolBox::Log;

using namespace OS::Internal;
using namespace ToolBox;

using MacOS::tool_return;
using MacOS::macos_error_from_errno;
using MacOS::macos_error;

namespace
{

	bool ResLoad = true;

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

	bool LoadResType(uint32_t type)
	{
		// this filtering was originally to
		// block cursors ('acur').  Can probably 
		// change it to a blacklist rather than 
		// a whitelist at some point.

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
			case 0x5041434b: // 'PACK' (PascalIIgs)
			case 0x4b4f4445: // 'KODE' (Link 32-bit Startup)
			case 0x45525253: // 'ERRS' (PPCLink)
			case 0x63667267: // 'cfrg' (PPCLink)
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

		// not to be confused with MacOS LoadResource(theHandle)

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

			// in OS X 10.8, ::SetResLoad(false) seems to be permanent.
			// therefore, explicitly load it if needed. (PascalIIgs)
			if (ResLoad) ::LoadResource(nativeHandle);

			size = ::GetHandleSize(nativeHandle);
			error = MM::Native::NewHandle(size, false, theHandle, ptr);
			// TODO -- need to lock if native handle locked.

			if (!theHandle)
			{
				::ReleaseResource(nativeHandle);
				return SetResError(error);
			}
			MM::Native::HSetRBit(theHandle);

			if (size)
				std::memcpy(memoryPointer(ptr), *(void **)nativeHandle, size);

			//::ReleaseResource(nativeHandle);

			rhandle_map.insert({theHandle, nativeHandle});

			return SetResError(0);
		}


		// used by GetString (utility.h)
		// used by Loader.
		uint16_t GetResource(uint32_t type, uint16_t id, uint32_t &theHandle)
		{
			return LoadResource(type, theHandle,
				[type, id](){
					return ::GetResource(type, id);
				});
		}

		uint16_t SetResLoad(bool load)
		{

			ResLoad = load;
			::SetResLoad(load);

			memoryWriteByte(load ? 0xff : 0x00, MacOS::ResLoad); // word or byte?
			return SetResError(0);
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

		Log("%04x Get1NamedResource(%08x ('%s'), %s)\n", 
			trap, theType, TypeToString(theType).c_str(), sname.c_str());

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

		Log("%04x GetResource(%08x ('%s'), %04x)\n", 
				trap, theType, TypeToString(theType).c_str(), theID);


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
		uint32_t sp;

		Log("%04x ResError()\n", trap);

		sp = cpuGetAReg(7);
		ToolReturn<2>(sp, memoryReadWord(MacOS::ResErr));
		return 0;
	}

	// SetResLoad (load: BOOLEAN);
	uint16_t SetResLoad(uint16_t trap)
	{
		uint16_t load;

		StackFrame<2>(load);

		Log("%04x SetResLoad(%04x)\n", trap, load);

		ResLoad = load;
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


	tool_return<void> CreateResFile(const std::string &path)
	{

		if (path.empty()) return MacOS::paramErr;

		FSRef ref;
		OSErr error;
		int fd;

		error = ::FSPathMakeRef((const UInt8 *)path.c_str(), &ref, NULL);
		if (error != noErr)
			return macos_error(error);

		// FSCreateResourceFork only works with existing files.

		fd = ::open(path.c_str(), O_CREAT | O_EXCL | O_RDWR, 0666);
		if (fd < 0)
		{
			if (errno != EEXIST) return macos_error_from_errno();
		}
		if (fd >= 0) close(fd);


		HFSUniStr255 fork = {0,{0}};
		::FSGetResourceForkName(&fork);


		error = ::FSCreateResourceFork(&ref, fork.length, fork.unicode, 0);

		return macos_error(error);
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
			if (errno != EEXIST) return SetResError(macos_error_from_errno());
		}
		if (fd >= 0) close(fd);

		HFSUniStr255 fork = {0,{0}};
		::FSGetResourceForkName(&fork);

		error = ::FSCreateResourceFork(&ref, fork.length, fork.unicode, 0);

		return SetResError(error);
	}


	uint16_t FSpCreateResFile(void)
	{

		// PROCEDURE FSpCreateResFile (spec: FSSpec; creator, fileType: OSType; scriptTag: ScriptCode);

		// creates the file, if necessary.

		uint32_t sp;
		uint32_t spec;
		uint32_t creator;
		uint32_t fileType;
		uint16_t scriptTag;

		sp = StackFrame<14>(spec, creator, fileType, scriptTag);


		int parentID = memoryReadLong(spec + 2);
		std::string sname = ToolBox::ReadPString(spec + 6, false);

		Log("     FSpCreateResFile(%s, %08x ('%s'), %08x ('%s'), %02x)\n",  
			sname.c_str(), 
			creator, ToolBox::TypeToString(creator).c_str(),
			fileType, ToolBox::TypeToString(fileType).c_str(),
			scriptTag);


		sname = OS::FSSpecManager::ExpandPath(sname, parentID);
		if (sname.empty())
		{
			return SetResError(MacOS::dirNFErr);
		}

		auto rv = CreateResFile(sname);
		// returns errFSForkExists if fork already exists.
		// therefore, if no error, set the ftype/ctype.
		if (rv.error() == 0)
		{
			char buffer[32];
			std::memset(buffer, 0, sizeof(buffer));
			buffer[0] = fileType >> 24;
			buffer[1] = fileType >> 16;
			buffer[2] = fileType >> 8;
			buffer[3] = fileType >> 0;

			buffer[4] = creator >> 24;
			buffer[5] = creator >> 16;
			buffer[6] = creator >> 8;
			buffer[7] = creator >> 0;

			std::memcpy(buffer+4, &creator, 4);
			OS::Internal::SetFinderInfo(sname, buffer, false);
		}

		return SetResError(rv.error() == errFSForkExists ? 0 : rv.error());
	}




	tool_return<int16_t> OpenResCommon(const std::string &path, uint16_t permission = 0)
	{
		OSErr error;
		FSRef ref;
		ResFileRefNum refNum;

		error = ::FSPathMakeRef( (const UInt8 *)path.c_str(), &ref, NULL);
		if (error != noErr)
			return (MacOS::macos_error)error;

		HFSUniStr255 fork = {0,{0}};
		::FSGetResourceForkName(&fork);

		refNum = -1;
		error = ::FSOpenResourceFile(&ref, 
			fork.length,
			fork.unicode,
			permission, 
			&refNum);

		if (error != noErr)
			return (MacOS::macos_error)error;

		return refNum;
	}

	uint16_t OpenResFile(uint16_t trap)
	{
		// OpenResFile (fileName: Str255) : INTEGER;
		
		uint32_t sp;
		uint32_t fileName;

		sp = StackFrame<4>(fileName);

		std::string sname = ToolBox::ReadPString(fileName, true);

		Log("%04x OpenResFile(%s)\n", trap, sname.c_str());

		auto rv = OpenResCommon(sname);

		ToolReturn<2>(sp, rv.value_or(-1));

		return SetResError(rv.error());
	}

	uint16_t HOpenResFile(uint16_t trap)
	{
		// FUNCTION HOpenResFile (vRefNum: Integer; dirID: LongInt;
		// fileName: Str255; permission: SignedByte): Integer;

		uint32_t sp;

		uint16_t vRefNum;
		uint32_t dirID;
		uint32_t fileName;
		uint16_t permission;

		sp = StackFrame<12>(vRefNum, dirID, fileName, permission);

		std::string sname = ToolBox::ReadPString(fileName, true);

		Log("%04x HOpenResFile(%04x, %08x, %s, %04x)\n", 
			trap, vRefNum, dirID, sname.c_str(), permission);

		if (vRefNum) {
			fprintf(stderr, "HOpenResFile: vRefNum not supported yet.\n");
			exit(1);
		}

		sname = OS::FSSpecManager::ExpandPath(sname, dirID);
		if (sname.empty())
		{
			ToolReturn<2>(sp, (uint16_t)-1);
			return SetResError(MacOS::dirNFErr);
		}

		auto rv = OpenResCommon(sname, permission);

		ToolReturn<2>(sp, rv.value_or(-1));

		return SetResError(rv.error());
	}

	uint16_t FSpOpenResFile(void)
	{
		// FUNCTION FSpOpenResFile (spec: FSSpec; permission: SignedByte): Integer;

		uint32_t sp;
		uint32_t spec;
		uint16_t permission;

		sp = StackFrame<6>(spec, permission);


		int parentID = memoryReadLong(spec + 2);

		std::string sname = ToolBox::ReadPString(spec + 6, false);

		Log("     FSpOpenResFile(%s, %04x)\n",  sname.c_str(), permission);


		sname = OS::FSSpecManager::ExpandPath(sname, parentID);
		if (sname.empty())
		{
			ToolReturn<2>(sp, (uint16_t)-1);
			return SetResError(MacOS::dirNFErr);
		}

		auto rv = OpenResCommon(sname, permission);

		ToolReturn<2>(sp, rv.value_or(-1));

		return SetResError(rv.error());
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
			trap, theData, theType, TypeToString(theType).c_str(), theID, sname.c_str()
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

	uint16_t GetResourceSizeOnDisk(uint16_t trap)
	{
		// FUNCTION GetResourceSizeOnDisk (theResource: Handle): LongInt;

		uint32_t sp;
		uint32_t theResource;

		sp = StackFrame<4>(theResource);

		Log("%04x GetResourceSizeOnDisk(%08x)\n", trap, theResource);


		auto iter = rhandle_map.find(theResource);
		if (iter == rhandle_map.end())
		{
			ToolReturn<4>(sp, (uint32_t)0);
			return SetResError(MacOS::resNotFound);
		}

		Handle nativeHandle = iter->second;
		uint32_t size = ::GetResourceSizeOnDisk(nativeHandle);

		ToolReturn<4>(sp, size);
		return SetResError(::ResError());
	}

	uint16_t GetResInfo(uint16_t trap)
	{
		// PROCEDURE GetResInfo (theResource: Handle; 
		// VAR theID: INTEGER; VAR theType: ResType; VAR name: Str255);

		uint32_t theResource;
		uint32_t theID;
		uint32_t theType;
		uint32_t name;
		StackFrame<16>(theResource, theID, theType, name);

		Log("%04x GetResInfo(%08x)\n", trap, theResource);

		auto iter = rhandle_map.find(theResource);
		if (iter == rhandle_map.end())
		{
			return SetResError(MacOS::resNotFound);
		}

		Handle nativeHandle = iter->second;
		ResID nativeID = 0;
		ResType nativeType = 0;		
		Str255 nativeName = {0};

		::GetResInfo(nativeHandle, &nativeID, &nativeType, nativeName);

		if (theID) memoryWriteWord(nativeID, theID);
		if (theType) memoryWriteLong(nativeType, theType);
		if (name)
		{
			std::string sname(nativeName + 1, nativeName + 1 + nativeName[0]);
			ToolBox::WritePString(name, sname);
		}

		return SetResError(::ResError());
	}

	uint16_t LoadResource(uint16_t trap)
	{
		// PROCEDURE LoadResource (theResource: Handle);

		// this loads the resource from disk, if not already
		// loaded. (if purgeable or SetResLoad(false)) 

		// this needs cooperation with MM to check if 
		// handle was purged.

		OSErr err;
		uint32_t theResource;

		StackFrame<4>(theResource);

		Log("%04x LoadResource(%08x)\n", trap, theResource);

		auto iter = rhandle_map.find(theResource);
		if (iter == rhandle_map.end())
		{
			return SetResError(MacOS::resNotFound);
		}


		// if it has a size, it's loaded...
		uint32_t handleSize;
		err = MM::Native::GetHandleSize(theResource, handleSize);
		if (err) return SetResError(err);
		if (handleSize) return SetResError(0);

		// otherwise, load it

		Handle nativeHandle;
		uint32_t size;

		nativeHandle = iter->second;


		::LoadResource(nativeHandle);
		err = ::ResError();
		if (err) return SetResError(err); // ?!?!


		size = ::GetHandleSize(nativeHandle);

		err = MM::Native::ReallocHandle(theResource, size);
		if (err) return SetResError(err);

		// todo -- need to lock if resource locked.

		if (size)
		{
			uint32_t ptr = memoryReadLong(theResource);
			std::memcpy(memoryPointer(ptr), *(void **)nativeHandle, size);
		}

		return SetResError(0);
	}


	uint16_t HomeResFile(uint16_t trap)
	{
		// PPCAsm

		// FUNCTION HomeResFile (theResource: Handle): Integer;

		uint32_t sp;
		uint32_t theResource;
		uint16_t resFile;

		sp = StackFrame<4>(theResource);
		Log("%04x HomeResFile(%08x)\n", trap, theResource);


		auto iter = rhandle_map.find(theResource);
		if (iter == rhandle_map.end())
		{
			return SetResError(MacOS::resNotFound);
		}

		resFile = ::HomeResFile(iter->second);

		ToolReturn<2>(sp, resFile);

		return SetResError(::ResError());
	}

	uint16_t Count1Types(uint16_t trap)
	{
		// FUNCTION Count1Types: Integer;

		uint16_t count;

		Log("%04x Count1Types\n", trap);

		count = ::Count1Types();

		ToolReturn<2>(-1, count);

		return SetResError(::ResError());
	}


	uint16_t Get1IndType(uint16_t trap)
	{
		// PROCEDURE Get1IndType (VAR theType: ResType; index: Integer);

		uint32_t theType;
		uint16_t index;

		StackFrame<6>(theType, index);

		Log("%04x Get1IndType(%08x, %04x)\n", trap, theType, index);

		ResType nativeType = 0;		

		::Get1IndType(&nativeType, index);

		memoryWriteLong(nativeType, theType);

		return SetResError(::ResError());
	}
}
