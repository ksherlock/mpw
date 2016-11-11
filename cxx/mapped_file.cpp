#include "mapped_file.h"
#include <memory>
#include <functional>
#include <system_error>

#include "unique_resource.h"

namespace {
	class defer {
	public:
		typedef std::function<void()> FX;
		defer() = default;

		defer(FX fx) : _fx(fx) {}
		defer(const defer &) = delete;
		defer(defer &&) = default;
		defer & operator=(const defer &) = delete;
		defer & operator=(defer &&) = default;

		void cancel() { _fx = nullptr;  }
		~defer() { if (_fx) _fx(); }
	private:
		FX _fx;
	};

	void throw_error(int error) {
		throw std::system_error(error, std::system_category());
	}


	void throw_error(int error, const std::string &what) {
		throw std::system_error(error, std::system_category(), what);
	}



}

#ifdef _WIN32
#include <windows.h>

namespace {

	void throw_error() {
		throw_error(GetLastError());
	}

	void throw_error(const std::string &what) {
		throw_error(GetLastError(), what);
	}

}

void mapped_file_base::close() {
	if (is_open()) {

		UnmapViewOfFile(_data);
		CloseHandle(_map_handle);
		CloseHandle(_file_handle);
		reset();
	}
}

void mapped_file_base::open(const path_type& p, mapmode flags, size_t length, size_t offset) {


	HANDLE fh;
	HANDLE mh;

	// length of 0 in CreateFileMapping / MapViewOfFile
	// means map the entire file.

	if (is_open()) {
		throw std::runtime_error("mapped_file_base::open - file already open");
	}

	fh = CreateFile(p.c_str(), 
		flags == readonly ? GENERIC_READ : GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ, 
		nullptr,
		OPEN_EXISTING, 
		flags == readonly ? FILE_ATTRIBUTE_READONLY : FILE_ATTRIBUTE_NORMAL,
		nullptr
	);
	if (fh == INVALID_HANDLE_VALUE) {
		throw_error();
	}

	auto fh_close = make_unique_resource(fh, CloseHandle);


	if (length == -1) {
		LARGE_INTEGER file_size;
		GetFileSizeEx(fh, &file_size);
		length = (size_t)file_size.QuadPart;
	}

	if (length == 0) return;

	DWORD protect = 0;
	DWORD access = 0;
	switch (flags) {
	case readonly:
		protect = PAGE_READONLY;
		access = FILE_MAP_READ;
		break;
	case readwrite:
		protect = PAGE_READWRITE;
		access = FILE_MAP_WRITE;
		break;
	case priv:
		protect = PAGE_WRITECOPY;
		access = FILE_MAP_WRITE;
		break;
	}

	mh = CreateFileMapping(fh, nullptr, protect, 0, 0, 0);
	if (mh == INVALID_HANDLE_VALUE) {
		throw_error();
	}

	auto mh_close = make_unique_resource(mh, CloseHandle);


	_data = MapViewOfFileEx(mh, 
		access, 
		(DWORD)(offset >> 32), 
		(DWORD)offset, 
		length, 
		nullptr);
	if (!_data) {
		throw_error();
	}


	_file_handle = fh_close.release();
	_map_handle = mh_close.release();
	_size = length;
	_flags = flags;
}


void mapped_file_base::create(const path_type& p, size_t length) {

	const size_t offset = 0;

	HANDLE fh;
	HANDLE mh;
	LARGE_INTEGER file_size;

	const DWORD protect = PAGE_READWRITE;
	const DWORD access = FILE_MAP_WRITE;


	if (is_open()) {
		throw std::runtime_error("mapped_file_base::create - file already open");
	}


	fh = CreateFile(p.c_str(), 
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ, 
		nullptr,
		CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL,
		nullptr
	);
	if (fh == INVALID_HANDLE_VALUE) {
		throw_error();
	}

	auto fh_close = make_unique_resource(fh, CloseHandle);


	file_size.QuadPart = length;
	if (!SetFilePointerEx(fh, file_size, nullptr, FILE_BEGIN)) throw_error();
	if (!SetEndOfFile(fh)) throw_error();

	mh = CreateFileMapping(fh, nullptr, protect, 0, 0, 0);
	if (mh == INVALID_HANDLE_VALUE) {
		throw_error();
	}

	auto mh_close = make_unique_resource(mh, CloseHandle);

	_data = MapViewOfFileEx(mh, 
		access, 
		(DWORD)(offset >> 32), 
		(DWORD)offset, 
		length, 
		nullptr);

	if (!_data) {
		throw_error();
	}

	_file_handle = fh_close.release();
	_map_handle = mh_close.release();

	_size = length;
	_flags = readwrite;
}



#else

#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <cerrno>

namespace {

	void throw_error() {
		throw_error(errno);
	}

	void throw_error(const std::string &what) {
		throw_error(errno, what);
	}

}

void mapped_file_base::close() {
	if (is_open()) {
		::munmap(_data, _size);
		::close(_fd);
		reset();
	}
}


void mapped_file_base::open(const path_type& p, mapmode flags, size_t length, size_t offset) {

	int fd;

	int oflags = 0;

	if (is_open()) {
		throw std::runtime_error("mapped_file_base::open - file already open");
	}

	switch (flags) {
	case readonly:
		oflags = O_RDONLY;
		break;
	default:
		oflags = O_RDWR;
		break;
	}

	fd = ::open(p.c_str(), oflags);
	if (fd < 0) {
		throw_error(errno);
	}

	//defer([fd](){::close(fd); });
	auto close_fd = make_unique_resource(fd, ::close);



	if (length == -1) {
		struct stat st;

		if (::fstat(fd, &st) < 0) {
			throw_error(errno);
		}
		length = st.st_size;
	}

	if (length == 0) return;

	_data = ::mmap(0, length, 
		flags == readonly ? PROT_READ : PROT_READ | PROT_WRITE, 
		flags == priv ? MAP_PRIVATE : MAP_SHARED, 
		fd, offset);

	if (_data == MAP_FAILED) {
		_data = nullptr;
		throw_error(errno);
	}

	_fd = close_fd.release();
	_size = length;
	_flags = flags;
}

void mapped_file_base::create(const path_type& p, size_t length) {

	int fd;
	const size_t offset = 0;

	if (is_open()) {
		throw std::runtime_error("mapped_file_base::create - file already open");
	}



	fd = ::open(p.c_str(), O_RDWR | O_CREAT | O_TRUNC);
	if (fd < 0) {
		throw_error(errno);
	}

	//defer([fd](){::close(fd); });

	auto close_fd = make_unique_resource(fd, ::close);


	if (::ftruncate(fd, length) < 0) {
		throw_error(errno);
	}


	_data = ::mmap(0, length, 
		PROT_READ | PROT_WRITE, 
		MAP_SHARED, 
		fd, offset);

	if (_data == MAP_FAILED) {
		_data = nullptr;
		throw_error(errno);
	}

	_fd = close_fd.release();
	_size = length;
	_flags = readwrite;
}



#endif


void mapped_file_base::reset() {
	_data = nullptr;
	_size = 0;
	_flags = readonly;
#ifdef _WIN32
	_file_handle = nullptr;
	_map_handle = nullptr;
#else
	_fd = -1;
#endif
}

void mapped_file_base::swap(mapped_file_base &rhs)
{
	if (std::addressof(rhs) != this) {
		std::swap(_data, rhs._data);
		std::swap(_size, rhs._size);
		std::swap(_flags, rhs._flags);
#ifdef _WIN32
		std::swap(_file_handle, rhs._file_handle);
		std::swap(_map_handle, rhs._map_handle);
#else
		std::swap(_fd, rhs._fd);
#endif
	}
}

mapped_file::mapped_file(mapped_file &&rhs) : mapped_file() {
	swap(rhs);
	//rhs.reset();
}

mapped_file& mapped_file::operator=(mapped_file &&rhs) {
	if (std::addressof(rhs) == this) return *this;

	swap(rhs);
	rhs.close();
	//rhs.reset();
	return *this;
}


