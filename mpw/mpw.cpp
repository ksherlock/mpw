#include <cpu/defs.h>
#include <cpu/fmem.h>
#include <cpu/cpuModule.h>

#include <cstdint>
#include <cstdio>
#include <memory>
#include <algorithm>

#include <unistd.h>
#include <fcntl.h>

#include <stdlib.h>



#include "mpw.h"


namespace {

	using namespace MPW;

	void ftrap_access(uint16_t trap)
	{
		// open a file, rename a file, or delete a file.

		uint32_t sp = cpuGetAReg(7);

		// hmmm not sure if 3 or 4 parameters.
		
		uint32_pt name = memoryReadLong(sp + 4);
		uint32_t op = memoryReadLong(sp + 8);
		uint32_t parm = memoryReadLong(sp + 12);


		MPWFile f;


		fprintf(stderr, "%04x Access()\n", trap);
		cpuSetDReg(0, 0);				
	}

	void ftrap_close(uint16_t trap)
	{
		// this can be nopped since it's stdin/stdout/stderr.

		fprintf(stderr, "%04x Close()\n", trap);
		cpuSetDReg(0, 0);				
	}

	void ftrap_read(uint16_t trap)
	{
		fprintf(stderr, "%04x Read()\n", trap);
	}

	void ftrap_write(uint16_t trap)
	{
		uint32_t sp = cpuGetAReg(7);
		uint32_t parm  = memoryReadLong(sp + 4);

		MPWFile f;

		f.flags = memoryReadWord(parm);
		f.error = memoryReadWord(parm + 2);
		f.device = memoryReadLong(parm + 4);
		f.cookie = memoryReadLong(parm + 8);
		f.count = memoryReadLong(parm + 12);
		f.buffer = memoryReadLong(parm + 16);

		fprintf(stderr, "%04x Write(%08x)\n", trap, parm);

		// hmmm how to handle crlf?


		if (f.count)
		{
			std::unique_ptr<uint8_t[]> buffer(new uint8_t[f.count]);
			uint8_t *ptr = memoryPointer(f.buffer);


			std::transform(ptr, ptr + f.count, buffer.get(), 
				[](uint8_t c) { return c == '\r' ? '\n' : c; }
			);

			int fd = f.cookie;

			::write(fd, buffer.get(), f.count);

			// hmm, also needs to update the count actually transferred?

		}
		cpuSetDReg(0, 0);		
	}

	void ftrap_ioctl(uint16_t trap)
	{
		// int ioctl(int fildes, unsigned int cmd, long *arg);

		uint32_t sp = cpuGetAReg(7);

		uint32_t fd = memoryReadLong(sp + 4);
		uint32_t cmd = memoryReadLong(sp + 8);
		uint32_t arg = memoryReadLong(sp + 12);

		fprintf(stderr, "%04x IOCtl(%08x, %08x, %08x)\n", trap, fd, cmd, arg);

		cpuSetDReg(0, 0);
	}

	void ftrap_quit(uint16_t trap)
	{
		fprintf(stderr, "%04x Quit()\n", trap);
		cpuSetStop(true);
	}

}

namespace MPW
{

	void dispatch(uint16_t trap)
	{
		switch (trap)
		{
			case fQuit:
				ftrap_quit(trap);
				break;
			case fAccess:
				ftrap_access(trap);
				break;

			case fClose:
				ftrap_close(trap);
				break;
			case fRead:
				ftrap_read(trap);
				break;
			case fWrite:
				ftrap_write(trap);
				break;
			case fIOCtl:
				ftrap_ioctl(trap);
				break;

			default:
				fprintf(stderr, "Unsupported f trap: %04x\n", trap);
				exit(1);
				break;
		}

	}

}
