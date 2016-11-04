/*
 * Copyright (c) 2016, Kelvin W Sherlock
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


#define XATTR_FINDERINFO_NAME "user.apple.FinderInfo"
#define XATTR_RESOURCEFORK_NAME "user.apple.ResourceFork"

#define XATTR_FILETYPE_NAME	"user.prodos.FileType"
#define XATTR_AUXTYPE_NAME	"user.prodos.AuxType"

#include <unistd.h>
#include <fcntl.h>
#include <sys/xattr.h>

#include "file.h"
#include <vector>

#include <macos/errors.h>

using MacOS::tool_return;
using MacOS::macos_error_from_errno;

namespace {

	class xattr_file final : public file {
	public:
		xattr_file(int fd): _fd(fd);
		~xattr_file(); 


		tool_return<size_t> read(void *out_buffer, size_t count) override;
		tool_return<size_t> write(void *in_buffer, size_t count) override;
		tool_return<size_t> get_mark() override;
		tool_return<void> set_mark(ssize_t new_mark) override;
		tool_return<size_t> get_eof() override;
		tool_return<void> set_eof(ssize_t new_eof) override;


	private:

		ssize_t read_rfork(std::vector<uint8_t> &buffer);

		int _fd = -1;
		off_t _displacement = 0;

	};

	xattr_file::~xattr_file() {
		close(_fd);
	}


	ssize_t xattr_file::read_rfork(std::vector<uint8_t> &buffer) {
		ssize_t eof = -1;
		buffer.clear();
		do {
			eof = fgetxattr(_fd, XATTR_RESOURCEFORK_NAME, NULL, 0);
			if (eof < 0) break;
			buffer.resize(eof);
			eof = fgetxattr(_fd, XATTR_RESOURCEFORK_NAME, buffer.data(), eof);
		} while (eof < 0 && errno == ERANGE);
		return eof;
	}

	tool_return<size_t> xattr_file::read(void *out_buffer, size_t count) {
		std::vector<uint8_t> buffer;
		ssize_t size;
		size = read_rfork(buffer);
		if (size < 0) size = 0;

		if (_displacement >= buffer.size()) return 0;

		count = std::min(count, buffer.size() - _displacement);
		std::copy_n(buffer.begin() + _displacement, count, (uint8_t *)out_buffer);
		_displacement += count;
		return count;
	}


	tool_return<size_t> xattr_file::write(const void *in_buffer, size_t count) {
		std::vector<uint8_t> buffer;
		ssize_t size;
		size = read_rfork(buffer);
		if (size < 0) size = 0;

		if (_displacement >= buffer.size()) {
			buffer.resize(_displacement, 0);
		}
		if (_displacement + count >= buffer.size()) {
			buffer.resize(_displacement + count, 0);
		}

		std::copy_n((const uint8_t *)in_buffer, count, buffer.begin() + _displacement);

		int ok = fsetxattr(_fd, XATTR_RESOURCEFORK_NAME, buffer.data(), buffer.size(), 0);
		if (ok < 0) return macos_error_from_errno();
		_displacement += count;
		return count;
	}



	tool_return<size_t> xattr_file::get_mark() {
		return _displacement;
	}

	tool_return<void> xattr_file::set_mark(ssize_t new_mark) {
		if (new_mark < 0) return paramErr;
		_displacement = new_mark;
	}

	tool_return<size_t> xattr_file::get_eof() {
		ssize_t eof = fgetxattr(_fd, XATTR_RESOURCEFORK_NAME, NULL, 0);
		if (eof < 0) eof = 0;
		return eof;
	}

	tool_return<void> xattr_file::set_eof(ssize_t new_eof) {

		if (new_eof < 0) return paramErr;

		std::vector<uint8_t> buffer;

		ssize_t eof = read_rfork(buffer);
		if (eof < 0) eof = 0;
		if (eof == new_eof) return noErr;

		buffer.resize(new_eof, 0);
		int ok = fsetxattr(_fd, XATTR_RESOURCEFORK_NAME, buffer->data(), buffer.size(), 0);
		if (ok < 0) return macos_error_from_errno();
		return noErr;
	}


}
 
namespace native {

 	tool_return<file_ptr> open_resource_fork(const std::string &path_name, int oflag) {

 		int fd = open(rname.c_str(), oflag);
 		if (fd < 0) return macos_error_from_errno();
 		return std::make_shared<file>(fd);
	}





	macos_error get_file_info(const std::string &path_name, file_info &fi)
	{
		struct stat st;

		if (stat(path_name.c_str(), &st) < 0)
			return macos_error_from_errno();

		fi.create_date = unix_to_mac(st.st_birthtime);
		fi.modify_date = unix_to_mac(st.st_mtime);
		fi.backup_date = 0;

		fi.attributes = 0;

		if (S_ISDIR(st.st_mode)) {
			fi.type = file_info::directory;
			fi.attributes = 1 << 4;

			int links = st.st_nlink - 2;
			if (links < 0) links = 0;
			if (links > 65535) links = 65535;

			fi.entry_count = links;
			return noErr;
		}

		// todo -- get actual block size instead of assuming 512.  oh well!
		
		fi.type = file_info::file;
		fi.data_logical_size = st.st_size;
		fi.data_physical_size = (st.st_size + 511) & ~511;
		fi.resource_physical_size = 0;
		fi.resource_logical_size = 0;

		get_finder_info(path_name, fi.finder_info);

		ssize_t rsize = rforksize(path_name);
		if (rsize > 0) {
			fi.resource_physical_size = rsize;
			fi.resource_logical_size = (rsize + 511) & ~511;
		}

		return noErr;
	}



}