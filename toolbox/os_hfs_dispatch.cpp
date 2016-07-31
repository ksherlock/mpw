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
#include <cassert>
#include <cctype>
#include <ctime>
#include <algorithm>
#include <chrono>
#include <deque>
#include <string>
#include <cstring>

#include <sys/xattr.h>
#include <sys/stat.h>
#include <sys/paths.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>

#include <strings.h>

#include <cpu/defs.h>
#include <cpu/CpuModule.h>
#include <cpu/fmem.h>

#include <macos/errors.h>

#include "os.h"
#include "os_internal.h"
#include "toolbox.h"
#include "stackframe.h"
#include "fs_spec.h"

#include <native/native.h>


#if __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ < 1050
#define st_birthtime st_mtime
#endif

using ToolBox::Log;

using MacOS::macos_error_from_errno;

namespace OS {

	uint16_t PBGetWDInfo(uint32_t parm)
	{

		enum { // WDPBRec
			_qLink = 0,
			_qType = 4,
			_ioTrap = 6,
			_ioCmdAddr = 8,
			_ioCompletion = 12,
			_ioResult = 16,
			_ioNamePtr = 18,
			_ioVRefNum = 22,
			_filler1 = 24,
			_ioWDIndex = 26,
			_ioWDProcID = 28,
			_ioWDVRefNum = 32,
			_filler2 = 34,
			_ioWDDirID = 48,
		};

		/*
		 * TODO - this is a Massive hack.
		 * MPW returns this: for a 0/0 entry.
		 * PBGetWDInfo
		 * 0
		 * ioNamePtr: MacOS
		 * ioVRefNum: -1
		 * ioWDProcID: 0
		 * ioWDVRefNum: -1
		 * ioWDDirID: 2 << Root volume is always entry #2.
		 */

		uint16_t d0 = 0;

		uint32_t ioNamePtr = memoryReadLong(parm + _ioNamePtr);
		uint16_t ioVRefNum = memoryReadWord(parm + _ioVRefNum);
		uint16_t ioWDIndex = memoryReadWord(parm + _ioWDIndex);
		//uint32_t ioWDProcID = memoryReadLong(parm + _ioWDProcID);
		uint16_t ioWDVRefNum = memoryReadWord(parm + _ioWDVRefNum);

		Log("     PBGetWDInfo($%04x, $%04x, $%04x)\n", ioVRefNum, ioWDIndex, ioWDVRefNum);

		// todo -- need to  expand the fsspec code to give a id #
		// to all filse and directories.
		// and move to common internal code.


		if (ioNamePtr) ToolBox::WritePString(ioNamePtr, std::string("MacOS"));
		memoryWriteWord(-1, parm + _ioVRefNum);
		memoryWriteLong(0, parm + _ioWDProcID);
		memoryWriteWord(-1, parm + _ioWDVRefNum);
		memoryWriteLong(2, parm + _ioWDDirID);

		d0 = 0;
		memoryWriteWord(d0, parm + _ioResult);
		return d0;
	}

	static uint16_t CatInfoByName(std::string &sname, uint32_t parm) {

		enum { // FileParam
			_qLink = 0,
			_qType = 4,
			_ioTrap = 6,
			_ioCmdAddr = 8,
			_ioCompletion = 12,
			_ioResult = 16,
			_ioNamePtr = 18,
			_ioVRefNum = 22,
			_ioFRefNum = 24,
			_ioFVersNum = 26,
			_filler1 = 27,
			_ioFDirIndex = 28,
			_ioFlAttrib = 30,
			_ioACUser = 31,

			/* HFileInfo */
			_ioFlFndrInfo = 32,
			_ioDirID = 48,
			_ioFlStBlk = 52,
			_ioFlLgLen = 54,
			_ioFlPyLen = 58,
			_ioFlRStBlk = 62,
			_ioFlRLgLen = 64,
			_ioFlRPyLen = 68,
			_ioFlCrDat = 72,
			_ioFlMdDat = 76,
			_ioFlBkDat = 80,
			_ioFlXFndrInfo = 84,
			_ioFlParID = 100,
			_ioFlClpSiz = 104,

			/* DirInfo */
			_ioDrUsrWds = 32,
			_ioDrDirID = 48,
			_ioDrNmFls = 52,
			_filler3 = 54,
			_ioDrCrDat = 72,
			_ioDrMdDat = 76,
			_ioDrBkDat = 80,
			_ioDrFndrInfo = 84,
			_ioDrParID = 100,
		};


		using namespace native;

		file_info fi;
		auto rv = get_file_info(sname, fi);
		if (rv) return rv;


		memoryWriteByte(0, parm + _ioACUser);

		if (fi.type == file_info::directory) {

			// bit 4 - is a directory.
			memoryWriteByte(1 << 4, parm + _ioFlAttrib);

			std::memset(memoryPointer(parm + _ioDrUsrWds), 0, 16); // DInfo

			uint32_t dirID = FSSpecManager::IDForPath(sname);

			memoryWriteLong(dirID, parm + _ioDrDirID);

			memoryWriteWord(fi.entry_count, parm + _ioDrNmFls); // ioDrNmFls - # of files in dir

			memoryWriteLong(fi.create_date, parm + _ioDrCrDat); // create
			memoryWriteLong(fi.modify_date, parm + _ioDrMdDat); // modify
			memoryWriteLong(fi.backup_date, parm + _ioDrBkDat); // backup		

			std::memset(memoryPointer(parm + _ioDrFndrInfo), 0, 16); // DXInfo
			memoryWriteLong(0, parm + _ioDrParID);

		} else {

			memoryWriteByte(0, parm + _ioFlAttrib);


			std::memcpy(memoryPointer(parm + _ioFlFndrInfo), fi.finder_info , 16); // finder info
			std::memcpy(memoryPointer(parm + _ioFlXFndrInfo), fi.finder_info + 16, 16); // FXInfo

			memoryWriteLong(0, parm + _ioDirID);
			memoryWriteWord(0, parm + _ioFlStBlk);
			memoryWriteWord(0, parm + _ioFlRStBlk);

			memoryWriteLong(fi.create_date, parm + _ioFlCrDat); // create
			memoryWriteLong(fi.modify_date, parm + _ioFlMdDat); // modify
			memoryWriteLong(fi.backup_date, parm + _ioFlBkDat); // backup

			memoryWriteLong(fi.data_logical_size, parm + _ioFlLgLen);
			memoryWriteLong(fi.data_physical_size, parm + _ioFlPyLen);
			memoryWriteLong(fi.resource_logical_size, parm + _ioFlRLgLen);
			memoryWriteLong(fi.resource_physical_size, parm + _ioFlRPyLen);

			memoryWriteWord(0, parm + _ioFlParID);
			memoryWriteWord(0, parm + _ioFlClpSiz);

		}
		return 0;

#if 0
		struct stat st;

		if (::stat(sname.c_str(), &st) < 0)
		{
			return macos_error_from_errno();
		}

		if (S_ISDIR(st.st_mode))
		{
			// bit 4 - is a directory.
			memoryWriteByte(1 << 4, parm + _ioFlAttrib);
			memoryWriteByte(0, parm + _ioACUser);

			std::memset(memoryPointer(parm + _ioDrUsrWds), 0, 16); // DInfo

			// cw68k expects the directory ID to be set.

			uint32_t dirID = FSSpecManager::IDForPath(sname);

			memoryWriteLong(dirID, parm + _ioDrDirID);

			// the links count should be ~= number of dirents ( +2 for . and ..)
			int links = st.st_nlink - 2;
			if (links < 0) links = 0;
			if (links > 65535) links = 65535;

			memoryWriteWord(links, parm + _ioDrNmFls); // ioDrNmFls - # of files in dir

			memoryWriteLong(UnixToMac(st.st_birthtime), parm + _ioDrCrDat); // create
			memoryWriteLong(UnixToMac(st.st_mtime), parm + _ioDrMdDat); // modify
			memoryWriteLong(UnixToMac(st.st_mtime), parm + _ioDrBkDat); // backup

			std::memset(memoryPointer(parm + _ioDrFndrInfo), 0, 16); // DXInfo
			memoryWriteLong(0, parm + _ioDrParID);
		}
		else
		{
			// st_blksize is the optimal block read size.  st_blocks is the number of 512-byte blocks.  I think.
			// st_blocks seems to include both data and resource blocks.  grrr....

			memoryWriteByte(0, parm + _ioFlAttrib);

			memoryWriteByte(0, parm + _ioACUser);
			Internal::GetFinderInfo(sname, memoryPointer(parm + _ioFlFndrInfo), false); // finder info
			memoryWriteLong(0, parm + _ioDirID);
			memoryWriteWord(0, parm + _ioFlStBlk);
			memoryWriteWord(0, parm + _ioFlRStBlk);

			memoryWriteLong(st.st_size, parm + _ioFlLgLen);
			memoryWriteLong(st.st_blocks * 512, parm + _ioFlPyLen);

			// resource info... below

			memoryWriteLong(UnixToMac(st.st_birthtime), parm + _ioFlCrDat); // create
			memoryWriteLong(UnixToMac(st.st_mtime), parm + _ioFlMdDat); // modify
			memoryWriteLong(UnixToMac(st.st_mtime), parm + _ioFlBkDat); // backup

			std::memset(memoryPointer(parm + _ioFlXFndrInfo), 0, 16); // FXInfo

			memoryWriteWord(0, parm + _ioFlParID);
			memoryWriteWord(0, parm + _ioFlClpSiz);

			struct {
				uint32_t length;
				off_t data_logical_size;
				off_t data_physical_size;
				off_t resource_logical_size;
				off_t resource_physical_size;
			} __attribute__((aligned(4), packed)) buffer;

			struct attrlist at;
			memset(&buffer, 0, sizeof(buffer));
			memset(&at, 0, sizeof(at));

			at.bitmapcount = ATTR_BIT_MAP_COUNT;
			at.commonattr = 0 ;
			at.fileattr = ATTR_FILE_DATALENGTH | ATTR_FILE_DATAALLOCSIZE | ATTR_FILE_RSRCLENGTH | ATTR_FILE_RSRCALLOCSIZE;

			int ok = getattrlist(sname.c_str(), &at, &buffer, sizeof(buffer), 0);
			if (ok == 0 && buffer.length == sizeof(buffer)) {


				memoryWriteLong(buffer.data_logical_size, parm + _ioFlLgLen);
				memoryWriteLong(buffer.data_physical_size, parm + _ioFlPyLen);

				memoryWriteLong(buffer.resource_logical_size, parm + _ioFlRLgLen);
				memoryWriteLong(buffer.resource_physical_size, parm + _ioFlRPyLen);

			} else {

				sname.append(_PATH_RSRCFORKSPEC);
				if (::stat(sname.c_str(), &st) >= 0)
				{
					memoryWriteWord(0, parm + _ioFlRStBlk);
					memoryWriteLong(st.st_size, parm + _ioFlRLgLen);
					memoryWriteLong(0/* st.st_blocks * 512*/, parm + _ioFlRPyLen);
				}
				else
				{
					memoryWriteLong(0, parm + _ioFlRLgLen);
					memoryWriteLong(0, parm + _ioFlRPyLen);
				}

			}



		}
		return 0;
		#endif
	}

	uint16_t PBGetCatInfo(uint32_t parm)
	{

		enum { // FileParam
			_qLink = 0,
			_qType = 4,
			_ioTrap = 6,
			_ioCmdAddr = 8,
			_ioCompletion = 12,
			_ioResult = 16,
			_ioNamePtr = 18,
			_ioVRefNum = 22,
			_ioFRefNum = 24,
			_ioFVersNum = 26,
			_filler1 = 27,
			_ioFDirIndex = 28,
			_ioFlAttrib = 30,
			_ioACUser = 31,

			/* HFileInfo */
			_ioFlFndrInfo = 32,
			_ioDirID = 48,
			_ioFlStBlk = 52,
			_ioFlLgLen = 54,
			_ioFlPyLen = 58,
			_ioFlRStBlk = 62,
			_ioFlRLgLen = 64,
			_ioFlRPyLen = 68,
			_ioFlCrDat = 72,
			_ioFlMdDat = 76,
			_ioFlBkDat = 80,
			_ioFlXFndrInfo = 84,
			_ioFlParID = 100,
			_ioFlClpSiz = 104,

			/* DirInfo */
			_ioDrUsrWds = 32,
			_ioDrDirID = 48,
			_ioDrNmFls = 52,
			_filler3 = 54,
			_ioDrCrDat = 72,
			_ioDrMdDat = 76,
			_ioDrBkDat = 80,
			_ioDrFndrInfo = 84,
			_ioDrParID = 100,
		};


		uint16_t d0;

		// yuck. this is sort of a getdirent/stat call....

		//uint32_t ioCompletion = memoryReadLong(parm + _ioCompletion);
		uint32_t ioNamePtr = memoryReadLong(parm + _ioNamePtr);
		//uint16_t ioVRefNum = memoryReadWord(parm + _ioVRefNum);
		//uint8_t ioFVersNum = memoryReadByte(parm + _ioFVersNum);
		int16_t ioFDirIndex = memoryReadWord(parm + _ioFDirIndex);
		uint32_t ioDirID = memoryReadLong(parm + _ioDirID);

		if (ioFDirIndex == 0)
		{
			/*
			 * If this field contains 0, PBGetCatInfo returns
			 * information about the file or directory whose name is
			 * specified in the ioNamePtr field and that is located in the
			 * directory specified by the ioVRefNum field. (Once again, if
			 * ioVRefNum contains a volume reference number, the specified
			 * directory is that volume's root directory.)
			 */

			if (!ioNamePtr)
			{
				memoryWriteWord(MacOS::bdNamErr, parm + _ioResult);
				return MacOS::bdNamErr;
			}

			std::string sname = ToolBox::ReadPString(ioNamePtr, true);
			sname = FSSpecManager::ExpandPath(sname, ioDirID);

			Log("     PBGetCatInfo(%s)\n", sname.c_str());
			d0 = CatInfoByName(sname, parm);


		}

		else if (ioFDirIndex < 0) {

			/*
			 * If this field contains a negative number, PBGetCatInfo ignores the
			 * ioNamePtr field and returns information about the directory specified
			 * in the ioDirID field. If both ioDirID and ioVRefNum are set to 0,
			 * PBGetCatInfo returns information about the current default directory.
			 */


			Log("     PBGetCatInfo(%04x)\n", ioDirID);

			std::string sname = FSSpecManager::PathForID(ioDirID);
			if (sname.empty()) {

				char buffer[MAXPATHLEN];
				char *cp;

				cp = getcwd(buffer, sizeof(buffer));
				sname = cp ? cp : ".";				
			}
			d0 = CatInfoByName(sname, parm);

			if (ioNamePtr) {
				std::string basename;
				auto pos = sname.rfind('/');
				if (pos == sname.npos) basename = std::move(sname);
				else basename = sname.substr(pos+1);
				ToolBox::WritePString(ioNamePtr, basename);
			}

		}

		else
		{

			/*
			 * If this field contains a positive number, PBGetCatInfo returns
			 * information about the file or directory having that directory index in
			 * the directory specified by the ioVRefNum field. (If ioVRefNum contains a
			 * volume reference number, the specified directory is that volume's root
			 * directory.)
			 */


			Log("     PBGetCatInfo(%04x, %04x)\n", ioDirID, ioFDirIndex);


			std::string sname = FSSpecManager::PathForID(ioDirID);
			if (sname.empty()) sname = ".";
			sname = OS::realpath(sname);

			// if sname == "", error...
			DIR *dp;
			struct dirent *dir;

			dp = opendir(sname.c_str());
			if (!dp) {
				d0 = macos_error_from_errno();
				memoryWriteWord(d0, parm + _ioResult);
				return d0;
			}

			while ((dir = readdir(dp))) {
				if (dir->d_name[0] == '.') {
					if (!strcmp(dir->d_name, ".")) continue;
					if (!strcmp(dir->d_name, "..")) continue;
				}
				if (dir->d_namlen > 255) continue;  // too long!
				if (--ioFDirIndex == 0) break;
			}

			if (!dir) {
				closedir(dp);
				d0 = MacOS::fnfErr;
				memoryWriteWord(d0, parm + _ioResult);
				return d0;
			}

			if (ioNamePtr) {
				ToolBox::WritePString(ioNamePtr, dir->d_name);
			}

			sname.push_back('/');
			sname.append(dir->d_name);
			closedir(dp);

	
			d0 = CatInfoByName(sname, parm);
		}

		memoryWriteWord(d0, parm + _ioResult);
		return d0;

		return 0;
	}


	uint16_t PBSetCatInfo(uint32_t parm)
	{

		enum { // FileParam
			_qLink = 0,
			_qType = 4,
			_ioTrap = 6,
			_ioCmdAddr = 8,
			_ioCompletion = 12,
			_ioResult = 16,
			_ioNamePtr = 18,
			_ioVRefNum = 22,
			_ioFRefNum = 24,
			_ioFVersNum = 26,
			_filler1 = 27,
			_ioFDirIndex = 28,
			_ioFlAttrib = 30,
			_ioACUser = 31,

			/* HFileInfo */
			_ioFlFndrInfo = 32,
			_ioDirID = 48,
			_ioFlStBlk = 52,
			_ioFlLgLen = 54,
			_ioFlPyLen = 58,
			_ioFlRStBlk = 62,
			_ioFlRLgLen = 64,
			_ioFlRPyLen = 68,
			_ioFlCrDat = 72,
			_ioFlMdDat = 76,
			_ioFlBkDat = 80,
			_ioFlXFndrInfo = 84,
			_ioFlParID = 100,
			_ioFlClpSiz = 104,

			/* DirInfo */
			_ioDrUsrWds = 32,
			_ioDrDirID = 48,
			_ioDrNmFls = 52,
			_filler3 = 54,
			_ioDrCrDat = 72,
			_ioDrMdDat = 76,
			_ioDrBkDat = 80,
			_ioDrFndrInfo = 84,
			_ioDrParID = 100,
		};

		uint16_t d0;


		//uint32_t ioCompletion = memoryReadLong(parm + _ioCompletion);
		uint32_t ioNamePtr = memoryReadLong(parm + _ioNamePtr);
		//uint16_t ioVRefNum = memoryReadWord(parm + _ioVRefNum);
		//uint8_t ioFVersNum = memoryReadByte(parm + _ioFVersNum);
		//int16_t ioFDirIndex = memoryReadWord(parm + _ioFDirIndex);

		if (!ioNamePtr)
		{
			memoryWriteWord(MacOS::bdNamErr, parm + _ioResult);
			assert("PGSetCatInfo - no name.");
			return MacOS::bdNamErr;
		}

		std::string sname = ToolBox::ReadPString(ioNamePtr, true);
		{
			uint32_t ioDirID = memoryReadLong(parm + _ioDirID);
			sname = FSSpecManager::ExpandPath(sname, ioDirID);
		}

		Log("     PBSetCatInfo(%s)\n", sname.c_str());


		// check if the file actually exists
		{
			struct stat st;
			int ok;

			ok = ::stat(sname.c_str(), &st);
			if (ok < 0)
			{
				d0 = macos_error_from_errno();
				memoryWriteWord(d0, parm + _ioResult);
				return d0;
			}

			// just nop if it's a directory.
			if (S_ISDIR(st.st_mode))
			{
				d0 = 0;

				d0 = Internal::SetFileDates(sname,
					memoryReadLong(parm + _ioDrCrDat),
					memoryReadLong(parm + _ioDrMdDat),
					memoryReadLong(parm + _ioDrBkDat));

				memoryWriteWord(d0, parm + _ioResult);
				return d0;
			}
		}


		// set the finder info.  could also call utimes or setattrlist, I suppose.
		d0 = Internal::SetFinderInfo(sname, memoryPointer(parm + _ioFlFndrInfo), false);

		if (d0 == 0) d0 = Internal::SetFileDates(sname,
			memoryReadLong(parm + _ioFlCrDat),
			memoryReadLong(parm + _ioFlMdDat),
			memoryReadLong(parm + _ioFlBkDat));


		memoryWriteWord(d0, parm + _ioResult);
		return d0;

	}


	uint16_t PBOpenDF(uint32_t paramBlock)
	{
		Log("     PBOpenDF\n");
		// same as Open but slightly different handling of . files.
		return OS::OpenCommon(paramBlock, false, false);
	}


	uint16_t PBHOpenDF(uint32_t paramBlock)
	{
		// PBHOpen... is identical to PBOpen... except
		// that it accepts a directory ID in ioDirID.

		Log("     PBHOpenDF\n");
		return OS::OpenCommon(paramBlock, true, false);
	}


	uint16_t PBHOpenDeny(uint32_t paramBlock)
	{
		// AccessParam.ioDenyModes short word matches
		// up with the permission byte considering it's big-endian.

		Log("     PBHOpenDeny\n");
		return OS::OpenCommon(paramBlock, true, false);
	}

	uint16_t PBHOpenRFDeny(uint32_t paramBlock)
	{
		Log("     PBHOpenRFDeny\n");
		return OS::OpenCommon(paramBlock, true, true);
	}

	uint16_t FSDispatch(uint16_t trap)
	{

		// TODO -- check new hfs bit ( (trap & 0x0200) == 0x0200)
		// TODO -- check async bit ((trap & 0x0400) == 0x0400)



		uint32_t selector = cpuGetDReg(0);
		uint32_t paramBlock = cpuGetAReg(0);

		Log("%04x FSDispatch(%08x, %08x)\n", trap, selector, paramBlock);

		switch (selector)
		{

			case 0x001a:
				return PBOpenDF(paramBlock);
				break;

			default:
				fprintf(stderr, "FSDispatch: selector %08x not implemented\n",
					selector);
				exit(1);
		}
		return 0;
	}

	uint16_t HFSDispatch(uint16_t trap)
	{

		uint32_t selector = cpuGetDReg(0);
		uint32_t paramBlock = cpuGetAReg(0);

		Log("%04x HFSDispatch(%08x, %08x)\n", trap, selector, paramBlock);

		switch (selector)
		{
			case 0x0007:
				return PBGetWDInfo(paramBlock);
				break;

			case 0x0009:
				return PBGetCatInfo(paramBlock);
				break;

			case 0x000a:
				return PBSetCatInfo(paramBlock);
				break;

			case 0x001a:
				return PBHOpenDF(paramBlock);

			case 0x0038:
				return PBHOpenDeny(paramBlock);

			case 0x0039:
				return PBHOpenRFDeny(paramBlock);

			default:
				fprintf(stderr, "HFSDispatch: selector %08x not implemented\n",
					selector);
				exit(1);
		}
		return 0;
	}



}
