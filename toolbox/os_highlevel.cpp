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

#include <macos/errors.h>

#include "os.h"
#include "os_internal.h"
#include "toolbox.h"
#include "stackframe.h"


using ToolBox::Log;
using namespace ToolBox::Errors;
using OS::Internal::errno_to_oserr;

namespace {

	// FSSpec garbage
	class FSSpecManager
	{
	public:

		static const std::string &pathForID(int32_t id);
		static int32_t idForPath(const std::string &path, bool insert = true);

		static void Init();

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

	void FSSpecManager::Init()
	{
		static bool initialized = false;

		if (!initialized)
		{
			// "/" is item #1
			idForPath("/", true);
			initialized = true;
		}

	}

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

	// MacOS: -> { -1, 1, "MacOS" }
	// MacOS:dumper -> {-1, 2 "dumper"}

	uint16_t FSMakeFSSpec(void)
	{
		// FSMakeFSSpec(vRefNum: Integer; dirID: LongInt; fileName: Str255; VAR spec: FSSpec): OSErr;

		/*
		 * See Chapter 2, File Manager / Using the File Manager, 2-35
		 *
		 *
		 */

		uint16_t vRefNum;
		uint32_t dirID;
		uint32_t fileName;
		uint32_t spec;




		StackFrame<14>(vRefNum, dirID, fileName, spec);

		std::string sname = ToolBox::ReadPString(fileName, true);
		Log("     FSMakeFSSpec(%04x, %08x, %s, %08x)\n", 
			vRefNum, dirID, sname.c_str(), spec);

		if (vRefNum == 0 && dirID > 0 && sname.length())
		{
			// SC uses dirID + relative path.

			std::string root = FSSpecManager::pathForID(dirID);
			if (root.empty())
			{
				std::memset(memoryPointer(spec), 0, 8);
				return MacOS::dirNFErr;
			}

			sname = root + sname;
			dirID = 0;
		}

		bool absolute = sname.length() ? sname[0] == '/' : false;
		if (absolute || (vRefNum == 0 && dirID == 0))
		{
			char buffer[PATH_MAX + 1];

			// TODO -- FSSpecs are valid for non-existant files
			// but not non-existant directories.
			// realpath does not behave in such a manner.

			// expand the path.  Also handles relative paths.
			char *cp = realpath(sname.c_str(), buffer);
			if (!cp)
			{
				std::memset(memoryPointer(spec), 0, 8);
				return MacOS::mFulErr;
			}

			std::string leaf;
			std::string path;


			path.assign(cp);

			// if sname is null then the target is the default directory... 
			// so this should be ok.

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

			// TODO -- return fnf if file does not exist.
			return 0;
		}

		else
		{
			fprintf(stderr, "FSMakeFSSpec(%04x, %08x) not yet supported\n", vRefNum, dirID);
			exit(1);
		}



		return 0;
	}


	uint16_t FSpGetFInfo()
	{
		// FSpGetFInfo (spec: FSSpec; VAR fndrInfo: FInfo): OSErr;

		uint32_t spec;
		uint32_t finderInfo;

		StackFrame<8>(spec, finderInfo);

		int parentID = memoryReadLong(spec + 2);

		std::string leaf = ToolBox::ReadPString(spec + 6, false);
		std::string path = FSSpecManager::pathForID(parentID);

		path += leaf;

		Log("     FSpGetFInfo(%s, %08x)\n",  path.c_str(), finderInfo);



		Internal::GetFinderInfo(path, memoryPointer(finderInfo), false);
		return 0;
	}

	uint16_t FSpSetFInfo()
	{
		// FSpSetFInfo (spec: FSSpec; VAR fndrInfo: FInfo): OSErr;

		uint32_t spec;
		uint32_t finderInfo;
		uint16_t d0;

		StackFrame<8>(spec, finderInfo);

		int parentID = memoryReadLong(spec + 2);

		std::string leaf = ToolBox::ReadPString(spec + 6, false);
		std::string path = FSSpecManager::pathForID(parentID);

		path += leaf;

		Log("     FSpSetFInfo(%s, %08x)\n",  path.c_str(), finderInfo);


		d0 = Internal::SetFinderInfo(path, memoryPointer(finderInfo), false);

		return d0;
	}



	uint16_t ResolveAliasFile()
	{
		uint32_t spec;
		uint16_t resolveAliasChains;
		uint32_t targetIsFolder;
		uint32_t wasAliased;

		StackFrame<14>(spec, resolveAliasChains, targetIsFolder, wasAliased);

		int parentID = memoryReadLong(spec + 2);

		std::string leaf = ToolBox::ReadPString(spec + 6, false);
		std::string path = FSSpecManager::pathForID(parentID);

		path += leaf;

		Log("    ResolveAliasFile(%s)\n", path.c_str());

		struct stat st;
		int rv;

		rv = ::stat(path.c_str(), &st);
		if (rv < 0) return errno_to_oserr(errno);

		if (targetIsFolder)
		{
			memoryWriteWord(S_ISDIR(st.st_mode) ? 1 : 0, targetIsFolder);
		}

		// don't bother pretending a soft link is an alias.
		if (wasAliased) memoryWriteWord(0, wasAliased);

		return 0;
	}



	uint16_t HighLevelHFSDispatch(uint16_t trap)
	{

		uint16_t selector;

		FSSpecManager::Init();

		selector = cpuGetDReg(0) & 0xffff;
		Log("%04x HighLevelHFSDispatch(%04x)\n", trap, selector);

		switch (selector)
		{
			case 0x0001:
				return FSMakeFSSpec();
				break;

			case 0x0007:
				return FSpGetFInfo();
				break;

			case 0x0008:
				return FSpSetFInfo();
				break;

			default:
				fprintf(stderr, "selector %04x not yet supported\n", selector);
				exit(1);

		}

	}

	uint16_t HGetVol(uint16_t trap)
	{
		uint32_t parm = cpuGetAReg(0);

		Log("%04x HGetVol(%08x)\n", trap, parm);

		//uint32_t ioCompletion = memoryReadLong(parm + 12);
		uint32_t namePtr = memoryReadLong(parm + 18);

		// ioResult
		memoryWriteWord(0, parm + 16);
		// ioVRefNum
		memoryWriteWord(0, parm + 22);

		// ioWDProcID
		memoryWriteLong(0, parm + 28);

		// ioWDVRefNum
		memoryWriteWord(0, parm + 32);


		// todo -- this should create an FSSpec entry for
		// the current wd and return the id.
		// (FSMakeSpec handles 0 as a dir, so ok for now)
		// ioWDDirID
		memoryWriteLong(0, parm + 48);

		std::string tmp = "MacOS";
		ToolBox::WritePString(namePtr, tmp);

		return 0;
	}


	uint16_t HOpen(uint16_t trap)
	{

		int fd;
		int d0;


		uint32_t parm = cpuGetAReg(0);

		Log("%04x HOpen(%08x)\n", trap, parm);

		uint32_t ioNamePtr = memoryReadLong(parm + 18);
		uint8_t ioPermission = memoryReadByte(parm + 27); 
		uint32_t ioDirID = memoryReadLong(parm + 48);

		std::string sname = ToolBox::ReadPString(ioNamePtr, true);
		bool absolute = sname.length() ? sname[0] == '/' : 0;

		if (ioDirID && !absolute)
		{
			std::string dir = FSSpecManager::pathForID(ioDirID);
			if (dir.empty())
			{
				d0 = MacOS::dirNFErr;
				memoryWriteWord(d0, parm + 16);

				return d0;
			}
			sname = dir + sname;
		}

		fd = Internal::FDEntry::open(sname, ioPermission, false);

		d0 = fd < 0 ? fd : 0;
		if (fd >= 0)
		{
			memoryWriteWord(fd, parm + 24);
		}

		memoryWriteWord(d0, parm + 16);
		return d0;
	}


}