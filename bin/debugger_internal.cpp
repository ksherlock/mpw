#include "debugger_internal.h"

namespace Debug {

	namespace Internal {

		Loader::DebugNameTable SymbolTable;

		std::map<std::string, uint16_t> ErrorTable;
		std::map<std::string, uint16_t> GlobalTable;
		std::map<std::string, uint16_t> TrapTable;

		std::unordered_multimap<uint16_t, std::string> ErrorTableInvert;
		std::unordered_map<uint32_t, std::string> SymbolTableInvert;

	}
}
