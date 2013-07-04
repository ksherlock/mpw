
#include <cstdint>
#include <cctype>
#include <cstring>
#include <cstdlib>

#include <string>
#include <vector>
#include <array>
#include <unordered_set>
#include <unordered_map>

#include <bitset>

#include <readline/readline.h>


#include "loader.h"

#include <debugger/commands.h>

#include <cpu/defs.h>
#include <cpu/CpuModule.h>

#include <macos/traps.h>
#include <macos/sysequ.h>

bool ParseLine(const char *iter, Command *command);

extern "C" {

	// any reason to have enable/disable flag
	// vs deleting it?
	std::unordered_map<uint16_t, bool> tbrkMap;
	
	std::unordered_map<uint16_t, unsigned> brkMap;
	std::array<uint32_t, 4096> brkPageMap; // bloom filter on a page-level.

	void tbrkAdd(uint16_t tool)
	{
		auto iter = tbrkMap.find(tool);
		if (iter == tbrkMap.end())
		{
			tbrkMap.emplace(tool, true);
		}
	}

	void tbrkRemove(uint16_t tool)
	{
		auto iter = tbrkMap.find(tool);
		if (iter == tbrkMap.end()) return;

		tbrkMap.erase(iter);
	}

	void tbrkRemoveAll()
	{
		tbrkMap.clear();
	}

	bool tbrkLookup(uint16_t tool)
	{
		if ((tool & 0xf000) != 0xa000) return false;

		return tbrkMap.find(tool) != tbrkMap.end();
	}


	void brkRemoveAll()
	{
		brkPageMap.fill(0);
		brkMap.clear();
	}

	void brkRemove(uint32_t address)
	{
		uint32_t page = address >> 12;
		if (page >= brkPageMap.size()) return;

		auto iter = brkMap.find(address);
		if (iter == brkMap.end())
			return;

		brkPageMap[page]--;
		brkMap.erase(iter);
	}

	void brkAdd(uint32_t address)
	{
		uint32_t page = address >> 12;
		if (page >= brkPageMap.size()) return;

		auto iter = brkMap.find(address);
		if (iter == brkMap.end())
		{
			brkPageMap[page]++;
			brkMap.emplace(address, 0);
		}

	}

	bool brkLookup(uint32_t address)
	{
		uint32_t page = address >> 12;
		if (page >= brkPageMap.size()) return false;

		if (!brkPageMap[page]) return false;

		return brkMap.find(address) != brkMap.end();
	}

	uint32_t debuggerReadLong(uint32_t address)
	{
		uint32_t tmp = 0;
		for (unsigned i = 0; i < 4; ++i)
		{
			if (address < Flags.memorySize)
				tmp = (tmp << 8) + Flags.memory[address++];
		}

		return tmp;
	}

	uint16_t debuggerReadWord(uint32_t address)
	{
		uint16_t tmp = 0;
		for (unsigned i = 0; i < 2; ++i)
		{
			if (address < Flags.memorySize)
				tmp = (tmp << 8) + Flags.memory[address++];
		}

		return tmp;
	}

	uint8_t debuggerReadByte(uint32_t address)
	{
		if (address < Flags.memorySize)
			return Flags.memory[address];

		return 0;
	}

}

namespace {

	void hexdump(const uint8_t *data, ssize_t size, uint32_t address = 0)
	{
	const char *HexMap = "0123456789abcdef";

	    char buffer1[16 * 3 + 1 + 1];
	    char buffer2[16 + 1];
	    ssize_t offset = 0;
	    unsigned i, j;
	    
	    
	    while(size > 0)
	    {        
	        std::memset(buffer1, ' ', sizeof(buffer1));
	        std::memset(buffer2, ' ', sizeof(buffer2));
	        
	        unsigned linelen = (unsigned)std::min(size, (ssize_t)16);
	        
	        
	        for (i = 0, j = 0; i < linelen; i++)
	        {
	            unsigned x = data[i];
	            buffer1[j++] = HexMap[x >> 4];
	            buffer1[j++] = HexMap[x & 0x0f];
	            j++;
	            if (i == 7) j++;
	            
	            // isascii not part of std:: and may be a macro.
	            buffer2[i] = isascii(x) && std::isprint(x) ? x : '.';
	            
	        }
	        
	        buffer1[sizeof(buffer1)-1] = 0;
	        buffer2[sizeof(buffer2)-1] = 0;
	        
	    
	        std::printf("%08x:\t%s\t%s\n", address + (unsigned)offset, buffer1, buffer2);
	        offset += 16;
	        data += 16;
	        size -= 16;
	    }
	    std::printf("\n");
	}

}

bool ParseLine(const char *iter, Command *command);

extern "C" {

	uint32_t debuggerReadLong(uint32_t address)
	{
		uint32_t tmp = 0;
		for (unsigned i = 0; i < 4; ++i)
		{
			if (address < Flags.memorySize)
				tmp = (tmp << 8) + Flags.memory[address++];
		}

		return tmp;
	}

	uint16_t debuggerReadWord(uint32_t address)
	{
		uint16_t tmp = 0;
		for (unsigned i = 0; i < 2; ++i)
		{
			if (address < Flags.memorySize)
				tmp = (tmp << 8) + Flags.memory[address++];
		}

		return tmp;
	}

	uint8_t debuggerReadByte(uint32_t address)
	{
		if (address < Flags.memorySize)
			return Flags.memory[address];

		return 0;
	}

}

void DebugHelp(const Command &cmd)
{
	printf("help\n");
	printf("break expression\n");
	printf("step\n");
	printf("continue\n");
	printf("\n");
	printf("print expression\n");
	printf("list expression\n");
	printf("dump expression\n");
	printf("register=expression\n");
	printf("\n");
	printf("registers: a0-7, d0-7, pc, sp, fp, csr\n");
	printf("\n");
}


void DebugPrint(const Command &cmd)
{
	for (unsigned i = 0; i < cmd.argc; ++i)
	{
		uint32_t data = cmd.argv[i];
		printf("$%08x %12u", data, data);
		if (data & 0x80000000)
			printf(" %12d", (int32_t)data);
		if ((data & 0xffff8000) == 0x8000)
			printf(" %6d", (int16_t)data);

		printf("\n");
	}	
}


void DebugDump(const Command &cmd)
{
	// TODO -- if no address, use previous address.
	// TODO -- support range?
	if (cmd.argc == 1)
	{
		uint32_t start = cmd.argv[0];
		if (start >= Flags.memorySize) return;

		uint32_t end = std::min(start + 512, Flags.memorySize);
		ssize_t size = end - start;

		hexdump(Flags.memory + start, size, start);

	}
}

void DebugList(const Command &cmd)
{
	// TODO -- if no address, use previous address.
	// TODO -- support range?
	if (cmd.argc == 1)
	{
		uint32_t pc = cmd.argv[0];
		if (pc & 0x01)
		{
			printf("address is not aligned: $%08x\n", pc);
			return;
		}

		static char strings[4][256];


		for (unsigned i = 0; i < 32; ++i)
		{

			if (pc >= Flags.memorySize) break;

			uint16_t opcode = debuggerReadWord(pc);

			if ((opcode & 0xf000) == 0xa000)
			{

				const char *name;

				name = TrapName(opcode);

				if (name)
				{
					printf("$%08X   %-51s ; %04X\n", pc, name, opcode);
				}
				else
				{
					printf("$%08X   Tool       #$%04X                                   ; %04X\n", 
						pc, opcode, opcode);
				}

				pc += 2;
				continue;
			}

			for (unsigned j = 0; j < 4; ++j) strings[j][0] = 0;

			pc = cpuDisOpcode(pc, strings[0], strings[1], strings[2], strings[3]);
			printf("%s   %-10s %-40s ; %s\n", strings[0], strings[2], strings[3], strings[1]);
		}
	}
}


void DebugPrintRegisters(const Command &cmd)
{
	printf("PC: %08X CSR: %04x\n", cpuGetPC(), cpuGetSR());
	
	printf("D: %08x %08x %08x %08x %08x %08x %08x %08x\n",
		cpuGetDReg(0), cpuGetDReg(1), cpuGetDReg(2), cpuGetDReg(3), 
		cpuGetDReg(4), cpuGetDReg(5), cpuGetDReg(6), cpuGetDReg(7)

	);

	printf("A: %08x %08x %08x %08x %08x %08x %08x %08x\n",
		cpuGetAReg(0), cpuGetAReg(1), cpuGetAReg(2), cpuGetAReg(3), 
		cpuGetAReg(4), cpuGetAReg(5), cpuGetAReg(6), cpuGetAReg(7)
	);

}

// todo -- add sigint trap.  it shall set a flag to break.

void DebugShell()
{
	char *cp;

	add_history("!Andy, it still has history!");
	for(;;)
	{
		cp = readline("] ");
		if (!cp)
		{
			printf("\n");
			break; // prompt for exit?
		}
		// parse the command...
		const char *iter = cp;
		while (*iter && isspace(*iter)) ++iter;

		if (*iter)
		{
			Command cmd;
			std::memset(&cmd, 0, sizeof(cmd));
			if (ParseLine(iter, &cmd))
			{
				switch(cmd.action)
				{
					case Print:
						DebugPrint(cmd);
						break;

					case Dump:
						DebugDump(cmd);
						break;

					case List:
						DebugList(cmd);
						break;

					case PrintRegisters:
						DebugPrintRegisters(cmd);
						break;

					default:
						DebugHelp(cmd);
						break;
				}
			}

			// todo -- don't add if same as previous command.
			add_history(cp);
		}
		free(cp);
	}

}

