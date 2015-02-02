#ifndef __string_splitter__
#define __string_splitter__

#include <string>

class string_splitter {
public:
	string_splitter(const std::string &str, char sep) :
		_parent(str), _sep(sep)
	{
		_begin = 0;
		_end = _parent.find(_sep);
		_str = _parent.substr(_begin, _end);
		// _begin is 0, _end is either npos or offset from 0,
		// so no need to calculate a count.
	}
	
	operator bool() const {
		return _begin != npos;
	}

	string_splitter &operator++() {
		increment();
		return *this;
	}

	const std::string &operator *() const {
		return _str;
	}

	const std::string *operator ->() const {
		return &_str;
	}
	
private:
	void increment() {
		_str.clear();
		if (_begin == npos) return;
		if (_end == npos) { _begin = _end; return; }

		_begin = _end + 1;
		_end = _parent.find(_sep, _begin);
		auto count = _end == npos ? _end : _end - _begin;
		_str = _parent.substr(_begin, count);
	}

	const static auto npos = std::string::npos;
	std::string _str;
	const std::string &_parent;
	char _sep;
	std::string::size_type _begin = 0;
	std::string::size_type _end = 0;
};


#ifdef TEST
#include <cstdio>
#include <cstring>

int main(int argc, char **argv)
{
	if (argc != 3) {
		fprintf(stderr, "Usage: %s sep string\n", argv[0]);
		return 1;
	}
	if (strlen(argv[1]) != 1) {
		fprintf(stderr, "Separator must be a single character\n");
		return 1;
	}
	char sep = argv[1][0];
	std::string str(argv[2]);

	for (auto iter = string_splitter(str, sep); iter; ++iter) {
		printf("%s\n", iter->c_str());
	}

	return 0;
}
#endif

#endif

