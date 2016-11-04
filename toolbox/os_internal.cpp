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
#include <vector>

#include <unistd.h>
#include <fcntl.h>

#include <native/native.h>
#include <native/file.h>

//#include <machine/endian.h>

using ToolBox::Log;
using MacOS::macos_error_from_errno;
using MacOS::tool_return;
using MacOS::macos_error;

namespace OS { namespace Internal {


	namespace {
		std::vector<native::file_ptr> file_table;
	}

	int open_file(native::file_ptr &&f) {

		int fd = 0;
		for (auto &ff : file_table) {
			if (!ff) {
				ff = std::move(f);
				return fd;
			}
			++fd;
		}

		file_table.emplace_back(std::move(f));
		return fd;
	}



	tool_return<int> open_file(const std::string &name, int resource, int ioPermission) {


		ioPermission &= 0x0f;
		int oflag;

		switch(ioPermission)
		{
			case fsWrPerm:
				oflag = O_RDWR;
				break;
			case fsRdWrPerm:
			case fsRdWrShPerm:
			case fsCurPerm:
				oflag = O_RDWR;
				break;
			case fsRdPerm:
				oflag = O_RDONLY;
				break;
			default:
				return MacOS::paramErr;
				break;
		}

		
		auto ff = native::open_fork(name, resource, oflag);
		if (ff.error() == MacOS::permErr && ioPermission == fsCurPerm)
			ff = native::open_fork(name, resource, O_RDONLY);


		if (!ff) return ff.error();

		auto f = std::move(ff).value();
		f->resource = resource;
		f->text = resource ? false : native::is_text_file(name);

		return open_file(std::move(f));
	}









	int close_file(int fd, bool force) {
		if (fd < 0 || fd >= file_table.size()) return -1;
		auto &f = file_table[fd];
		if (!f) return -1;
		if (force || --f->refcount == 0) f.reset();
		return 0;
	}

	native::file *find_file(int fd) {
		if (fd < 0 || fd >= file_table.size()) return nullptr;
		return file_table[fd].get();
	}


	macos_error remap_iopermission(int ioPermission) {
		switch(ioPermission & 0x0f)
		{
			case fsWrPerm:
			case fsRdWrPerm:
			case fsRdWrShPerm:
			case fsCurPerm:
				ioPermission = O_RDWR;
				return MacOS::noErr;
				break;
			case fsRdPerm:
				ioPermission = O_RDONLY;
				return MacOS::noErr;
				break;
			default:
				ioPermission = O_RDWR;
				return MacOS::paramErr;
				break;
		}
	}


	macos_error remap_seek(ssize_t &offset, int &mode) {

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

		return MacOS::noErr;
	}


} }
