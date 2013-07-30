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

#include <cstdio>
#include <algorithm>
#include <memory>

#include <unistd.h>

#include <cpu/defs.h>
#include <cpu/fmem.h>
#include <cpu/cpuModule.h>

#include <macos/errors.h>

#include <toolbox/os.h>
#include <toolbox/os_internal.h>

namespace MPW
{


	void ftrap_close(uint16_t trap)
	{
		// returns an mpw_errno
		// close actually checks the error in the File Entry and converts that to unix.
		// (sigh)

		uint32_t d0 = 0;

		uint32_t sp = cpuGetAReg(7);
		uint32_t parm  = memoryReadLong(sp + 4);

		MPWFile f;

		f.flags = memoryReadWord(parm);
		f.error = memoryReadWord(parm + 2);
		f.device = memoryReadLong(parm + 4);
		f.cookie = memoryReadLong(parm + 8);
		f.count = memoryReadLong(parm + 12);
		f.buffer = memoryReadLong(parm + 16);


		Log("%04x Close(%08x)\n", trap, parm);

		if (!parm)
		{
			cpuSetDReg(0, kEINVAL);
			return;
		}


		int fd = f.cookie;

		int rv = OS::Internal::FDEntry::close(fd);

		if (rv < 0)
		{
			f.error = MacOS::notOpenErr;
			d0 = kEINVAL;
		}
		else
		{
			f.error = 0;
			d0 = 0;
		}


#if 0
		if (fd < 0 || fd >= OS::Internal::FDTable.size())
		{
			f.error = OS::notOpenErr;
			d0 = kEINVAL;
		}
		else
		{
			auto &e = OS::Internal::FDTable[fd];
			if (e.refcount == 0)
			{
				f.error = OS::notOpenErr;
				d0 = kEINVAL;
			}
			else 
			{
				if (--e.refcount == 0)
				{
					Log("     close(%02x)\n", fd);
					::close(fd);
				}
				f.error = 0;
				d0 = 0;
			}
		}
#endif

		memoryWriteWord(f.error, parm + 2);
		cpuSetDReg(0, 0);
	}


}
