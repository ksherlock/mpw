/*
 * Copyright (c) 2015, Kelvin W Sherlock
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
#include <macos/traps.h>

#include "os.h"
#include "os_internal.h"
#include "toolbox.h"
#include "fs_spec.h"

using ToolBox::Log;

using MacOS::macos_error_from_errno;

namespace OS {


	uint16_t HGetVInfo(uint16_t trap)
	{


		/*
		 * The PBHGetVInfo function returns information about the specified
		 * volume. If the value of ioVolIndex is positive, the File Manager
		 * attempts to use it to find the volume; for instance, if the
		 * value of ioVolIndex is 2, the File Manager attempts to access
		 * the second mounted volume in the VCB queue. If the value of
		 * ioVolIndex is negative, the File Manager uses ioNamePtr and
		 * ioVRefNum in the standard way to determine the volume. If the
		 * value of ioVolIndex is 0, the File Manager attempts to access
		 * the volume by using ioVRefNum only. The volume reference number
		 * is returned in ioVRefNum, and the volume name is returned in
		 * the buffer whose address you passed in ioNamePtr. You should
		 * pass a pointer to a Str31 value if you want that name returned.
		 * If you pass NIL in the ioNamePtr field, no volume name is
		 * returned.
		 * 
		 * If you pass a working directory reference number in ioVRefNum
		 * (or if the default directory is a subdirectory), the number
		 * of files and directories in the specified directory (the
		 * directory’s valence) is returned in ioVNmFls.
		 * 
		 * You can read the ioVDrvInfo and ioVDRefNum fields to determine
		 * whether the specified volume is online, offline, or ejected.
		 * For online volumes, ioVDrvInfo contains the drive number of
		 * the drive containing the specified volume and hence is always
		 * greater than 0. If the value returned in ioVDrvInfo is 0, the
		 * volume is either offline or ejected. You can determine whether
		 * the volume is offline or ejected by inspecting the value of
		 * the ioVDRefNum field. For online volumes, ioVDRefNum contains
		 * a driver reference number; these numbers are always less than
		 * 0. If the volume is not online, the value of ioVDRefNum is
		 * either the negative of the drive number (if the volume is
		 * offline) or the drive number itself (if the volume is ejected).
		 * 
		 * 
		 * You can get information about all the online volumes by making
		 * repeated calls to PBHGetVInfo, starting with the value of
		 * ioVolIndex set to 1 and incrementing that value until PBHGetVInfo
		 * returns nsvErr.
		 */


		enum {
			/* HVolumeParam */
			_qLink = 0,
			_qType = 4,
			_ioTrap = 6,
			_ioCmdAddr = 8,
			_ioCompletion = 12,
			_ioResult = 16,
			_ioNamePtr = 18,
			_ioVRefNum = 22,
			_filler2 = 24,
			_ioVolIndex = 28,
			_ioVCrDate = 30,
			_ioVLsMod = 34,
			_ioVAtrb = 38,
			_ioVNmFls = 40,
			_ioVBitMap = 42,
			_ioAllocPtr = 44,
			_ioVNmAlBlks = 46,
			_ioVAlBlkSiz = 48,
			_ioVClpSiz = 52,
			_ioAlBlSt = 56,
			_ioVNxtCNID = 58,
			_ioVFrBlk = 62,
			_ioVSigWord = 64,
			_ioVDrvInfo = 66,
			_ioVDRefNum = 68,
			_ioVFSID = 70,
			_ioVBkUp = 72,
			_ioVSeqNum = 76,
			_ioVWrCnt = 78,
			_ioVFilCnt = 82,
			_ioVDirCnt = 86,
			_ioVFndrInfo = 90,
		};



		uint16_t d0;

		uint32_t parm = cpuGetAReg(0);

		Log("%04x HGetVInfo(%08x)\n", trap, parm);

		d0 = MacOS::nsvErr;
		memoryWriteWord(d0, parm + _ioResult);

		return d0;
	}



	uint16_t FlushVol(uint16_t trap)
	{
		/*
		 * On the volume specified by ioNamePtr or ioVRefNum, the PBFlushVol
		 * function writes descriptive information about the volume, the
		 * contents of the associated volume buffer, and all access path buffers
		 * for the volume (if they’ve changed since the last time PBFlushVol
		 * was called).
		 */

		enum {
			_qLink = 0,
			_qType = 4,
			_ioTrap = 6,
			_ioCmdAddr = 8,
			_ioCompletion = 12,
			_ioResult = 16,
			_ioNamePtr = 18,
			_ioVRefNum = 22,
		};

		uint32_t parm = cpuGetAReg(0);

		Log("%04x FlushVol(%08x)\n", trap, parm);		

		// volume is specified with ioNamePtr or ioVRefNum.
		// could go through open fds and fsync(fd), fcntl(fd, F_FULLFSYNC) them.
		memoryWriteWord(0, parm + _ioResult);

		return 0;
	}


	uint16_t GetVol(uint16_t trap)
	{

		/*
		 * The PBGetVol function returns, in ioNamePtr, a pointer to the name
		 * of the default volume (unless ioNamePtr is NIL) and, in ioVRefNum,
		 * its volume reference number. If a default directory was set with a
		 * previous call to PBSetVol, a pointer to its name is returned in
		 * ioNamePtr and its working directory reference number is returned
		 * in ioVRefNum. However, if, in a previous call to HSetVol (or
		 * PBHSetVol), a working directory reference number was passed in,
		 * PBGetVol returns a volume reference number in the ioVRefNum field.
		 *
		 */

		enum {
			_qLink = 0,
			_qType = 4,
			_ioTrap = 6,
			_ioCmdAddr = 8,
			_ioCompletion = 12,
			_ioResult = 16,
			_ioNamePtr = 18,
			_ioVRefNum = 22,
		};

		uint32_t parm = cpuGetAReg(0);

		Log("%04x GetVol(%08x)\n", trap, parm);


		uint32_t namePtr = memoryReadLong(parm + _ioNamePtr);

		memoryWriteWord(0, parm + _ioResult);
		memoryWriteWord(0, parm + _ioVRefNum);

		std::string tmp = "MacOS";
		ToolBox::WritePString(namePtr, tmp);

		return 0;
	}


	uint16_t HGetVol(uint16_t trap)
	{

		/*
		 * The PBHGetVol function returns the default volume and directory last set
		 * by a call to either PBSetVol or PBHSetVol. The reference number of the
		 * default volume is returned in ioVRefNum. The PBHGetVol function returns
		 * a pointer to the volume’s name in the ioNamePtr field. You should pass a
		 * pointer to a Str31 value if you want that name returned. If you pass NIL
		 * in the ioNamePtr field, no volume name is returned.
		 *
		 * WARNING
		 * On exit, the ioVRefNum field contains a working directory reference
		 * number (instead of the volume reference number) if, in the last call to
		 * PBSetVol or PBHSetVol, a working directory reference number was passed
		 * in this field.
		 *
		 * The volume reference number of the volume on which the default directory
		 * exists is returned in ioWDVRefNum. The directory ID of the default
		 * directory is returned in ioWDDirID.
		 */

		 enum {
			/* WDPBRec */
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


		uint32_t parm = cpuGetAReg(0);

		Log("%04x HGetVol(%08x)\n", trap, parm);

		uint32_t namePtr = memoryReadLong(parm + _ioNamePtr);

		memoryWriteWord(0, parm + _ioResult);
		memoryWriteWord(0, parm + _ioVRefNum);
		memoryWriteLong(0, parm + _ioWDProcID);
		memoryWriteWord(0, parm + _ioWDVRefNum);


		// todo -- this should create an FSSpec entry for
		// the current wd and return the id.
		// (FSMakeSpec handles 0 as a dir, so ok for now)
		// ioWDDirID
		memoryWriteLong(0, parm + _ioWDDirID);

		std::string tmp = "MacOS";
		ToolBox::WritePString(namePtr, tmp);

		return 0;
	}


	uint16_t SetVol(uint16_t trap)
	{

		/*
		 * If you pass a volume reference number in ioVRefNum, the PBSetVol
		 * function makes the specified volume the default volume and the root
		 * directory of that volume the default directory. If you pass a working
		 * directory reference number, PBSetVol makes the specified directory the
		 * default directory, and the volume containing that directory the default
		 * volume.
		 */

		enum {
			_qLink = 0,
			_qType = 4,
			_ioTrap = 6,
			_ioCmdAddr = 8,
			_ioCompletion = 12,
			_ioResult = 16,
			_ioNamePtr = 18,
			_ioVRefNum = 22,
		};

		uint16_t d0;

		uint32_t parm = cpuGetAReg(0);

		Log("%04x SetVol(%08x)\n", trap, parm);


		uint32_t ioNamePtr = memoryReadLong(parm + _ioNamePtr);
		uint32_t ioVRefNum = memoryReadWord(parm + _ioVRefNum);


		std::string name = ToolBox::ReadPString(ioNamePtr);

		Log("    SetVol(%s, %d)\n", name.c_str(), ioVRefNum);

		if (name.length() || ioVRefNum)
		{
			fprintf(stderr, "SetVol(%s, %d) is not supported yet.\n", name.c_str(), ioVRefNum);
			exit(1);
		}
		d0 = 0;
		memoryWriteWord(d0, parm + _ioResult);
		return d0;
	}


	uint16_t HSetVol(uint16_t trap)
	{
		/*
		 * The PBHSetVol function sets the default volume and directory to the
		 * volume and directory specified by the ioNamePtr, ioVRefNum, and
		 * ioWDDirID fields.
		 *
		 * The PBHSetVol function sets the default volume to the volume specified
		 * by the ioVRefNum field, which can contain either a volume reference
		 * number or a working directory reference number. If the ioNamePtr
		 * field specifies a full pathname, however, the default volume is set
		 * to the volume whose name is contained in that pathname. (A full
		 * pathname overrides the ioVRefNum field.)
		 *
		 * The PBHSetVol function also sets the default directory. If the
		 * ioVRefNum field contains a volume reference number, then the default
		 * directory is set to the directory on that volume having the partial
		 * pathname specified by ioNamePtr in the directory specified by
		 * ioWDDirID. If the value of ioNamePtr is NIL, the default directory
		 * is simply the directory whose directory ID is contained in ioWDDirID.
		 *
		 * If the ioVRefNum field contains a working directory reference number,
		 * then ioWDDirID is ignored and the default directory is set to the
		 * directory on that volume having the partial pathname specified by
		 * ioNamePtr in the directory specified by the working directory
		 * reference number. If the value of ioNamePtr is NIL, the default
		 * directory is simply the directory specified in ioVRefNum.
		 *
		 * WARNING
		 * Use of the PBHSetVol function is discouraged if your application
		 * may execute in system software versions prior to version 7.0. Because
		 * the specified directory might not itself be a working directory,
		 * PBHSetVol records the default volume and directory separately, using
		 * the volume reference number of the volume and the actual directory
		 * ID of the specified directory. Subsequent calls to GetVol (or
		 * PBGetVol) return only the volume reference number, which will cause
		 * that volume’s root directory (rather than the default directory,
		 * as expected) to be accessed.
		 *
		 * NOTE
		 * Both the default volume and the default directory are used in calls
		 * made with no volume name, a volume reference number of 0, and a
		 * directory ID of 0.
		 */

		 enum {
			/* WDPBRec */
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

		uint16_t d0;

		uint32_t parm = cpuGetAReg(0);

		Log("%04x HSetVol(%08x)\n", trap, parm);

		d0 = MacOS::nsvErr;
		memoryWriteWord(d0, parm + _ioResult);

		return d0;
	}


}
