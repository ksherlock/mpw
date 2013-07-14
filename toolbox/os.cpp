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

	using namespace OS;

	// time stuff.
	const long EpochAdjust = 86400 * (365 * (1970 - 1904) + 17); // 17 leap years.
	std::chrono::time_point<std::chrono::steady_clock> BootTime;




	std::string extension(const std::string &s)
	{
		std::string tmp;
		int pos;

		pos = s.find_last_of("./:");

		if (pos == s.npos) return tmp;		
		if (s[pos++] != '.') return tmp;
		if (pos >= s.length()) return tmp;

		tmp = s.substr(pos);

		std::transform(tmp.begin(), tmp.end(), tmp.begin(), 
			[](char c) { return tolower(c); }
		);

		return tmp;
	}

	std::string basename(const std::string &s)
	{
		int pos = s.find_last_of("/:");
		if (pos == s.npos) return s;

		return s.substr(pos + 1);
	}



}

namespace OS
{

	bool Init()
	{
		BootTime = std::chrono::steady_clock::now();
		memoryWriteLong(0, MacOS::Ticks); // 0 ticks since boot.

		//std::chrono::system_clock::now(), to_time_t
		// set global variable Time to the current time
		time_t now = UnixToMac(::time(NULL));

		memoryWriteLong(now, MacOS::TimeLM);

		return true;
	}


	// known text file extensions
	bool IsTextFile(const std::string &s)
	{
		std::string ext = extension(s);
		if (ext.empty()) return false;

		char c = ext[0];
		switch(c)
		{
			case 'a':
				if (ext == "aii") // assembler
					return true;
				if (ext == "asm")
					return true;
				break;

			case 'c':
				if (ext == "c")
					return true;
				break;

			case 'e':
				if (ext == "equ") // asm iigs include file.
					return true;
				break;
			
			case 'i':
				if (ext == "i") // asmiigs include file
					return true;
				if (ext == "inc")
					return true;
				break;

			case 'h':
				if (ext == "h") // c header 
					return true;
				break;

			case 'l':
				if (ext == "lst") // asm iigs listing
					return true;
				break;

			case 'm':
				if (ext == "macros")
					return true;
				break;

			case 'p':
				if (ext == "p") // pascal
					return true;
				if (ext == "pas") // pascal
					return true;
				if (ext == "pii") // pascal
					return true;
				break;

			case 'r':
				if (ext == "r")
					return true;
				if (ext == "rez")
					return true;
				if (ext == "rii") // rez
					return true;
				break;

			case 's':
				if (ext == "src") // asm equates
					return true;
				break;

		}

		// check for e16.xxxx or m16.xxxx
		ext = basename(s);
		if (ext.length() > 4)
		{
			switch (ext[0])
			{
				case 'm':
				case 'M':
				case 'e':
				case 'E':
					if (!strncmp("16.", ext.c_str() + 1, 3))
						return true;
					break;
			}
		}

		return false;
	}

	// known binary file extensions
	bool IsBinaryFile(const std::string &s)
	{

		// first -- check for a finder info extension.
		{
			uint8_t buffer[32];
			int rv;

			rv = ::getxattr(s.c_str(), XATTR_FINDERINFO_NAME, buffer, 32, 0, 0);

			if (rv >= 8 && ::memcmp(buffer + 4, "pdos",4) == 0)
			{
				// Bx__ ?
				if (buffer[0] == 'B' && buffer[2] == ' ' && buffer[3] == ' ')
					return true;

				// "p" $uv $wx $yz 
				if (buffer[0] == 'p')
				{
					uint8_t fileType = buffer[1];
					//uint16_t auxType = buffer[2] | (buffer[3] << 8);

					if (fileType >= 0xb1 && fileType <= 0xbf)
						return true;
				}
			}
		}

		std::string ext = extension(s);
		if (ext.empty()) return false;

		char c = ext[0];
		switch(c)
		{
			case 'l':
				if (ext == "lib")
					return true;
				break;

			case 'o':
				if (ext == "o")
					return true;
				if (ext == "obj")
					return true;

				break;
		}

		return false;
	}

	uint16_t Close(uint16_t trap)
	{
		uint32_t d0;
		uint32_t parm = cpuGetAReg(0);

		Log("%04x Close(%08x)\n", trap, parm);

		//uint32_t ioCompletion = memoryReadLong(parm + 12);
		uint16_t ioRefNum = memoryReadWord(parm + 24);


		int rv = OS::Internal::FDEntry::close(ioRefNum, true);
		if (rv < 0) d0 = errno_to_oserr(errno);
		else d0 = 0;

		memoryWriteWord(d0, parm + 16);
		return d0;
	}


	uint16_t Create(uint16_t trap)
	{
		uint32_t d0;

		uint32_t parm = cpuGetAReg(0);

		Log("%04x Create(%08x)\n", trap, parm);

		//uint32_t ioCompletion = memoryReadLong(parm + 12);
		uint32_t namePtr = memoryReadLong(parm + 18);

		//uint16_t ioVRefNum = memoryReadWord(parm + 22);
		//uint8_t ioFVersNum = memoryReadByte(parm + 26);

		std::string sname = ToolBox::ReadPString(namePtr, true);

		if (!sname.length())
		{
			memoryWriteWord(MacOS::bdNamErr, parm + 16);
			return MacOS::bdNamErr;
		}
		Log("     Create(%s)\n", sname.c_str());

		int fd;
		fd = ::open(sname.c_str(), O_WRONLY | O_CREAT | O_EXCL, 0666);

		if (fd < 0)
		{
			d0 = errno_to_oserr(errno);
		}
		else
		{
			::close(fd);
			d0 = 0;		
		}

		memoryWriteWord(d0, parm + 16);
		return d0;
	}


	uint16_t Open(uint16_t trap)
	{
		uint32_t d0;

		int fd;

		uint32_t parm = cpuGetAReg(0);


		Log("%04x Open(%08x)\n", trap, parm);

		uint32_t namePtr = memoryReadLong(parm + 18);

		uint8_t ioPermission = memoryReadByte(parm + 27); 

		std::string sname = ToolBox::ReadPString(namePtr, true);

		fd = Internal::FDEntry::open(sname, ioPermission, false);
		d0 = fd < 0 ? fd : 0;
		if (fd >= 0)
		{
			memoryWriteWord(fd, parm + 24);				
		}

		memoryWriteWord(d0, parm + 16);
		return d0;
	}

	uint16_t OpenRF(uint16_t trap)
	{
		uint32_t d0;

		int fd;

		uint32_t parm = cpuGetAReg(0);


		Log("%04x OpenRF(%08x)\n", trap, parm);

		uint32_t namePtr = memoryReadLong(parm + 18);

		uint8_t ioPermission = memoryReadByte(parm + 27); 

		std::string sname = ToolBox::ReadPString(namePtr, true);

		fd = Internal::FDEntry::open(sname, ioPermission, true);
		d0 = fd < 0 ? fd : 0;
		if (fd >= 0)
		{
			memoryWriteWord(fd, parm + 24);				
		}

		memoryWriteWord(d0, parm + 16);
		return d0;
	}


	uint16_t Read(uint16_t trap)
	{
		uint32_t d0;
		int32_t pos;
		uint32_t parm = cpuGetAReg(0);

		Log("%04x Read(%08x)\n", trap, parm);

		//uint32_t ioCompletion = memoryReadLong(parm + 12);

		uint16_t ioRefNum = memoryReadWord(parm + 24);
		uint32_t ioBuffer = memoryReadLong(parm + 32);
		int32_t ioReqCount = memoryReadLong(parm + 36);
		uint16_t ioPosMode = memoryReadWord(parm + 44);
		int32_t ioPosOffset = memoryReadLong(parm + 46);

		if (ioReqCount < 0)
		{
			d0 = MacOS::paramErr;
			memoryWriteWord(d0, parm + 16);
			return d0;
		}

		pos = Internal::mac_seek(ioRefNum, ioPosMode, ioPosOffset);
		if (pos < 0)
		{
			d0 = pos;
			pos = 0;

			memoryWriteLong(pos, parm + 46); // new offset.
			memoryWriteWord(d0, parm + 16);
			return d0;
		}

		Log("     read(%04x, %08x, %08x)\n", ioRefNum, ioBuffer, ioReqCount);
		ssize_t count = OS::Internal::FDEntry::read(ioRefNum, memoryPointer(ioBuffer), ioReqCount);
		if (count >= 0)
		{
			d0 = 0;
			pos += count;
			memoryWriteLong(count, parm + 40);
		} 

		if (count == 0)
		{
			d0 = MacOS::eofErr;
		}
		if (count < 0)
		{
			d0 = errno_to_oserr(errno);
		}

		memoryWriteLong(pos, parm + 46); // new offset.
		memoryWriteWord(d0, parm + 16);
		return d0;

	}


	uint16_t Write(uint16_t trap)
	{
		uint32_t d0;
		int32_t pos;
		uint32_t parm = cpuGetAReg(0);

		Log("%04x Write(%08x)\n", trap, parm);

		//uint32_t ioCompletion = memoryReadLong(parm + 12);

		uint16_t ioRefNum = memoryReadWord(parm + 24);
		uint32_t ioBuffer = memoryReadLong(parm + 32);
		int32_t ioReqCount = memoryReadLong(parm + 36);
		uint16_t ioPosMode = memoryReadWord(parm + 44);
		int32_t ioPosOffset = memoryReadLong(parm + 46);

		if (ioReqCount < 0)
		{
			d0 = MacOS::paramErr;
			memoryWriteWord(d0, parm + 16);
			return d0;
		}

		pos = Internal::mac_seek(ioRefNum, ioPosMode, ioPosOffset);
		if (pos < 0)
		{
			d0 = pos;
			pos = 0;

			memoryWriteLong(pos, parm + 46); // new offset.
			memoryWriteWord(d0, parm + 16);
			return d0;

		}

		Log("     write(%04x, %08x, %08x)\n", ioRefNum, ioBuffer, ioReqCount);
		ssize_t count = OS::Internal::FDEntry::write(ioRefNum, memoryPointer(ioBuffer), ioReqCount);
		if (count >= 0)
		{
			d0 = 0;
			pos += count;
			memoryWriteLong(count, parm + 40);
		} 

		if (count < 0)
		{
			d0 = errno_to_oserr(errno);
		}

		memoryWriteLong(pos, parm + 46); // new offset.
		memoryWriteWord(d0, parm + 16);
		return d0;

	}




	uint16_t Delete(uint16_t trap)
	{
		uint32_t d0;

		uint32_t parm = cpuGetAReg(0);

		Log("%04x Delete(%08x)\n", trap, parm);

		//uint32_t ioCompletion = memoryReadLong(parm + 12);
		uint32_t namePtr = memoryReadLong(parm + 18);

		//uint16_t ioVRefNum = memoryReadWord(parm + 22);
		//uint8_t ioFVersNum = memoryReadByte(parm + 26);

		std::string sname = ToolBox::ReadPString(namePtr, true);

		if (!sname.length())
		{
			memoryWriteWord(MacOS::bdNamErr, parm + 16);
			return MacOS::bdNamErr;
		}
		Log("     Delete(%s)\n", sname.c_str());

		int ok = ::unlink(sname.c_str());
		if (ok < 0)
			d0 = errno_to_oserr(errno);
		else
			d0 = 0;

		memoryWriteWord(d0, parm + 16);
		return d0;
	}

	uint16_t GetEOF(uint16_t trap)
	{
		uint32_t d0;
		size_t size;

		uint32_t parm = cpuGetAReg(0);

		Log("%04x GetEOF(%08x)\n", trap, parm);

		//uint32_t ioCompletion = memoryReadLong(parm + 12);
		uint16_t ioRefNum = memoryReadWord(parm + 24);

		struct stat st;

		if (::fstat(ioRefNum, &st) < 0)
		{
			d0 = errno_to_oserr(errno);
			size = 0;
		}
		else
		{
			d0 = 0;
			size = st.st_size;
		}

		memoryWriteWord(d0, parm + 16);
		memoryWriteLong(size, parm + 28);

		return d0;
	}

	uint16_t SetEOF(uint16_t trap)
	{
		uint32_t d0;

		uint32_t parm = cpuGetAReg(0);

		Log("%04x SetEOF(%08x)\n", trap, parm);

		//uint32_t ioCompletion = memoryReadLong(parm + 12);
		uint16_t ioRefNum = memoryReadWord(parm + 24);
		uint32_t ioMisc = memoryReadLong(parm + 28);

		int rv = ::ftruncate(ioRefNum, ioMisc);

		d0 = rv < 0  ? errno_to_oserr(errno) : 0;

		memoryWriteWord(d0, parm + 16);
		return d0;
	}

	uint16_t GetFPos(uint16_t trap)
	{
		uint32_t d0;

		uint32_t parm = cpuGetAReg(0);

		Log("%04x GetFPos(%08x)\n", trap, parm);

		//uint32_t ioCompletion = memoryReadLong(parm + 12);
		uint16_t ioRefNum = memoryReadWord(parm + 24);

		int rv = ::lseek(ioRefNum, 0, SEEK_CUR);
		if (rv < 0)
		{
			d0 = errno_to_oserr(errno);
		}
		else
		{
			memoryWriteLong(0, parm + 36); // ioReqCount
			memoryWriteLong(0, parm + 40); // ioActCount
			memoryWriteWord(0, parm + 44); // ioPosMode
			memoryWriteLong(rv, parm + 46); // ioPosOffset
			d0 = 0;
		}

		memoryWriteWord(d0, parm + 16);
		return d0;
	}

	uint16_t SetFPos(uint16_t trap)
	{
		uint32_t d0;

		uint32_t parm = cpuGetAReg(0);

		Log("%04x SetFPos(%08x)\n", trap, parm);

		//uint32_t ioCompletion = memoryReadLong(parm + 12);
		uint16_t ioRefNum = memoryReadWord(parm + 24);
		uint16_t ioPosMode = memoryReadWord(parm + 44);
		int32_t ioPosOffset = memoryReadLong(parm + 46);


		ioPosOffset = Internal::mac_seek(ioRefNum, ioPosMode, ioPosOffset);
		d0 = 0;
		if (ioPosOffset < 0)
		{
			d0 = ioPosOffset;
			ioPosOffset = 0;
		}

		memoryWriteLong(ioPosOffset, parm + 46); // new offset.
		memoryWriteWord(d0, parm + 16);
		return d0;
	}

	uint16_t FlushVol(uint16_t trap)
	{
		uint32_t parm = cpuGetAReg(0);

		Log("%04x FlushVol(%08x)\n", trap, parm);		

		// ioResult
		memoryWriteWord(0, parm + 16);

		return 0;
	}

	// return the name of the default volume.
	// this does not translate well.
	uint16_t GetVol(uint16_t trap)
	{
		uint32_t parm = cpuGetAReg(0);

		Log("%04x GetVol(%08x)\n", trap, parm);


		//uint32_t ioCompletion = memoryReadLong(parm + 12);
		uint32_t namePtr = memoryReadLong(parm + 18);

		// ioResult
		memoryWriteWord(0, parm + 16);
		// ioVRefNum
		memoryWriteWord(0, parm + 22);

		std::string tmp = "MacOS";
		ToolBox::WritePString(namePtr, tmp);

		return 0;
	}


	uint16_t SetVol(uint16_t trap)
	{

		enum { // WDPBRec
			_qLink = 0,
			_qType = 4,
			_ioTrap = 6,
			_ioCmdAddr = 8,
			_ioCompletion = 12,
			_ioResult = 16,
			_ioNamePtr = 18,
			_ioVRefNum = 22,

			_ioWDDirID = 48,
		};

		uint16_t d0;

		uint32_t parm = cpuGetAReg(0);

		Log("%04x SetVol(%08x)\n", trap, parm);


		uint32_t ioNamePtr = memoryReadLong(parm + _ioNamePtr);
		uint32_t ioVRefNum = memoryReadWord(parm + _ioVRefNum);
		uint32_t ioWDDirID = trap & 0x0200 ? memoryReadLong(parm + _ioWDDirID) : 0;


		std::string name = ToolBox::ReadPString(ioNamePtr);

		Log("    SetVol(%s, %d, %d)\n", name.c_str(), ioVRefNum, ioWDDirID);

		if (name.length() || ioVRefNum || ioWDDirID)
		{
			fprintf(stderr, "SetVol(%s, %d, %d) is not supported yet.\n", name.c_str(), ioVRefNum, ioWDDirID);
			exit(1);
		}
		d0 = 0;
		memoryWriteWord(d0, parm + _ioResult);
		return d0;
	}


#if 0
	uint16_t GetFileInfo(uint16_t trap)
	{

		uint16_t d0;

		uint32_t parm = cpuGetAReg(0);

		Log("%04x GetFileInfo(%08x)\n", trap, parm);

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

			Log("     GetFileInfo(%s)\n", sname.c_str());

			// todo -- how are absolute, relative, etc paths handled...


			struct stat st;

			if (::stat(sname.c_str(), &st) < 0)
			{
				d0 = errno_to_oserr(errno);

				memoryWriteWord(d0, parm + 16);
				return d0;
			}


			Internal::GetFinderInfo(sname, memoryPointer(parm + 32), false); 


			// file reference number
			memoryWriteWord(0, parm + 24);
			// file attributes
			memoryWriteByte(0, parm + 30);
			// version (unused)
			memoryWriteByte(0, parm + 31);

			// file id
			memoryWriteLong(0, parm + 48);


			// file size
			memoryWriteWord(0, parm + 52);
			memoryWriteLong(st.st_size, parm + 54);
			memoryWriteLong(st.st_size, parm + 58);

			// create date.
			memoryWriteLong(UnixToMac(st.st_birthtime), parm + 72);
			memoryWriteLong(UnixToMac(st.st_mtime), parm + 76);

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

		Log("%04x SetFileInfo(%08x)\n", trap, parm);

		//uint32_t ioCompletion = memoryReadLong(parm + 12);
		uint32_t ioNamePtr = memoryReadLong(parm + 18);
		//uint16_t ioVRefNum = memoryReadWord(parm + 22);
		//uint8_t ioFVersNum = memoryReadByte(parm + 26);
		//int16_t ioFDirIndex = memoryReadWord(parm + 28);

		// + 32 = finder data - 16 bytes.

		//uint32_t ioFlCrDat = memoryReadLong(parm + 72);
		//uint32_t ioFlMdDat = memoryReadLong(parm + 76);

		// currently, only sets finder info.

		if (!ioNamePtr)
		{
			memoryWriteWord(MacOS::bdNamErr, parm + 16);
			return MacOS::bdNamErr;
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
				memoryWriteWord(d0, parm + 16);
				return d0; 
			}


		}

		d0 = Internal::SetFinderInfo(sname, memoryPointer(parm + 32), false);

		memoryWriteWord(d0, parm + 16);
		return d0;
	}
#endif

	#pragma mark - String Utilities

	uint16_t CmpString(uint16_t trap)
	{

		/* 
		 * on entry:
		 * A0 Pointer to first character of first string
		 * A1 Pointer to first character of second string
		 * D0 (high) length of first string 
		 * D0 (low) length of second string
		 *
		 * on exit:
		 * D0 0 if strings equal, 1 if strings not equal.
		 *
		 */

		bool caseSens = trap & (1 << 9);
		//bool diacSens = trap & (1 << 10); // ignore for now...

		uint32_t aStr = cpuGetAReg(0);
		uint32_t bStr = cpuGetAReg(1);

		uint32_t length = cpuGetDReg(0);

		uint32_t aLen = (length >> 16);
		uint32_t bLen = (length & 0xffff);

		std::string a = ToolBox::ReadString(aStr, aLen);
		std::string b = ToolBox::ReadString(bStr, bLen);

		Log("%04x CmpString(%s, %s)\n", trap, a.c_str(), b.c_str());

		if (aLen != bLen) return 1; // different length...
		if (aStr == bStr) return 0; // same ptr...

		bool eq;
		eq = std::equal(
			a.begin(), 
			a.end(), 
			b.begin(), 
			[caseSens](char a, char b){
				if (!caseSens)
				{
					a = toupper(a);
					b = toupper(b);
				}
				return a == b;
			}
		);

		return eq ? 0 : 1;
	}


	#pragma mark - Time Utilities

	time_t UnixToMac(time_t t)
	{
		return t + EpochAdjust;
	}

	time_t MacToUnix(time_t t)
	{
		return t - EpochAdjust;
	}

	uint16_t ReadDateTime(uint16_t trap)
	{

		/* 
		 * on entry:
		 * A0 Pointer to long word secs
		 *
		 * on exit:
		 * A0 pointer to long word secs
		 * D0 Result code
		 *
		 */

		time_t now;

		uint32_t secsPtr = cpuGetAReg(0);

		Log("%04x ReadDateTime(%08x)\n", trap, secsPtr);

		now = ::time(NULL);


		now = UnixToMac(now);
		if (secsPtr) memoryWriteLong(now, secsPtr);

		// also set global variable Time.
		memoryWriteLong(now, MacOS::TimeLM);
		return 0;
	}

	uint16_t SecondsToDate(uint16_t trap)
	{
		/* 
		 * on entry:
		 * D0 Seconds since midnight, January 1, 1904
		 * A0 pointer to date-time record
		 *
		 * on exit:
		 * D0 Result code
		 *
		 */

		uint32_t s = cpuGetDReg(0);
		uint32_t dtPtr = cpuGetAReg(0);

		Log("%04x SecondsToDate(%08x, %08x)\n", trap, s, dtPtr);


		if (dtPtr)
		{
			struct tm *tm;
			time_t t;
			t = MacToUnix(s);

			tm = ::localtime(&t);

			memoryWriteWord(tm->tm_year + 1900, dtPtr + 0);
			memoryWriteWord(tm->tm_mon + 1, dtPtr + 2);
			memoryWriteWord(tm->tm_mday, dtPtr + 4);
			memoryWriteWord(tm->tm_hour, dtPtr + 6);
			memoryWriteWord(tm->tm_min, dtPtr + 8);
			memoryWriteWord(tm->tm_sec, dtPtr + 10);
			memoryWriteWord(tm->tm_wday + 1, dtPtr + 12);
		}

		return 0;
	}

	uint16_t TickCount(uint16_t trap)
	{
		typedef std::chrono::duration<int32_t, std::ratio<1, 60> > ticks;

		Log("%04x TickCount()\n", trap);

		auto now = std::chrono::steady_clock::now();

		uint32_t t = std::chrono::duration_cast< ticks >(now - BootTime).count();


		// global Ticks
		memoryWriteLong(t, MacOS::Ticks);
		ToolReturn<4>(-1, t);

		return 0;
	}

	uint16_t Pack6(uint16_t trap)
	{
		char buffer[256];
		int length;
		std::string out;

		uint32_t dateTime;
		uint16_t flag;
		uint32_t result;
		uint16_t selector;

		// todo -- variable number of args.  Pop selector, act from that.

		StackFrame<12>(dateTime, flag, result, selector);

		Log("%04x Pack6(%08x, %04x, %08x, %04x)\n", 
			trap, dateTime, flag, result, selector);

		struct tm *tm;
		time_t t;
		t = MacToUnix(dateTime);

		tm = ::localtime(&t);

		if (selector == 0x00)
		{
			// void IUDateString(long dateTime,DateForm longFlag,Str255 result)
			// DateForm doesn't seem to do anything.

			// not strictly correct -- uses %d/%d/%2d form.
			length = std::strftime(buffer, sizeof(buffer), "%m/%d/%y", tm);
			out.assign(buffer, length);
		}
		else if (selector == 0x02)
		{
			// void IUTimeString(long dateTime,Boolean wantSeconds,Str255 result)
			// output: 12:00:00 AM or 12:00 AM

			length = std::strftime(buffer, sizeof(buffer), flag ? "%I:%M:%S %p" : "%I:%M %p", tm);

			out.assign(buffer, length);
		}
		else
		{
			fprintf(stderr, "Pack6: selector %04x not supported\n", selector);
			exit(1);
		}

		ToolBox::WritePString(result, out);
		return 0;
	}


	#pragma mark - Trap Manager

	uint16_t GetToolTrapAddress(uint16_t trap)
	{
		/* 
		 * on entry:
		 * D0 trap number
		 *
		 * on exit:
		 * A0 Address of patch
		 *
		 */
		uint16_t trapNumber = cpuGetDReg(0);
		Log("%04x GetToolTrapAddress($%04x)\n", trap, trapNumber);

		cpuSetAReg(0, 0);
		return MacOS::dsCoreErr;
	}

	uint16_t GetOSTrapAddress(uint16_t trap)
	{
		/* 
		 * on entry:
		 * D0 trap number
		 *
		 * on exit:
		 * A0 Address of patch
		 *
		 */
		uint16_t trapNumber = cpuGetDReg(0);
		Log("%04x GetOSTrapAddress($%04x)\n", trap, trapNumber);

		cpuSetAReg(0, 0);
		return MacOS::dsCoreErr;
	}



}