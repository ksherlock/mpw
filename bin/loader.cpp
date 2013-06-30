// clang++ -c -std=c++11 -stdlib=libc++ -Wno-deprecated-declarations loader.cpp

#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <chrono>

#include <sysexits.h>
#include <getopt.h>
#include <libgen.h>
#include <unistd.h>
#include <sys/stat.h>

#include <CoreServices/CoreServices.h>

#include <cpu/defs.h>
#include <cpu/CpuModule.h>
#include <cpu/fmem.h>

#include <toolbox/toolbox.h>
#include <toolbox/mm.h>
#include <mpw/mpw.h>

#include <mplite/mplite.h>

#include <macos/sysequ.h>

struct {
	uint32_t ram;
	uint32_t stack;
	uint32_t machine;

	bool traceCPU;
	bool traceMacsbug;
	bool traceGlobals;
	bool traceToolBox;
	bool traceMPW;

	bool memoryStats;

} Flags = { 16 * 1024 * 1024, 8 * 1024, 68030, false, false, false, false, false, false};


const uint32_t kGlobalSize = 0x10000;
// retained to make debugging easier.
uint8_t *Memory = nullptr;
uint32_t MemorySize = 0;

#if 0
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
#endif

uint8_t ReadByte(const void *data, uint32_t offset)
{
	offset &= 0xffffff;
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
	((uint8_t *)data)[offset] = value;
}

void WriteWord(void *data, uint32_t offset, uint16_t value)
{
	offset &= 0xffffff;

	((uint8_t *)data)[offset++] = value >> 8;
	((uint8_t *)data)[offset++] = value;
}

void WriteLong(void *data, uint32_t offset, uint32_t value)
{
	offset &= 0xffffff;

	((uint8_t *)data)[offset++] = value >> 24;
	((uint8_t *)data)[offset++] = value >> 16;
	((uint8_t *)data)[offset++] = value >> 8;
	((uint8_t *)data)[offset++] = value;
}

uint32_t load(const char *file)
{

	ResFileRefNum refNum;
	FSRef ref;

	uint32_t returnAddress = 0;

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

		uint16_t error;


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

			// TODO -- verify numbers are on word boundary?


			error = MM::Native::NewPtr(a5size, true, address);
			if (error)
			{
				fprintf(stderr, "Memory allocation error.\n");
				return 0;
			}

			a5 = address + below;
			std::memcpy(memoryPointer(a5 + jtOffset), data + 16 , jtSize);

			segments[resID] = std::make_pair(address, a5size);

			jtStart = a5 + jtOffset;
			jtEnd = jtStart + jtSize;


			// 0x0934 - CurJTOffset (16-bit)
			memoryWriteWord(jtOffset, MacOS::CurJTOffset);

			// 0x0904 -- CurrentA5 (32-bit)
			memoryWriteLong(a5, MacOS::CurrentA5);
			cpuSetAReg(5, a5);
		}
		else
		{
			error = MM::Native::NewPtr(size, false, address);
			if (error)
			{
				fprintf(stderr, "Memory allocation error.\n");
				return 0;
			}

			std::memcpy(memoryPointer(address), data, size);

			segments[resID] = std::make_pair(address, size);
		}

		ReleaseResource(h);
    }	

    // now link the segment 0 jump table...
    assert(a5);

    bool first = true;
    for (; jtStart < jtEnd; jtStart += 8)
    {
    	uint16_t offset = memoryReadWord(jtStart);
    	uint16_t seg = memoryReadWord(jtStart + 4);

    	assert(memoryReadWord(jtStart + 2) == 0x3F3C);
    	assert(memoryReadWord(jtStart + 6) == 0xA9F0);

    	assert(seg < segments.size());

    	auto p = segments[seg];
    	assert(p.first); // missing segment?!
    	assert(offset < p.second);

    	// +4 for the jump table info header.
    	uint32_t address = p.first + offset + 4;

    	// JMP absolute long
    	memoryWriteWord(0x4EF9, jtStart + 2);
    	memoryWriteLong(address, jtStart + 4);

    	if (first)
    	{
	    	//cpuSetPC(address);
	    	returnAddress = address;
    		first = false;
    	}
    }


    // set pc to jump table entry 0.
    return returnAddress;
}



void GlobalInit()
{
	// todo -- move this somewhere better.


	// 0x031a - Lo3Bytes
	memoryWriteLong(0x00ffffff, MacOS::Lo3Bytes);

	// 0x0a02 - OneOne
	memoryWriteLong(0x00010001, MacOS::OneOne);

	// 0x0a06 - MinusOne
	memoryWriteLong(0xffffffff, MacOS::MinusOne);


	// todo -- expects high stack, low heap.
	// the allocator currently works from the top down,
	// so put stack at top of memory?
	
	// 0x0130 -- ApplLimit
	memoryWriteLong(Flags.ram - 1, MacOS::ApplLimit);
}


extern "C" { const char *TrapName(uint16_t trap); }

void LogToolBox(uint32_t pc, uint16_t trap)
{
	const char *name;

	name = TrapName(trap);

	if (name)
	{
		fprintf(stderr, "$%08X   %-51s ; %04X\n", pc, name, trap);
	}
	else
	{
		fprintf(stderr, "$%08X   Tool       #$%04X                                   ; %04X\n", pc, trap, trap);
	}
}

void InstructionLogger()
{

	static char strings[4][256];
	for (unsigned j = 0; j < 4; ++j) strings[j][0] = 0;

	uint32_t pc = cpuGetPC();
	uint16_t opcode = ReadWord(Memory, pc);

	if ((opcode & 0xf000) == 0xa000)
	{
		LogToolBox(pc, opcode);
		return;
	}


	#if 0	
	fprintf(stderr, "D: %08x %08x %08x %08x %08x %08x %08x %08x\n",
		cpuGetDReg(0), cpuGetDReg(1), cpuGetDReg(2), cpuGetDReg(3), 
		cpuGetDReg(4), cpuGetDReg(5), cpuGetDReg(6), cpuGetDReg(7)

	);

	fprintf(stderr, "A: %08x %08x %08x %08x %08x %08x %08x %08x\n",
		cpuGetAReg(0), cpuGetAReg(1), cpuGetAReg(2), cpuGetAReg(3), 
		cpuGetAReg(4), cpuGetAReg(5), cpuGetAReg(6), cpuGetAReg(7)
	);
	#endif

	if (Flags.traceCPU)
	{
		cpuDisOpcode(pc, strings[0], strings[1], strings[2], strings[3]);

		// address, data, instruction, operand
		fprintf(stderr, "%s   %-10s %-40s ; %s\n", strings[0], strings[2], strings[3], strings[1]);

		// todo -- trace registers (only print changed registers?)

		#if 0
		if (pc >= 0x00010E94 && pc <= 0x00010FC0)
		{
			fprintf(stderr, "d7 = %08x\n", cpuGetDReg(7));
		}
		#endif
	}

	int mboffset = 0;
	switch (opcode)
	{
		case 0x4E75: // rts
		case 0x4ED0: // jmp (a0)
			mboffset = 2;
			break;
		case 0x4E74: // rtd #
			mboffset = 4;
			break;
	}

	if (mboffset) // RTS or JMP (A0)
	{
		pc += mboffset;
		// check for MacsBug name after rts.
		std::string s;
		unsigned b = memoryReadByte(pc);
		if (b > 0x80 && b < 0xa0)
		{
			b -= 0x80;
			pc++;
			s.reserve(b);
			for (unsigned i = 0; i < b; ++i)
			{
				s.push_back(memoryReadByte(pc++));
			}
			fprintf(stderr, "%s\n\n", s.c_str());
		}
	}

}

#define MPW_VERSION "0.4"
void help()
{
	printf("MPW " MPW_VERSION "\n");
	printf("Usage: mpw [options] utility ...\n");
	printf("\n");
	printf(" --help              display usage information\n");
	printf(" --trace-cpu         print cpu information\n");
	printf(" --trace-macsbug     print macsbug names\n");
	printf(" --trace-toolbox     print toolbox calls\n");
	printf(" --trace-mpw         print mpw calls\n");
	printf(" --memory-stats      print memory usage information\n");
	printf(" --ram=<number>      set the ram size.  Default=16M\n");
	printf(" --stack=<number>    set the stack size.  Default=8K\n");
	printf("\n");
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

bool file_exists(const std::string & name)
{
	struct stat st;

	return ::stat(name.c_str(), &st) == 0 && S_ISREG(st.st_mode);
}

std::string find_exe(const std::string &name)
{
	if (file_exists(name)) return name;

	// if name is a path, then it doesn't exist.
	if (name.find('/') != name.npos) return std::string();

	std::string path = MPW::RootDir();
	if (path.empty()) return path;


	if (path.back() != '/') path.push_back('/');
	path.append("Tools/");
	path.append(name);

	if (file_exists(path)) return path;

	return std::string();

}


int main(int argc, char **argv)
{
	// getopt...

	enum
	{
		kTraceCPU = 1,
		kTraceMacsBug,
		kTraceGlobals,
		kTraceToolBox,
		kTraceMPW,
		kMemoryStats,
	};
	static struct option LongOpts[] = 
	{
		{ "ram",required_argument, NULL, 'r' },
		{ "stack", required_argument, NULL, 's' },
		{ "machine", required_argument, NULL, 'm' },
		{ "trace-cpu", no_argument, NULL, kTraceCPU },
		{ "trace-macsbug", no_argument, NULL, kTraceMacsBug },
		{ "trace-globals", no_argument, NULL, kTraceGlobals },
		{ "trace-toolbox", no_argument, NULL, kTraceToolBox },
		{ "trace-tools", no_argument, NULL, kTraceToolBox },
		{ "trace-mpw", no_argument, NULL, kTraceMPW },

		{ "memory-stats", no_argument, NULL, kMemoryStats },

		{ "help", no_argument, NULL, 'h' },
		{ "version", no_argument, NULL, 'V' },
		{ NULL, 0, NULL, 0 }
	};

	//auto start_time = std::chrono::high_resolution_clock::now();

	int c;
	while ((c = getopt_long(argc, argv, "+hVm:r:s:", LongOpts, NULL)) != -1)
	{
		switch(c)
		{
			case kTraceCPU:
				Flags.traceCPU = true;
				break;

			case kTraceMacsBug:
				Flags.traceMacsbug = true;
				break;

			case kTraceGlobals:
				Flags.traceGlobals = true;
				break;

			case kTraceToolBox:
				Flags.traceToolBox = true;
				break;

			case kTraceMPW:
				Flags.traceMPW = true;
				break;
				
			case kMemoryStats:
				Flags.memoryStats = true;
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
				printf("mpw version " MPW_VERSION "\n");
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

	std::string command(argv[0]); // InitMPW updates argv...

	command = find_exe(command);
	if (command.empty())
	{
		const char *mpw = getenv("MPW");
		fprintf(stderr, "Unable to find command %s\n", argv[0]);
		fprintf(stderr, "$MPW = %s\n", mpw ? mpw : "<null>");
		exit(EX_USAGE);
	}
	argv[0] = ::strdup(command.c_str()); // hmm.. could setenv(mpw_command) instead.



	Memory = new uint8_t[Flags.ram];
	MemorySize = Flags.ram;


	/// ahhh... need to set PC after memory.
	// for pre-fetch.
	memorySetMemory(Memory, MemorySize);

	// should we subtract memory from the top
	// for the stack vs allocating it?

	MM::Init(Memory, MemorySize, kGlobalSize);

	MPW::Init(argc, argv);


	cpuStartup();
	cpuSetModel(3,0);



	if (!Flags.stack)
	{
		fprintf(stderr, "Invalid stack size\n");
		exit(EX_CONFIG);
	}

	std::pair<uint32_t, uint32_t> StackRange;
	// allocate stack, set A7...
	{
		uint32_t address;
		uint16_t error;

		Flags.stack = (Flags.stack + 3) & ~0x03;

		error = MM::Native::NewPtr(Flags.stack, true, address);
		if (error)
		{
			fprintf(stderr, "Unable to allocate stack (%08x bytes)\n", Flags.stack);
			exit(EX_CONFIG);
		}

		StackRange.first = address;
		StackRange.second = address + Flags.stack;

		// TODO -- is there a global for the max (min) stack pointer?

		// address grows down
		// -4 is for the return address.
		cpuSetAReg(7, address + Flags.stack - 4);
		// return address.
		memoryWriteLong(MacOS::MinusOne, StackRange.second - 4); // MinusOne Global -- 0xffff ffff
	}


	uint32_t address = load(command.c_str());
	if (!address) exit(EX_CONFIG);

	GlobalInit();


	cpuSetALineExceptionFunc(ToolBox::dispatch);
	cpuSetFLineExceptionFunc(MPW::dispatch);


	if (Flags.traceGlobals) memorySetGlobalLog(kGlobalSize);


	MPW::Trace = Flags.traceMPW;
	ToolBox::Trace = Flags.traceToolBox;

	
	if (Flags.traceCPU || Flags.traceMacsbug)
	{
		#ifdef CPU_INSTRUCTION_LOGGING
		cpuSetInstructionLoggingFunc(InstructionLogger);
		#endif
		// else do it manually below.
	}


	cpuInitializeFromNewPC(address);

	#if 0
	auto begin_emu_time = std::chrono::high_resolution_clock::now();
	fprintf(stderr, "Begin Emulation Time: %20lld\n", (begin_emu_time - start_time).count());
	#endif

	uint64_t cycles = 0;
	for (;;)
	{
		uint32_t pc = cpuGetPC();
		uint32_t sp = cpuGetAReg(7);

		if (pc == 0x00000000)
		{
			fprintf(stderr, "Exiting - PC = 0\n");
			exit(EX_SOFTWARE);
		}

		if (sp < StackRange.first)
		{
			fprintf(stderr, "Stack overflow error - please increase the stack size (--stack=size)\n");
			fprintf(stderr, "Current stack size is 0x%06x\n", Flags.stack);
			exit(EX_SOFTWARE);
		}

		if (sp > StackRange.second)
		{
			fprintf(stderr, "Stack underflow error\n");
			exit(EX_SOFTWARE);
		}


		if (cpuGetStop()) break; // will this also be set by an interrupt?

	
		#ifndef CPU_INSTRUCTION_LOGGING
		if (Flags.traceCPU || Flags.traceMacsbug)
		{
			InstructionLogger();
		}
		#endif



		cycles += cpuExecuteInstruction();
	}

	#if 0
	auto end_emu_time = std::chrono::high_resolution_clock::now();
	fprintf(stderr, "  End Emulation Time: %20lld\n", (end_emu_time - start_time).count());
	fprintf(stderr, "              Cycles: %20lld\n", cycles);
	#endif

	if (Flags.memoryStats)
	{
		MM::Native::PrintMemoryStats();
	}

	uint32_t rv = MPW::ExitStatus();
	if (rv > 0xff) rv = 0xff;


	
	exit(rv);
}