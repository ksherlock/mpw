#ifndef __mpw_os_internal_h__
#define __mpw_os_internal_h__

#include <deque>
#include <string>
#include <sys/types.h>

#include <native/file.h>

namespace OS {

	std::string realpath(const std::string &path);

namespace Internal {

	using MacOS::tool_return;

	//int32_t mac_seek(uint16_t refNum, uint16_t mode, int32_t offset);

	MacOS::macos_error remap_iopermission(int ioPermission);
	MacOS::macos_error remap_seek(ssize_t &offset, int &mode);


	int open_file(native::file_ptr &&f);

	tool_return<int> open_file(const std::string &name, int fork, int permission);

	int close_file(int fd, bool force = false);
	native::file *find_file(int fd);




#if 0
	template<class FX>
	macos_error with_file(int fd, FX fx) {
		if (fd < 0 || fd >= FDTable.size()) return MacOS::rfNumErr;
		auto &e = FDTable[fd];
		if (!e) return MacOS::rfNumErr; 
		return fx(e);
	}
#endif

#if 0
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

#endif

} }

#endif
