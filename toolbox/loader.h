#ifndef __mpw_loader_h__
#define __mpw_loader_h__

#include <string>
#include <map>

#include <cstdint>

#include <macos/tool_return.h>


namespace Loader {

	typedef std::map<std::string, std::pair<uint32_t, uint32_t>> DebugNameTable;
	namespace Native {

		using MacOS::tool_return;

		/*
		 * loads the file
		 * sets registers PC and A5.
		 * sets certain globals.
		 *
		 * Dependencies: MM, RM
		 */
		tool_return<void> LoadFile(const std::string &path);

		// scans segments for MacsBug debug names.
		// associates them with the start of the segment.
		void LoadDebugNames(DebugNameTable &table);

	}

	uint16_t LoadSeg(uint16_t trap);
	uint16_t UnloadSeg(uint16_t trap);

}

#endif
