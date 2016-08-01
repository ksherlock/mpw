#ifndef __mpw_loader__
#define __mpw_loader__

#include <cstdint>

struct Settings {
	Settings() {}

	// command-line settings.

	uint32_t memorySize = 16 * 1024 * 1024;
	uint32_t stackSize = 32 * 1024;
	uint32_t machine = 68030;

	bool traceCPU = false;
	bool traceMacsbug = false;
	bool traceGlobals = false;
	bool traceToolBox = false;
	bool traceMPW = false;

	bool debugger = false;

	bool memoryStats = false;


	// updated later.
	std::pair<uint32_t, uint32_t> stackRange = {0, 0};
	uint8_t *memory = nullptr;

	uint64_t cycles = 0;

	const uint32_t kGlobalSize = 0x10000;
};


extern Settings Flags;

void DebugShell();


#endif
