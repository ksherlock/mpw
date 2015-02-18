#ifndef __mpw_os_internal_h__
#define __mpw_os_internal_h__

#include <deque>
#include <string>
#include <sys/types.h>

namespace OS { namespace Internal {

	uint16_t GetFinderInfo(const std::string &pathname, void *info, bool extended);
	uint16_t SetFinderInfo(const std::string &pathname, void *info, bool extended);

	uint16_t SetFinderInfo(const std::string &pathName, uint32_t fileType, uint32_t creator);

	uint16_t SetFileDates(const std::string &pathname, uint32_t createDate, uint32_t modificationDate, uint32_t backupDate);

	uint16_t GetFileType(const std::string &pathname, uint16_t *fileType, uint32_t *auxType);

	int32_t mac_seek(uint16_t refNum, uint16_t mode, int32_t offset);

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

		static int open(const std::string &filename, int permission, int fork);


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
