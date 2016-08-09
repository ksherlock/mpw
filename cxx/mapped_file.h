#ifndef __mapped_file_h__
#define __mapped_file_h__

#ifdef HAVE_TSFS
#include <filesystem>
#else
#include <string>
#endif

#include <cstddef>

class mapped_file_base {
public:

#ifdef HAVE_TSFS
	typedef std::filesystem::path path_type ;
#else
	typedef std::string path_type ;
#endif

	enum mapmode { readonly, readwrite, priv };

	void close();

	bool is_open() const {
		return _data != nullptr;
	}

	size_t size() const {
		return _size;
	}

	operator bool() const { return is_open(); }
	bool operator !() const { return !is_open(); }

	~mapped_file_base() { close(); }

protected:

	void swap(mapped_file_base &rhs);

	void open(const path_type& p, mapmode flags, size_t length, size_t offset);
	void create(const path_type &p, size_t new_size); // always creates readwrite.
	void reset();


	size_t _size = 0;
	void *_data = nullptr;
	mapmode _flags = readonly;

#ifdef _WIN32
	void *_file_handle = nullptr;
	void *_map_handle = nullptr;
#else
	int _fd = -1;
#endif
};



class mapped_file : public mapped_file_base {

	typedef mapped_file_base base;

public:

	typedef unsigned char value_type;

	typedef value_type *iterator;
	typedef const value_type *const_iterator;

	typedef value_type &reference ;
	typedef const value_type &const_reference;


	mapped_file() = default;
	mapped_file(const path_type& p, mapmode flags = readonly, size_t length = -1, size_t offset = 0) {
		open(p, flags, length, offset);
	}

	mapped_file(mapped_file &&);
	mapped_file(const mapped_file &) = delete;

	mapped_file &operator=(mapped_file &&);
	mapped_file &operator=(const mapped_file &) = delete;


	void open(const path_type& p, mapmode flags, size_t length = -1, size_t offset = 0) {
		base::open(p, flags, length, offset);
	}


	const value_type *data() const {
		return (const value_type *)_data;
	}

	value_type *data() {
		return _flags == readonly ? (value_type *)nullptr : (value_type *)_data;
	}

	const_iterator cbegin() const {
		return data();
	}

	const_iterator cend() const {
		return data() + size();
	}

	const_iterator begin() const {
		return cbegin();
	}
	
	const_iterator end() const {
		return cend();
	}


	iterator begin() {
		return _flags == readonly ? (iterator)nullptr : (iterator)_data;
	}

	iterator end() {
		return _flags == readonly ? (iterator)nullptr : (iterator)_data + size();
	}

	mapmode flags() const {
		return _flags;
	}
	
	void swap(mapped_file &rhs) {
		base::swap(rhs);
	}

};

namespace std {
	template<class T>
	void swap(mapped_file &a, mapped_file &b) {
		a.swap(b);
	}
}

#endif
