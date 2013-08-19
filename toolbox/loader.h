#ifndef __mpw_loader_h__
#define __mpw_loader_h__

#include <string>
#include <map>

#include <cstdint>

namespace Loader {

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
		void LoadDebugNames(std::map<std::string, uint32_t> &table);

	}

	uint16_t UnloadSeg(uint16_t trap);

}

#endif
