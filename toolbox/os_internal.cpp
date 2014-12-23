/*
 * Copyright (c) 2013, Kelvin W Sherlock
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: 
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "os_internal.h"
#include "os.h"
#include "toolbox.h"

#include <macos/errors.h>

#include <stdexcept>
#include <cerrno>

#include <unistd.h>
#include <fcntl.h>
#include <sys/xattr.h>
#include <sys/paths.h>

#include <machine/endian.h>

using ToolBox::Log;
using MacOS::macos_error_from_errno;

namespace OS { namespace Internal {




	/*
     
     tech note PT515
     ProDOS -> Macintosh conversion
     
     ProDOS             Macintosh
     Filetype    Auxtype    Creator    Filetype
     $00          $0000     'pdos'     'BINA'
     $B0 (SRC)    (any)     'pdos'     'TEXT'
     $04 (TXT)    $0000     'pdos'     'TEXT'
     $FF (SYS)    (any)     'pdos'     'PSYS'
     $B3 (S16)    (any)     'pdos'     'PS16'
     $uv          $wxyz     'pdos'     'p' $uv $wx $yz
     
     Programmer's Reference for System 6.0:
     
     ProDOS Macintosh 
     File Type Auxiliary Type Creator Type File Type 
     $00        $0000           “pdos”  “BINA” 
     $04 (TXT)  $0000           “pdos”  “TEXT” 
     $FF (SYS)  (any)           “pdos”  “PSYS” 
     $B3 (S16)  $DByz           “pdos”  “p” $B3 $DB $yz 
     $B3 (S16)  (any)           “pdos”  “PS16” 
     $D7        $0000           “pdos”  “MIDI” 
     $D8        $0000           “pdos”  “AIFF” 
     $D8        $0001           “pdos”  “AIFC” 
     $E0        $0005           “dCpy”  “dImg” 
     $FF (SYS)  (any)           “pdos”  “PSYS” 
     $uv        $wxyz           “pdos”  “p” $uv $wx $yz 
     
	
	  mpw standard:
     $uv        (any)          "pdos"  printf("%02x  ",$uv)

     */

	uint16_t GetFinderInfo(const std::string &pathName, void *info, bool extended)
	{
		// todo -- move to separate function? used in multiple places.
		uint8_t buffer[32];
		std::memset(buffer, 0, sizeof(buffer));
		int rv;

		rv = ::getxattr(pathName.c_str(), XATTR_FINDERINFO_NAME, buffer, 32, 0, 0);

		if (rv < 0)
		{
			switch (errno)
			{
				case ENOENT:
				case EACCES:
					return macos_error_from_errno();
			}

			// check for prodos ftype/auxtype
			uint8_t ftype;
			uint16_t atype;

			int rv1, rv2;

			rv1 = ::getxattr(pathName.c_str(), "prodos.FileType", &ftype, 1, 0, 0);
			rv2 = ::getxattr(pathName.c_str(), "prodos.AuxType", &atype, 2, 0, 0);

			if (rv1 == 1 && rv2 == 2)
			{
				#if BYTE_ORDER == BIG_ENDIAN
				ftype = (ftype >> 8) | (ftype << 8)
				#endif

				char tmp[8] = {
					'p', ' ', ' ', ' ', 
					'p', 'd', 'o', 's'
				};
				tmp[1] = (char)ftype;
				tmp[2] = (char)((atype >> 8) & 0xff);
				tmp[3] = (char)(atype & 0xff);

				switch (atype)
				{
					case 0x00:
						std::memcpy(tmp, "BINA", 4);
						break;

					case 0x04:
					case 0xb0:
						std::memcpy(tmp, "TEXT", 4);
						break;

					case 0xff:
						std::memcpy(tmp, "PSYS", 4);
						break;

					case 0xd7:
						std::memcpy(tmp, "MIDI", 4);
						break;

					case 0xd8:
						switch (atype)
						{
							case 0x0000:
								std::memcpy(tmp, "AIFF", 4);
								break;
							case 0x0001:
								std::memcpy(tmp, "AIFC", 4);
								break;
						}
						break;

					case 0xe0:
						switch (atype)
						{
							case 0x0005:
								std::memcpy(tmp, "dImgdCpy", 8);
								break;

						}

					case 0xb3:
						if (atype == 0)
						{
							std::memcpy(tmp, "PS16", 4); // verify dumpobj.
							break;
						}

				}
				std::memcpy(buffer, tmp, 8);
			}
		}

		// override for source files.
		// TODO -- only override if missing?
		if (IsTextFile(pathName))
		{
			std::memcpy(buffer, "TEXTMPS ", 8);
		}	


		// convert pdos types...
		if (::memcmp(buffer + 4, "pdos", 4) == 0)
		{
			// mpw expects 'xx  ' where
			// xx are the ascii-encode hex value of the file type.
			// the hfs fst uses 'p' ftype8 auxtype16
			
			// todo -- but only if auxtype is $0000 ??
			if (buffer[0] == 'p')
			{
				static char Hex[] = "0123456789ABCDEF";

				uint8_t ftype = buffer[1];
				buffer[0] = Hex[ftype >> 4];
				buffer[1] = Hex[ftype & 0x0f];
				buffer[2] = ' ';
				buffer[3] = ' ';
			}
		}

		if (extended)
			std::memcpy(info, buffer, 32);
		else
			std::memcpy(info, buffer, 16);
		return 0;
	}

	unsigned tox(unsigned x)
	{
		if (x >= '0' && x <= '9') return x - '0';
		if (x >= 'a' && x <= 'f') return x - 'a' + 10;
		if (x >= 'A' && x <= 'F') return x - 'A' + 10;
		return 0;
	}

	uint16_t SetFinderInfo(const std::string &pathName, void *info, bool extended)
	{
		uint8_t buffer[32];
		std::memset(buffer, 0, sizeof(buffer));
		int rv;

		if (extended)
		{
			std::memmove(buffer, info, 32);

		}
		else
		{
			rv = ::getxattr(pathName.c_str(), XATTR_FINDERINFO_NAME, buffer, 32, 0, 0);

			if (rv < 0)
			{
				switch (errno)
				{
					case ENOENT:
					case EACCES:
						return macos_error_from_errno();
				}
			}
			std::memmove(buffer, info, 16);

		}

		// convert pdos types.
		if (::memcmp(buffer + 2, "  pdos", 6) == 0)
		{
			unsigned a = buffer[0];
			unsigned b = buffer[1];

			if (isxdigit(a) && isxdigit(b))
			{
				buffer[0] = 'p';
				buffer[1] = (tox(a) << 4) | tox(b);
				buffer[2] = 0;
				buffer[3] = 0;
			}
		}

		rv = ::setxattr(pathName.c_str(), XATTR_FINDERINFO_NAME, buffer, 32, 0, 0);
		if (rv < 0) return macos_error_from_errno();

		return 0;
	}


	int32_t mac_seek(uint16_t refNum, uint16_t mode, int32_t offset)
	{
		off_t rv;
		switch (mode & 0x03)
		{
		case OS::fsAtMark:
			mode = SEEK_CUR;
			offset = 0;
			break;
		case OS::fsFromStart:
			mode = SEEK_SET;
			break;
		case OS::fsFromLEOF:
			mode = SEEK_END;
			break;
		case OS::fsFromMark:
			mode = SEEK_CUR;
			break;
		}

		rv = ::lseek(refNum, offset, mode);
		if (rv < 0) return macos_error_from_errno();
		return rv;
	}

	//std::deque<FDEntry> FDTable;

	std::deque<FDEntry> FDEntry::FDTable;
	FDEntry& FDEntry::allocate(int fd)
	{
		std::string noname;

		return allocate(fd, noname);
	}

	FDEntry& FDEntry::allocate(int fd, std::string &&filename)
	{
		if (fd < 0) throw std::out_of_range("Invalid FD");

		if (FDTable.size() <= fd)
			FDTable.resize(fd + 1);

		auto &e = FDTable[fd];
		e.refcount = 1;
		e.text = false;
		e.resource = false;
		e.filename = std::move(filename);
		return e;
	}

	FDEntry& FDEntry::allocate(int fd, const std::string &filename)
	{
		if (fd < 0) throw std::out_of_range("Invalid FD");

		if (FDTable.size() <= fd)
			FDTable.resize(fd + 1);

		auto &e = FDTable[fd];
		e.refcount = 1;
		e.text = false;
		e.resource = false;
		e.filename = filename;
		return e;
	}


	int FDEntry::close(int fd, bool force)
	{
		if (fd < 0 || fd >= FDTable.size())
		{
			errno = EBADF;
			return -1;
		}
		auto &e = FDTable[fd];
		if (!e.refcount)
		{
			errno = EBADF;
			return -1;
		}

		if (--e.refcount == 0 || force)
		{
			e.refcount = 0;
			return ::close(fd);
		}
		return 0;
	}


	ssize_t FDEntry::read(int fd, void *buffer, size_t count)
	{
		if (fd < 0 || fd >= FDTable.size())
		{
			errno = EBADF;
			return -1;
		}

		auto const &e = FDTable[fd];
		if (!e.refcount)
		{
			errno = EBADF;
			return -1;
		}

		// hmm... keep a current seek position?

		ssize_t size;
		if (e.text)
		{
			std::unique_ptr<uint8_t[]> trbuffer(new uint8_t[count]);

			size = ::read(fd, trbuffer.get(), count);

			if (size > 0)
			{
				std::transform(trbuffer.get(), trbuffer.get() + size, (uint8_t *)buffer, 
					[](uint8_t c) { return c == '\n' ? '\r' : c; }
				);
			}
		}
		else
		{
			size = ::read(fd, buffer, count);
		}
		return size;
	}

	ssize_t FDEntry::write(int fd, const void *buffer, size_t count)
	{
		if (fd < 0 || fd >= FDTable.size())
		{
			errno = EBADF;
			return -1;
		}

		auto const &e = FDTable[fd];
		if (!e.refcount)
		{
			errno = EBADF;
			return -1;
		}

		// hmm... keep a current seek position?

		ssize_t size;
		if (e.text)
		{
			std::unique_ptr<uint8_t[]> trbuffer(new uint8_t[count]);

			if (count > 0)
			{
				std::transform((const uint8_t *)buffer, (const uint8_t *)buffer + count, trbuffer.get(), 
					[](uint8_t c) { return c == '\r' ? '\n' : c; }
				);
			}

			size = ::write(fd, trbuffer.get(), count);
		}
		else
		{
			size = ::write(fd, buffer, count);
		}
		return size;
	}


	int FDEntry::open(const std::string &filename, int ioPermission, int fork)
	{
		int fd;

		if (filename.empty()) return MacOS::bdNamErr;

		int access = 0;
		if (ioPermission != 0x33) ioPermission &= ~0x30; // drop deny read/write
		switch(ioPermission)
		{
			case 0x33:
				// PBHOpenDeny exclusive access.
				access = O_RDWR | O_CREAT; // | O_EXCL;
				break;

			case fsWrPerm:
			case fsRdWrPerm:
			case fsRdWrShPerm:
			case fsCurPerm:
				access = O_RDWR;
				break;
			case fsRdPerm:
				access = O_RDONLY;
				break;
			default:
				return MacOS::paramErr;
				break;
		}

		std::string xname = filename;
		if (fork) 
			xname.append(_PATH_RSRCFORKSPEC);

		Log("     open(%s, %04x)\n", xname.c_str(), access);

		fd = ::open(xname.c_str(), access, 0666);
		if (fd < 0 && ioPermission == fsCurPerm && errno == EACCES)
		{
			fd = ::open(xname.c_str(), O_RDONLY);
		}

		if (fd < 0)
		{
			return macos_error_from_errno();
		}

		// allocate the fd entry

		auto &e = OS::Internal::FDEntry::allocate(fd, filename);
		e.resource = fork;
		e.text = fork ? false : IsTextFile(filename);

		return fd;
	}


} }
