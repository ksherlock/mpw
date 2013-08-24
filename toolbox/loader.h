#ifndef __mpw_loader_h__
#define __mpw_loader_h__

#include <string>
#include <map>

#include <cstdint>

namespace Loader {

	typedef std::map<std::string, std::pair<uint32_t, uint32_t>> DebugNameTable;
	namespace Native {

		/* 
		 * loads the file
		 * sets registers PC and A5.
		 * sets certain globals.
		 *
		 * Dependencies: MM, RM 
		 */
		uint16_t LoadFile(const std::string &path);

		// scans segments for MacsBug debug names.
		// associates them with the start of the segment.
		void LoadDebugNames(DebugNameTable &table);

	}

	uint16_t UnloadSeg(uint16_t trap);

}

#endif
