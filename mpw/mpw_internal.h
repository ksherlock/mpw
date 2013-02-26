#ifndef __MPW_INTERNAL_H__
#define __MPW_INTERNAL_H__

#include <vector>
#include <cstdio>

namespace MPW
{

	template<typename... Args>
	inline void Log(const char *format, Args... args)
	{
		if (Trace) fprintf(stderr, format, args...);
	}

/*
	namespace Internal
	{
		// for dup counts, etc.
		extern std::vector<int> FDTable;
	}
*/
	
}

#endif