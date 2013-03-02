#include "mpw.h"
#include "mpw_internal.h"
#include "mpw_errno.h"

#include <algorithm>
#include <memory>
#include <string>


#include <cstdio>
#include <cstring>
#include <cerrno>

#include <unistd.h>
#include <fcntl.h>
#include <sys/paths.h>

#include <cpu/defs.h>
#include <cpu/fmem.h>
#include <cpu/cpuModule.h>

#include <toolbox/toolbox.h>
#include <toolbox/os.h>
#include <toolbox/os_internal.h>

/*
 * access return errors are |= 0x40000000.  Not entirely sure why...
 * may return an errno or an oserr, too, apparently.
 */


namespace MPW
{


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


		sname = ToolBox::ReadCString(name, true);
		std::string xname = sname;

		Log("     open(%s, %04x)\n", sname.c_str(), f.flags);

		// TODO -- can you create a resource file like this?

		if (f.flags & kO_RSRC)
			sname.append(_PATH_RSRCFORKSPEC);

		if (f.flags & kO_CREAT)
			fd = ::open(sname.c_str(), nativeFlags, 0666);
		else
			fd = ::open(sname.c_str(), nativeFlags);

		if (fd < 0)
		{
			// return an errno.
			d0 = 0x40000000 | errno_to_errno(errno);
			f.error = -36; // ioErr ... whatever.
			f.cookie = 0;
		}
		else
		{
			d0 = 0;
			f.error = 0;
			f.cookie = fd;


			// adjust the binary flags...
			// some apps are good about this but 
			// dumpobj, makelib, linkiigs don't set O_BINARY (but should)
			// MPW Assembler sets O_BINARY (but shouldn't)

			if (OS::IsTextFile(sname)) f.flags &= ~kO_BINARY;
			if (OS::IsBinaryFile(sname)) f.flags |= kO_BINARY;

			if (f.flags & kO_RSRC) f.flags |= kO_BINARY;

			auto &e = OS::Internal::FDEntry::allocate(fd, std::move(xname));
			e.text = !(f.flags & kO_BINARY);
			e.resource = f.flags & kO_RSRC;
		}

		memoryWriteWord(f.flags, parm + 0);
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

		Log("%04x Access(%08x, %04x, %08x)\n", trap, name, op, parm);
		
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



}