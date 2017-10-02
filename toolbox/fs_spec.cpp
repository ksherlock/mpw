#include "fs_spec.h"

#include <algorithm>
#include <cassert>
#include <unistd.h>
#include <sys/param.h>

namespace OS {

	const int RootPathID = 2;

	std::deque<FSSpecManager::Entry> FSSpecManager::_pathQueue;

	void FSSpecManager::Init()
	{
		static bool initialized = false;

		if (!initialized)
		{
			// "/" is item #2.  0 and 1 are reserved.
			//can't just call IDForPath because that calls... Init();
			static std::string RootPath("/");

			std::hash<std::string> hasher;
			size_t hash = hasher(RootPath);
			_pathQueue.emplace_back(FSSpecManager::Entry(RootPath, hash));
			assert(_pathQueue.size() == 1);
			initialized = true;
		}
	}

	int32_t FSSpecManager::IDForCWD()
	{
		char buffer[MAXPATHLEN];
		char *cp;

		cp = getcwd(buffer, sizeof(buffer));
		if (cp == 0) return 0;

		std::string path(cp);

		return FSSpecManager::IDForPath(std::move(path), true);
	}


	int32_t FSSpecManager::IDForPath(const std::string &path, bool insert)
	{
		if (path.empty()) return -1;
		if (path.back() != '/')
		{
			std::string tmp(path);
			return IDForPath(std::move(tmp), insert);
		}

		/*
		char buffer[PATH_MAX + 1];

		char *cp = realpath(path.c_str(), buffer);
		if (!cp) return -1;

		std::string s(cp);
		*/

		std::hash<std::string> hasher;
		size_t hash = hasher(path);

		Init();

		// this only works well for a small number of directories,
		// which should be the common case.
		int i = RootPathID;
		for (const auto &e : _pathQueue)
		{
			if (e.hash == hash && e.path == path) return i;
			++i;
		}

		if (!insert) return -1;

		_pathQueue.emplace_back(FSSpecManager::Entry(path, hash));
		return _pathQueue.size() + RootPathID - 1;
	}

	int32_t FSSpecManager::IDForPath(std::string &&path, bool insert)
	{
		/*
		char buffer[PATH_MAX + 1];

		char *cp = realpath(path.c_str(), buffer);
		if (!cp) return -1;

		std::string s(cp);
		*/

		// trailing / is required.
		if (path.empty()) return -1;
		if (path.back() != '/') path.push_back('/');

		std::hash<std::string> hasher;
		size_t hash = hasher(path);

		Init();

		int i = RootPathID;
		for (const auto &e : _pathQueue)
		{
			if (e.hash == hash && e.path == path) return i;
			++i;
		}

		if (!insert) return -1;

		_pathQueue.emplace_back(FSSpecManager::Entry(std::move(path), hash));
		return _pathQueue.size() + RootPathID - 1;
	}


	const std::string &FSSpecManager::PathForID(int32_t id)
	{
		static std::string NullString;

		Init();

		id -= RootPathID;
		if (id < 0) return NullString;
		if (id >= _pathQueue.size()) return NullString;

		return _pathQueue[id].path;
	}

	std::string FSSpecManager::ExpandPath(const std::string &path, int32_t id)
	{
		if (path.length() && path.front() == '/') return path;
		if (id == 0) return path;

		const std::string &dir = PathForID(id);

		if (dir.empty()) return dir;
		return dir + path;
	}



}
