#include <cstdint>
#include <cstdio>
#include <cerrno>

#include <cpu/defs.h>
#include <cpu/fmem.h>
#include <cpu/cpuModule.h>

#include <memory>
#include <string>
#include <algorithm>

#include <unistd.h>
#include <fcntl.h>

#include <stdlib.h>
#include <sys/paths.h>



#include "mpw.h"
#include "mpw_errno.h"

namespace {

	using namespace MPW;


	int remap_errno(int xerrno)
	{
		switch (xerrno)
		{
		case EPERM: return kEPERM;
		case ENOENT: return kENOENT;

		#ifdef ENORSRC
		case ENORSRC: return kENORSRC;
		#endif

		case EINTR: return kEINTR;
		case EIO: return kEIO;
		case ENXIO: return kENXIO;
		case E2BIG: return kE2BIG;
		case ENOEXEC: return kENOEXEC;
		case EBADF: return kEBADF;
		case ECHILD: return kECHILD;
		case EAGAIN: return kEAGAIN;
		case ENOMEM: return kENOMEM;
		case EACCES: return kEACCES;
		case EFAULT: return kEFAULT;
		case ENOTBLK: return kENOTBLK;
		case EBUSY: return kEBUSY;
		case EEXIST: return kEEXIST;
		case EXDEV: return kEXDEV;
		case ENODEV: return kENODEV;
		case ENOTDIR: return kENOTDIR;
		case EISDIR: return kEISDIR;
		case EINVAL: return kEINVAL;
		case ENFILE: return kENFILE;
		case EMFILE: return kEMFILE;
		case ENOTTY: return kENOTTY;
		case ETXTBSY: return kETXTBSY;
		case EFBIG: return kEFBIG;
		case ENOSPC: return kENOSPC;
		case ESPIPE: return kESPIPE;
		case EROFS: return kEROFS;
		case EMLINK: return kEMLINK;
		case EPIPE: return kEPIPE;
		case EDOM: return kEDOM;
		case ERANGE: return kERANGE;
		}
		return kEINVAL;

	}

	uint32_t ftrap_open(uint32_t name, uint32_t parm)
	{
		uint32_t d0;
		int fd;
		std::string sname;

		MPWFile f;
		int nativeFlags;
		std::memset(&f, 0, sizeof(f));

		f.flags = memoryReadWord(parm);


		nativeFlags = 0;
		switch (f.flags & 0x03)
		{
			case 0x01:
				nativeFlags = O_RDONLY;
				break;
			case 0x02:
				nativeFlags = O_WRONLY;
				break;
			case 0x00: // ????
			case 0x03:
				nativeFlags = O_RDWR;
				break;
		}

		if (f.flags & kO_APPEND) nativeFlags |= O_APPEND;
		if (f.flags & kO_CREAT) nativeFlags |= O_CREAT;
		if (f.flags & kO_TRUNC) nativeFlags |= O_TRUNC;
		if (f.flags & kO_EXCL) nativeFlags |= O_EXCL;


		const char *cp = (const char *)memoryPointer(name);
		sname.assign(cp);

		if (f.flags & kO_RSRC)
			sname.append(_PATH_RSRCFORKSPEC);

		if (f.flags & kO_CREAT) fd = ::open(sname.c_str(), nativeFlags, 0666);
		else fd = ::open(sname.c_str(), nativeFlags);

		if (fd < 0)
		{
			d0 = 0x40000000 | remap_errno(errno);
			f.error = -36; // ioErr ... whatever.
			f.cookie = 0;
		}
		else
		{
			d0 = 0;
			f.error = 0;
			f.cookie = fd;
			// todo -- keep a separate array of fd/counts for dup/close?

			// ... do I need to create the buffer?
		}

		memoryWriteWord(f.error, parm + 2);
		memoryWriteLong(f.cookie, parm + 8);

		return d0;
	}

	void ftrap_access(uint16_t trap)
	{
		// open a file, rename a file, or delete a file.
		std::string sname;
		uint32_t d0;

		uint32_t sp = cpuGetAReg(7);

		// hmmm not sure if 3 or 4 parameters.

		uint32_t name = memoryReadLong(sp + 4);
		uint32_t op = memoryReadLong(sp + 8);
		uint32_t parm = memoryReadLong(sp + 12);

		fprintf(stderr, "%04x Access(%08x, %04x, %08x)\n", trap, name, op, parm);

		switch (op)
		{
		case kF_OPEN:
			d0 = ftrap_open(name, parm);
			break;
		default:
			d0 = 0x40000000 | kEINVAL;
			fprintf(stderr, "faccess - unsupported op %04x\n", op);		
		}

		cpuSetDReg(0, d0);				
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

		fprintf(stderr, "%04x Write(%08x)\n", trap, parm);

		// hmmm how to handle crlf?


		d0 = 0;
		if (f.count)
		{
			ssize_t size;

			int fd = f.cookie;

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
				f.count = 0;
				f.error = -36; // ioErr
				d0 = 0x40000000 | remap_errno(errno);
			}
			else
			{
				f.count = size;
				f.error = 0;
			}

			// write back...
			memoryWriteWord(f.error, parm + 2);
			memoryWriteLong(f.count, parm + 12);
		}
		cpuSetDReg(0, d0);		
	}

	void ftrap_ioctl(uint16_t trap)
	{
		// int ioctl(int fildes, unsigned int cmd, long *arg);
		uint32_t d0;
		uint32_t sp = cpuGetAReg(7);

		uint32_t fd = memoryReadLong(sp + 4);
		uint32_t cmd = memoryReadLong(sp + 8);
		uint32_t arg = memoryReadLong(sp + 12);

		fprintf(stderr, "%04x IOCtl(%08x, %08x, %08x)\n", trap, fd, cmd, arg);

		// dup doesn't appear to dup so much as increment a refCount
		// close decrements this refCount and doesn't close until it hits 0.
		d0 = 0;

		cpuSetDReg(0, d0);
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
