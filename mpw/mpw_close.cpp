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

#include <toolbox/os.h>

namespace MPW
{

	using namespace Internal;

	void ftrap_close(uint16_t trap)
	{
		// returns an mpw_errno
		// close actually checks the error in the File Entry and converts that to unix.
		// (sigh)

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


		//fprintf(stderr, "%04x Close(%08x)\n", trap, parm);
		Log("%04x Close(%08x)\n", trap, parm);

		if (!parm)
		{
			cpuSetDReg(0, kEINVAL);
			return;
		}


		int fd = f.cookie;

		if (fd < 0 || fd >= FDTable.size() || !FDTable[fd])
		{
			f.error = OS::notOpenErr;
			d0 = kEINVAL;
		}
		else
		{
			if (--FDTable[fd] == 0)
			{
				Log("     close(%02x)\n", fd);
				::close(fd);
				f.error = 0;
				d0 = 0;
			}
		}

		memoryWriteWord(f.error, parm + 2);
		cpuSetDReg(0, 0);
	}


}
