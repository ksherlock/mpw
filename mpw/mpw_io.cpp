#include "mpw.h"
#include "mpw_internal.h"

#include <algorithm>
#include <memory>

#include <cerrno>
#include <cstdio>

#include <unistd.h>

#include <cpu/defs.h>
#include <cpu/fmem.h>
#include <cpu/cpuModule.h>

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
			f.error = OS::ioErr; // ioErr
			d0 = errno_to_errno(errno);
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
			f.error = OS::ioErr; // ioErr
			d0 = errno_to_errno(errno);
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