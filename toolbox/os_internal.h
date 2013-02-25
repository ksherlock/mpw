#ifndef __mpw_os_internal_h__
#define __mpw_os_internal_h__

#include <deque>
#include <string>

namespace OS { namespace Internal {


	struct FDEntry
	{
		int refcount;
		bool text;
		bool resource;

		// std::string path?

		FDEntry() :
			refcount(0), 
			text(false), 
			resource(false)
		{}
	};

	extern std::deque<FDEntry> FDTable;

} }

#endif
