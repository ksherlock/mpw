#include "mpw.h"
#include "mpw_internal.h"
#include "mpw_errno.h"

#include <algorithm>
#include <memory>
#include <string>

#include <cstdio>
#include <cstring>
#include <cerrno>

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/paths.h>

#include <cpu/defs.h>
#include <cpu/fmem.h>
#include <cpu/cpuModule.h>

#include <toolbox/os.h>




namespace MPW
{
	using namespace Internal;
	
	uint32_t ftrap_dup(uint32_t parm, uint32_t arg)
	{
		uint32_t d0;
		MPWFile f;

		f.flags = memoryReadWord(parm);
		f.error = memoryReadWord(parm + 2);
		f.device = memoryReadLong(parm + 4);
		f.cookie = memoryReadLong(parm + 8);
		f.count = memoryReadLong(parm + 12);
		f.buffer = memoryReadLong(parm + 16);

		int fd = f.cookie;
		if (fd < 0 || fd >= FDTable.size() || !FDTable[fd])
		{
			d0 = kEINVAL; // & 0x40000000 ?
			f.error = 0;
		}
		else
		{
			FDTable[fd]++;
			d0 = 0;
			f.error = 0;
		}

		memoryWriteWord(f.error, parm + 2);
		return d0;
	}

	uint32_t ftrap_bufsize(uint32_t parm, uint32_t arg)
	{
		// should return the preferred buffsize in *arg
		// an error will use the default size (0x400 bytes).

		MPWFile f;

		f.flags = memoryReadWord(parm);
		f.error = memoryReadWord(parm + 2);
		f.device = memoryReadLong(parm + 4);
		f.cookie = memoryReadLong(parm + 8);
		f.count = memoryReadLong(parm + 12);
		f.buffer = memoryReadLong(parm + 16);

		f.error = 0;

		memoryWriteWord(f.error, parm + 2);
		return kEINVAL;
	}


	uint32_t ftrap_interactive(uint32_t parm, uint32_t arg)
	{
		// return 0 if interactive, an error if 
		// non-interactive.

		uint32_t d0;

		MPWFile f;

		f.flags = memoryReadWord(parm);
		f.error = memoryReadWord(parm + 2);
		f.device = memoryReadLong(parm + 4);
		f.cookie = memoryReadLong(parm + 8);
		f.count = memoryReadLong(parm + 12);
		f.buffer = memoryReadLong(parm + 16);

		f.error = 0;

		int fd = f.cookie;

		if (fd < 0 || fd >= FDTable.size() || !FDTable[fd])
		{
			d0 = kEINVAL;
		}
		else
		{
			int tty = ::isatty(fd);
			d0 = tty ? 0 : kEINVAL;
		}

		memoryWriteWord(f.error, parm + 2);
		return kEINVAL;
	}

	uint32_t ftrap_iofname(uint32_t parm, uint32_t arg)
	{
		// return file name.
		// AsmIIgs uses this...
		memoryWriteWord(0, parm + 2);
		return kEINVAL;
	}

	uint32_t ftrap_lseek(uint32_t parm, uint32_t arg)
	{
		MPWFile f;
		uint32_t d0;

		uint32_t whence = memoryReadLong(arg);
		int32_t offset =memoryReadLong(arg + 4);
		int nativeWhence = 0;

		f.flags = memoryReadWord(parm);
		f.error = memoryReadWord(parm + 2);
		f.device = memoryReadLong(parm + 4);
		f.cookie = memoryReadLong(parm + 8);
		f.count = memoryReadLong(parm + 12);
		f.buffer = memoryReadLong(parm + 16);


		int fd = f.cookie;


		// TODO - MacOS returns eofERR and sets mark to eof
		// if seeking past the eof.
		// TODO - MacOS treats offset as a signed value, unix is unsigned [?]
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

		fprintf(stderr, "     seek(%02x, %08x, %02x)\n", fd, offset, nativeWhence);

		off_t rv = ::lseek(fd, offset, nativeWhence);
		if (rv < 0)
		{
			d0 = errno_to_errno(errno);
			f.error = 0;
		}
		else
		{
			d0 = 0;
			f.error = 0;
		}

		memoryWriteLong(rv, arg + 4);
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

		fprintf(stderr, "%04x IOCtl(%08x, %08x, %08x)\n", trap, fd, cmd, arg);

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
				d0 = ftrap_iofname(fd, arg);
				break;

			case kFIOREFNUM:
			case kFIOSETEOF:
				fprintf(stderr, "ioctl - unsupported op %04x\n", cmd);	
				exit(1);
				break;
		}

		cpuSetDReg(0, d0);
	}


}