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

#include <cerrno>
#include <cstdio>

#include <unistd.h>

#include <cpu/defs.h>
#include <cpu/fmem.h>
#include <cpu/CpuModule.h>

#include <macos/errors.h>

#include <toolbox/os.h>
#include <toolbox/os_internal.h>


namespace MPW
{


	void ftrap_read(uint16_t trap)
	{

		uint32_t d0;

		uint32_t sp = cpuGetAReg(7);
		uint32_t parm  = memoryReadLong(sp + 4);

		MPWFile f;

		f.flags = memoryReadWord(parm);
		f.error = memoryReadWord(parm + 2);
		f.device = memoryReadLong(parm + 4);
		f.cookie = memoryReadLong(parm + 8);
		f.count = memoryReadLong(parm + 12);
		f.buffer = memoryReadLong(parm + 16);


		Log("%04x Read(%08x)\n", trap, parm);

		d0 = 0;
		int fd = f.cookie;
		ssize_t size;

		Log("     read(%04x, %08x, %08x)", fd, f.buffer, f.count);
		size = OS::Internal::FDEntry::read(fd, memoryPointer(f.buffer), f.count);
		//Log(" -> %ld\n", size);

		if (size < 0)
		{
			//f.count = 0;
			f.error = MacOS::ioErr; // ioErr
			d0 = mpw_errno_from_errno();
		}
		else
		{
			f.count -= size;
			f.error = 0;
		}

		// write back...
		memoryWriteWord(f.error, parm + 2);
		memoryWriteLong(f.count, parm + 12);

		cpuSetDReg(0, d0);
	}

	void ftrap_write(uint16_t trap)
	{
		uint32_t d0;

		uint32_t sp = cpuGetAReg(7);
		uint32_t parm  = memoryReadLong(sp + 4);

		MPWFile f;

		f.flags = memoryReadWord(parm);
		f.error = memoryReadWord(parm + 2);
		f.device = memoryReadLong(parm + 4);
		f.cookie = memoryReadLong(parm + 8);
		f.count = memoryReadLong(parm + 12);
		f.buffer = memoryReadLong(parm + 16);

		Log("%04x Write(%08x)\n", trap, parm);


		d0 = 0;
		int fd = f.cookie;
		ssize_t size;

		Log("     write(%04x, %08x, %08x)\n", fd, f.buffer, f.count);
		size = OS::Internal::FDEntry::write(fd, memoryPointer(f.buffer), f.count);

		if (size < 0)
		{
			//f.count = 0;
			f.error = MacOS::ioErr; // ioErr
			d0 = mpw_errno_from_errno();
		}
		else
		{
			f.count -= size;
			f.error = 0;
		}

		// write back...
		memoryWriteWord(f.error, parm + 2);
		memoryWriteLong(f.count, parm + 12);

		cpuSetDReg(0, d0);
	}

}
