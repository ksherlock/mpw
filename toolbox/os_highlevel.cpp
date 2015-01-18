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

#include <cerrno>
#include <cctype>
#include <ctime>
#include <algorithm>
#include <chrono>
#include <deque>
#include <string>

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

#include <macos/errors.h>

#include "os.h"
#include "rm.h"
#include "os_internal.h"
#include "toolbox.h"
#include "stackframe.h"
#include "fs_spec.h"

using ToolBox::Log;
using MacOS::macos_error_from_errno;

extern "C" {
	char * fs_spec_realpath(const char * __restrict path, char * __restrict resolved);
}

namespace OS {

	/*

	struct FSSpec {
		short               vRefNum;
		long                parID;
		StrFileName         name;                   // a Str63 on MacOS
	};
	*/	

	// MacOS: -> { -1, 1, "MacOS" }
	// MacOS:dumper -> {-1, 2 "dumper"}

	std::string realpath(const std::string &path)
	{
		char buffer[PATH_MAX + 1];

		// FSSpecs are valid for non-existant files
		// but not non-existant directories.
		// realpath does not behave in such a manner.

		// expand the path.  Also handles relative paths.
		char *cp = ::fs_spec_realpath(path.c_str(), buffer);
		if (!cp) return "";
		
		return std::string(cp);
	}

	std::string ReadFSSpec(uint32_t spec) {
		if (!spec) return "";

		int parentID = memoryReadLong(spec + 2);
		std::string sname = ToolBox::ReadPString(spec + 6, false);
		return OS::FSSpecManager::ExpandPath(sname, parentID);
	}

	uint16_t FSMakeFSSpec(void)
	{
		// FSMakeFSSpec(vRefNum: Integer; dirID: LongInt; fileName: Str255; VAR spec: FSSpec): OSErr;

		// todo -- if the file does not exist (but the path is otherwise valid), create the spec but return fnfErr.
		
		/*
		 * See Chapter 2, File Manager / Using the File Manager, 2-35
		 *
		 *
		 */

		uint16_t vRefNum;
		uint32_t dirID;
		uint32_t fileName;
		uint32_t spec;


		StackFrame<14>(vRefNum, dirID, fileName, spec);

		std::string sname = ToolBox::ReadPString(fileName, true);
		Log("     FSMakeFSSpec(%04x, %08x, %s, %08x)\n", 
			vRefNum, dirID, sname.c_str(), spec);

		if (vRefNum == 0 && dirID > 0 && sname.length())
		{
			// SC uses dirID + relative path.

			std::string root = FSSpecManager::PathForID(dirID);
			if (root.empty())
			{
				std::memset(memoryPointer(spec), 0, 8);
				return MacOS::dirNFErr;
			}

			sname = root + sname;
			dirID = 0;
		}

		bool absolute = sname.length() ? sname[0] == '/' : false;
		if (absolute || (vRefNum == 0 && dirID == 0))
		{

			std::string leaf;
			std::string path;
			int parentID;

			path = realpath(sname);
			if (path.empty())
			{
				std::memset(memoryPointer(spec), 0, 8);
				return MacOS::mFulErr;	
			}

			int pos = path.find_last_of('/');
			if (pos == path.npos)
			{
				// file is relative to cwd.
				leaf = std::move(path);
				parentID = 0;
			}
			else
			{
				leaf = path.substr(pos + 1);
				path = path.substr(0, pos + 1); // include the /
				parentID = FSSpecManager::IDForPath(path, true);
			}


			memoryWriteWord(vRefNum, spec + 0);
			memoryWriteLong(parentID, spec + 2);
			// write the filename...
			ToolBox::WritePString(spec + 6, leaf);

			return 0;
		}

		else
		{
			fprintf(stderr, "FSMakeFSSpec(%04x, %08x) not yet supported\n", vRefNum, dirID);
			exit(1);
		}



		return 0;
	}

	uint16_t FSpOpenDF()
	{
		// FUNCTION FSpOpenDF (spec: FSSpec; permission: SignedByte; VAR refNum: Integer): OSErr;

		uint32_t spec;
		uint8_t permission;
		uint32_t refNum;

		StackFrame<10>(spec, permission, refNum);

		int parentID = memoryReadLong(spec + 2);
		std::string sname = ToolBox::ReadPString(spec + 6, false);

		Log("     FSpOpenDF(%s, %02x, %04x)\n",  sname.c_str(), permission, refNum);

		sname = OS::FSSpecManager::ExpandPath(sname, parentID);
		if (sname.empty())
		{
			return MacOS::dirNFErr;
		}

		int fd = Internal::FDEntry::open(sname, permission, 0);
		if (fd < 0) return fd;

		memoryWriteWord(fd, refNum);				

		return 0;
	}

	uint16_t FSpGetFInfo()
	{
		// FSpGetFInfo (spec: FSSpec; VAR fndrInfo: FInfo): OSErr;

		uint32_t spec;
		uint32_t finderInfo;
		uint16_t d0;

		StackFrame<8>(spec, finderInfo);

		int parentID = memoryReadLong(spec + 2);

		std::string leaf = ToolBox::ReadPString(spec + 6, false);
		std::string path = FSSpecManager::PathForID(parentID);

		path += leaf;

		Log("     FSpGetFInfo(%s, %08x)\n",  path.c_str(), finderInfo);


		d0 = Internal::GetFinderInfo(path, memoryPointer(finderInfo), false);
		return d0;
	}

	uint16_t FSpSetFInfo()
	{
		// FSpSetFInfo (spec: FSSpec; VAR fndrInfo: FInfo): OSErr;

		uint32_t spec;
		uint32_t finderInfo;
		uint16_t d0 = 0;

		StackFrame<8>(spec, finderInfo);

		int parentID = memoryReadLong(spec + 2);

		std::string leaf = ToolBox::ReadPString(spec + 6, false);
		std::string path = FSSpecManager::PathForID(parentID);

		path += leaf;

		Log("     FSpSetFInfo(%s, %08x)\n",  path.c_str(), finderInfo);


		d0 = Internal::SetFinderInfo(path, memoryPointer(finderInfo), false);

		return d0;
	}


	uint16_t FSpCreate(void)
	{

		// FUNCTION FSpCreate (spec: FSSpec; creator: OSType;
		//                     fileType: OSType; scriptTag: ScriptCode): 
		// OSErr;

		uint16_t d0 = 0;
		uint32_t sp;
		uint32_t spec;
		uint32_t creator;
		uint32_t fileType;
		uint16_t scriptTag;
		int fd;

		sp = StackFrame<14>(spec, creator, fileType, scriptTag);


		int parentID = memoryReadLong(spec + 2);
		std::string sname = ToolBox::ReadPString(spec + 6, false);

		Log("     FSpCreate(%s, %08x ('%s'), %08x ('%s'), %02x)\n",  
			sname.c_str(), 
			creator, ToolBox::TypeToString(creator).c_str(),
			fileType, ToolBox::TypeToString(fileType).c_str(),
			scriptTag);

		sname = OS::FSSpecManager::ExpandPath(sname, parentID);
		if (sname.empty())
		{
			return MacOS::dirNFErr;
		}


		fd = ::open(sname.c_str(), O_WRONLY | O_CREAT | O_EXCL, 0666);

		if (fd < 0)
		{
			return macos_error_from_errno();
		}
		else
		{
			::close(fd);
		}

		d0 = OS::Internal::SetFinderInfo(sname, fileType, creator);

		return d0;
	}




	uint16_t FSpDelete(void)
	{
		// FUNCTION FSpDelete (spec: FSSpec): OSErr;

		/*
		 * The FSpDelete function  removes a file or  directory. If the
		 * specified  target is  a file,  both  forks of  the file  are
		 * deleted. The file ID reference, if any, is removed.
		 *
		 * A file must be closed before you can delete it. Similarly, a
		 * directory must  be empty  before you can  delete it.  If you
		 * attempt  to delete  an open  file or  a nonempty  directory,
		 * FSpDelete returns  the result  code fBsyErr.  FSpDelete also
		 * returns the result code fBsyErr if the directory has an open
		 * working directory associated with it.
		 */

		uint32_t spec;
		struct stat st;

		StackFrame<4>(spec);

		std::string sname = ReadFSSpec(spec);

		Log("     FSpDelete(%s)\n", sname.c_str());


		if (::lstat(sname.c_str(), &st) < 0)
			return macos_error_from_errno();

		int ok = 0;
		if (S_ISDIR(st.st_mode))
			ok = ::rmdir(sname.c_str());
		else 
			ok = ::unlink(sname.c_str());

		if (ok < 0)
			return macos_error_from_errno();

		return 0;
	}


	uint16_t ResolveAliasFile()
	{
		// FUNCTION ResolveAliasFile (VAR theSpec: FSSpec;
		//                            resolveAliasChains: Boolean;
		//                            VAR targetIsFolder: Boolean;
		//                            VAR wasAliased: Boolean): OSErr;

		uint32_t spec;
		uint16_t resolveAliasChains;
		uint32_t targetIsFolder;
		uint32_t wasAliased;

		StackFrame<14>(spec, resolveAliasChains, targetIsFolder, wasAliased);

		int parentID = memoryReadLong(spec + 2);

		std::string leaf = ToolBox::ReadPString(spec + 6, false);
		std::string path = FSSpecManager::PathForID(parentID);

		path += leaf;

		Log("     ResolveAliasFile(%s)\n", path.c_str());

		struct stat st;
		int rv;

		rv = ::stat(path.c_str(), &st);
		if (rv < 0)
		{
			if (wasAliased) memoryWriteWord(0, wasAliased);
			if (targetIsFolder) memoryWriteWord(0, targetIsFolder);

			return macos_error_from_errno();
		}

		if (targetIsFolder)
				memoryWriteWord(S_ISDIR(st.st_mode) ? 1 : 0, targetIsFolder);

		// don't bother pretending a soft link is an alias.
		if (wasAliased) memoryWriteWord(0, wasAliased);

		return 0;
	}



	uint16_t HighLevelHFSDispatch(uint16_t trap)
	{

		/*
		 * $0001 FSMakeFSSpec
		 * $0002 FSpOpenDF
		 * $0003 FSpOpenRF
		 * $0004 FSpCreate
		 * $0005 FSpDirCreate
		 * $0006 FSpDelete
		 * $0007 FSpGetFInfo
		 * $0008 FSpSetFInfo
		 * $0009 FSpSetFLock
		 * $000A FSpRstFLock
		 * $000B FSpRename
		 * $000C FSpCatMove
		 * $000D FSpOpenResFile
		 * $000E FSpCreateResFile
		 * $000F FSpExchangeFiles
		 */		

		uint16_t selector;
		uint16_t d0;

		selector = cpuGetDReg(0) & 0xffff;
		Log("%04x HighLevelHFSDispatch(%04x)\n", trap, selector);

		switch (selector)
		{
			case 0x0001:
				d0 = FSMakeFSSpec();
				break;

			case 0x0002:
				d0 = FSpOpenDF();
				break;

			case 0x0004:
				d0 = FSpCreate();
				break;

			case 0x0006:
				d0 = FSpDelete();
				break;

			case 0x0007:
				d0 = FSpGetFInfo();
				break;

			case 0x0008:
				d0 = FSpSetFInfo();
				break;

				// RM calls handle their return value.
			case 0x000d:
				d0 = RM::FSpOpenResFile();
				return d0;
				break;

			case 0x000e:
				d0 = RM::FSpCreateResFile();
				return d0;
				break;

			default:
				fprintf(stderr, "HighLevelHFSDispatch selector %04x not yet supported\n", selector);
				exit(1);

		}

		ToolReturn<2>(-1, d0);
		return d0;

	}


}
