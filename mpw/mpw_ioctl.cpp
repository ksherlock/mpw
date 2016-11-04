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

#include "mpw.h"
#include "mpw_internal.h"
#include "mpw_errno.h"

#include <algorithm>
#include <memory>
#include <string>
#include <stdexcept>

#include <cstdio>
#include <cstring>
#include <cerrno>

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <cpu/defs.h>
#include <cpu/fmem.h>
#include <cpu/CpuModule.h>


#include <macos/errors.h>

#include <toolbox/os.h>
#include <toolbox/os_internal.h>

using MacOS::macos_error_from_errno;


namespace MPW
{

	uint32_t ftrap_dup(uint32_t parm, uint32_t arg)
	{
		uint32_t d0 = 0;
		MPWFile f;

		f.flags = memoryReadWord(parm);
		f.error = memoryReadWord(parm + 2);
		f.device = memoryReadLong(parm + 4);
		f.cookie = memoryReadLong(parm + 8);
		f.count = memoryReadLong(parm + 12);
		f.buffer = memoryReadLong(parm + 16);

		f.error = 0;

		int fd = f.cookie;

		Log("     dup(%02x)\n", fd);

		auto ff = OS::Internal::find_file(fd);
		if (ff) {
			ff->refcount++;
		}
		else {
			d0 = kEBADF;
		}

		memoryWriteWord(f.error, parm + 2);
		return d0;
	}

	uint32_t ftrap_bufsize(uint32_t parm, uint32_t arg)
	{
		// should return the preferred buffsize in *arg
		// an error will use the default size (0x400 bytes).

		uint32_t d0 = 0;
		MPWFile f;

		f.flags = memoryReadWord(parm);
		f.error = memoryReadWord(parm + 2);
		f.device = memoryReadLong(parm + 4);
		f.cookie = memoryReadLong(parm + 8);
		f.count = memoryReadLong(parm + 12);
		f.buffer = memoryReadLong(parm + 16);

		f.error = 0;

		int fd = f.cookie;

		Log("     bufsize(%02x)\n", fd);

		auto ff = OS::Internal::find_file(fd);
		if (ff) {
			d0 = kEINVAL;
		} else {
			d0 = kEBADF;
		}

		memoryWriteWord(f.error, parm + 2);
		return d0;
	}


	uint32_t ftrap_interactive(uint32_t parm, uint32_t arg)
	{
		// return 0 if interactive, an error if
		// non-interactive. arg is null.

		uint32_t d0 = 0;
		MPWFile f;

		f.flags = memoryReadWord(parm);
		f.error = memoryReadWord(parm + 2);
		f.device = memoryReadLong(parm + 4);
		f.cookie = memoryReadLong(parm + 8);
		f.count = memoryReadLong(parm + 12);
		f.buffer = memoryReadLong(parm + 16);

		// linkgs reads from stdin and
		// doesn't work quite right when
		// this returns 0.  So, don't.

		f.error = 0;

		int fd = f.cookie;

		Log("     interactive(%02x)\n", fd);


		auto ff = OS::Internal::find_file(fd);
		if (!ff) {
			d0 = kEBADF;
		} else if (!ff->interactive()) {
			f.error = d0 = MacOS::paramErr;
		}

		memoryWriteWord(f.error, parm + 2);
		return d0;
	}

	uint32_t ftrap_fname(uint32_t parm, uint32_t arg)
	{
		// return file name (full path).
		// AsmIIgs uses this...
		// arg is a c-string ptr.

		uint32_t d0 = 0;
		MPWFile f;

		f.flags = memoryReadWord(parm);
		f.error = memoryReadWord(parm + 2);
		f.device = memoryReadLong(parm + 4);
		f.cookie = memoryReadLong(parm + 8);
		f.count = memoryReadLong(parm + 12);
		f.buffer = memoryReadLong(parm + 16);

		f.error = 0;

		int fd = f.cookie;

		Log("     fname(%02x)\n", fd);

		auto ff = OS::Internal::find_file(fd);
		if (ff) {
			f.error = d0 = MacOS::paramErr;
		} else {
			d0 = kEBADF;
		}

		memoryWriteWord(f.error, parm + 2);
		return d0;
	}

	uint32_t ftrap_refnum(uint32_t parm, uint32_t arg)
	{
		// returns the refnum in *arg
		// on mpw, stdin/stdout/stderr return a param error.
		uint32_t d0 = 0;
		MPWFile f;

		f.flags = memoryReadWord(parm);
		f.error = memoryReadWord(parm + 2);
		f.device = memoryReadLong(parm + 4);
		f.cookie = memoryReadLong(parm + 8);
		f.count = memoryReadLong(parm + 12);
		f.buffer = memoryReadLong(parm + 16);

		f.error = 0;

		int fd = f.cookie;

		Log("     refnum(%02x)\n", fd);

		auto ff = OS::Internal::find_file(fd);
		if (ff) {
			if (ff->interactive()) f.error = d0 = MacOS::paramErr;
			else memoryWriteWord(fd, arg);
		} else {
			d0 = kEBADF;
		}

		memoryWriteWord(f.error, parm + 2);
		return d0;
	}



	uint32_t ftrap_lseek(uint32_t parm, uint32_t arg)
	{
		uint32_t d0 = 0;
		MPWFile f;

		uint32_t whence = memoryReadLong(arg);
		int32_t offset = memoryReadLong(arg + 4); // signed value.
		int nativeWhence = 0;

		f.flags = memoryReadWord(parm);
		f.error = memoryReadWord(parm + 2);
		f.device = memoryReadLong(parm + 4);
		f.cookie = memoryReadLong(parm + 8);
		f.count = memoryReadLong(parm + 12);
		f.buffer = memoryReadLong(parm + 16);

		f.error = 0;


		/*
		 * LinkIIgs does a seek on stdin.  If it doesn't cause an
		 * error, then it attempts to read a list of files from stdin,
		 * which causes problems.
		 * (Get_Standard_Input -> fseek -> lseek -> ioctl )
		 * to compensate, error out if isatty.
		 */

		// TODO - MacOS returns eofERR and sets mark to eof
		// if seeking past the eof.
		switch (whence)
		{
			case kSEEK_CUR:
				nativeWhence = SEEK_CUR;
				break;
			case kSEEK_END:
				nativeWhence = SEEK_END;
				break;
			case kSEEK_SET:
				nativeWhence = SEEK_SET;
				break;

			default:
				memoryWriteWord(0, parm + 2);
				return kEINVAL;
		}

		int fd = f.cookie;
		Log("     lseek(%02x, %08x, %02x)\n", fd, offset, nativeWhence);


		off_t rv = - 1;
		auto ff = OS::Internal::find_file(fd);
		if (ff) {
			MacOS::tool_return<size_t> e(MacOS::noErr);
			if (ff->interactive()) e = MacOS::paramErr;
			else e = ff->seek(offset, nativeWhence);
			rv = e.value_or(-1);
			f.error = d0 = e.error();
		} else {
			d0 = kEBADF;
		} 

		memoryWriteLong(rv, arg + 4);
		memoryWriteWord(f.error, parm + 2);
		return d0;
	}


	uint32_t ftrap_seteof(uint32_t parm, uint32_t arg)
	{

		uint32_t d0 = 0;
		MPWFile f;

		f.flags = memoryReadWord(parm);
		f.error = memoryReadWord(parm + 2);
		f.device = memoryReadLong(parm + 4);
		f.cookie = memoryReadLong(parm + 8);
		f.count = memoryReadLong(parm + 12);
		f.buffer = memoryReadLong(parm + 16);

		f.error = 0;

		int fd = f.cookie;

		Log("     seteof(%02x, %08x)\n", fd, arg);

		auto ff = OS::Internal::find_file(fd);
		if (ff) {
			MacOS::tool_return<size_t> e(MacOS::noErr);
			if (ff->interactive()) e = MacOS::paramErr;
			else e = ff->set_eof(arg);
			f.error = d0 = e.error();
		} else {
			d0 = kEBADF;
		} 

		memoryWriteWord(f.error, parm + 2);
		return d0;
	}


	void ftrap_ioctl(uint16_t trap)
	{
		// returns an mpw_errno in d0 [???]

		// int ioctl(int fildes, unsigned int cmd, long *arg);
		uint32_t d0;
		uint32_t sp = cpuGetAReg(7);

		uint32_t fd = memoryReadLong(sp + 4);
		uint32_t cmd = memoryReadLong(sp + 8);
		uint32_t arg = memoryReadLong(sp + 12);

		Log("%04x IOCtl(%08x, %08x, %08x)\n", trap, fd, cmd, arg);

		switch (cmd)
		{
			case kFIOLSEEK:
				d0 = ftrap_lseek(fd, arg);
				break;

			case kFIODUPFD:
				d0 = ftrap_dup(fd, arg);
				break;

			case kFIOBUFSIZE:
				d0 = ftrap_bufsize(fd, arg);
				break;

			case kFIOINTERACTIVE:
				d0 = ftrap_interactive(fd, arg);
				break;

			case kFIOFNAME:
				d0 = ftrap_fname(fd, arg);
				break;

			case kFIOREFNUM:
				d0 = ftrap_refnum(fd, arg);
				break;

			case kFIOSETEOF:
				d0 = ftrap_seteof(fd, arg);
				break;

			default:
				fprintf(stderr, "ioctl - unsupported op %04x\n", cmd);
				exit(1);
				break;
		}

		cpuSetDReg(0, d0);
	}


}
