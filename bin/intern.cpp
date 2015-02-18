#include "intern.h"
#include <unordered_map>
#include <cstring>

namespace {

	std::unordered_multimap<unsigned, std::string *> InternTable;

	std::string EmptyString;

	unsigned int DJBHash(const char* begin, size_t length)
	{
		unsigned int hash = 5381;

		for(size_t i = 0; i < length; ++i)
		{
			hash = ((hash << 5) + hash) + (begin[i]);
		}

		return hash;
	}
}

namespace Intern {

	const std::string *String(std::string &&str)
	{
		size_t size = str.size();
		if (!size) return &EmptyString;

		unsigned hash = DJBHash(str.data(), size);

		auto range = InternTable.equal_range(hash);
		auto iter = range.first;
		auto endit = range.second;

		for( ; iter != endit; ++iter)
		{
			// hash matches, make sure the string does.
			const std::string *s = iter->second;

			if (s->size() == size && std::memcmp(s->data(), str.data(), size) == 0)
				return s;
		}

		// insert it. I suppose this could throw, in which case a string would leak.
		std::string *s = new std::string(std::move(str));
		InternTable.emplace(std::make_pair(hash, s));
		return s;
	}

	const std::string *String(const char *begin, size_t size)
	{
		if (!size) return &EmptyString;

		unsigned hash = DJBHash(begin, size);

		auto range = InternTable.equal_range(hash);
		auto iter = range.first;
		auto endit = range.second;

		for( ; iter != endit; ++iter)
		{
			// hash matches, make sure the string does.
			const std::string *s = iter->second;

			if (s->size() == size && std::memcmp(s->data(), begin, size) == 0)
				return s;
		}

		// insert it. I suppose this could throw, in which case a string would leak.
		std::string *s = new std::string(begin, size);
		InternTable.emplace(std::make_pair(hash, s));
		return s;
	}

	const std::string *String(const char *begin, const char *end)
	{
		return String(begin, end - begin);
	}


	const std::string *String(const char *cp)
	{
		if (!cp || !*cp) return &EmptyString;

		return String(cp, strlen(cp));
	}

	const std::string *String(const std::string &s)
	{
		return String(s.data(), s.size());
	}




}
