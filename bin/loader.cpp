// clang++ -c -std=c++11 -stdlib=libc++ -Wno-deprecated-declarations loader.cpp

#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <sysexits.h>
#include <getopt.h>

#include <CoreServices/CoreServices.h>

#include <cpu/defs.h>
#include <cpu/CpuModule.h>
#include <cpu/fmem.h>

uint8_t *Memory;
uint32_t HighWater = 0x1000;
uint32_t MemorySize = 0;

uint32_t EmulatedNewPtr(uint32_t size)
{
	if (size & 0x01) size++;

	if (HighWater + size > MemorySize)
	{
		fprintf(stderr, "Insufficient Memory!\n");
		exit(EX_CONFIG);
	}


	uint32_t address = HighWater;


	HighWater += size;

	std::memset(Memory + HighWater, 0, size);

	return address;
}

uint8_t ReadByte(const void *data, uint32_t offset)
{
	offset &= 0xffffff;
	if (offset >= MemorySize) return 0;
	return ((uint8_t *)data)[offset];
}

uint16_t ReadWord(const void *data, uint32_t offset)
{
	offset &= 0xffffff;
	return (ReadByte(data, offset) << 8) | ReadByte(data, offset+1);
}

uint32_t ReadLong(const void *data, uint32_t offset)
{
	offset &= 0xffffff;
	return (ReadWord(data, offset) << 16) | ReadWord(data, offset+2);
}

void WriteByte(void *data, uint32_t offset, uint8_t value)
{
	offset &= 0xffffff;
	if (offset >= MemorySize) return;
	((uint8_t *)data)[offset] = value;
}

void WriteWord(void *data, uint32_t offset, uint16_t value)
{
	offset &= 0xffffff;

	if (offset + 1 >= MemorySize) return;

	((uint8_t *)data)[offset++] = value >> 8;
	((uint8_t *)data)[offset++] = value;
}

void WriteLong(void *data, uint32_t offset, uint32_t value)
{
	offset &= 0xffffff;

	if (offset + 3 >= MemorySize) return;

	((uint8_t *)data)[offset++] = value >> 24;
	((uint8_t *)data)[offset++] = value >> 16;
	((uint8_t *)data)[offset++] = value >> 8;
	((uint8_t *)data)[offset++] = value;
}

/*
//0x0316 = MPWArgs *
struct MPWArgs
{
	uint32_t MPGM; // = 'MPGM';
	int32_t argptr;

	args:
	uint16_t SH;
	uint32_t argc;
	uint32_t argv;
	uint32_t envp;

	envp stored as key\0value\0, not key=value\0
};

0x910 = name ptr (32-char pstring)

*/

bool load(const char *file)
{

	ResFileRefNum refNum;
	FSRef ref;

	std::vector< std::pair<uint32_t, uint32_t> > segments; // segment, address


	uint32_t a5 = 0;

	uint32_t jtStart = 0;
	uint32_t jtEnd = 0;

	assert(FSPathMakeRef( (const UInt8 *)file, &ref, NULL) == noErr);
    refNum = FSOpenResFile(&ref, fsRdPerm);
    assert(refNum != -1 );

	int l = Count1Resources('CODE');
	segments.reserve(l);

	assert(l > 0);

    for (int i = 0; i < l; ++i)
    {
		ResAttributes attr;
		ResID resID;
		ResType resType;
		Str255 name;
		uint32_t size;
		uint32_t address;
		Handle h;
		const uint8_t *data;


		h = Get1IndResource('CODE', i + 1);
		if (!h) continue;
		HLock(h);
		data = *(const uint8_t **)h;

		attr = GetResAttrs(h);
		GetResInfo(h, &resID, &resType, name);

		size = GetHandleSize(h);

		if (segments.size() <= resID) segments.resize(resID + 1);
		// can't have duplicate resIDs, so no need to check that...

		if (resID == 0)
		{
			// jump table/a5
			uint32_t above = ReadLong(data, 0);
			uint32_t below = ReadLong(data, 4);
			uint32_t jtSize = ReadLong(data, 8);
			uint32_t jtOffset = ReadLong(data, 12);

			uint32_t a5size = above + below;

			address = EmulatedNewPtr(a5size);

			a5 = address + below;
			std::memcpy(Memory + a5 + jtOffset, data + 16 , jtSize);

			segments[resID] = std::make_pair(address, a5size);

			jtStart = a5 + jtOffset;
			jtEnd = jtStart + jtSize;

			// 0x0934 - CurJTOffset (16-bit)
			WriteWord(Memory, 0x0934, jtOffset);

			// 0x0904 -- CurrentA5 (32-bit)
			WriteLong(Memory, 0x0904, a5);
			cpuSetAReg(5, a5);
		}
		else
		{
			address = EmulatedNewPtr(size);
			std::memcpy(Memory + address, data, size);

			segments[resID] = std::make_pair(address, size);
		}

		ReleaseResource(h);
    }	

    // now link the segment 0 jump table...
    assert(a5);

    bool first = true;
    for (; jtStart < jtEnd; jtStart += 8)
    {
    	uint16_t offset = ReadWord(Memory, jtStart);
    	uint16_t seg = ReadWord(Memory, jtStart + 4);

    	assert(ReadWord(Memory, jtStart + 2) == 0x3F3C);
    	assert(ReadWord(Memory, jtStart + 6) == 0xA9F0);

    	assert(seg < segments.size());

    	auto p = segments[seg];
    	assert(p.first); // missing segment?!
    	assert(offset < p.second);

    	// +4 for the jump table info header.
    	uint32_t address = p.first + offset + 4;

    	// JMP absolute long
    	WriteWord(Memory, jtStart + 2, 0x4EF9);
    	WriteLong(Memory, jtStart + 4, address);

    	if (first)
    	{
	    	cpuSetPC(address);
    		first = false;
    	}
    }



    // set pc to jump table entry 0.
    return true;
}

void InitializeMPW(int argc, char **argv)
{

	// 0x0910 CurApName 
	{

		// todo -- basename it.
		char str32[32];
		int l = strlen(argv[0]);
		l = std::min(l, 32);
		str32[0] = l;
		memcpy(str32 + 1, argv[0], l);
		while (l < 32) str32[l++] = 0;

		memcpy(Memory + 0x910, str32, 32);
	}


	// 0x0316 is a pointer to the argc, argv, envp.
	// very ugly.

	uint32_t argvAddress = 0;
	uint32_t envpAddress = 0;

	{
		std::vector<uint32_t> argvSpine;
		argvSpine.reserve(argc + 1);

		for (int i = 0; i < argc; ++i)
		{
			int length;
			uint32_t address;
			length = strlen(argv[i]);
			address = EmulatedNewPtr(length + 1);
			memcpy(Memory + address, argv[i], length + 1);

			argvSpine.push_back(address);
		}
		argvSpine.push_back(0);

		argvAddress = EmulatedNewPtr(4 * argvSpine.size());
		uint32_t address = argvAddress;
		for (uint32_t x : argvSpine)
		{
			WriteLong(Memory, address, x);
			address += 4;
		}
	}
	// same thing for envp... but \0 instead of =

	uint32_t address = EmulatedNewPtr(8 + 10);
	WriteLong(Memory, 0x0316, address);
	WriteLong(Memory, address, 0x4d50474d); // MPGM - magic
	WriteLong(Memory, address + 4, address + 8); // block ptr
	WriteWord(Memory, address + 8, 0x5348); // SH - more magic
	WriteLong(Memory, address + 10, argc);
	WriteLong(Memory, address + 14, argvAddress);
	WriteLong(Memory, address + 18, envpAddress);
}

void help()
{


}

bool parse_number(const char *input, uint32_t *dest)
{
	char *end;
	long value;
	int base = 0;

	// octal is dumb so don't allow it.

	while (isspace(*input)) ++input;
	if (*input == '0' && isdigit(input[1])) base = 10;

	errno = 0;
	value = strtol(input, &end, base);

	if (errno || value < 0 || input == end)
	{
		fprintf(stderr, "%s - invalid input\n", input);
		return false;
	}

	// M/K
	if (*end)
	{
		int old = value;
		if (strcasecmp(end, "M") == 0)
			value *= 1024 * 1024;
		else if (strcasecmp(end, "K") == 0)
			value *= 1024;
		else 
		{
			fprintf(stderr, "%s - invalid input\n", input);
			return false;
		}
		if (value < old)
		{
			// overflow
			fprintf(stderr, "%s - invalid input\n", input);
			return false;
		}
	}

	if (dest) *dest = value;
	return true;
}

struct {
	uint32_t ram;
	uint32_t stack;
	uint32_t machine;
	bool traceCPU;

} Flags = { 16 * 1024 * 1024, 8 * 1024, 68030, false };

int main(int argc, char **argv)
{
	// getopt...

	enum
	{
		kTraceCPU = 1,
	};
	static struct option LongOpts[] = 
	{
		{ "ram",required_argument, NULL, 'r' },
		{ "stack", required_argument, NULL, 's' },
		{ "machine", required_argument, NULL, 'm' },
		{ "trace-cpu", no_argument, NULL, kTraceCPU },
		{ "help", no_argument, NULL, 'h' },
		{ "version", no_argument, NULL, 'V' },
		{ NULL, 0, NULL, 0 }
	};

	int c;
	while ((c = getopt_long(argc, argv, "+hVm:r:s:", LongOpts, NULL)) != -1)
	{
		switch(c)
		{
			case kTraceCPU:
				Flags.traceCPU = true;
				break;

			case 'm':
				if (!parse_number(optarg, &Flags.machine))
					exit(EX_CONFIG);
				break;

			case 'r':
				if (!parse_number(optarg, &Flags.ram))
					exit(EX_CONFIG);
				break;

			case 's':
				if (!parse_number(optarg, &Flags.stack))
					exit(EX_CONFIG);
				break;

			case ':':
			case '?':
				help();
				exit(EX_USAGE);
				break;

			case 'h':
				help();
				exit(EX_OK);
				break;

			case 'V':
				printf("mpw version 0");
				exit(EX_OK);
				break;
		}

	}

	argc -= optind;
	argv += optind;

	if (!argc)
	{
		help();
		exit(EX_USAGE);
	}

	Memory = new uint8_t[Flags.ram];
	MemorySize = Flags.ram;

	cpuStartup();
	cpuSetModel(3,0);

	load(argv[0]);

	InitializeMPW(argc, argv);


	if (!Flags.stack)
	{
		fprintf(stderr, "Invalid stack size\n");
		exit(EX_CONFIG);
	}

	std::pair<uint32_t, uint32_t> StackRange;
	// allocate stack, set A7...
	{
		Flags.stack = (Flags.stack + 3) & ~0x03;
		uint32_t address = EmulatedNewPtr(Flags.stack);


		StackRange.first = address;
		StackRange.second = address + Flags.stack;

		// address grows down
		// -4 is for the return address.
		cpuSetAReg(7, address + Flags.stack - 4);
		// return address.
		WriteLong(Memory, address + Flags.stack - 4, 0xffffffff);
	}

	memorySetMemory(Memory, MemorySize);
	for (unsigned i = 0; i < 10000; ++i)
	{
		if (Flags.traceCPU)
		{
			static char strings[4][256];
			for (unsigned j = 0; j < 4; ++j) strings[j][0] = 0;

			uint32_t pc = cpuGetPC();
			uint16_t opcode = ReadWord(Memory, pc);

			if ((opcode & 0xf000) == 0xa000)
			{
				fprintf(stderr, "tool %04x\n", opcode);
			}

			#if 0
			fprintf(stderr, "D0       D1       D2       D3       D4       D5       D6       D7       A0       A1       A2       A3       A4       A5       A6       A7\n");
			fprintf(stderr, "%08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x %08x\n",
				cpuGetDReg(0), cpuGetDReg(1), cpuGetDReg(2), cpuGetDReg(3), 
				cpuGetDReg(4), cpuGetDReg(5), cpuGetDReg(6), cpuGetDReg(7), 
				cpuGetAReg(0), cpuGetAReg(1), cpuGetAReg(2), cpuGetAReg(3), 
				cpuGetAReg(4), cpuGetAReg(5), cpuGetAReg(6), cpuGetAReg(7)
			);
			#endif
			// todo - check for Axxx, recognize as a toolcall.
			// move this to the cpuLogging facility?
			cpuDisOpcode(pc, strings[0], strings[1], strings[2], strings[3]);

			// address, data, instruction, operand
			fprintf(stderr, "%s   %-10s %-40s ; %s\n", strings[0], strings[2], strings[3], strings[1]);

			// todo -- trace registers (only print changed registers?)
		}
		cpuExecuteInstruction();
	}
	
	exit(0); // cpuGetDReg(0)
}