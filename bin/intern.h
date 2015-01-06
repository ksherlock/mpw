#ifndef __intern_h__
#define __intern_h__

#include <string>

namespace Intern {
	const std::string *String(const std::string &s);
	const std::string *String(std::string &&s);
	const std::string *String(const char *);
	const std::string *String(const char *, size_t size);
	const std::string *String(const char *, const char *);
};

#endif