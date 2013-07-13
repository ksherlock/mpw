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
#include <sys/paths.h>

#include <cpu/defs.h>
#include <cpu/fmem.h>
#include <cpu/cpuModule.h>


#include <macos/errors.h>

#include <toolbox/os.h>
#include <toolbox/os_internal.h>




namespace MPW
{
	
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

		f.error = 0;

		int fd = f.cookie;

		Log("     dup(%02x)\n", fd);


		d0 = OS::Internal::FDEntry::action(fd,
			[](int fd, OS::Internal::FDEntry &e){
				e.refcount++;
				return 0;
			},
			[](int fd){
				return kEINVAL;
			}
		);

		#if 0
		try
		{
			auto &e = OS::Internal::FDTable.at(fd);

			if (e.refcount)
			{
				d0 = 0;
				fd.refcount++;
			} 
			else
			{
				d0 = kEINVAL;
			}
		}
		catch(std::out_of_range &ex)
		{
			d0 = kEINVAL;
		}
		#endif

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

		int fd = f.cookie;

		Log("     bufsize(%02x)\n", fd);

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

		// linkgs reads from stdin and 
		// doesn't work quite right when 
		// this returns 0.  So, don't.

		f.error = 0;

		int fd = f.cookie;

		Log("     interactive(%02x)\n", fd);

		d0 = OS::Internal::FDEntry::action(fd,
			[](int fd, OS::Internal::FDEntry &e){

				int tty = ::isatty(fd);
				return tty ? 0 : kEINVAL;
			},
			[](int fd){
				return kEINVAL;
			}
		);

#if 0
		try
		{
			auto &e = OS::Internal::FDTable.at(fd);

			if (e.refcount)
			{
				int tty = ::isatty(fd);
				d0 = tty ? 0 : kEINVAL;
			} 
			else
			{
				d0 = kEINVAL;
			}
		}
		catch(std::out_of_range &ex)
		{
			d0 = kEINVAL;
		}
#endif

		memoryWriteWord(f.error, parm + 2);
		return d0;
	}

	uint32_t ftrap_fname(uint32_t parm, uint32_t arg)
	{
		// return file name.
		// AsmIIgs uses this...

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

		memoryWriteWord(f.error, parm + 2);
		return kEINVAL;
	}

	uint32_t ftrap_refnum(uint32_t parm, uint32_t arg)
	{
		// returns the refnum in *arg
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

		Log("     refnum(%02x)\n", fd);

		d0 = OS::Internal::FDEntry::action(fd,
			[arg](int fd, OS::Internal::FDEntry &e){
				memoryWriteWord(fd, arg);
				return 0;
			},
			[](int fd){
				return kEINVAL;
			}
		);
		
#if 0
		if (fd < 0 || fd >= FDTable.size() || !FDTable[fd])
		{
			d0 = kEINVAL;
		}
		else
		{
			d0 = 0;
			memoryWriteWord(fd, arg);
		}
#endif

		memoryWriteWord(f.error, parm + 2);
		return d0;
	}



	uint32_t ftrap_lseek(uint32_t parm, uint32_t arg)
	{
		MPWFile f;
		uint32_t d0;

		uint32_t whence = memoryReadLong(arg);
		int32_t offset = memoryReadLong(arg + 4); // signed value.
		int nativeWhence = 0;

		f.flags = memoryReadWord(parm);
		f.error = memoryReadWord(parm + 2);
		f.device = memoryReadLong(parm + 4);
		f.cookie = memoryReadLong(parm + 8);
		f.count = memoryReadLong(parm + 12);
		f.buffer = memoryReadLong(parm + 16);


		int fd = f.cookie;

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

		Log("     lseek(%02x, %08x, %02x)\n", fd, offset, nativeWhence);

		if (::isatty(fd))
		{
			off_t rv = -1;

			d0 = kEINVAL;
			f.error = 0;

			memoryWriteLong(rv, arg + 4);
			memoryWriteWord(f.error, parm + 2);
			return d0;

		}

		off_t rv = ::lseek(fd, offset, nativeWhence);
		if (rv < 0)
		{
			d0 = errno_to_errno(errno);
			f.error = OS::Internal::errno_to_oserr(errno);
			//perror(NULL);
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


	uint32_t ftrap_seteof(uint32_t parm, uint32_t arg)
	{

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

		Log("     seteof(%02x, %08x)\n", fd, arg);

		d0 = OS::Internal::FDEntry::action(fd,
			[arg, &f](int fd, OS::Internal::FDEntry &e){
				int ok = ftruncate(fd, arg);
				if (ok == 0) return 0;
				f.error = OS::Internal::errno_to_oserr(errno);
				return errno_to_errno(errno);
			},
			[](int fd){
				return kEINVAL;
			}
		);
		

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