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

#include <native/native.h>

#include <machine/endian.h>

using ToolBox::Log;
using MacOS::macos_error_from_errno;

namespace OS { namespace Internal {



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

/*
		if (fork)
			xname.append(_PATH_RSRCFORKSPEC);
*/

		Log("     open(%s, %04x, %04x)\n", xname.c_str(), access, fork);


		fd = native::open_fork(xname, fork, access);

		if (fd < 0 && ioPermission == fsCurPerm && errno == EACCES)
			fd = native::open_fork(xname, fork, O_RDONLY);

		if (fd < 0)
			return macos_error_from_errno();

		// allocate the fd entry

		auto &e = OS::Internal::FDEntry::allocate(fd, filename);
		e.resource = fork;
		e.text = fork ? false : IsTextFile(filename);

		return fd;
	}


} }
