#include "os.h"
#include "toolbox.h"

#include <cpu/defs.h>
#include <cpu/CpuModule.h>
#include <cpu/fmem.h>

#include <string>
#include <cerrno>

#include <sys/xattr.h>
#include <sys/stat.h>
#include <sys/paths.h>
#include <stdlib.h>

namespace {

	using namespace OS;

	uint16_t errno_to_oserr(int xerrno)
	{
		switch (xerrno)
		{
			case EBADF: return rfNumErr;
			case EIO: return ioErr;
			case EACCES: return permErr;
			case ENOENT: return fnfErr;
			case ENOTDIR: return dirNFErr;
			case EISDIR: return notAFileErr;
			case ENOTSUP: return extFSErr;
			case EROFS: return wPrErr;

			case EDQUOT: return dskFulErr;
			case ENOSPC: return dskFulErr;


			default:
				return ioErr;
		}

	}
}

namespace OS
{

	uint16_t GetFileInfo(uint16_t trap)
	{

		uint16_t d0;

		uint32_t parm = cpuGetAReg(0);

		fprintf(stderr, "%04x GetFileInfo(%08x)\n", trap, parm);

		uint32_t ioCompletion = memoryReadLong(parm + 12);
		uint32_t ioNamePtr = memoryReadLong(parm + 18);
		uint16_t ioVRefNum = memoryReadWord(parm + 22);
		uint8_t ioFVersNum = memoryReadByte(parm + 26);
		int16_t ioFDirIndex = memoryReadWord(parm + 28);

		if (ioFDirIndex <= 0)
		{
			// based name
			std::string sname;

			if (!ioNamePtr)
			{
				memoryWriteWord(bdNamErr, parm + 16);
				return bdNamErr;
			}

			sname = ToolBox::ReadPString(ioNamePtr);

			// todo -- how are absolute, relative, etc paths handled...


			struct stat st;

			if (::stat(sname.c_str(), &st) < 0)
			{
				d0 = errno_to_oserr(errno);

				memoryWriteWord(d0, parm + 16);
				return d0;
			}

			// finder info
			{
				uint8_t buffer[32];
				std::memset(buffer, 0, sizeof(buffer));

				::getxattr(sname.c_str(), XATTR_FINDERINFO_NAME, buffer, 32, 0, 0);

				// only 16 bytes copied.
				std::memcpy(memoryPointer(parm + 32), buffer, 16);

			}


			// file reference number
			memoryWriteWord(0, parm + 24);
			// file attributes
			memoryWriteWord(0, parm + 30);
			// version (unused)
			memoryWriteWord(0, parm + 32);

			// file id
			memoryWriteLong(0, parm + 48);


			// file size
			memoryWriteWord(0, parm + 52);
			memoryWriteLong(st.st_size, parm + 54);
			memoryWriteLong(st.st_size, parm + 58);

			// todo -- create date, create time (seconds since 1904, I believe...)
			const long EpochAdjust = 86400 * (365 * (1970 - 1904) + 17); // 17 leap years.

			// create date.
			memoryWriteLong(st.st_birthtime + EpochAdjust, parm + 72);
			memoryWriteLong(st.st_mtime + EpochAdjust, parm + 76);

			// res fork...
			// do this last since it adjusts the name and the stat.

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

			// no error.
			memoryWriteWord(0, parm + 16);


		}
		else
		{
			fprintf(stderr, "GetFileInfo -- ioFDirIndex not yet supported\n");
			exit(1);
		}

		// if iocompletion handler && asyn call....

		return 0;
	}

	uint16_t SetFileInfo(uint16_t trap)
	{
		std::string sname;
		uint16_t d0;


		uint32_t parm = cpuGetAReg(0);

		fprintf(stderr, "%04x SetFileInfo(%08x)\n", trap, parm);

		uint32_t ioCompletion = memoryReadLong(parm + 12);
		uint32_t ioNamePtr = memoryReadLong(parm + 18);
		uint16_t ioVRefNum = memoryReadWord(parm + 22);
		uint8_t ioFVersNum = memoryReadByte(parm + 26);
		//int16_t ioFDirIndex = memoryReadWord(parm + 28);

		// + 32 = finder data - 16 bytes.

		uint32_t ioFlCrDat = memoryReadLong(parm + 72);
		uint32_t ioFlMdDat = memoryReadLong(parm + 76);

		// currently, only sets finder info.

		if (!ioNamePtr)
		{
			memoryWriteWord(bdNamErr, parm + 16);
			return bdNamErr;
		}

		sname = ToolBox::ReadPString(ioNamePtr);

		// check if the file actually exists
		{
			struct stat st;
			int ok;

			ok = ::stat(sname.c_str(), &st);
			if (ok < 0)
			{
				d0 = errno_to_oserr(errno);
				memoryWriteWord(d0, parm + 16);
				return d0; 
			}


		}

		// finder info is actually 32 bytes, so read and update the first 16.
		{
			uint8_t buffer[32];
			int ok;

			std::memset(buffer, 0, sizeof(buffer));

			ok = ::getxattr(sname.c_str(), XATTR_FINDERINFO_NAME, buffer, 32, 0, 0);

			// only 16 bytes copied.
			std::memcpy(buffer, memoryPointer(parm + 32), 16);

			ok = ::setxattr(sname.c_str(), XATTR_FINDERINFO_NAME, buffer, 32, 0, 0);	

			if (ok < 0)
			{
				d0 = errno_to_oserr(errno);
				memoryWriteWord(d0, parm + 16);
				return d0; 				
			}
		} 

		d0 = 0;
		memoryWriteWord(d0, parm + 16);
		return d0;
	}


}