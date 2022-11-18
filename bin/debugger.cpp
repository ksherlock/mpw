/*
 * Copyright (c) 2013, Kelvin W Sherlock
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <cstdint>
#include <cctype>
#include <cstring>
#include <cstdlib>
#include <algorithm>

#include <signal.h>

#include <string>
#include <vector>
#include <array>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <deque>

#include <bitset>

#include <readline/readline.h>
#include <readline/history.h>


#include "loader.h"
#include "address_map.h"

#include "debugger.h"
#include "debugger_internal.h"
#include "template.h"

#include <cpu/defs.h>
#include <cpu/CpuModule.h>

#include <macos/traps.h>
#include <macos/sysequ.h>
#include <macos/errors.h>

#include <mpw/mpw.h>

#include <toolbox/loader.h>
#include <toolbox/mm.h>

namespace {

	using namespace Debug::Internal;

	const uint32_t kGlobalSize = 0x10000;
	const uint32_t kBackTraceSize = 50;

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


	std::unordered_map<std::string, Debug::Template> TemplateTable;

	struct BackTraceInfo {
		uint32_t a[8];
		uint32_t d[8];
		uint32_t pc;
		uint16_t csr;

		BackTraceInfo(bool populate = false) {
			if (populate)
			{
				for (unsigned i = 0; i < 8; ++i) a[i] = cpuGetAReg(i);
				for (unsigned i = 0; i < 8; ++i) d[i] = cpuGetDReg(i);
				pc = cpuGetPC();
				csr = cpuGetSR();
			}
		}
	};

	std::deque<BackTraceInfo> BackTrace;




	void printMacsbug(uint32_t pc, uint32_t opcode, uint32_t *newPC = nullptr)
	{
		// pc is actually pc after the opcode.

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
		if (b >= 0x80 && b <= 0x9f)
		{
			b -= 0x80;
			pc++;
			if (!b) b = Debug::ReadByte(pc++);

			s.reserve(b);
			for (unsigned i = 0; i < b; ++i)
			{
				s.push_back(Debug::ReadByte(pc++));
			}
			printf("            %s\n\n", s.c_str());

			// word-align
			pc = pc + 1 & ~0x01;

			// and possibly a zero-word after it.
			if (Debug::ReadWord(pc) == 0x0000) pc += 2;

			if (newPC) *newPC = pc;
		}

	}


	// TODO -- state indicator for code/data
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


		// replace jsr address w/ jsr macsbug name, if possible.

		// jsr offset(pc)
		uint32_t address = 0;

		switch (opcode)
		{
			case 0x4EBA: // jsr offset(pc)
			{
				int16_t offset = Debug::ReadWord(pc + 2);
				address = pc + 2 + offset;
				break;
			}
			case 0x4EB9: // jsr address
			{
				address = Debug::ReadLong(pc + 2);
				break;
			}
			case 0x4EF9: // jmp address
			{
				address = Debug::ReadLong(pc + 2);
				break;
			}
			case 0x4EAD: // jsr offset(a5)
			{
				// check if address is a jmp address (see above)
				// and follow it. a5 should never change.
				int16_t offset = Debug::ReadWord(pc + 2);
				address = cpuGetAReg(5) + offset;

				if (Debug::ReadWord(address) == 0x4EF9)
					address = Debug::ReadLong(address + 2);
				else address = 0;

				break;
			}

			// consider checking branches?
		}

		if (address) {

			auto iter = SymbolTableInvert.find(address);
			if (iter != SymbolTableInvert.end())
			{
				strncpy(strings[3], iter->second.c_str(), 40);
				strings[3][40] = 0;
			}
		}


		printf("%s   %-10s %-40s ; %s\n", strings[0], strings[2], strings[3], strings[1]);

		printMacsbug(pc, opcode, &newpc);

		return newpc;
	}


	bool step(bool trace)
	{
		// return false to break (toolbreak, address break, etc.)

		uint16_t op;
		memBreak = false;




		//uint32_t prevPC = cpuGetPC();

		// backtracing.  store contents before the instruction.
		if (BackTrace.size() == kBackTraceSize)
		{
			BackTrace.pop_front();
		}
		BackTrace.emplace_back(true);

		//BackTrace.back().pc = prevPC;

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
			if (!trace) disasm(pc);
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


	static void LogWrite(int size, uint32_t value)
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

	void MemoryLogger(uint32_t address, int size, int readWrite, uint32_t value)
	{

		if (address < kGlobalSize && Flags.traceGlobals)
		{
			const char *name = GlobalName(address);
			if (!name) name = "unknown";

			fprintf(stdout, "%-20s %08x - ", name, address);
			if (readWrite)
			{
				LogWrite(size, value);
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

			printf("Memory Break $%08x - ", address);
			LogWrite(size, value);
			// todo -- print previous value, old value.

			memBreak = true;
		}
		else
		{
			if (!rbrkMap.lookup(address)) return;

			printf("Memory Break $%08x - read %d bytes\n", address, size);

			memBreak = true;
		}
	}



}

#pragma mark - Debugger

namespace Debug {

std::string ReadPString(uint32_t address)
{
	std::string tmp;
	unsigned size = ReadByte(address++);

	tmp.reserve(size);
	for (unsigned i = 0; i < size; ++i)
		tmp.push_back(ReadByte(address++));


	return tmp;
}

std::string ReadCString(uint32_t address)
{
	std::string tmp;

	for (;;)
	{
		char c = ReadByte(address++);
		if (!c) break;
		tmp.push_back(c);
	}

	return tmp;
}


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
	printf("bt | backtrace     -- print cpu backtrace\n");
	printf("expression;h       -- print hexdump\n");
	printf("expression;i       -- print information\n");
	printf("expression;l       -- print assembly listing\n");
	printf("\n");
	printf("registers: a0-7, d0-7, pc, sp, fp, csr\n");
	printf("\n");
}


void Print(uint32_t data)
{

	// 32-bit unsigned int
	printf("$%08x", data);



	// 32-but unsigned
	printf(" %12u", data);

	// 32-bit signed int
	int32_t negValue = 0;
	if (data & 0x80000000)
		negValue = (int32_t)data;

	if ((data & 0xffff8000) == 0x8000)
		negValue = (int16_t)data;

	if (negValue != 0)
		printf(" %12d", negValue);
	else printf("             ");




	// print binary value
	{
		std::string bins;
		bins.reserve(32);

		bins.push_back('%');

		if (data > 0xffff)
		{
			for (unsigned i = 0, mask = 0x80000000; i < 16; ++i, mask >>= 1)
				bins.push_back( data & mask ? '1' : '0');
		}

		if (data > 0xff)
		{
			for (unsigned i = 0, mask = 0x8000; i < 8; ++i, mask >>= 1)
				bins.push_back( data & mask ? '1' : '0');
		}

		for (unsigned i = 0, mask = 0x80; i < 8; ++i, mask >>= 1)
			bins.push_back( data & mask ? '1' : '0');


		printf(" %33s", bins.c_str());
	}



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
	if ((int32_t)endpc < (int32_t)pc) return;

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

const char *srBits(uint16_t sr)
{
	static char srbits[20];

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

	return srbits;
}


void PrintRegisters(const BackTraceInfo &i)
{
	const char *srbits = srBits(i.csr);

	printf("    0        1        2        3        4        5        6        7\n");
	printf("D:  %08x %08x %08x %08x %08x %08x %08x %08x\n",
		i.d[0], i.d[1], i.d[2], i.d[3],
		i.d[4], i.d[5], i.d[6], i.d[7]

	);

	printf("A:  %08x %08x %08x %08x %08x %08x %08x %08x\n",
		i.a[0], i.a[1], i.a[2], i.a[3],
		i.a[4], i.a[5], i.a[6], i.a[7]
	);

	printf("PC: %08x CSR: %04x %s\n", i.pc, i.csr, srbits);

}

void PrintRegisters()
{
	uint16_t sr = cpuGetSR();
	const char *srbits = srBits(sr);

	printf("    0        1        2        3        4        5        6        7\n");
	printf("D:  %08x %08x %08x %08x %08x %08x %08x %08x\n",
		cpuGetDReg(0), cpuGetDReg(1), cpuGetDReg(2), cpuGetDReg(3),
		cpuGetDReg(4), cpuGetDReg(5), cpuGetDReg(6), cpuGetDReg(7)

	);

	printf("A:  %08x %08x %08x %08x %08x %08x %08x %08x\n",
		cpuGetAReg(0), cpuGetAReg(1), cpuGetAReg(2), cpuGetAReg(3),
		cpuGetAReg(4), cpuGetAReg(5), cpuGetAReg(6), cpuGetAReg(7)
	);

	printf("PC: %08X CSR: %04x %s\n", cpuGetPC(), sr, srbits);

}

void btdiff(const BackTraceInfo &prev, const BackTraceInfo &current)
{
	bool nl = false;

	for (unsigned i = 0; i < 8; ++i)
		if (current.d[i] != prev.d[i]) {
			printf("            D%u: %08x\n", i, current.d[i]);
			nl = true;
		}

	for (unsigned i = 0; i < 8; ++i)
		if (current.a[i] != prev.a[i]) {
			printf("            A%u: %08x\n", i, current.a[i]);
			nl = true;
		}


	// pc always changes, but it's included in the listing.
	//printf("PC: %08x\n", current.pc);
	if (current.csr != prev.csr) {
		printf("            CSR: %04x %s\n", current.csr, srBits(current.csr));
		nl = true;
	}

	if (nl) printf("\n");
}
void PrintBackTrace()
{
	// backtrace.

	auto iter = BackTrace.cbegin();
	auto end = BackTrace.cend();

	if (iter == end) return;

	const BackTraceInfo *prev;

	{
	 	const BackTraceInfo &info = *iter;

		// print all registers for the first entry.
		// for subsequent entries, print changed registers.

		//disasm(info.pc);
		PrintRegisters(info);
		printf("\n");

		prev = &info;
	 	++iter;
	 }



	for ( ; iter != end; ++iter)
	{
		const BackTraceInfo &current = *iter;

		disasm(prev->pc);
		btdiff(*prev, current);

		//
		prev = &current;
	}

	// print current registers.
	{
		BackTraceInfo current(true);
		disasm(prev->pc);
		btdiff(*prev, current);
	}

	// finally, print the current instruction.
	printf("---------\n");
	disasm(cpuGetPC());
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

	// 24-bit address only.
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
		printf("$%08x\n", address);
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


// todo -- return a range
uint32_t VariableGet(const std::string &s)
{
	auto iter = SymbolTable.find(s);
	if (iter == SymbolTable.end()) return 0;
	return iter->second.first;
}

// TODO -- take a Token and construct a pair, if it's a range.
// var = expr : expr or var = expr@count

void VariableSet(const std::string &key, uint32_t value)
{
	SymbolTable.emplace(key, std::make_pair(value, 0));
}

void Info(uint32_t address)
{
	// print info on the value.

	Print(address);

	// 1. as a pointer.
	MM::Native::MemoryInfo(address);

	// 2. (todo) - check SymbolTable for procedure address.
	for (const auto &kv : SymbolTable)
	{
		const auto &name = kv.first;
		auto range = kv.second;

		//printf("%s: %x %x\n", name.c_str(), range.first, range.second);

		// range end may be 0
		if ((address == range.first) || (address >= range.first && address < range.second))
		{
			uint32_t offset = address - range.first;
			if (offset)
				printf("Routine: %s+$%x\n", name.c_str(), offset);
			else
				printf("Routine: %s\n", name.c_str());
			break;
		}
	}


	// 2. as a tool trap.
	if (address >= 0xa000 && address <= 0xafff)
	{
		const char *cp = TrapName(address);
		if (cp)
			printf("Tool: %s\n", cp);
	}

	// 3. as a global
	if (address <= 0xffff)
	{
		const char *cp = GlobalName(address);
		if (cp)
			printf("Global: %s\n", cp);

	}

	// 4 as an error
	// almost all are negative 16-bit values,
	// but may also be a positive 16-bit value.
	uint16_t error = 0;
	if (address <= 0xffff) error = address;
	if ((address & 0xffff8000) == 0xffff8000) error = address;
	if (error)
	{
		const char *cp = ErrorName(error);



		// find returns an iterator to the hash bucket, which may contain
		// other errors.
		auto range = ErrorTableInvert.equal_range(error);

		for(auto iter = range.first; iter != range.second; ++iter) {

			printf("Error: %s", iter->second.c_str());
			if (cp) printf(" %s", cp);
			printf("\n");
			cp = nullptr;
		}
	}

}

	void ApplyTemplate(int32_t address, const std::string &name)
	{
		// find the template..

		auto iter = TemplateTable.find(name);
		if (iter == TemplateTable.end()) {
			fprintf(stderr, "Unknown template: %s\n", name.c_str());
			return;
		}

		FieldEntry *e = iter->second;
		ApplyTemplate(address, e);
	}


namespace {

	/*
	 * returns a list of possible matches.
	 * Item[0] is the longest match.
	 */
	char **mpw_attempted_completion_function(const char* text, int _start, int _end)
	{
		std::string s(text);

		// returns iter to first element _not less_ than key
		// ie, >= key.
		auto iter = SymbolTable.lower_bound(s);

		unsigned count = 0;
		unsigned length = s.length();

		auto begin = iter;

		for (;;)
		{
			if (iter == SymbolTable.end()) break;
			if (iter->first.compare(0, length, s) != 0) break;

			++count;
			++iter;
		}
		auto end = iter;

		if (!count) return NULL;
		if (count > 100) return NULL;

		if (count == 1)
		{
			char **buffer = (char **)malloc(3 * sizeof(char *));
			buffer[0] = strdup(begin->first.c_str()); // longest substring match
			buffer[1] = strdup(begin->first.c_str()); // the match
			buffer[2] = NULL;
			return buffer;
		}

		char **buffer = (char **)malloc((count + 2) * sizeof(char *));

		unsigned i = 0;
		auto min_length = begin->first.length();

		// item 0 is the longest match. (fill in later.)
		buffer[i++] = NULL;
		for (iter = begin; iter != end; ++iter)
		{
			buffer[i++] = strdup(iter->first.c_str());
			min_length = std::min(min_length, iter->first.length());
		}
		buffer[i] = NULL;


		// assume the first is the longest, then search until not true.
		buffer[0] = strdup(begin->first.c_str());
		for (unsigned i = length; ; ++i)
		{

			if (i >= min_length)
			{
				buffer[0][i] = 0;
				break;
			}

			char c = buffer[0][i];
			if (!c) break;

			bool nomatch = false;
			for (int j = 1; ; ++j)
			{
				char *cp = buffer[j];
				if (!cp) break;
				if (cp[i] != c) nomatch = true;
			}

			if (nomatch)
			{
				buffer[0][i] = 0;
				break;
			}
		}

		return buffer;
	}

	// this is here to prevent filename tab completion, for now.
	// state is 0 for first call, non-zero for subsequent calls.  It
	// should return 1 match per invocation, NULL if no more matches.
	char *mpw_completion_entry_function(const char *text, int state)
	{
		return NULL;
	}

	void readline_init()
	{
		rl_readline_name = (char *)"mpw";
		rl_attempted_completion_function = mpw_attempted_completion_function;
		rl_completion_entry_function = (rl_compentry_func_t *)mpw_completion_entry_function;
	}
}

// TODO -- RUN command - reload, re-initialize, re-execute
// TODO -- parser calls commands directly (except trace/step/run/etc)
void Shell()
{
	char *cp;

	readline_init();

	add_history("!Andy, it still has history!");


	Loader::Native::LoadDebugNames(SymbolTable);

	LoadTrapFile(MPW::RootDirPathForFile("Errors.text"), ErrorTable);
	LoadTrapFile(MPW::RootDirPathForFile("Globals.text"), GlobalTable);
	LoadTrapFile(MPW::RootDirPathForFile("Traps.text"), TrapTable);

	LoadTemplateFile(MPW::RootDirPathForFile("Templates.text"), TemplateTable);



	// load the error code to error mnemonic
	ErrorTableInvert.reserve(ErrorTable.size());
	for (const auto &kv : ErrorTable) {
		ErrorTableInvert.emplace(std::make_pair(kv.second, kv.first));
	}

	// address to function name.
	SymbolTableInvert.reserve(SymbolTable.size());
	for (const auto &kv : SymbolTable) {
		SymbolTableInvert.emplace(std::make_pair(kv.second.first, kv.first));
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

			// don't add if same as previous entry.
			HIST_ENTRY *he = current_history();
			if (he == nullptr || strcmp(he->line, cp) != 0)
				add_history(cp);
		}
		free(cp);
	}

}

} // namespace Debugger
