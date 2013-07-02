#include <cstdio>
#include <cstdint>
#include <cassert>

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

	while (offset < data_size)
	{
		uint16_t off = memoryReadWord(offset);
		uint16_t seg = -1;
		if (memoryReadWord(offset + 2) == 0x3F3C && memoryReadWord(offset + 6) == 0xA9F0)
		{
			uint16_t seg = memoryReadWord(offset + 4);

			// pc +2 since the first byte is the offset, not code.
			printf("$%08X   %04X : %04X\n", pc + 2, seg, off);
		}
		else
		{
			printf("$%08X   ???\n", pc + 2);
		}
		offset += 8;
		pc += 8;
	}

	printf("\n\n");
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
				for (unsigned i = 0; i < len; ++ i)
				{

					s.push_back(memoryReadByte(pc + 1 + i));
				}

				printf("%s\n\n", s.c_str());
				pc = (pc + 1) & ~0x01;
				// if next word is 0, skip as well.
				if (memoryReadWord(pc) == 0) pc += 2; 

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
			// 4-byte header for segment stuff.
			data += 4;
			size -= 4;
			memorySetMemory(data, size);
			disasm(cname.c_str(), resID, size);
		}

		memorySetMemory(nullptr, 0);

		ReleaseResource(h);
	}


    CloseResFile(refNum);

    return 0;
}