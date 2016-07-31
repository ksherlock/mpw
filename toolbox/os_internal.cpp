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
#include <cstring>
#include <algorithm>
#include <memory>

#include <unistd.h>
#include <fcntl.h>
#include <sys/xattr.h>
#include <sys/attr.h>
#include <sys/paths.h>

#include <machine/endian.h>

using ToolBox::Log;
using MacOS::macos_error_from_errno;

namespace OS { namespace Internal {



	unsigned tox(unsigned x)
	{
		if (x >= '0' && x <= '9') return x - '0';
		if (x >= 'a' && x <= 'f') return x - 'a' + 10;
		if (x >= 'A' && x <= 'F') return x - 'A' + 10;
		return 0;
	}

#if 0
	uint16_t SetFinderInfo(const std::string &pathName, uint32_t fileType, uint32_t creator)
	{
		uint8_t buffer[32];
		std::memset(buffer, 0, sizeof(buffer));

		buffer[0] = fileType >> 24;
		buffer[1] = fileType >> 16;
		buffer[2] = fileType >> 8;
		buffer[3] = fileType >> 0;

		buffer[4] = creator >> 24;
		buffer[5] = creator >> 16;
		buffer[6] = creator >> 8;
		buffer[7] = creator >> 0;

		return SetFinderInfo(pathName, buffer, true);
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
#endif

#if 0
	uint16_t SetFileDates(const std::string &pathname, uint32_t createDate, uint32_t modificationDate, uint32_t backupDate)
	{
		// utimes(2) sets the access and mod times.
		// setattrlist sets the create and mod times (among other things)

		// this is kind of unfortunate in that a roundtrip will strip any nanoseconds, etc from the date/time.

		int rv;
		struct attrlist list;
		unsigned i = 0;

		timespec dates[3];


		memset(&list, 0, sizeof(list));
		memset(dates, 0, sizeof(dates));

		list.bitmapcount = ATTR_BIT_MAP_COUNT;
		list.commonattr  = 0;

		if (createDate)
		{
			dates[i++].tv_sec = MacToUnix(createDate);
			list.commonattr |= ATTR_CMN_CRTIME;
		}

		if (modificationDate)
		{
			dates[i++].tv_sec = MacToUnix(modificationDate);
			list.commonattr |= ATTR_CMN_MODTIME;
		}

		if (backupDate)
		{
			dates[i++].tv_sec = MacToUnix(backupDate);
			list.commonattr |= ATTR_CMN_BKUPTIME;
		}



		if (!i) return 0;

		rv = setattrlist(pathname.c_str(), &list, dates, i * sizeof(timespec), 0);

		if (rv < 0 && errno == ENOTSUP)
		{
			// try utimes.

			struct timeval tv[2];
			memset(tv, 0, sizeof(tv));

			if (modificationDate) {
				tv[1].tv_sec = MacToUnix(modificationDate);
				rv = utimes(pathname.c_str(), tv);
			}

		}

		if (rv < 0) return macos_error_from_errno();

		return 0;
	}
#endif

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

		// ignore the deny bits for now.
		switch(ioPermission & 0x0f)
		{
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

		fd = ::open(xname.c_str(), access);
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
