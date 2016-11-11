
#ifndef __native_file_h__
#define __native_file_h__

#include <memory>
#include <string>
#include <stddef.h>
#include <sys/types.h>

#include <macos/tool_return.h>

#ifdef _WIN32
typedef long ssize_t;
#endif

namespace native {

using MacOS::tool_return;

class file {
public:


	int refcount = 1;
	bool text = false;
	bool resource = false;

	std::string filename;

	virtual ~file() = default;
	virtual tool_return<size_t> read(void *out_buffer, size_t count) = 0;
	virtual tool_return<size_t> write(const void *in_buffer, size_t count) = 0;
	virtual tool_return<size_t> seek(ssize_t position, int whence);
	virtual tool_return<size_t> get_mark() = 0;
	virtual tool_return<size_t> set_mark(ssize_t new_mark) = 0;
	virtual tool_return<size_t> get_eof() = 0;
	virtual tool_return<size_t> set_eof(ssize_t new_eof) = 0;


	virtual bool interactive();

protected:
		file() = default;
		file(const std::string &s) : filename(s)
		{}
		file(std::string &&s) : filename(std::move(s))
		{}
};


class fd_file final : public file {
public:
	fd_file(const std::string &s, int fd);
	virtual ~fd_file();

	virtual tool_return<size_t> read(void *out_buffer, size_t count) override;
	virtual tool_return<size_t> write(const void *in_buffer, size_t count) override;
	virtual tool_return<size_t> seek(ssize_t position, int whence) override;
	virtual tool_return<size_t> get_mark() override;
	virtual tool_return<size_t> set_mark(ssize_t new_mark) override;
	virtual tool_return<size_t> get_eof() override;
	virtual tool_return<size_t> set_eof(ssize_t new_eof) override;
	virtual bool interactive() override;

private:
	int _fd;
	bool _interactive = false;
};


/* empty, read-only pseudo file (for empty resource forks?) */
class empty_file final : public file {
public:
	empty_file(const std::string &s) : file(s) {}

	virtual tool_return<size_t> read(void *out_buffer, size_t count) override;
	virtual tool_return<size_t> write(const void *in_buffer, size_t count) override;
	virtual tool_return<size_t> seek(ssize_t position, int whence) override;
	virtual tool_return<size_t> get_mark() override;
	virtual tool_return<size_t> set_mark(ssize_t new_mark) override;
	virtual tool_return<size_t> get_eof() override;
	virtual tool_return<size_t> set_eof(ssize_t new_eof) override;
};


typedef std::unique_ptr<file> file_ptr;
}
#endif
