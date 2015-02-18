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

#include <cstdio>
#include <cstdint>
#include <cassert>
#include <cctype>

#include <string>

#include <CoreServices/CoreServices.h>

#include <cpu/defs.h>
#include <cpu/fmem.h>
#include <cpu/CpuModule.h>

#include <macos/traps.h>
#include <macos/sysequ.h>

char strings[4][256];

const uint8_t *Memory = NULL;
uint32_t MemorySize = 0;


void ToolBox(uint32_t pc, uint16_t trap)
{
	const char *name;

	name = TrapName(trap);

	if (name)
	{
		printf("$%08X   %-51s ; %04X\n", pc, name, trap);
	}
	else
	{
		printf("$%08X   Tool       #$%04X                                   ; %04X\n", pc, trap, trap);
	}
}

void help()
{

}


void code0(uint32_t data_size)
{
	// dump the code 0 jump table.

	uint32_t offset = 0;
	uint32_t pc;
	uint32_t size;
	printf("Above A5:          $%08X\n", memoryReadLong(0));
	printf("Below A5:          $%08X\n", memoryReadLong(4));
	printf("Jump Table Size:   $%08X\n", size = memoryReadLong(8));
	printf("Jump Table Offset: $%08X\n", pc = memoryReadLong(12)); // aka CurJTOffset.  should be 32.

	offset = 16;

	bool longA5 = false;
	while (offset < data_size)
	{

		if (longA5)
		{
			uint16_t segment = memoryReadWord(offset);
			uint32_t segOffset = memoryReadLong(offset + 4);

			if (memoryReadWord(offset + 2) == 0xA9F0)
			{
				printf("$%08X   %04X : %08X\n", pc + 2, segment, segOffset);
			}
			else
			{
				printf("$%08X   ???\n", pc + 2);
			}
		}
		else
		{
			uint16_t data[4];
			for (unsigned i = 0; i < 4; ++i)
				data[i] = memoryReadWord(offset + 2 * i);

			if (data[1] == 0xffff)
			{
				longA5 = true;
				printf("--------\n");
			}
			else if (data[1] == 0x3F3C && data[3] == 0xA9F0)
			{
				uint16_t segOffset = data[0];
				uint16_t segment = data[2];

				// pc +2 since the first byte is the offset, not code.
				printf("$%08X   %04X : %04X\n", pc + 2, segment, segOffset);
			}
			else
			{
				printf("$%08X   ???\n", pc + 2);
			}
		}
		offset += 8;
		pc += 8;
	}

	printf("\n\n");
}

inline char *cc2(uint16_t value, char out[3])
{
	uint8_t tmp;

	tmp = (value >> 8) & 0xff;
	if (tmp & 0x80 || !isprint(tmp)) out[0] = '.';
	else out[0] = tmp;

	tmp = (value >> 0) & 0xff;
	if (tmp & 0x80 || !isprint(tmp)) out[1] = '.';
	else out[1] = tmp;

	out[2] = 0;
	return out;
}

void disasm(const char *name, int segment, uint32_t data_size)
{

	if (name && *name) printf("segment %d - %s\n", segment, name);
	else printf("segment %d\n", segment);

	uint32_t pc = 0;


	uint16_t prevOP = 0;

	while (pc < data_size)
	{
		for (unsigned j = 0; j < 4; ++j) strings[j][0] = 0;

		uint16_t op = memoryReadWord(pc);

		if (prevOP == 0x4E75  || prevOP == 0x4ED0 || prevOP == 0x4E74)
		{
			if (op > 0x8000)
			{
				// RTS followed by debug symbol.
				unsigned len = (op >> 8) - 0x80;

				std::string s;
				s.reserve(len);
				pc += 1; // skip the length byte.
				for (unsigned i = 0; i < len; ++ i)
				{
					s.push_back(memoryReadByte(pc++));
				}

				printf("%s\n", s.c_str());
				pc = (pc + 1) & ~0x01;

#if 0
				if (memoryReadWord(pc) == 0x0000) pc += 2;
#else
				// treat the remainder as data until 4E56. [Link A6,#]
				while (pc < data_size)
				{
					char tmp[3];
					uint16_t data = memoryReadWord(pc);
					if (data == 0x4e56) break;
					printf("$%08X   $%04X                                               ; '%s'\n", pc, data, cc2(data, tmp));
					pc += 2;
				}
#endif
				printf("\n");

				prevOP = 0;
				continue;
			}
		}

		if ((op & 0xf000) == 0xa000)
		{
			// tool call!

			ToolBox(pc, op);
			pc += 2;
			prevOP = op;
			continue;
		}

		pc = cpuDisOpcode(pc, strings[0], strings[1], strings[2], strings[3]);

		// address, data, instruction, operand
		printf("%s   %-10s %-40s ; %s\n", strings[0], strings[2], strings[3], strings[1]);

		prevOP = op;
	}

	printf("\n\n");
}

int main(int argc, char **argv)
{

	ResFileRefNum refNum;
	FSRef ref;
	Handle h;

	if (argc != 2)
	{
		help();
		return -1;
	}

	assert( FSPathMakeRef( (const UInt8 *)argv[1], &ref, NULL) == noErr);

    refNum = FSOpenResFile(&ref, fsRdPerm);
    assert(refNum != -1 );
    //UseResFile(refNum);


	cpuSetModel(3, 0); // 68030

	int l = Count1Resources('CODE');

	for (int i = 0; i < l; ++i)
	{
		ResAttributes attr;
		ResID resID;
		ResType resType;
		Str255 name;
		std::string cname;

		h = Get1IndResource('CODE', i + 1);
		if (!h) continue;
		HLock(h);


		attr = GetResAttrs(h);
		GetResInfo(h, &resID, &resType, name);

		// name is a p-string.
		if (name[0])
		{
			int l = name[0];
			cname.assign(name + 1, name + 1 + l);
		}

		uint32_t size = GetHandleSize(h);
		uint8_t *data = *( uint8_t **)h;

		if (resID == 0)
		{
			memorySetMemory(data, size);
			code0(size);
		}
		else
		{
			// near model uses a $4-byte header.
			// far model uses a $28-byte header.
			if (data[0] == 0xff && data[1] == 0xff)
			{
				data += 0x28;
				size -= 0x28;
			}
			else
			{
				data += 0x04;
				size -= 0x04;
			}
			memorySetMemory(data, size);
			disasm(cname.c_str(), resID, size);
		}

		memorySetMemory(nullptr, 0);

		ReleaseResource(h);
	}


    CloseResFile(refNum);

    return 0;
}
