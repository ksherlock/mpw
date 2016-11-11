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

#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <strings.h>

#include <filesystem>

namespace fs = std::experimental::filesystem;

#include <cpu/defs.h>
#include <cpu/CpuModule.h>
#include <cpu/fmem.h>

#include <macos/sysequ.h>
#include <macos/errors.h>
#include <macos/traps.h>

#include <native/native.h>
#include <native/file.h>

#include "os.h"
#include "os_internal.h"
#include "toolbox.h"
#include "stackframe.h"
#include "fs_spec.h"

using ToolBox::Log;

using MacOS::macos_error_from_errno;

namespace {

	using namespace OS;

	// time stuff.
	const long EpochAdjust = 86400 * (365 * (1970 - 1904) + 17); // 17 leap years.
	std::chrono::time_point<std::chrono::steady_clock> BootTime;



#if 0
	std::string extension(const std::string &s)
	{
		std::string tmp;
		int pos;

		pos = s.find_last_of("./:");

		if (pos == s.npos) return tmp;
		if (s[pos++] != '.') return tmp;
		if (pos >= s.length()) return tmp;

		tmp = s.substr(pos);

		std::transform(tmp.begin(), tmp.end(), tmp.begin(),
			[](char c) { return tolower(c); }
		);

		return tmp;
	}

	std::string basename(const std::string &s)
	{
		int pos = s.find_last_of("/:");
		if (pos == s.npos) return s;

		return s.substr(pos + 1);
	}
#endif


}

namespace OS
{

	bool Init()
	{
		BootTime = std::chrono::steady_clock::now();
		memoryWriteLong(0, MacOS::Ticks); // 0 ticks since boot.

		//std::chrono::system_clock::now(), to_time_t
		// set global variable Time to the current time
		time_t now = UnixToMac(::time(NULL));

		memoryWriteLong(now, MacOS::TimeLM);

		return true;
	}


	uint16_t Close(uint16_t trap)
	{
		uint32_t d0;
		uint32_t parm = cpuGetAReg(0);

		Log("%04x Close(%08x)\n", trap, parm);

		//uint32_t ioCompletion = memoryReadLong(parm + 12);
		uint16_t ioRefNum = memoryReadWord(parm + 24);


		int rv = OS::Internal::close_file(ioRefNum, true);
		if (rv < 0) d0 = MacOS::rfNumErr;
		else d0 = 0;

		memoryWriteWord(d0, parm + 16);
		return d0;
	}


	uint16_t Create(uint16_t trap)
	{

		enum {
			/* FileParam */
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
			_ioFlNum = 48,
			_ioFlStBlk = 52,
			_ioFlLgLen = 54,
			_ioFlPyLen = 58,
			_ioFlRStBlk = 62,
			_ioFlRLgLen = 64,
			_ioFlRPyLen = 68,
			_ioFlCrDat = 72,
			_ioFlMdDat = 76,

			// HFileParam
			_ioDirID = 48,
		};

		bool htrap = trap & 0x0200;
		const char *func = htrap ? "HCreate" : "Create";

		uint32_t d0;

		uint32_t parm = cpuGetAReg(0);

		Log("%04x %s(%08x)\n", trap, func, parm);

		uint32_t namePtr = memoryReadLong(parm + _ioNamePtr);

		std::string sname = ToolBox::ReadPString(namePtr, true);

		if (!sname.length())
		{
			memoryWriteWord(MacOS::bdNamErr, parm + _ioResult);
			return MacOS::bdNamErr;
		}

		if (htrap)
		{
			uint32_t ioDirID = memoryReadLong(parm + _ioDirID);
			sname = FSSpecManager::ExpandPath(sname, ioDirID);
		}

		Log("     %s(%s)\n", func, sname.c_str());

		int fd;
		fd = ::open(sname.c_str(), O_WRONLY | O_CREAT | O_EXCL, 0666);

		if (fd < 0)
		{
			d0 = macos_error_from_errno();
		}
		else
		{
			::close(fd);
			d0 = 0;
		}

		memoryWriteWord(d0, parm + _ioResult);
		return d0;
	}


	uint16_t OpenCommon(uint32_t parm, bool fsspec, bool resource)
	{

		enum {
			/* IOParam */
			_qLink = 0,
			_qType = 4,
			_ioTrap = 6,
			_ioCmdAddr = 8,
			_ioCompletion = 12,
			_ioResult = 16,
			_ioNamePtr = 18,
			_ioVRefNum = 22,
			_ioRefNum = 24,
			_ioVersNum = 26,
			_ioPermssn = 27,
			_ioMisc = 28,
			_ioBuffer = 32,
			_ioReqCount = 36,
			_ioActCount = 40,
			_ioPosMode = 44,
			_ioPosOffset = 46,

			_ioDirID = 48,

		};

		uint32_t d0 = 0;

		uint8_t ioPermission = memoryReadByte(parm + _ioPermssn);
		uint32_t namePtr = memoryReadLong(parm + _ioNamePtr);

		std::string sname = ToolBox::ReadPString(namePtr, true);

		if (fsspec)
		{
			uint32_t ioDirID = memoryReadLong(parm + _ioDirID);
			sname = FSSpecManager::ExpandPath(sname, ioDirID);
		}

		Log("     open(%s, %02x, %02x)\n", sname.c_str(), resource, ioPermission);

		auto fd = Internal::open_file(sname, resource, ioPermission);
		if (fd) memoryWriteWord(fd.value(), parm + _ioRefNum);
		d0 = fd.error();

		memoryWriteWord(d0, parm + _ioResult);
		return d0;
	}

	uint16_t Open(uint16_t trap)
	{

		bool htrap = trap & 0x0200;
		const char *func = htrap ? "HOpen" : "Open";

		uint32_t parm = cpuGetAReg(0);
		Log("%04x %s(%08x)\n", trap, func, parm);
		return OpenCommon(parm, htrap, false);
	}

	uint16_t OpenRF(uint16_t trap)
	{

		bool htrap = trap & 0x0200;
		const char *func = htrap ? "HOpenRF" : "OpenRF";

		uint32_t parm = cpuGetAReg(0);
		Log("%04x %s(%08x)\n", trap, func, parm);
		return OpenCommon(parm, htrap, true);
	}


	uint16_t Read(uint16_t trap)
	{
		uint32_t d0 = 0;
		uint32_t parm = cpuGetAReg(0);

		Log("%04x Read(%08x)\n", trap, parm);

		//uint32_t ioCompletion = memoryReadLong(parm + 12);

		uint16_t ioRefNum = memoryReadWord(parm + 24);
		uint32_t ioBuffer = memoryReadLong(parm + 32);
		int32_t ioReqCount = memoryReadLong(parm + 36);
		uint16_t ioPosMode = memoryReadWord(parm + 44);
		int32_t ioPosOffset = memoryReadLong(parm + 46);

		Log("     read(%04x, %08x, %08x)\n", ioRefNum, ioBuffer, ioReqCount);

		/*
		 * per inside macintosh II-101:
		 *  bit 6 (0x40) of ioPosOffset is used for read-verify operation
		 * (write date, call read w/ bit 6 to verify disk matches memory)
		 * returns ioError if it doesn't match.
		 *
		 * bit 7 (0x80) of ioPosMode indicate new-line mode is enabled --
		 * bits 8-15 are a newline character. reading terminates when newline
		 * char is encountered (or eof or ioReqCount).
		 *
		 * bit 4 is please cache bit
		 * bit 5 is please don't cache bit
		 * (if neither or both set, no preference)
		 *
		 */

		if (ioReqCount < 0)
		{
			d0 = MacOS::paramErr;
			memoryWriteWord(d0, parm + 16);
			return d0;
		}

		if (ioPosMode & 0x80) {
			fprintf(stderr, "Newline Read (eof char = %02x)\n", ioPosMode >> 8);
			exit(1);
		}

		ssize_t offset = ioPosOffset;
		int whence = ioPosMode;
		uint32_t transferCount = 0;

		Internal::remap_seek(offset, whence);

		auto ff = Internal::find_file(ioRefNum);
		if (!ff) {
			d0 = MacOS::rfNumErr;
			memoryWriteWord(d0, parm + 16);
			return d0;
		}

		auto pos = ff->seek(offset, whence);
		if (!pos) {
			d0 = pos.error();
			memoryWriteWord(d0, parm + 16);
			return d0;
		}

		auto xfer = ff->read(memoryPointer(ioBuffer), ioReqCount);
		if (xfer && *xfer == 0) xfer = MacOS::eofErr;

		d0 = xfer.error();
		transferCount = xfer.value_or(0);
		offset = ff->get_mark().value_or((size_t)0);

		memoryWriteLong(offset, parm + 46); // new offset.
		memoryWriteLong(transferCount, parm + 40);
		memoryWriteWord(d0, parm + 16);
		return d0;
	}


	uint16_t Write(uint16_t trap)
	{
		uint32_t d0 = 0;
		uint32_t parm = cpuGetAReg(0);

		Log("%04x Write(%08x)\n", trap, parm);

		//uint32_t ioCompletion = memoryReadLong(parm + 12);

		uint16_t ioRefNum = memoryReadWord(parm + 24);
		uint32_t ioBuffer = memoryReadLong(parm + 32);
		int32_t ioReqCount = memoryReadLong(parm + 36);
		uint16_t ioPosMode = memoryReadWord(parm + 44);
		int32_t ioPosOffset = memoryReadLong(parm + 46);

		Log("     write(%04x, %08x, %08x)\n", ioRefNum, ioBuffer, ioReqCount);

		if (ioReqCount < 0)
		{
			d0 = MacOS::paramErr;
			memoryWriteWord(d0, parm + 16);
			return d0;
		}

		ssize_t offset = ioPosOffset;
		int whence = ioPosMode;
		uint32_t transferCount = 0;

		Internal::remap_seek(offset, whence);

		auto ff = Internal::find_file(ioRefNum);
		if (!ff) {
			d0 = MacOS::rfNumErr;
			memoryWriteWord(d0, parm + 16);
			return d0;
		}

		auto pos = ff->seek(offset, whence);
		if (!pos) {
			d0 = pos.error();
			memoryWriteWord(d0, parm + 16);
			return d0;
		}

		auto xfer = ff->write(memoryPointer(ioBuffer), ioReqCount);
		if (xfer && *xfer == 0) xfer = MacOS::eofErr;

		d0 = xfer.error();
		transferCount = xfer.value_or(0);
		offset = ff->get_mark().value_or((size_t)0);

		memoryWriteLong(offset, parm + 46); // new offset.
		memoryWriteLong(transferCount, parm + 40);
		memoryWriteWord(d0, parm + 16);
		return d0;
	}




	uint16_t Delete(uint16_t trap)
	{
		enum {
			/* FileParam */
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
			_ioFlNum = 48,
			_ioFlStBlk = 52,
			_ioFlLgLen = 54,
			_ioFlPyLen = 58,
			_ioFlRStBlk = 62,
			_ioFlRLgLen = 64,
			_ioFlRPyLen = 68,
			_ioFlCrDat = 72,
			_ioFlMdDat = 76,

			// HFileParam
			_ioDirID = 48,
		};

		bool htrap = trap & 0x0200;
		const char *func = htrap ? "HDelete" : "Delete";

		uint32_t d0;

		uint32_t parm = cpuGetAReg(0);

		Log("%04x %s(%08x)\n", trap, func, parm);

		uint32_t namePtr = memoryReadLong(parm + _ioNamePtr);

		std::string sname = ToolBox::ReadPString(namePtr, true);

		if (!sname.length())
		{
			memoryWriteWord(MacOS::bdNamErr, parm + _ioResult);
			return MacOS::bdNamErr;
		}

		if (htrap)
		{
			uint32_t ioDirID = memoryReadLong(parm + _ioDirID);
			sname = FSSpecManager::ExpandPath(sname, ioDirID);
		}

		Log("     %s(%s)\n", func, sname.c_str());

		std::error_code ec;
		if (fs::remove(sname, ec)) d0 = 0;
		else if (ec) d0 = macos_error_from_errno(ec);
		else d0 = MacOS::fnfErr;

		memoryWriteWord(d0, parm + _ioResult);
		return d0;
	}



	uint16_t GetEOF(uint16_t trap)
	{
		uint32_t d0 = 0;
		size_t size = 0;

		uint32_t parm = cpuGetAReg(0);

		Log("%04x GetEOF(%08x)\n", trap, parm);

		//uint32_t ioCompletion = memoryReadLong(parm + 12);
		uint16_t ioRefNum = memoryReadWord(parm + 24);

		auto ff = Internal::find_file(ioRefNum);
		if (ff) {
			auto eof = ff->get_eof();
			d0 = eof.error();
			size = eof.value_or(0);
		} else {
			d0 = MacOS::rfNumErr;
			size = 0;
		}

		memoryWriteWord(d0, parm + 16);
		memoryWriteLong(size, parm + 28);

		return d0;
	}

	uint16_t SetEOF(uint16_t trap)
	{
		uint32_t d0;

		uint32_t parm = cpuGetAReg(0);

		Log("%04x SetEOF(%08x)\n", trap, parm);

		//uint32_t ioCompletion = memoryReadLong(parm + 12);
		uint16_t ioRefNum = memoryReadWord(parm + 24);
		uint32_t ioMisc = memoryReadLong(parm + 28);


		auto ff = Internal::find_file(ioRefNum);
		if (ff) {
			auto eof = ff->set_eof(ioMisc);
			d0 = eof.error();
		} else {
			d0 = MacOS::rfNumErr;
		}

		memoryWriteWord(d0, parm + 16);
		return d0;
	}

	uint16_t GetFPos(uint16_t trap)
	{
		uint32_t d0 = 0;
		size_t size = 0;

		uint32_t parm = cpuGetAReg(0);

		Log("%04x GetFPos(%08x)\n", trap, parm);

		//uint32_t ioCompletion = memoryReadLong(parm + 12);
		uint16_t ioRefNum = memoryReadWord(parm + 24);


		auto ff = Internal::find_file(ioRefNum);
		if (ff) {
			auto mark = ff->get_mark();
			d0 = mark.error();
			size = mark.value_or(0);
		} else {
			d0 = MacOS::rfNumErr;
			size = 0;
		}


		memoryWriteLong(0, parm + 36); // ioReqCount
		memoryWriteLong(0, parm + 40); // ioActCount
		memoryWriteWord(0, parm + 44); // ioPosMode
		memoryWriteLong(size, parm + 46); // ioPosOffset

		memoryWriteWord(d0, parm + 16);
		return d0;
	}

	uint16_t SetFPos(uint16_t trap)
	{
		uint32_t d0;

		uint32_t parm = cpuGetAReg(0);

		Log("%04x SetFPos(%08x)\n", trap, parm);

		//uint32_t ioCompletion = memoryReadLong(parm + 12);
		uint16_t ioRefNum = memoryReadWord(parm + 24);
		uint16_t ioPosMode = memoryReadWord(parm + 44);
		int32_t ioPosOffset = memoryReadLong(parm + 46);


		ssize_t offset = ioPosOffset;
		int whence = ioPosMode;

		Internal::remap_seek(offset, whence);

		auto ff = Internal::find_file(ioRefNum);
		if (ff) {
			auto pos = ff->seek(offset, whence);
			d0 = pos.error();
			offset = pos.value_or(0);
		} else {
			d0 = MacOS::rfNumErr;
			offset = 0;
		}


		memoryWriteLong(offset, parm + 46); // new offset.
		memoryWriteWord(d0, parm + 16);
		return d0;
	}



	#pragma mark - String Utilities

	uint16_t CmpString(uint16_t trap)
	{

		/*
		 * on entry:
		 * A0 Pointer to first character of first string
		 * A1 Pointer to first character of second string
		 * D0 (high) length of first string
		 * D0 (low) length of second string
		 *
		 * on exit:
		 * D0 0 if strings equal, 1 if strings not equal.
		 *
		 */

		bool caseSens = trap & (1 << 9);
		//bool diacSens = trap & (1 << 10); // ignore for now...

		uint32_t aStr = cpuGetAReg(0);
		uint32_t bStr = cpuGetAReg(1);

		uint32_t length = cpuGetDReg(0);

		uint32_t aLen = (length >> 16);
		uint32_t bLen = (length & 0xffff);

		std::string a = ToolBox::ReadString(aStr, aLen);
		std::string b = ToolBox::ReadString(bStr, bLen);

		Log("%04x CmpString(%s, %s)\n", trap, a.c_str(), b.c_str());

		if (aLen != bLen) return 1; // different length...
		if (aStr == bStr) return 0; // same ptr...

		bool eq;
		eq = std::equal(
			a.begin(),
			a.end(),
			b.begin(),
			[caseSens](char a, char b){
				if (!caseSens)
				{
					a = toupper(a);
					b = toupper(b);
				}
				return a == b;
			}
		);

		return eq ? 0 : 1;
	}


	#pragma mark - Time Utilities

	time_t UnixToMac(time_t t)
	{
		return t + EpochAdjust;
	}

	time_t MacToUnix(time_t t)
	{
		return t - EpochAdjust;
	}

	uint16_t ReadDateTime(uint16_t trap)
	{

		/*
		 * on entry:
		 * A0 Pointer to long word secs
		 *
		 * on exit:
		 * A0 pointer to long word secs
		 * D0 Result code
		 *
		 */

		time_t now;

		uint32_t secsPtr = cpuGetAReg(0);

		Log("%04x ReadDateTime(%08x)\n", trap, secsPtr);

		now = ::time(NULL);


		now = UnixToMac(now);
		if (secsPtr) memoryWriteLong(now, secsPtr);

		// also set global variable Time.
		memoryWriteLong(now, MacOS::TimeLM);
		return 0;
	}

	uint16_t SecondsToDate(uint16_t trap)
	{
		/*
		 * on entry:
		 * D0 Seconds since midnight, January 1, 1904
		 * A0 pointer to date-time record
		 *
		 * on exit:
		 * D0 Result code
		 *
		 */

		uint32_t s = cpuGetDReg(0);
		uint32_t dtPtr = cpuGetAReg(0);

		Log("%04x SecondsToDate(%08x, %08x)\n", trap, s, dtPtr);


		if (dtPtr)
		{
			struct tm *tm;
			time_t t;
			t = MacToUnix(s);

			tm = ::localtime(&t);

			memoryWriteWord(tm->tm_year + 1900, dtPtr + 0);
			memoryWriteWord(tm->tm_mon + 1, dtPtr + 2);
			memoryWriteWord(tm->tm_mday, dtPtr + 4);
			memoryWriteWord(tm->tm_hour, dtPtr + 6);
			memoryWriteWord(tm->tm_min, dtPtr + 8);
			memoryWriteWord(tm->tm_sec, dtPtr + 10);
			memoryWriteWord(tm->tm_wday + 1, dtPtr + 12);
		}

		return 0;
	}

	uint16_t TickCount(uint16_t trap)
	{
		typedef std::chrono::duration<int32_t, std::ratio<1, 60> > ticks;

		Log("%04x TickCount()\n", trap);

		auto now = std::chrono::steady_clock::now();

		uint32_t t = std::chrono::duration_cast< ticks >(now - BootTime).count();


		// global Ticks
		memoryWriteLong(t, MacOS::Ticks);
		ToolReturn<4>(-1, t);

		return 0;
	}


	uint16_t Microseconds(uint16_t trap)
	{

		// UnsignedWide is a uint64_t
		// Microseconds(UnsignedWide * microTickCount)
		// FOURWORDINLINE(0xA193, 0x225F, 0x22C8, 0x2280);


		uint32_t microTickCount;
		StackFrame<4>(microTickCount);

		Log("%04x %s(%08x)\n", trap, __func__, microTickCount);

		auto now = std::chrono::steady_clock::now();

		uint64_t t = std::chrono::duration_cast< std::chrono::microseconds >(now - BootTime).count();

		if (microTickCount)
			memoryWriteLongLong(t, microTickCount);


		return 0;
	}



	#pragma mark - Trap Manager

#ifdef OLD_TRAP_DISPATCH
	uint16_t GetToolTrapAddress(uint16_t trap)
	{
		/*
		 * on entry:
		 * D0 trap number
		 *
		 * on exit:
		 * A0 Address of patch
		 *
		 */
		uint16_t trapNumber = cpuGetDReg(0);
		const char *trapName = TrapName(trapNumber | 0xa800);
		if (!trapName) trapName = "Unknown";

		Log("%04x GetToolTrapAddress($%04x %s)\n", trap, trapNumber, trapName);

		cpuSetAReg(0, 0);
		return MacOS::dsCoreErr;
	}

	uint16_t SetToolTrapAddress(uint16_t trap)
	{
		//pascal void SetToolTrapAddress(long trapAddr, short trapNum);

		/*
		 * on entry:
		 * A0 Address of patch
		 * D0 trap number
		 *
		 * on exit:
		 *
		 */

		// this is used by the far model stub, presumably
		// to replace LoadSeg.

		uint16_t trapNumber = cpuGetDReg(0);
		uint32_t trapAddress = cpuGetAReg(0);
		const char *trapName = TrapName(trapNumber | 0xa800);
		if (!trapName) trapName = "Unknown";

		Log("%04x SetToolTrapAddress($%08x, $%04x %s)\n",
			trap, trapAddress, trapNumber, trapName);


		return MacOS::dsCoreErr;
	}

	uint16_t GetOSTrapAddress(uint16_t trap)
	{
		/*
		 * on entry:
		 * D0 trap number
		 *
		 * on exit:
		 * A0 Address of patch
		 *
		 */
		uint16_t trapNumber = cpuGetDReg(0);
		const char *trapName = TrapName(trapNumber | 0xa000);
		if (!trapName) trapName = "Unknown";

		Log("%04x GetOSTrapAddress($%04x %s)\n", trap, trapNumber, trapName);

		cpuSetAReg(0, 0);
		return MacOS::dsCoreErr;
	}

	uint16_t SetOSTrapAddress(uint16_t trap)
	{
		//pascal void SetOSTrapAddress(long trapAddr, short trapNum);

		/*
		 * on entry:
		 * A0 Address of patch
		 * D0 trap number
		 *
		 * on exit:
		 *
		 */

		uint16_t trapNumber = cpuGetDReg(0);
		uint32_t trapAddress = cpuGetAReg(0);
		const char *trapName = TrapName(trapNumber | 0xa000);
		if (!trapName) trapName = "Unknown";

		Log("%04x SetOSTrapAddress($%08x, $%04x %s)\n",
			trap, trapAddress, trapNumber, trapName);


		return MacOS::dsCoreErr;
	}
#endif


	#pragma mark XP - RAM

	// these are not particularly documented.
	uint16_t ReadXPRam(uint16_t trap)
	{
		// a0 = address?
		// d0 = count? item?
		Log("%04x ReadXPRam()\n", trap);
		return MacOS::prWrErr;
	}

	uint16_t WriteXPRam(uint16_t trap)
	{
		Log("%04x WriteXPRam()\n", trap);
		return MacOS::prWrErr;
	}

	#pragma mark - Timer

	struct TimerEntry {
		uint32_t tmTaskPtr = 0; // address of the queue.  passed back in A1.
		uint32_t tmAddr = 0;
		bool extended = false;
		bool active = false;

		std::chrono::time_point<std::chrono::steady_clock> when;

		TimerEntry(uint32_t a, uint32_t b) : tmTaskPtr(a), tmAddr(b)
		{}
	};

	// heap sorted by next task to run?
	static std::deque<TimerEntry> TimerQueue;

	namespace TMTask {
		enum  {
			_qLink = 0,
			_qType = 4,
			_tmAddr = 6,
			_tmCount = 10,
			_tmWakeUp = 14,
			_tmReserved = 18
		};
	}

	uint16_t InsTime(uint16_t trap)
	{
		// PROCEDURE InsTime (tmTaskPtr: QElemPtr);

		// this adds an entry to the queue but does not schedule it.

		/*
		 * on entry
		 * A0 Address of the task record
		 *
		 * on exit
		 * D0 Result code
		 */

		using namespace TMTask;

		uint32_t tmTaskPtr = cpuGetAReg(0);

		Log("%04x InsTime(%08x)\n", trap, tmTaskPtr);

		if (tmTaskPtr)
		{
			memoryWriteLong(0, tmTaskPtr + _qLink);
			memoryWriteWord(0, tmTaskPtr + _qType);
			memoryWriteLong(0, tmTaskPtr + _tmCount);
			memoryWriteLong(0, tmTaskPtr + _tmWakeUp);
			memoryWriteLong(0, tmTaskPtr + _tmReserved);

			TimerQueue.emplace_back(tmTaskPtr, memoryReadLong(tmTaskPtr + _tmAddr));
		}

		return MacOS::noErr;
	}

	uint16_t PrimeTime(uint16_t trap)
	{
		// PROCEDURE PrimeTime (tmTaskPtr: QElemPtr; count: LongInt);
		// this activates an entry.

		/*
		 * on entry
		 * A0 Address of the task record
		 * D0 Specified delay time (long)
		 *
		 * on exit
		 * D0 Result code
		 */

		using namespace TMTask;

		uint32_t tmTaskPtr = cpuGetAReg(0);
		uint32_t count = cpuGetDReg(0);

		Log("%04x PrimeTime(%08x, %08x)\n", trap, tmTaskPtr, count);

		if (tmTaskPtr)
		{
			auto iter = std::find_if(TimerQueue.begin(), TimerQueue.end(), [tmTaskPtr](const TimerEntry &e){
				return e.tmTaskPtr == tmTaskPtr;
			});

			if (iter != TimerQueue.end() && !iter->active)
			{
				auto now = std::chrono::steady_clock::now();

				iter->active = true;

				if (count == 0) {
					// retain the original time or set it to now.
					iter->when = std::max(now, iter->when);
				}
				else
				{
					int64_t micro;
					if (count < 0x80000000)
						micro = count * 1000;
					else
						micro = -(int32_t)count;


					iter->when = now + std::chrono::microseconds(micro);

				}
				memoryWriteWord(0x8000, tmTaskPtr + _qType);
			}
		}


		return MacOS::noErr;

	}

	uint16_t RmvTime(uint16_t trap)
	{
		// PROCEDURE RmvTime (tmTaskPtr: QElemPtr);

		// unschedule (but not actually remove)

		/*
		 * on entry
		 * A0 Address of the task record
		 *
		 * on exit
		 * D0 Result code
		 */

		using namespace TMTask;

		uint32_t tmTaskPtr = cpuGetAReg(0);

		Log("%04x RmvTime(%08x)\n", trap, tmTaskPtr);

		if (tmTaskPtr)
		{
			auto iter = std::find_if(TimerQueue.begin(), TimerQueue.end(), [tmTaskPtr](const TimerEntry &e){
				return e.tmTaskPtr == tmTaskPtr;
			});

			if (iter != TimerQueue.end())
			{
				uint32_t count = 0;
				if (iter->active)
				{
					iter->active = false;


					// update tmCount to the amount of time remaining.
					// uses negative microseconds
					// or positive milliseconds.

					auto now = std::chrono::steady_clock::now();

					int64_t micro = std::chrono::duration_cast< std::chrono::microseconds >(iter->when - now).count();

					if (micro < 0)
						count = 0;
					else if (micro < 0x80000000)
						count = -micro;
					else
						count = micro / 10000;
				}

				memoryWriteWord(0, tmTaskPtr + _qType);
				memoryWriteLong(count, tmTaskPtr + _tmCount);
			}
		}

		return MacOS::noErr;
	}

}
