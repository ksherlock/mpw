#ifndef __fs_spec_h__
#define __fs_spec_h__

#include <string>
#include <stdint.h>
#include <deque>

namespace OS {

	class FSSpecManager
	{
	public:

		static std::string ExpandPath(const std::string &path, int32_t id);
		static const std::string &PathForID(int32_t id);
		static int32_t IDForPath(const std::string &path, bool insert = true);
		static int32_t IDForPath(std::string &&path, bool insert = true);

		static void Init();

	private:

		struct Entry
		{
			Entry(const std::string &p, size_t h) :
				path(p), hash(h)
			{}

			Entry(std::string &&p, size_t h) :
				path(p), hash(h)
			{}

			std::string path;
			size_t hash = 0;
		};

		static std::deque<Entry> _pathQueue;
	};

}

#endif