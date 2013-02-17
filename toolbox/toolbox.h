#ifndef __mpw_toolbox_h__
#define __mpw_toolbox_h__

#include <string>

#include <cstdint>
#include <cstdio>

namespace ToolBox
{
	extern bool Trace;

	template<typename... Args>
	inline void Log(const char *format, Args... args)
	{
		if (Trace) fprintf(stderr, format, args...);
	}


	void dispatch(uint16_t trap);


	std::string ReadCString(uint32_t address, bool fname = false);
	std::string ReadPString(uint32_t address, bool fname = false);

	std::string ReadString(uint32_t address, uint32_t length);

	bool WritePString(uint32_t address, const std::string &s);

}


#endif