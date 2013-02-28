#include <cerrno>
#include <cctype>
#include <ctime>
#include <algorithm>
#include <chrono>
#include <deque>
#include <string>

#include <sys/xattr.h>
#include <sys/stat.h>
#include <sys/paths.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <strings.h>

#include <cpu/defs.h>
#include <cpu/CpuModule.h>
#include <cpu/fmem.h>

#include "os.h"
#include "os_internal.h"
#include "toolbox.h"
#include "stackframe.h"

using ToolBox::Log;
using OS::Internal::errno_to_oserr;

namespace {

	// FSSpec garbage
	class FSSpecManager
	{
	public:

		static const std::string &pathForID(int32_t id);
		static int32_t idForPath(const std::string &path, bool insert = true);

	private:

		struct Entry
		{
			#if 0
			Entry(std::string &&p) : path(p), hash(std::hash(path))
			{}
			Entry(const std::string &p) : path(p), hash(std::hash(path))
			{} 
			#endif

			Entry(const std::string &p, size_t h) :
				path(p), hash(h)
			{}

			Entry(std::string &&p, size_t h) :
				path(p), hash(h)
			{}

			std::string path;
			size_t hash;
		};

		static std::deque<Entry> _pathQueue;
	};

	std::deque<FSSpecManager::Entry> FSSpecManager::_pathQueue;

	int32_t FSSpecManager::idForPath(const std::string &path, bool insert)
	{
		/*
		char buffer[PATH_MAX + 1];

		char *cp = realpath(path.c_str(), buffer);
		if (!cp) return -1;

		std::string s(cp);
		*/

		std::hash<std::string> hasher;
		size_t hash = hasher(path);

		int i = 1;
		for (const auto &e : _pathQueue)
		{
			if (e.hash == hash && e.path == path) return i;
			++i;
		}

		if (!insert) return -1;

		_pathQueue.emplace_back(FSSpecManager::Entry(path, hash));
		return _pathQueue.size();
	}


	const std::string &FSSpecManager::pathForID(int32_t id)
	{
		static std::string NullString;
		if (id < 1) return NullString;
		if (id > _pathQueue.size()) return NullString;

		return _pathQueue[id - 1].path;
	}

}

namespace OS {

	/*

	struct FSSpec {
		short               vRefNum;
		long                parID;
		StrFileName         name;                   // a Str63 on MacOS
	};
	*/	

	uint16_t FSMakeFSSpec(void)
	{
		// FSMakeFSSpec(vRefNum: Integer; dirID: LongInt; fileName: Str255; VAR spec: FSSpec): OSErr;

		uint16_t vRefNum;
		uint32_t dirID;
		uint32_t fileName;
		uint32_t spec;


		StackFrame<14>(vRefNum, dirID, fileName, spec);

		std::string sname = ToolBox::ReadPString(fileName, true);
		Log("     FSMakeFSSpec(%04x, %08x, %s, %08x)\n", 
			vRefNum, dirID, sname.c_str(), spec);

		bool absolute = sname.length() ? sname[0] == '/' : false;
		if (absolute || (vRefNum == 0 && dirID == 0))
		{
			char buffer[PATH_MAX + 1];

			// expand the path.  Also handles relative paths.
			char *cp = realpath(sname.c_str(), buffer);
			if (!cp)
			{
				return mFulErr;
			}

			std::string leaf;
			std::string path;


			path.assign(cp);


			int pos = path.find_last_of('/');
			if (pos == path.npos)
			{
				// ? should never happen...
				std::swap(leaf, path);
			}
			else
			{
				leaf = path.substr(pos + 1);
				path = path.substr(0, pos + 1); // include the /
			}

			int parentID = FSSpecManager::idForPath(path, true);

			memoryWriteWord(vRefNum, spec + 0);
			memoryWriteLong(parentID, spec + 2);
			// write the filename...
			ToolBox::WritePString(spec + 6, leaf);

			return 0;
		}
		else
		{
			fprintf(stderr, "FSMakeFSSpec(%04x, %08x) not yet supported\n", vRefNum, dirID);
			exit(1);
		}



		return 0;
	}

	uint16_t HighLevelHFSDispatch(uint16_t trap)
	{

		uint16_t selector;

		selector = cpuGetDReg(0) & 0xffff;
		Log("%04x HighLevelHFSDispatch(%04x)\n", trap, selector);

		switch (selector)
		{
			case 0x0001:
				return FSMakeFSSpec();
				break;

			default:
				fprintf(stderr, "selector %04x not yet supported\n", selector);
				exit(1);

		}

	}




}