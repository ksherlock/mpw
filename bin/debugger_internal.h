#ifndef __debug_internal_h__
#define __debug_internal_h__

#include <cstdint>
#include <string>
#include <map>
#include <unordered_map>

#include <toolbox/loader.h>

namespace Debug {

	namespace Internal {

		extern Loader::DebugNameTable SymbolTable;

		extern std::map<std::string, uint16_t> ErrorTable;
		extern std::map<std::string, uint16_t> GlobalTable;
		extern std::map<std::string, uint16_t> TrapTable;

		extern std::unordered_multimap<uint16_t, std::string> ErrorTableInvert;
		extern std::unordered_map<uint32_t, std::string> SymbolTableInvert;

	}

}

#endif
