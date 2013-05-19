#include <cerrno>
#include <cctype>
#include <ctime>
#include <algorithm>
#include <chrono>
#include <deque>
#include <string>

#include <sys/xattr.h>
#include <sys/stat.h>
#include <sys/paths.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <strings.h>

#include <cpu/defs.h>
#include <cpu/CpuModule.h>
#include <cpu/fmem.h>

#include <macos/errors.h>

#include "os.h"
#include "os_internal.h"
#include "toolbox.h"
#include "stackframe.h"

using ToolBox::Log;

using OS::Internal::errno_to_oserr;

namespace OS {

	uint16_t PBGetCatInfo(uint32_t parm)
	{
		uint16_t d0;

		// yuck. this is sort of a getdirent/stat call....

		//uint32_t ioCompletion = memoryReadLong(parm + 12);
		uint32_t ioNamePtr = memoryReadLong(parm + 18);
		//uint16_t ioVRefNum = memoryReadWord(parm + 22);
		//uint8_t ioFVersNum = memoryReadByte(parm + 26);
		int16_t ioFDirIndex = memoryReadWord(parm + 28);

		if (ioFDirIndex <= 0)
		{
			// based name
			std::string sname;

			if (!ioNamePtr)
			{
				memoryWriteWord(MacOS::bdNamErr, parm + 16);
				return MacOS::bdNamErr;
			}

			sname = ToolBox::ReadPString(ioNamePtr, true);

			Log("     PBGetCatInfo(%s)\n", sname.c_str());

			struct stat st;

			if (::stat(sname.c_str(), &st) < 0)
			{
				d0 = errno_to_oserr(errno);

				memoryWriteWord(d0, parm + 16);
				return d0;
			}

			if (S_ISDIR(st.st_mode))
			{
				// bit 4 - is a directory.
				memoryWriteByte(1 << 4, parm + 30); // ioFlAttrib
				memoryWriteByte(0, parm + 31); //ioACUser

				std::memset(memoryPointer(parm + 32), 0, 16); // DInfo
				memoryWriteLong(0, parm + 48); // ioDrDirID
				memoryWriteWord(0, parm + 52); // ioDrNmFls - # of files in dir

				memoryWriteLong(UnixToMac(st.st_birthtime), parm + 60); // create
				memoryWriteLong(UnixToMac(st.st_mtime), parm + 64); // modify
				memoryWriteLong(UnixToMac(st.st_mtime), parm + 68); // backup

				std::memset(memoryPointer(parm + 72), 0, 16); // DXInfo
				memoryWriteLong(0, parm + 88); // ioDrParID
			}
			else
			{
				memoryWriteByte(0, parm + 30);

				memoryWriteByte(0, parm + 31); //ioACUser				
				Internal::GetFinderInfo(sname, memoryPointer(parm + 32), false); // finder info
				memoryWriteLong(0, parm + 48); // ioDrDirID
				memoryWriteWord(0, parm + 52); // ioFlStBlk
				memoryWriteLong(st.st_size, parm + 54); // ioFlLgLen
				memoryWriteLong(st.st_size, parm + 58); // ioFlPyLen

				// resource info... below

				memoryWriteLong(UnixToMac(st.st_birthtime), parm + 72); // create
				memoryWriteLong(UnixToMac(st.st_mtime), parm + 76); // modify
				memoryWriteLong(UnixToMac(st.st_mtime), parm + 80); // backup

				std::memset(memoryPointer(parm + 84), 0, 16); // FXInfo

				memoryWriteWord(0, parm + 100); // ioFlParID
				memoryWriteWord(0, parm + 104); // ioFlClpSiz

				sname.append(_PATH_RSRCFORKSPEC);
				if (::stat(sname.c_str(), &st) >= 0)
				{
					memoryWriteWord(0, parm + 62);
					memoryWriteLong(st.st_size, parm + 64);
					memoryWriteLong(st.st_size, parm + 68);
				}
				else
				{
					memoryWriteWord(0, parm + 62);
					memoryWriteLong(0, parm + 64);
					memoryWriteLong(0, parm + 68);
				}


			}

			// no error.
			memoryWriteWord(0, parm + 16);
			return 0;
		}

		else
		{
			fprintf(stderr, "GetFileInfo -- ioFDirIndex not yet supported\n");
			exit(1);
		}

		return 0;
	}

	uint16_t PBOpenDF(uint32_t paramBlock)
	{
		Log("     PBOpenDF\n");
		// same as Open but slightly different handling of . files.
		return OS::Open(0xa000);
	}

	uint16_t FSDispatch(uint16_t trap)
	{

		// TODO -- check new hfs bit ( (trap & 0x0200) == 0x0200)
		// TODO -- check async bit ((trap & 0x0400) == 0x0400)



		uint32_t selector = cpuGetDReg(0);
		uint32_t paramBlock = cpuGetAReg(0);

		Log("%04x FSDispatch(%08x, %08x)\n", trap, selector, paramBlock);

		switch (selector)
		{
			//case 0x0009:
				//return PBGetCatInfo(paramBlock);
				//break;

			case 0x1a:
				return PBOpenDF(paramBlock);
				break;

			default:
				fprintf(stderr, "HFSDispatch: selector %08x not implemented\n", 
					selector);
				exit(1);
		}
		return 0;
	}

	uint16_t HFSDispatch(uint16_t trap)
	{

		uint32_t selector = cpuGetDReg(0);
		uint32_t paramBlock = cpuGetAReg(0);

		Log("%04x HFSDispatch(%08x, %08x)\n", trap, selector, paramBlock);

		switch (selector)
		{
			case 0x0009:
				return PBGetCatInfo(paramBlock);
				break;

			default:
				fprintf(stderr, "HFSDispatch: selector %08x not implemented\n", 
					selector);
				exit(1);
		}
		return 0;
	}



}
