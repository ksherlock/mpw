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


namespace MPW
{

	using namespace Internal;

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


		Log("%04x Read(%08x)\n", parm);

		d0 = 0;

		if (f.count)
		{
			ssize_t size;

			int fd = f.cookie;

			Log("     read(%02x, %08x, %08x)\n", f.cookie, f.buffer, f.count);


			if (f.flags & kO_BINARY)
			{
				size = ::read(fd, memoryPointer(f.buffer), f.count);
			}
			else
			{
				std::unique_ptr<uint8_t[]> buffer(new uint8_t[f.count]);
				uint8_t *ptr = memoryPointer(f.buffer);

				size = ::read(fd, buffer.get(), f.count);

				if (size > 0)
				{
					std::transform(buffer.get(), buffer.get() + size, ptr, 
						[](uint8_t c) { return c == '\n' ? '\r' : c; }
					);
				}
			}

			if (size < 0)
			{
				//f.count = 0;
				f.error = OS::ioErr; // ioErr
				d0 = 0x40000000 | errno_to_errno(errno);
			}
			else
			{
				f.count -= size;
				f.error = 0;
			}

			// write back...
			memoryWriteWord(f.error, parm + 2);
			memoryWriteLong(f.count, parm + 12);
		}

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
		if (f.count)
		{
			ssize_t size;

			int fd = f.cookie;

			Log("     write(%02x, %08x, %08x)\n", f.cookie, f.buffer, f.count);


			if (f.flags & kO_BINARY)
			{
				size = ::write(fd, memoryPointer(f.buffer), f.count);
			}
			else
			{
				std::unique_ptr<uint8_t[]> buffer(new uint8_t[f.count]);
				uint8_t *ptr = memoryPointer(f.buffer);

				std::transform(ptr, ptr + f.count, buffer.get(), 
					[](uint8_t c) { return c == '\r' ? '\n' : c; }
				);

				size = ::write(fd, buffer.get(), f.count);
			}

			if (size < 0)
			{
				//f.count = 0;
				f.error = -36; // ioErr
				d0 = 0x40000000 | errno_to_errno(errno);
			}
			else
			{
				// this is, apparently, correct.
				f.count -= size;
				f.error = 0;
			}

			// write back...
			memoryWriteWord(f.error, parm + 2);
			memoryWriteLong(f.count, parm + 12);
		}
		cpuSetDReg(0, d0);		
	}

}