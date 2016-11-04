
#include "file.h"

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#include <macos/errors.h>

#include <algorithm>

using MacOS::tool_return;
using MacOS::macos_error_from_errno;

namespace native {


tool_return<size_t> file::seek(ssize_t position, int whence) {
	if (interactive()) {
		if (position == 0) return 0;
		return MacOS::paramErr;
	}

	switch(whence) {
		case SEEK_SET:
			return set_mark(position);
			break;
		case SEEK_CUR: {
				auto current = get_mark();
				if (!current) return current;
				return set_mark(*current + position);
			}
			break;
		case SEEK_END: {
				auto eof = get_eof();
				if (!eof) return eof;
				return set_mark(*eof + position);
			}
			break;
		default:
			return MacOS::paramErr;
	}
}

bool file::interactive() {
	return false;
}


fd_file::fd_file(const std::string &s, int fd): file(s), _fd(fd) {
	_interactive = ::isatty(fd);
}

fd_file::~fd_file() {
	// don't close stdin/stdout/stderr.
	if (_fd >= 3) ::close(_fd);
};


namespace {

	ssize_t safe_read(int fd, void *buffer, size_t count) {
		ssize_t rv;
		do {
			rv = read(fd, buffer, count);
		} while (rv < 0 && errno == EINTR);
		return rv;
	}

	ssize_t safe_write(int fd, const void *buffer, size_t count) {
		ssize_t rv;
		do {
			rv = write(fd, buffer, count);
		} while (rv < 0 && errno == EINTR);
		return rv;
	}


}
tool_return<size_t> fd_file::read(void *buffer, size_t count) {


	if (_interactive || text) {

		std::unique_ptr<uint8_t[]> trbuffer(new uint8_t[count]);

		ssize_t ok = ::read(_fd, trbuffer.get(), count);
		if (ok < 0) return macos_error_from_errno();

		std::transform(trbuffer.get(), trbuffer.get() + ok, (uint8_t *)buffer,
			[](uint8_t c) { return c == '\n' ? '\r' : c; }
		);

		return ok;
	}


	ssize_t ok = ::read(_fd, buffer, count);
	if (ok < 0) return macos_error_from_errno();
	return ok;
}

tool_return<size_t> fd_file::write(const void *buffer, size_t count) {


	if (_interactive || text) {

		std::unique_ptr<uint8_t[]> trbuffer(new uint8_t[count]);

		std::transform((const uint8_t *)buffer, (const uint8_t *)buffer + count, trbuffer.get(),
			[](uint8_t c) { return c == '\r' ? '\n' : c; }
		);

		ssize_t ok = ::write(_fd, trbuffer.get(), count);
		if (ok < 0) return macos_error_from_errno();
		return ok;
	}

	ssize_t ok = ::write(_fd, buffer, count);
	if (ok < 0) return macos_error_from_errno();
	return ok;
}

tool_return<size_t> fd_file::seek(ssize_t position, int whence) {

	if (_interactive) {
		// seek set, cur, eof w/ position 0 ok.
		if (position == 0) return 0;
		return MacOS::paramErr;
	}

	off_t pos = ::lseek(_fd, position, whence);
	if (pos < 0) return macos_error_from_errno();
	return pos;
}


tool_return<size_t> fd_file::get_mark() {

	if (_interactive) return 0;

	off_t pos = ::lseek(_fd, 0, SEEK_CUR);
	if (pos < 0) return macos_error_from_errno();
	return pos;
}

tool_return<size_t> fd_file::set_mark(ssize_t new_mark) {

	if (_interactive) {
		if (new_mark == 0) return 0;
		return MacOS::paramErr;
	}

	off_t pos = ::lseek(_fd, new_mark, SEEK_SET);
	if (pos < 0) return macos_error_from_errno();
	return new_mark;
}

tool_return<size_t> fd_file::get_eof() {

	if (_interactive) return MacOS::paramErr;

	struct stat st;
	int ok = ::fstat(_fd, &st);
	if (ok < 0) return macos_error_from_errno();
	return st.st_size;
}

tool_return<size_t> fd_file::set_eof(ssize_t new_eof) {

	if (_interactive) return MacOS::paramErr;

	int ok = ::ftruncate(_fd, new_eof);
	if (ok < 0) return macos_error_from_errno();
	return new_eof;
}

bool fd_file::interactive() {
	return _interactive;
}




tool_return<size_t> empty_file::read(void *out_buffer, size_t count) {
	if (count == 0) return 0;
	return 0; // eofErr handled elsewhere.
}
tool_return<size_t> empty_file::write(const void *in_buffer, size_t count) {
	return MacOS::wrPermErr;
}
tool_return<size_t> empty_file::seek(ssize_t position, int whence) {
	if (position == 0) return 0;
	return MacOS::posErr;
}
tool_return<size_t> empty_file::get_mark() {
	return 0;
}
tool_return<size_t> empty_file::set_mark(ssize_t new_mark) {
	if (new_mark == 0) return 0;
	return MacOS::posErr;
}
tool_return<size_t> empty_file::get_eof() {
	return 0;
}

tool_return<size_t> empty_file::set_eof(ssize_t new_eof) {
	return MacOS::wrPermErr;
}




} // namespace native
