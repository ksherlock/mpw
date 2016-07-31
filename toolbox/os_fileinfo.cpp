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

#include <macos/sysequ.h>
#include <macos/errors.h>

#include "os.h"
#include "os_internal.h"
#include "toolbox.h"
#include "stackframe.h"
#include "fs_spec.h"

#include <native/native.h>

using ToolBox::Log;

using MacOS::macos_error_from_errno;


#if __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ < 1050
#define st_birthtime st_mtime
#endif


namespace OS {


	static uint16_t FileInfoByName(const std::string &sname, uint32_t parm) {

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
			_ioFlVersNum = 31,
			_ioFlFndrInfo = 32,
			_ioFlNum = 48, // ioDirID in other
			_ioFlStBlk = 52,
			_ioFlLgLen = 54,
			_ioFlPyLen = 58,
			_ioFlRStBlk = 62,
			_ioFlRLgLen = 64,
			_ioFlRPyLen = 68,
			_ioFlCrDat = 72,
			_ioFlMdDat = 76,

			_ioDirID = 48,
		};

		using namespace native;

		file_info fi;
		auto err = native::get_file_info(sname, fi);
		if (err) return err;


		// if this is not a file, error out?

		std::memcpy(memoryPointer(parm + _ioFlFndrInfo), fi.finder_info, 16);


		// file reference number
		memoryWriteWord(0, parm + _ioFRefNum);
		// file attributes
		memoryWriteByte(0, parm + _ioFlAttrib);
		// version (unused)
		memoryWriteByte(0, parm + _ioFlVersNum);

		// file id
		memoryWriteLong(0, parm + _ioFlNum);


		// file size
		memoryWriteWord(0, parm + _ioFlStBlk);
		memoryWriteLong(fi.data_logical_size, parm + _ioFlLgLen);
		memoryWriteLong(fi.data_physical_size, parm + _ioFlPyLen);

		// create date.
		memoryWriteLong(fi.create_date, parm + _ioFlCrDat);
		memoryWriteLong(fi.modify_date, parm + _ioFlMdDat);

		// res fork...
		memoryWriteWord(0, parm + _ioFlRStBlk);
		memoryWriteLong(fi.resource_logical_size, parm + _ioFlRLgLen);
		memoryWriteLong(fi.resource_physical_size, parm + _ioFlRPyLen);

		return 0;
	}

	uint16_t GetFileInfo(uint16_t trap)
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
			_ioFlVersNum = 31,
			_ioFlFndrInfo = 32,
			_ioFlNum = 48, // ioDirID in other
			_ioFlStBlk = 52,
			_ioFlLgLen = 54,
			_ioFlPyLen = 58,
			_ioFlRStBlk = 62,
			_ioFlRLgLen = 64,
			_ioFlRPyLen = 68,
			_ioFlCrDat = 72,
			_ioFlMdDat = 76,

			_ioDirID = 48,
		};




		uint16_t d0;

		uint32_t parm = cpuGetAReg(0);

		Log("%04x GetFileInfo($%08x)\n", trap, parm);

		//uint32_t ioCompletion = memoryReadLong(parm + _ioCompletion);
		uint32_t ioNamePtr = memoryReadLong(parm + _ioNamePtr);
		//uint16_t ioVRefNum = memoryReadWord(parm + _ioVRefNum);
		//uint8_t ioFVersNum = memoryReadByte(parm + _ioFVersNum);
		int16_t ioFDirIndex = memoryReadWord(parm + _ioFDirIndex);

		if (ioFDirIndex <= 0)
		{
			// based name
			std::string sname;

			if (!ioNamePtr)
			{
				d0 = MacOS::bdNamErr;
				memoryWriteWord(d0, parm + _ioResult);
				return d0;
			}

			sname = ToolBox::ReadPString(ioNamePtr, true);
			// a20c HGetFileInfo uses a the dir id
			if (trap == 0xa20c)
			{
				uint32_t ioDirID = memoryReadLong(parm + _ioDirID);
				sname = FSSpecManager::ExpandPath(sname, ioDirID);
			}

			Log("     GetFileInfo(%s)\n", sname.c_str());

			d0 = FileInfoByName(sname, parm);
			memoryWriteWord(d0, parm + _ioResult);
			return d0;

		}
		else
		{
			fprintf(stderr, "GetFileInfo -- ioFDirIndex not yet supported\n");
			exit(1);
		}

		// if iocompletion handler && asyn call....

		return 0;
	}


	uint16_t SetFileInfo(uint16_t trap)
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
			_ioFlVersNum = 31,
			_ioFlFndrInfo = 32,
			_ioFlNum = 48, // ioDirID in other
			_ioFlStBlk = 52,
			_ioFlLgLen = 54,
			_ioFlPyLen = 58,
			_ioFlRStBlk = 62,
			_ioFlRLgLen = 64,
			_ioFlRPyLen = 68,
			_ioFlCrDat = 72,
			_ioFlMdDat = 76,

			_ioDirID = 48,
		};


		std::string sname;
		uint16_t d0;


		uint32_t parm = cpuGetAReg(0);

		Log("%04x SetFileInfo($%08x)\n", trap, parm);

		//uint32_t ioCompletion = memoryReadLong(parm + _ioCompletion);
		uint32_t ioNamePtr = memoryReadLong(parm + _ioNamePtr);
		//uint16_t ioVRefNum = memoryReadWord(parm + _ioVRefNum);
		//uint8_t ioFVersNum = memoryReadByte(parm + _ioFVersNum);
		//int16_t ioFDirIndex = memoryReadWord(parm + _ioFDirIndex);

		if (!ioNamePtr)
		{
			d0 = MacOS::bdNamErr;
			memoryWriteWord(d0, parm + _ioResult);
			return d0;
		}

		sname = ToolBox::ReadPString(ioNamePtr, true);

		// a20d HSetFileInfo uses a the dir id
		if (trap == 0xa20d)
		{
			uint32_t ioDirID = memoryReadLong(parm + _ioDirID);
			sname = FSSpecManager::ExpandPath(sname, ioDirID);
		}

		Log("     SetFileInfo(%s)\n", sname.c_str());




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


		}

		d0 = Internal::SetFinderInfo(sname, memoryPointer(parm + 32), false);
		if (d0 == 0) d0 = Internal::SetFileDates(sname, memoryReadLong(parm + _ioFlCrDat), memoryReadLong(parm + _ioFlMdDat), 0);
		memoryWriteWord(d0, parm + _ioResult);
		return d0;
	}







	uint16_t HGetFileInfo(uint16_t trap)
	{

		enum { // HFileParam
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
			_ioFlVersNum = 31,
			_ioFlFndrInfo = 32,
			_ioDirID = 48, // ioFLNum in other
			_ioFlStBlk = 52,
			_ioFlLgLen = 54,
			_ioFlPyLen = 58,
			_ioFlRStBlk = 62,
			_ioFlRLgLen = 64,
			_ioFlRPyLen = 68,
			_ioFlCrDat = 72,
			_ioFlMdDat = 76,
		};

		// close enough... for now.
		return GetFileInfo(trap);
	}



}
