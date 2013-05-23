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

#include <macos/sysequ.h>
#include <macos/errors.h>

#include "os.h"
#include "os_internal.h"
#include "toolbox.h"
#include "stackframe.h"


using ToolBox::Log;

using OS::Internal::errno_to_oserr;



namespace {


	uint32_t rforksize(const std::string &path)
	{
		ssize_t rv;

		rv = getxattr(path.c_str(), XATTR_RESOURCEFORK_NAME, nullptr, 0, 0, 0);
		if (rv < 0) return 0;
		return rv;
	}

	uint32_t rforksize(int fd)
	{
		ssize_t rv;

		rv = fgetxattr(fd, XATTR_RESOURCEFORK_NAME, nullptr, 0, 0, 0);
		if (rv < 0) return 0;
		return rv;

	}

}

namespace OS {
	

	uint16_t GetFileInfo(uint16_t trap)
	{

		enum { // FileParam
			_qLink = 0,
			_qType = 4,
			_ioTrap = 6,
			_ioCmdAddr = 8,
			_ioCompletion = 12,
			_ioResult = 16,
			_ioNamePtr = 18,
			_ioVRefNum = 22,
			_ioFRefNum = 24,
			_ioFVersNum = 26,
			_filler1 = 27,
			_ioFDirIndex = 28,
			_ioFlAttrib = 30,
			_ioFlVersNum = 31,
			_ioFlFndrInfo = 32,
			_ioFlNum = 48, // ioDirID in other
			_ioFlStBlk = 52,
			_ioFlLgLen = 54,
			_ioFlPyLen = 58,
			_ioFlRStBlk = 62,
			_ioFlRLgLen = 64,
			_ioFlRPyLen = 68,
			_ioFlCrDat = 72,
			_ioFlMdDat = 76,
		};




		uint16_t d0;

		uint32_t parm = cpuGetAReg(0);

		Log("%04x GetFileInfo($%08x)\n", trap, parm);

		//uint32_t ioCompletion = memoryReadLong(parm + _ioCompletion);
		uint32_t ioNamePtr = memoryReadLong(parm + _ioNamePtr);
		//uint16_t ioVRefNum = memoryReadWord(parm + _ioVRefNum);
		//uint8_t ioFVersNum = memoryReadByte(parm + _ioFVersNum);
		int16_t ioFDirIndex = memoryReadWord(parm + _ioFDirIndex);

		if (ioFDirIndex <= 0)
		{
			// based name
			std::string sname;

			if (!ioNamePtr)
			{
				d0 = MacOS::bdNamErr;
				memoryWriteWord(d0, parm + _ioResult);
				return d0;
			}

			sname = ToolBox::ReadPString(ioNamePtr, true);

			Log("     GetFileInfo(%s)\n", sname.c_str());

			// todo -- how are absolute, relative, etc paths handled...


			struct stat st;

			if (::stat(sname.c_str(), &st) < 0)
			{
				d0 = errno_to_oserr(errno);

				memoryWriteWord(d0, parm + _ioResult);
				return d0;
			}


			Internal::GetFinderInfo(sname, memoryPointer(parm + _ioFlFndrInfo), false); 


			// file reference number
			memoryWriteWord(0, parm + _ioFRefNum);
			// file attributes
			memoryWriteByte(0, parm + _ioFlAttrib);
			// version (unused)
			memoryWriteByte(0, parm + _ioFlVersNum);

			// file id
			memoryWriteLong(0, parm + _ioFlNum);


			// file size
			memoryWriteWord(0, parm + _ioFlStBlk);
			memoryWriteLong(st.st_size, parm + _ioFlLgLen);
			memoryWriteLong(st.st_size, parm + _ioFlPyLen);

			// create date.
			memoryWriteLong(UnixToMac(st.st_birthtime), parm + _ioFlCrDat);
			memoryWriteLong(UnixToMac(st.st_mtime), parm + _ioFlMdDat);

			// res fork...

			uint32_t rf = rforksize(sname);

			memoryWriteWord(0, parm + _ioFlRStBlk);
			memoryWriteLong(rf, parm + _ioFlRLgLen);
			memoryWriteLong(rf, parm + _ioFlRPyLen);


			// no error.
			memoryWriteWord(0, parm + _ioResult);
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

		enum { // FileParam
			_qLink = 0,
			_qType = 4,
			_ioTrap = 6,
			_ioCmdAddr = 8,
			_ioCompletion = 12,
			_ioResult = 16,
			_ioNamePtr = 18,
			_ioVRefNum = 22,
			_ioFRefNum = 24,
			_ioFVersNum = 26,
			_filler1 = 27,
			_ioFDirIndex = 28,
			_ioFlAttrib = 30,
			_ioFlVersNum = 31,
			_ioFlFndrInfo = 32,
			_ioFlNum = 48, // ioDirID in other
			_ioFlStBlk = 52,
			_ioFlLgLen = 54,
			_ioFlPyLen = 58,
			_ioFlRStBlk = 62,
			_ioFlRLgLen = 64,
			_ioFlRPyLen = 68,
			_ioFlCrDat = 72,
			_ioFlMdDat = 76,
		};


		std::string sname;
		uint16_t d0;


		uint32_t parm = cpuGetAReg(0);

		Log("%04x SetFileInfo($%08x)\n", trap, parm);

		//uint32_t ioCompletion = memoryReadLong(parm + _ioCompletion);
		uint32_t ioNamePtr = memoryReadLong(parm + _ioNamePtr);
		//uint16_t ioVRefNum = memoryReadWord(parm + _ioVRefNum);
		//uint8_t ioFVersNum = memoryReadByte(parm + _ioFVersNum);
		//int16_t ioFDirIndex = memoryReadWord(parm + _ioFDirIndex);

		// + 32 = finder data - 16 bytes.

		//uint32_t ioFlCrDat = memoryReadLong(parm + 72);
		//uint32_t ioFlMdDat = memoryReadLong(parm + 76);

		// currently, only sets finder info.

		if (!ioNamePtr)
		{
			d0 = MacOS::bdNamErr;
			memoryWriteWord(d0, parm + _ioResult);
			return d0;
		}

		sname = ToolBox::ReadPString(ioNamePtr, true);
		Log("     SetFileInfo(%s)\n", sname.c_str());

		// check if the file actually exists
		{
			struct stat st;
			int ok;

			ok = ::stat(sname.c_str(), &st);
			if (ok < 0)
			{
				d0 = errno_to_oserr(errno);
				memoryWriteWord(d0, parm + _ioResult);
				return d0; 
			}


		}

		d0 = Internal::SetFinderInfo(sname, memoryPointer(parm + 32), false);

		memoryWriteWord(d0, parm + _ioResult);
		return d0;
	}







	uint16_t HGetFileInfo(uint16_t trap)
	{
		
		enum { // HFileParam
			_qLink = 0,
			_qType = 4,
			_ioTrap = 6,
			_ioCmdAddr = 8,
			_ioCompletion = 12,
			_ioResult = 16,
			_ioNamePtr = 18,
			_ioVRefNum = 22,
			_ioFRefNum = 24,
			_ioFVersNum = 26,
			_filler1 = 27,
			_ioFDirIndex = 28,
			_ioFlAttrib = 30,
			_ioFlVersNum = 31,
			_ioFlFndrInfo = 32,
			_ioDirID = 48, // ioFLNum in other
			_ioFlStBlk = 52,
			_ioFlLgLen = 54,
			_ioFlPyLen = 58,
			_ioFlRStBlk = 62,
			_ioFlRLgLen = 64,
			_ioFlRPyLen = 68,
			_ioFlCrDat = 72,
			_ioFlMdDat = 76,
		};

		// close enough... for now.
		return GetFileInfo(trap);
	}



}