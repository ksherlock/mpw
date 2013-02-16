#include "mpw.h"
#include "mpw_errno.h"
#include "mpw_internal.h"

#include <vector>

#include <cstdint>
#include <cstdio>
#include <cerrno>

#include <stdlib.h>
#include <unistd.h>


#include <cpu/defs.h>
#include <cpu/fmem.h>
#include <cpu/cpuModule.h>


namespace MPW { namespace Internal {

	// for dup counts, etc.
	std::vector<int> FDTable;

} }


namespace MPW
{
	using namespace Internal;

	bool Trace = false;

	int errno_to_errno(int xerrno)
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

	void Init()
	{
		FDTable.resize(16);

		FDTable[STDIN_FILENO] = 1;
		FDTable[STDOUT_FILENO] = 1;
		FDTable[STDERR_FILENO] = 1;

		// todo -- should eventually set up the mpw environment.
	}


	void ftrap_quit(uint16_t trap)
	{
		Log("%04x Quit()\n", trap);
		//fprintf(stderr, "%04x Quit()\n", trap);
		cpuSetStop(true);
	}

	void dispatch(uint16_t trap)
	{
		switch (trap)
		{
			case fQuit:
			case 0xffff:
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
