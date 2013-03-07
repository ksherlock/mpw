// clang++ -c -std=c++11 -stdlib=libc++ -Wno-deprecated-declarations loader.cpp

#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
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
			memoryWriteWord(jtOffset, 0x0934);

			// 0x0904 -- CurrentA5 (32-bit)
			memoryWriteLong(a5, 0x0904);
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

#if 0 
void InitializeMPW(int argc, char **argv)
{

	argv[0] = basename(argv[0]);
	// 0x0910 CurApName 
	{
		char * name = argv[0];
		char str32[32];
		int l = strlen(name);
		l = std::min(l, 32);
		str32[0] = l;
		memcpy(str32 + 1, name, l);
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

	{
		envpAddress = EmulatedNewPtr(4);
		WriteLong(Memory, envpAddress, 0);
	}


	// _macpgminfo
	uint32_t address = EmulatedNewPtr(8 + 0x30);

	address = 0x2000; // monitor reads...
	WriteLong(Memory, 0x0316, address);
	WriteLong(Memory, address, 0x4d50474d); // MPGM - magic
	WriteLong(Memory, address + 4, 0x2100 /* address + 8 */); // block ptr

	// address += 8;	
	address = 0x2100;
	WriteWord(Memory, address + 0x00, 0x5348); // SH - more magic
	WriteLong(Memory, address + 0x02, argc);
	WriteLong(Memory, address + 0x06, argvAddress);
	WriteLong(Memory, address + 0x0a, envpAddress);



	// possible the application exit code...
	WriteLong(Memory, address + 0x0e, 0x00007000);

	WriteLong(Memory, address + 0x12, 0x00008000);
	WriteLong(Memory, address + 0x16, 0x00009000);
	WriteWord(Memory, address + 0x1a, 0x0190); //  ???? 

	// default file table? block of size 0x3c.
	// copied into IntEnv+0x1c
	// _initIOPtable
	WriteLong(Memory, address + 0x1c, 0x0000a000);

	// stdin
	WriteLong(Memory, 0x0000a000+0, 0x00010000); // ???
	WriteLong(Memory, 0x0000a000+4, 0x00003000); // type ptr (FSYS)
	WriteLong(Memory, 0x0000a000+8, STDIN_FILENO); // ptr to refcount/fd struct?
	WriteLong(Memory, 0x0000a000+12, 0x00000000); // transferCount
	WriteLong(Memory, 0x0000a000+16, 0x00000000); // transferBuffer

	// stdout
	//0x0001 = readable
	//0x0002 = writable
	// others?...
	WriteLong(Memory, 0x0000a000+20, 0x00020000); // ??? {uint16_t flags, uint16_t error? }
	WriteLong(Memory, 0x0000a000+24, 0x00003000); // type ptr (FSYS)
	WriteLong(Memory, 0x0000a000+28, STDOUT_FILENO); // ptr to refcount/fd struct?
	WriteLong(Memory, 0x0000a000+32, 0x00000000); //???
	WriteLong(Memory, 0x0000a000+36, 0x00000000); //???

	// stderr
	WriteLong(Memory, 0x0000a000+40, 0x00020000); // ???
	WriteLong(Memory, 0x0000a000+44, 0x00003000); // type ptr (FSYS)
	WriteLong(Memory, 0x0000a000+48, STDERR_FILENO); // ptr to refcount/fd struct?
	WriteLong(Memory, 0x0000a000+52, 0x00000000); //???
	WriteLong(Memory, 0x0000a000+56, 0x00000000); //???


	WriteWord(Memory, 0xf000, MPW::fQuit);
	WriteWord(Memory, 0xf002, 0x4E75); // rts

	WriteWord(Memory, 0xf004, MPW::fAccess);
	WriteWord(Memory, 0xf006, 0x4E75); // rts	

	WriteWord(Memory, 0xf008, MPW::fClose);
	WriteWord(Memory, 0xf00a, 0x4E75); // rts

	WriteWord(Memory, 0xf00c, MPW::fRead);
	WriteWord(Memory, 0xf00e, 0x4E75); // rts

	WriteWord(Memory, 0xf010, MPW::fWrite);
	WriteWord(Memory, 0xf012, 0x4E75); // rts

	WriteWord(Memory, 0xf014, MPW::fIOCtl);
	WriteWord(Memory, 0xf016, 0x4E75); // rts


	// StdDevs (0x78 bytes)
	// copied into a $78 byte buffer stored at _IntEnv+20
	// this has pointers to read/write functions
	// (although the executable has it's own functions as well...)
	WriteLong(Memory, address + 0x20, 0x00003000);

	WriteLong(Memory, 0x00003000+0, 0x46535953); // 'FSYS'
	WriteLong(Memory, 0x00003000+4, 0xf004); //access
	WriteLong(Memory, 0x00003000+8, 0xf008); // close
	WriteLong(Memory, 0x00003000+12, 0xf00c); // read
	WriteLong(Memory, 0x00003000+16, 0xf010); // write
	WriteLong(Memory, 0x00003000+20, 0xf014); // ioctl

	WriteLong(Memory, 0x00003000+24, 0x45434f4e); // 'ECON'
	WriteLong(Memory, 0x00003000+28, 0); //access
	WriteLong(Memory, 0x00003000+32, 0); // close
	WriteLong(Memory, 0x00003000+36, 0); // read
	WriteLong(Memory, 0x00003000+40, 0); // write
	WriteLong(Memory, 0x00003000+44, 0); // ioctl

	WriteLong(Memory, 0x00003000+48, 0x53595354); // 'SYST'
	WriteLong(Memory, 0x00003000+52, 0); //access
	WriteLong(Memory, 0x00003000+56, 0); // close
	WriteLong(Memory, 0x00003000+60, 0); // read
	WriteLong(Memory, 0x00003000+68, 0); // write
	WriteLong(Memory, 0x00003000+72, 0); // ioctl

	WriteLong(Memory, 0x00003000+76, 0);
	WriteLong(Memory, 0x00003000+80, 0);
	WriteLong(Memory, 0x00003000+84, 0);
	WriteLong(Memory, 0x00003000+88, 0);
	WriteLong(Memory, 0x00003000+92, 0);
	WriteLong(Memory, 0x00003000+96, 0);
	WriteLong(Memory, 0x00003000+100, 0);
	WriteLong(Memory, 0x00003000+104, 0);
	WriteLong(Memory, 0x00003000+108, 0);
	WriteLong(Memory, 0x00003000+112, 0);
	WriteLong(Memory, 0x00003000+116, 0);



	// _RTInit fills in this location with &_IntEnv.
	WriteLong(Memory, address + 0x24, 0x00004000);
	WriteLong(Memory, address + 0x28, 0x00005000); 
	WriteLong(Memory, address + 0x2c, 0x00006000); 



	// 0x1c should have something, too .... :(


	// 0x031a - Lo3Bytes
	WriteLong(Memory, 0x031a, 0x00ffffff);
	// 0x0a02 - OneOne
	WriteLong(Memory, 0x0a02, 0x00010001);
	// 0x0a06 - MinusOne
	WriteLong(Memory, 0x0a06, 0xffffffff);


	// todo -- expects high stack, low heap.
	// the allocator currently works from the top down,
	// so put stack at top of memory?
	
	// 0x0130 -- ApplLimit
	WriteLong(Memory, 0x0130, Flags.ram - 1);

}
#endif

void GlobalInit()
{
	// todo -- move this somewhere better.


	// 0x031a - Lo3Bytes
	memoryWriteLong(0x00ffffff, 0x031a);

	// 0x0a02 - OneOne
	memoryWriteLong(0x00010001, 0x0a02);

	// 0x0a06 - MinusOne
	memoryWriteLong(0xffffffff, 0x0a06);


	// todo -- expects high stack, low heap.
	// the allocator currently works from the top down,
	// so put stack at top of memory?
	
	// 0x0130 -- ApplLimit
	memoryWriteLong(Flags.ram - 1, 0x0130);
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
	printf(" --memory stats      print memory usage information\n");
	printf(" --ram=<number>      set the ram size.  Default=16M");
	printf(" --stack=<number>    set the stack size.  Default=8K");
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

	return ::stat(name.c_str(), &st) == 0;
}

std::string find_exe(const std::string &name)
{
	if (file_exists(name)) return name;

	// if name is a path, then it doesn't exist.
	if (name.find('/') != name.npos) return std::string();


	const char *mpw = getenv("MPW");
	if (!mpw || !*mpw) return std::string();

	std::string path(mpw);

	if (path.back() != '/') path.push_back('/');
	path.append("Tools/");
	path.append(name);

	if (file_exists(path)) return path;

	return std::string();



#if 0
	std::string subpath;
	// check in $MPW/name.
	const char *cpath = getenv("mpw_path");
	if (!cpath) return std::string();

	path = cpath; 
	// split on :

	if (path.empty()) return std::string();

	int start = 0, end = 0;

	while ((end = path.find(':', start)) != path.npos)
	{
		subpath = path.substr(start, end - start);

		if (subpath.length())
		{
			subpath.push_back('/');
			subpath.append(name);

			if (file_exists(subpath)) return subpath;			
		}

		start = end + 1;
	}

	subpath = path.substr(start);
	if (subpath.length())
	{
		subpath.push_back('/');
		subpath.append(name);

		if (file_exists(subpath)) return subpath;
	}

	return std::string();
#endif
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
		memoryWriteLong(0x0a06, StackRange.second - 4); // MinusOne Global -- 0xffff ffff
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
		cpuSetInstructionLoggingFunc(InstructionLogger);
	}


	cpuInitializeFromNewPC(address);


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
		cpuExecuteInstruction();
	}

	if (Flags.memoryStats)
	{
		MM::Native::PrintMemoryStats();
	}

	uint32_t rv = MPW::ExitStatus();
	if (rv > 0xff) rv = 0xff;

	exit(rv);
}