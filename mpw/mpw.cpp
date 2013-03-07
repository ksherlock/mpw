#include "mpw.h"
#include "mpw_errno.h"
#include "mpw_internal.h"

#include <vector>
#include <string>
#include <deque>

#include <cstdint>
#include <cstdio>
#include <cerrno>

#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>

#include <cpu/defs.h>
#include <cpu/fmem.h>
#include <cpu/cpuModule.h>

#include <toolbox/toolbox.h>
#include <toolbox/mm.h>
#include <toolbox/os_internal.h>

extern char **environ;


namespace MPW { namespace Internal {

	// for dup counts, etc.
	//std::vector<int> FDTable;

	uint32_t MacProgramInfo = 0;

} }


namespace MPW
{
	using namespace Internal;

	bool Trace = false;

	int errno_to_errno(int xerrno)
	{
		switch (xerrno)
		{
		case EPERM: return kEPERM;
		case ENOENT: return kENOENT;

		#ifdef ENORSRC
		case ENORSRC: return kENORSRC;
		#endif

		case EINTR: return kEINTR;
		case EIO: return kEIO;
		case ENXIO: return kENXIO;
		case E2BIG: return kE2BIG;
		case ENOEXEC: return kENOEXEC;
		case EBADF: return kEBADF;
		case ECHILD: return kECHILD;
		case EAGAIN: return kEAGAIN;
		case ENOMEM: return kENOMEM;
		case EACCES: return kEACCES;
		case EFAULT: return kEFAULT;
		case ENOTBLK: return kENOTBLK;
		case EBUSY: return kEBUSY;
		case EEXIST: return kEEXIST;
		case EXDEV: return kEXDEV;
		case ENODEV: return kENODEV;
		case ENOTDIR: return kENOTDIR;
		case EISDIR: return kEISDIR;
		case EINVAL: return kEINVAL;
		case ENFILE: return kENFILE;
		case EMFILE: return kEMFILE;
		case ENOTTY: return kENOTTY;
		case ETXTBSY: return kETXTBSY;
		case EFBIG: return kEFBIG;
		case ENOSPC: return kENOSPC;
		case ESPIPE: return kESPIPE;
		case EROFS: return kEROFS;
		case EMLINK: return kEMLINK;
		case EPIPE: return kEPIPE;
		case EDOM: return kEDOM;
		case ERANGE: return kERANGE;
		}
		return kEINVAL;

	}

	uint16_t Init(int argc, char **argv)
	{
		/*
		FDTable.resize(16);

		FDTable[STDIN_FILENO] = 1;
		FDTable[STDOUT_FILENO] = 1;
		FDTable[STDERR_FILENO] = 1;
		*/

		/*
		OS::Internal::FDTable.resize(3);
		FDTable[STDIN_FILENO].refcount = 1;
		FDTable[STDIN_FILENO].text = true;

		FDTable[STDOUT_FILENO].refcount = 1;
		FDTable[STDOUT_FILENO].text = true;

		FDTable[STDERR_FILENO].refcount = 1;
		FDTable[STDERR_FILENO].text = true;		
		*/

		OS::Internal::FDEntry::allocate(STDIN_FILENO).text = true;
		OS::Internal::FDEntry::allocate(STDOUT_FILENO).text = true;
		OS::Internal::FDEntry::allocate(STDERR_FILENO).text = true;


		std::string command = argv[0];

		command = ToolBox::UnixToMac(command);
		//std::replace(command.begin(), command.end(), '/', ':');

		argv[0] = basename(argv[0]);


		// 0x0910 CurApName 
		{
			char str32[32];

			char * name = argv[0];
			int l = strlen(name);
			l = std::min(l, 32);
			str32[0] = l;
			std::memcpy(str32 + 1, name, l);
			while (l < 32) str32[l++] = 0;

			std::memcpy(memoryPointer(0x0910), str32, 32);
		}


		uint32_t argvptr = 0;
		uint32_t envptr = 0;
		uint32_t ioptr = 0;
		uint32_t devptr = 0;
		uint32_t fptr = 0;

		uint16_t error;

		// create the argv-data.
		{
			uint32_t size = 0;

			size = 4 * (argc + 1); // argv data.

			for (int i = 0; i < argc; ++i)
			{
				int l = strlen(argv[i]) + 1;
				if (l & 0x01) l++;
				size += l;
			}

			error = MM::Native::NewPtr(size, true, argvptr);
			if (error) return error;


			uint8_t *xptr = memoryPointer(argvptr);
			uint32_t offset = 0;

			offset = 4 * (argc + 1);

			for (int i = 0; i < argc; ++i)
			{
				memoryWriteLong(argvptr + offset, argvptr + 4 * i);

				// just use strcat?
				int l = strlen(argv[i]) + 1;
				if (l & 0x01) l++;

				strcpy((char *)xptr + offset, argv[i]);

				offset += l;
			}

			// null-terminate it.
			memoryWriteLong(0, argvptr + 4 * argc);

		}

		// environment,
		// just use $MPW and synthesize the other ones.
		{

			std::deque<std::string> e;

			{
				// command name (includes path)
				// asm iigs stores error text in the data fork,
				// using {Command} to access it.
				std::string tmp;
				tmp.append("Command");
				tmp.push_back(0);
				tmp.append(command);

				e.emplace_back(std::move(tmp));

			}

			const char *mpw = getenv("MPW");
			if (mpw && *mpw)
			{
				std::string tmp;
				std::string root(mpw);

				root = ToolBox::UnixToMac(root);
				//std::replace(root.begin(), root.end(), '/', ':');
				if (root.back() != ':') root.push_back(':');

				tmp = "MPW";
				tmp.push_back(0);
				tmp.append(root);
				e.emplace_back(std::move(tmp));


				// SysErrs.err
				tmp = "ShellDirectory";
				tmp.push_back(0);
				tmp.append(root);
				e.emplace_back(std::move(tmp));				

				tmp = "AIIGSIncludes";
				tmp.push_back(0);
				tmp.append(root);
				tmp.append("Interfaces:AIIGSIncludes:");
				e.emplace_back(std::move(tmp));

				// 1.0 compatibility
				tmp = "AIIGSInclude";
				tmp.push_back(0);
				tmp.append(root);
				tmp.append("Interfaces:AIIGSIncludes:");
				e.emplace_back(std::move(tmp));

				tmp = "RIIGSIncludes";
				tmp.push_back(0);
				tmp.append(root);
				tmp.append("Interfaces:RIIGSIncludes:");
				e.emplace_back(std::move(tmp));	

				// 1.0 compatibility
				tmp = "RIIGSInclude";
				tmp.push_back(0);
				tmp.append(root);
				tmp.append("Interfaces:RIIGSIncludes:");
				e.emplace_back(std::move(tmp));				

				tmp = "CIIGSIncludes";
				tmp.push_back(0);
				tmp.append(root);
				tmp.append("Interfaces:CIIGSIncludes:");
				e.emplace_back(std::move(tmp));

				// 1.0 compatibility
				tmp = "CIIGSinclude"; // lowercase include [??]
				tmp.push_back(0);
				tmp.append(root);
				tmp.append("Interfaces:CIIGSIncludes:");
				e.emplace_back(std::move(tmp));

				tmp = "CIIGSLibraries";
				tmp.push_back(0);
				tmp.append(root);
				tmp.append("Libraries:CIIGSIncludes:");
				e.emplace_back(std::move(tmp));

				// 1.0 compatibility
				tmp = "CIIGSLibrary";
				tmp.push_back(0);
				tmp.append(root);
				tmp.append("Libraries:CIIGSIncludes:");
				e.emplace_back(std::move(tmp));

				tmp = "PIIGSIncludes";
				tmp.push_back(0);
				tmp.append(root);
				tmp.append("Interfaces:PIIGSIncludes:");
				e.emplace_back(std::move(tmp));

				tmp = "PIIGSLibraries";
				tmp.push_back(0);
				tmp.append(root);
				tmp.append("Libraries:PIIGSIncludes:");
				e.emplace_back(std::move(tmp));


			}

			uint32_t size = 0;
			for(const std::string &s : e)
			{
				int l = s.length() + 1;
				if (l & 0x01) l++;
				size = size + l + 4;
			}

			size += 4; // space for null terminator.

			error = MM::Native::NewPtr(size, true, envptr);
			if (error) return error;


			uint8_t *xptr = memoryPointer(envptr);
			uint32_t offset = 0;

			offset = 4 * (e.size() + 1);
			unsigned i = 0;
			for (const std::string &s : e)
			{
				// ptr
				memoryWriteLong(envptr + offset, envptr + i * 4);

				int l = s.length() + 1;

				std::memcpy(xptr + offset, s.c_str(), l);

				if (l & 0x01) l++;
				offset += l;
				++i;
			}


			// null-terminate it.
			memoryWriteLong(0, envptr + 4 * e.size());
		}

#if 0
		// do the same for envp.
		// mpw_* variables in the native environment are imported.
		// values are stored as key\0value\0, not key=value\0
		{

			std::deque<std::string> e;
			uint32_t size = 0;

			{
				// command name (includes path)
				// asm iigs stores error text in the data fork,
				// using {Command} to access it.
				std::string tmp;
				tmp.append("Command");
				tmp.push_back(0);
				tmp.append(command);

				e.emplace_back(std::move(tmp));

			}

			for (unsigned i = 0 ; environ[i]; ++i)
			{
				int pos;

				char *cp = environ[i];
				if (std::strncmp("mpw_", cp, 4)) continue;

				std::string tmp = cp + 4;

				pos = tmp.find('=');
				if (pos == tmp.npos) continue;
				tmp[pos] = 0;

				e.emplace_back(std::move(tmp));
			}

			size = 0;
			for(const std::string &s : e)
			{
				int l = s.length() + 1;
				if (l & 0x01) l++;
				size = size + l + 4;
			}

			size += 4; // space for null terminator.

			error = MM::Native::NewPtr(size, true, envptr);
			if (error) return error;


			uint8_t *xptr = memoryPointer(envptr);
			uint32_t offset = 0;

			offset = 4 * (e.size() + 1);
			unsigned i = 0;
			for (const std::string &s : e)
			{
				// ptr
				memoryWriteLong(envptr + offset, envptr + i * 4);

				int l = s.length() + 1;

				std::memcpy(xptr + offset, s.c_str(), l);

				if (l & 0x01) l++;
				offset += l;
				++i;
			}


			// null-terminate it.
			memoryWriteLong(0, envptr + 4 * e.size());
		}
#endif


		// ftable
		{
			// these are ftraps for emulated/native function ptrs.
			uint32_t size = 6 * 4;

			error = MM::Native::NewPtr(size, true, fptr);

			if (error) return error;

			memoryWriteWord(fQuit, fptr + 0);
			memoryWriteWord(0x4E75, fptr + 2); // rts

			memoryWriteWord(fAccess, fptr + 4);
			memoryWriteWord(0x4E75, fptr + 6); // rts

			memoryWriteWord(fClose, fptr + 8);
			memoryWriteWord(0x4E75, fptr + 10); // rts			

			memoryWriteWord(fRead, fptr + 12);
			memoryWriteWord(0x4E75, fptr + 14); // rts

			memoryWriteWord(fWrite, fptr + 16);
			memoryWriteWord(0x4E75, fptr + 18); // rts

			memoryWriteWord(fIOCtl, fptr + 20);
			memoryWriteWord(0x4E75, fptr + 22); // rts

		}


		// dev table
		{
			uint32_t size = 0x78;

			error = MM::Native::NewPtr(size, true, devptr);

			if (error) return error;

			memoryWriteLong(0x46535953, devptr + 0); // 'FSYS'
			memoryWriteLong(fptr + 4, devptr + 4);
			memoryWriteLong(fptr + 8, devptr + 8);
			memoryWriteLong(fptr + 12, devptr + 12);
			memoryWriteLong(fptr + 16, devptr + 16);
			memoryWriteLong(fptr + 20, devptr + 20);

			memoryWriteLong(0x45434f4e, devptr + 24); // 'ECON' -- not implemented.
			memoryWriteLong(0x53595354, devptr + 48); // 'SYST' -- not implemented.

		}

		// io table.
		{

			uint32_t size = 0x3c;
			uint32_t ptr;

			error = MM::Native::NewPtr(size, true, ioptr);

			if (error) return error;

			ptr = ioptr;
			// stdin
			memoryWriteWord(0x0001, ptr + 0); // open flags (read)
			memoryWriteWord(0x0000, ptr + 2); // os err
			memoryWriteLong(devptr, ptr + 4); // -> 'FSYS'
			memoryWriteLong(STDIN_FILENO, ptr + 8); // cookie
			memoryWriteLong(0, ptr + 12); // transfer count.
			memoryWriteLong(0, ptr + 16); // buffer

			ptr = ioptr + 20;
			// stdout
			memoryWriteWord(0x0002, ptr + 0); // open flags (write)
			memoryWriteWord(0x0000, ptr + 2); // os err
			memoryWriteLong(devptr, ptr + 4); // -> 'FSYS'
			memoryWriteLong(STDOUT_FILENO, ptr + 8); // cookie
			memoryWriteLong(0, ptr + 12); // transfer count.
			memoryWriteLong(0, ptr + 16); // buffer

			ptr = ioptr + 40;
			// stderr
			memoryWriteWord(0x0002, ptr + 0); // open flags (write)
			memoryWriteWord(0x0000, ptr + 2); // os err
			memoryWriteLong(devptr, ptr + 4); // -> 'FSYS'
			memoryWriteLong(STDERR_FILENO, ptr + 8); // cookie
			memoryWriteLong(0, ptr + 12); // transfer count.
			memoryWriteLong(0, ptr + 16); // buffer

		}



		uint32_t mpi = 0;

		error = MM::Native::NewPtr(8 + 0x30, true, mpi);
		if (error) return error;

		MacProgramInfo = mpi + 8;

		memoryWriteLong(0x4d50474d, mpi); // 'MPGM' - magic
		memoryWriteLong(mpi + 8, mpi + 4);

		memoryWriteLong(mpi, 0x0316);

		mpi += 8;
		memoryWriteWord(0x5348, mpi + 0x00); // 'SH' - more magic

		memoryWriteLong(argc, mpi + 0x02);
		memoryWriteLong(argvptr, mpi + 0x06);
		memoryWriteLong(envptr, mpi + 0x0a);

		// 0x0e = exit code

		// ??? default fd table size?
		memoryWriteWord(0x190, mpi + 0x1a);

		// io table - stdin/stdout/stderr
		memoryWriteLong(ioptr, mpi + 0x1c);

		// device table
		memoryWriteLong(devptr, mpi + 0x20);


		return 0;
	}

	uint32_t ExitStatus()
	{
		if (!MacProgramInfo) return -1;

		return memoryReadLong(MacProgramInfo + 0x0e);
	}


	void ftrap_quit(uint16_t trap)
	{
		Log("%04x Quit()\n", trap);
		cpuSetStop(true);
	}

	void dispatch(uint16_t trap)
	{
		switch (trap)
		{
			case fQuit:
			case 0xffff:
				ftrap_quit(trap);
				break;

			case fAccess:
				ftrap_access(trap);
				break;

			case fClose:
				ftrap_close(trap);
				break;

			case fRead:
				ftrap_read(trap);
				break;

			case fWrite:
				ftrap_write(trap);
				break;

			case fIOCtl:
				ftrap_ioctl(trap);
				break;

			default:
				fprintf(stderr, "Unsupported f trap: %04x\n", trap);
				exit(1);
				break;
		}

	}


}
