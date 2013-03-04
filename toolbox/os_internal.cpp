#include "os_internal.h"
#include "os.h"
#include "toolbox.h"

#include <stdexcept>
#include <cerrno>

#include <unistd.h>
#include <fcntl.h>


namespace OS { namespace Internal {


	uint16_t errno_to_oserr(int xerrno)
	{
		using namespace ToolBox::Errors;
		
		switch (xerrno)
		{
			case 0: return 0;
			case EBADF: return rfNumErr;
			case EIO: return ioErr;
			case EACCES: return permErr;
			case ENOENT: return fnfErr;
			case ENOTDIR: return dirNFErr;
			case EISDIR: return notAFileErr;
			case ENOTSUP: return extFSErr;
			case EROFS: return wPrErr;

			case EEXIST: return dupFNErr;

			case EBUSY: return fBsyErr;

			case EDQUOT: return dskFulErr;
			case ENOSPC: return dskFulErr;


			default:
				return ioErr;
		}

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
		if (rv < 0) return errno_to_oserr(errno);
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


} }
