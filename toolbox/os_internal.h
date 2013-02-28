#ifndef __mpw_os_internal_h__
#define __mpw_os_internal_h__

#include <deque>
#include <string>

namespace OS { namespace Internal {

	uint16_t errno_to_oserr(int xerrno);

	struct FDEntry
	{
		int refcount;
		bool text;
		bool resource;

		std::string filename;

		FDEntry() :
			refcount(0), 
			text(false), 
			resource(false)
		{}

		static std::deque<FDEntry> FDTable;

		static FDEntry& allocate(int fd);
		static FDEntry& allocate(int fd, std::string &&filename);
		static FDEntry& allocate(int fd, const std::string &filename);

		static ssize_t read(int fd, void *buffer, size_t count);
		static ssize_t write(int fd, const void *buffer, size_t count);

		static int close(int fd, bool force = false);


		template<class F1, class F2>
		static int32_t action(int fd, F1 good, F2 bad)
		{
			if (fd < 0 || fd >= FDTable.size())
			{
				return bad(fd);
			}

			auto &e = FDTable[fd];
			if (e.refcount)
			{
				return good(fd, e);
			}
			else
			{
				return bad(fd);
			}

		}
	};


} }

#endif
