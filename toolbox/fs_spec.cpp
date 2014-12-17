#include "fs_spec.h"

#include <algorithm>
#include <cassert>

namespace OS {

	std::deque<FSSpecManager::Entry> FSSpecManager::_pathQueue;

	void FSSpecManager::Init()
	{
		static bool initialized = false;

		if (!initialized)
		{
			// "/" is item #1
			//IDForPath("/", true);
			static std::string RootPath("/");

			std::hash<std::string> hasher;
			size_t hash = hasher(RootPath);
			_pathQueue.emplace_back(FSSpecManager::Entry(RootPath, hash));
			assert(_pathQueue.size() == 1);
			initialized = true;
		}

	}

	int32_t FSSpecManager::IDForPath(const std::string &path, bool insert)
	{
		/*
		char buffer[PATH_MAX + 1];

		char *cp = realpath(path.c_str(), buffer);
		if (!cp) return -1;

		std::string s(cp);
		*/

		std::hash<std::string> hasher;
		size_t hash = hasher(path);

		Init();

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

	int32_t FSSpecManager::IDForPath(std::string &&path, bool insert)
	{
		/*
		char buffer[PATH_MAX + 1];

		char *cp = realpath(path.c_str(), buffer);
		if (!cp) return -1;

		std::string s(cp);
		*/

		std::hash<std::string> hasher;
		size_t hash = hasher(path);

		Init();

		int i = 1;
		for (const auto &e : _pathQueue)
		{
			if (e.hash == hash && e.path == path) return i;
			++i;
		}

		if (!insert) return -1;

		_pathQueue.emplace_back(FSSpecManager::Entry(std::move(path), hash));
		return _pathQueue.size();
	}


	const std::string &FSSpecManager::PathForID(int32_t id)
	{
		static std::string NullString;

		Init();

		if (id < 1) return NullString;
		if (id > _pathQueue.size()) return NullString;

		return _pathQueue[id - 1].path;
	}

	std::string FSSpecManager::ExpandPath(const std::string &path, int32_t id)
	{
		if (path.length() && path[0] == '/') return path;
		if (id == 0) return path;

		const std::string &dir = PathForID(id);

		if (dir.empty()) return dir;
		return dir + path;
	}



}