#ifndef __mpw_loader_h__
#define __mpw_loader_h__

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

	}

	uint16_t UnloadSeg(uint16_t trap);

}

#endif
