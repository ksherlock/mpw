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


		d0 = OS::Internal::FDEntry::action(fd, 
			// success callback.
			[&f](int fd, OS::Internal::FDEntry &e)
			{
				f.error = 0;
				if (--e.refcount == 0)
				{
					Log("     close(%02x)\n", fd);
					::close(fd);
				}
				return 0;
			},
			// error callback.
			[&f](int fd){
				f.error = OS::notOpenErr;
				return kEINVAL;
			}
		);

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
