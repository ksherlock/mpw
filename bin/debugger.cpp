
#include <cstdint>
#include <cctype>
#include <cstring>
#include <cstdlib>

#include <signal.h>

#include <string>
#include <vector>
#include <array>
#include <unordered_set>
#include <unordered_map>

#include <bitset>

#include <readline/readline.h>


#include "loader.h"
#include "address_map.h"

#include "debugger.h"

#include <cpu/defs.h>
#include <cpu/CpuModule.h>

#include <macos/traps.h>
#include <macos/sysequ.h>

#include <mpw/mpw.h>

namespace {

	const uint32_t kGlobalSize = 0x10000;

	bool sigInt = false;
	bool memBreak = false;
	void sigIntHandler(int)
	{
		sigInt = true;
	}

	AddressMap brkMap; // address breaks
	AddressMap rbrkMap; // read breaks.
	AddressMap wbrkMap; // write breaks.
	ToolMap tbrkMap; // tool breaks.

	std::unordered_map<std::string, uint16_t> toolMap;


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


	void printMacsbug(uint32_t pc, uint32_t opcode)
	{

		unsigned mboffset;
		switch(opcode)
		{
		case 0x4E75: // rts
		case 0x4ED0: // jmp (a0)
			mboffset = 2;
			break;
		case 0x4E74: // rtd #
			mboffset = 4;
			break;			
		default:
			return;
			break;
		}


		pc += mboffset;
		// check for MacsBug name after rts.
		std::string s;
		unsigned b = Debug::ReadByte(pc);
		if (b > 0x80 && b < 0xa0)
		{
			b -= 0x80;
			pc++;
			s.reserve(b);
			for (unsigned i = 0; i < b; ++i)
			{
				s.push_back(Debug::ReadByte(pc++));
			}
			printf("%s\n", s.c_str());
		}

	}


	uint32_t disasm(uint32_t pc, uint16_t *op = nullptr)
	{

		static char strings[4][256];

		if (pc >= Flags.memorySize)
		{
			if (op) *op = 0;
			return pc;
		}

		uint16_t opcode = Debug::ReadWord(pc);
		if (op) *op = opcode;

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
			return pc;
		}

		for (unsigned j = 0; j < 4; ++j) strings[j][0] = 0;

		uint32_t newpc = cpuDisOpcode(pc, strings[0], strings[1], strings[2], strings[3]);
		printf("%s   %-10s %-40s ; %s\n", strings[0], strings[2], strings[3], strings[1]);

		printMacsbug(pc, opcode);

		return newpc;
	}



	bool step(bool trace)
	{
		// return false to break (toolbreak, address break, etc.)

		uint16_t op;
		memBreak = false;

		cpuExecuteInstruction();

		uint32_t pc = cpuGetPC();
		if (trace) disasm(pc, &op);
		else op = Debug::ReadWord(pc);

		if (Flags.traceMacsbug && !trace)
			printMacsbug(pc, op);

		// will this also be set by an interrupt?
		if (cpuGetStop())
		{
			if (!trace) disasm(pc);
			printf("CPU stopped\n");
			return false; 
		}

		if (sigInt)
		{
			if (!trace) disasm(pc);
			printf("^C break\n");
			sigInt = false;
			return false; 			
		}

		if (memBreak)
		{
			printf("Memory break\n");
			memBreak = false;
			return false;
		}


		// check for pc breaks
		if (brkMap.lookup(pc))
		{
			if (!trace) disasm(pc);
			printf("Address break: $%08x\n", pc);
			return false;
		}

		// todo -- instruction break for rts /rtn

		// check for toolbreaks.
		if ((op & 0xf000) == 0xa000)
		{
			if (tbrkMap.lookup(op))
			{
				if (!trace) disasm(pc);
				printf("Tool break: $%04x\n", op);
				return false;
			}
		}

		if (pc > Flags.memorySize)
		{
			printf("PC out of range\n");
			return false;
		}

		uint32_t sp = cpuGetAReg(7);
		if (sp < Flags.stackRange.first)
		{
			printf("Stack overflow error\n");
			return false;
		}

		if (sp > Flags.stackRange.second)
		{
			printf("Stack underflow error\n");
			return false;
		}

		return true;
	}


	void MemoryLogger(uint32_t address, int size, int readWrite, uint32_t value)
	{

		if (address < kGlobalSize && Flags.traceGlobals)
		{
			const char *name = GlobalName(address);
			if (!name) name = "unknown";

			fprintf(stdout, "%-20s %08x - ", name, address);
			if (readWrite)
			{
				fprintf(stdout, " write %d bytes", size);
				switch(size)
				{
					case 1:
						fprintf(stdout, " [%02x]\n", value);
						break;
					case 2:
						fprintf(stdout, " [%04x]\n", value);
						break;
					case 3:
						fprintf(stdout, " [%06x]\n", value);
						break;
					case 4:
						fprintf(stdout, " [%08x]\n", value);
						break;				
					default:
						fprintf(stdout, "\n");
						break;
				}
			}
			else
			{
				fprintf(stdout, " read  %d bytes\n", size);
			}
		}


		// check for memory breaks.
		if (readWrite)
		{
			// todo -- what if writing 1 byte 4-bit address?
			// todo -- if not single stepping at time of break, should
			// disasm the prev pc before printing.


			if (!wbrkMap.lookup(address)) return;

			printf("Write $%08x\n", address);
			// todo -- print previous value, old value.

			memBreak = true;
		}
		else
		{
			if (!rbrkMap.lookup(address)) return;

			printf("Read $%08x\n", address);

			memBreak = true;
		}
	}



}

#pragma mark - Debugger

namespace Debug {

uint32_t ReadLong(uint32_t address)
{
	uint32_t tmp = 0;
	for (unsigned i = 0; i < 4; ++i)
	{
		if (address < Flags.memorySize)
			tmp = (tmp << 8) + Flags.memory[address++];
	}

	return tmp;
}


uint16_t ReadWord(uint32_t address)
{
	uint16_t tmp = 0;
	for (unsigned i = 0; i < 2; ++i)
	{
		if (address < Flags.memorySize)
			tmp = (tmp << 8) + Flags.memory[address++];
	}

	return tmp;
}

uint8_t ReadByte(uint32_t address)
{
	if (address < Flags.memorySize)
		return Flags.memory[address];

	return 0;
}



void Help()
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


void Print(uint32_t data)
{

	// 32-bit unsigned int
	printf("$%08x %12u", data, data);

	// 32-bit signed int
	if (data & 0x80000000)
		printf(" %12d", (int32_t)data);

	// 16-bit signed int
	if ((data & 0xffff8000) == 0x8000)
		printf(" %6d", (int16_t)data);

	// 4-cc code? 2-cc code? 1-cc code?
	char tmp[5];
	int bits = 0;
	tmp[0] = (data >> 24) & 0xff;
	tmp[1] = (data >> 16) & 0xff;
	tmp[2] = (data >> 8) & 0xff;
	tmp[3] = (data >> 0) & 0xff;
	tmp[4] = 0;

	if (isprint(tmp[0])) bits |= (1 << 0);
	if (isprint(tmp[1])) bits |= (1 << 1);
	if (isprint(tmp[2])) bits |= (1 << 2);
	if (isprint(tmp[3])) bits |= (1 << 3);

	switch(bits)
	{
		case 0x0f:
			printf(" '%s'", tmp);
			break;

		case 0x0e:
			if (data <= 0xffffff)
				printf(" '%s'", tmp + 1);
			break;

		case 0x0c:
			if (data <= 0xffff)
				printf(" '%s'", tmp + 2);
			break;
			
		case 0x08:
			if (data <= 0xff)
				printf(" '%s'", tmp + 3);
			break;
	}


	printf("\n");

}


void Dump(uint32_t start, int size)
{
	// TODO -- if no address, use previous address.
	// TODO -- support range?


	if (size <= 0) return;

	uint32_t end = start + size;

	if (start >= Flags.memorySize) return;

	end = std::min(end, Flags.memorySize);
	size = end - start;

	hexdump(Flags.memory + start, size, start);
}



// grr... need separate count/range options.
void List(uint32_t pc, int count)
{
	// TODO -- if no address, use previous address.
	if (pc & 0x01)
	{
		printf("address is not aligned: $%08x\n", pc);
		return;
	}

	for (int i = 0; i < count; ++i)
	{
		if (pc >= Flags.memorySize) break;
		pc = disasm(pc);
	}
}

void List(uint32_t pc, uint32_t endpc)
{
	if (endpc < pc) return;

	if (pc & 0x01)
	{
		printf("address is not aligned: $%08x\n", pc);
		return;
	}

	while (pc <= endpc)
	{
		if (pc >= Flags.memorySize) break;
		pc = disasm(pc);
	}

}


void PrintRegisters()
{
	char srbits[20];

	uint16_t sr = cpuGetSR();

	srbits[0] = sr & (1 << 15) ? 'T' : ' ';
	srbits[1] = sr & (1 << 14) ? 'T' : ' ';
	srbits[2] = sr & (1 << 13) ? 'S' : ' ';
	srbits[3] = sr & (1 << 12) ? 'M' : ' ';
	srbits[4] = ' ';
	srbits[5] = sr & (1 << 10) ? 'I' : ' ';
	srbits[6] = sr & (1 << 9) ? 'I' : ' ';
	srbits[7] = sr & (1 << 8) ? 'I' : ' ';
	srbits[8] = ' ';
	srbits[9] = ' ';
	srbits[10] = ' ';
	srbits[11] = sr & (1 << 4) ? 'X' : ' ';
	srbits[12] = sr & (1 << 3) ? 'N' : ' ';
	srbits[13] = sr & (1 << 2) ? 'Z' : ' ';
	srbits[14] = sr & (1 << 1) ? 'V' : ' ';
	srbits[15] = sr & (1 << 0) ? 'C' : ' ';
	srbits[16] = 0;


	printf("   0        1        2        3        4        5        6        7\n");	
	printf("D: %08x %08x %08x %08x %08x %08x %08x %08x\n",
		cpuGetDReg(0), cpuGetDReg(1), cpuGetDReg(2), cpuGetDReg(3), 
		cpuGetDReg(4), cpuGetDReg(5), cpuGetDReg(6), cpuGetDReg(7)

	);

	printf("A: %08x %08x %08x %08x %08x %08x %08x %08x\n",
		cpuGetAReg(0), cpuGetAReg(1), cpuGetAReg(2), cpuGetAReg(3), 
		cpuGetAReg(4), cpuGetAReg(5), cpuGetAReg(6), cpuGetAReg(7)
	);

	printf("PC: %08X CSR: %04x %s\n", cpuGetPC(), sr, srbits);

}


void ToolBreak(int32_t tool)
{

	bool remove = false;

	if (tool < 0)
	{
		tool = -tool;
		remove = true;
	}

	if (tool >= 0xa000 && tool <= 0xafff)
	{
		if (remove) tbrkMap.remove(tool);
		else tbrkMap.add(tool);
	}
	else
	{
		fprintf(stderr, "Invalid tool: $%04x\n", tool);
	}
}

void ToolBreak()
{
	// list all tool breaks.

	if (!tbrkMap.size())
	{
		printf("No tool breaks\n");
		return;
	}

	std::vector<unsigned> v;
	v.reserve(tbrkMap.size());

	for (auto kv : tbrkMap)
	{
		v.push_back(kv.first);
	}

	std::sort(v.begin(), v.end());

	for (auto trap : v)
	{
		const char *name = TrapName(trap);
		if (!name) name = "";
		printf("$%04x %s\n", trap, name);
	}
}

void Break(int32_t address)
{
	// 24-bit only, - address to remove.

	bool remove = false;

	if (address < 0)
	{
		address = -address;
		remove = true;
	}

	if ((address & 0xff000000) == 0)
	{
		if (remove) brkMap.remove(address);
		else brkMap.add(address);
	}
	else
	{
		fprintf(stderr, "Invalid address: $%08x\n", address);
	}
}


void Break()
{
	// list all tool breaks.

	if (!brkMap.size())
	{
		printf("No breaks\n");
		return;
	}

	std::vector<unsigned> v;
	v.reserve(brkMap.size());

	for (auto kv : brkMap)
	{
		v.push_back(kv.first);
	}

	std::sort(v.begin(), v.end());

	for (auto address : v)
	{
		printf("$%08x", address);
	}
}


void ReadBreak()
{}

void WriteBreak()
{}



void ReadWriteBreak()
{
}



void ReadBreak(int32_t address)
{
	// sanity check address?

	if (address < 0) rbrkMap.remove(-address);
	else rbrkMap.add(address);
}

void WriteBreak(int32_t address)
{
	// sanity check address?
	if (address < 0) wbrkMap.remove(-address);
	else wbrkMap.add(address);
}

void ReadWriteBreak(int32_t address)
{
	ReadBreak(address);
	WriteBreak(address);
}

void Step(const Command &cmd)
{
	// TODO - step n to step specified # of instructions.
	// TODO -- step @address to step until address?

	// disasm 1 line, execute it.

	int count = 0;
	if (cmd.argc == 1) count = (int)cmd.argv[0];
	if (count < 1) count = 1;
	
	// TODO -- move to common function...
	for (int i = 0; i < count; ++i)
	{
		if (!step(true)) break;
	}
}


void Continue(const Command &cmd)
{
	while (step(false)) ;
}

void SetARegister(unsigned reg, uint32_t value)
{
	if (reg > 7) return;
	if (reg == 7)
	{
		// sp/7 must be aligned.
		if (value & 0x01)
		{
			fprintf(stderr, "Address is not aligned: $%08x\n", value);
			return;
		}
	}

	if (value > Flags.memorySize)
	{
		fprintf(stderr, "Warning: address exceeeds memory size: $%08x\n", value);
	}

	cpuSetAReg(reg, value);
}


void SetDRegister(unsigned reg, uint32_t value)
{
	if (reg > 7) return;

	cpuSetDReg(reg, value);
}

void SetXRegister(unsigned reg, uint32_t value)
{

	if (reg == 0)
	{
		if (value & 0x01)
		{
			fprintf(stderr, "Address is not aligned: $%08x\n", value);
			return;			
		}
		if (value > Flags.memorySize)
		{
			fprintf(stderr, "Warning: address exceeeds memory size: $%08x\n", value);
		}
		//cpuSetPC(value);
		cpuInitializeFromNewPC(value);
		return;
	}

	if (reg == 1)
	{
		cpuSetSR(value & 0xffff);
	}

}


uint16_t TrapNumber(const std::string &s)
{
	auto iter = toolMap.find(s);
	if (iter == toolMap.end()) return 0;
	return iter->second;
}

uint16_t TrapNumber(const char *cp)
{
	if (!cp || !*cp) return 0;
	std::string s(cp);
	return TrapNumber(s);
}


// TODO -- RUN command - reload, re-initialize, re-execute
// TODO -- parser calls commands directly (except trace/step/run/etc)
void Shell()
{
	char *cp;

	add_history("!Andy, it still has history!");

	{
		// load the tool trap file.
		std::string path = MPW::RootDir();
		path += "/Traps.text";
		toolMap = LoadTrapFile(path);
	}

	// start it up
	printf("MPW Debugger shell\n\n");
	disasm(cpuGetPC());

	signal(SIGINT, sigIntHandler);
	memorySetLoggingFunc(MemoryLogger);

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
					case cmdNull:
						break;

					case cmdStep:
						Step(cmd);
						break;

					case cmdContinue:
						Continue(cmd);
						break;

					default:
						Help();
						break;
				}
			}

			// todo -- don't add if same as previous command.
			add_history(cp);
		}
		free(cp);
	}

}

} // namespace Debugger


