#include "mpw.h"
#include "mpw_errno.h"
#include "mpw_internal.h"

#include <vector>
#include <string>
#include <deque>
#include <unordered_map>

#include <cstdint>
#include <cstdio>
#include <cerrno>

#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>
#include <sys/stat.h>
#include <pwd.h>

#include <cpu/defs.h>
#include <cpu/fmem.h>
#include <cpu/cpuModule.h>

#include <toolbox/toolbox.h>
#include <toolbox/mm.h>
#include <toolbox/os_internal.h>

#include <macos/sysequ.h>

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

	static bool isdir(const std::string &path)
	{
		struct stat st;
		if (stat(path.c_str(), &st) < 0) return false;
		return S_ISDIR(st.st_mode);
	}

	const std::string RootDir()
	{
		static bool initialized = false;
		static std::string path;

		static const std::string paths[] = {
			"/usr/local/share/mpw",
			"/usr/share/mpw",
		};

		char *cp;
		struct passwd *pw;

		if (initialized) return path;

		initialized = true;

		// check $MPW, $HOME/mpw, /usr/local/share/mpw/, /usr/share/mpw
		// for a directory.

		cp = getenv("MPW");
		if (cp && *cp)
		{
			std::string s(cp);
			if (isdir(s))
			{
				path = std::move(s);
				return path;
			}
		}

		// home/mpw
		pw = getpwuid(getuid());
		if (pw && pw->pw_dir && pw->pw_dir[0])
		{
			std::string s(pw->pw_dir);
			if (s.back() != '/') s.push_back('/');
			s.append("mpw");

			if (isdir(s))
			{
				path = std::move(s);
				return path;
			}
		}
#if 0
		// thread-safe
		{
			int size;

			size = sysconf(_SC_GETPW_R_SIZE_MAX);
			if (size >= 0)
			{
				struct passwd pwd, *result = nullptr;
				char *buffer = alloca(size);

				if (getpwuid_r(getuid(), &pwd, buffer, size, &result) == 0 && result)
				{
					std::string s(pwd.pw_dir);
					if (s.back() != '/') s.push_back('/');
					s.append("mpw");					
					if (isdir(s))
					{
						path = std::move(s);
						return path;
					}
				}
			}
		}
#endif
		for (auto &iter : paths)
		{
			if (isdir(iter))
			{
				path = iter;
				return path;
			}

		}



		return path; // unknown.
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

			std::memcpy(memoryPointer(MacOS::CurApName), str32, 32);
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
			std::unordered_map<std::string, std::string> env;

			std::string m(RootDir());
			if (!m.empty())
			{
				std::string mm = ToolBox::UnixToMac(m);
				if (mm.back() != ':') mm.push_back(':');

				env.emplace(std::string("MPW"), mm);
			}
			env.emplace(std::string("Command"), command);

			if (!m.empty())
			{

				void LoadEnvironment(std::string &envfile, std::unordered_map<std::string, std::string> &env);

				if (m.back() != '/') m.push_back('/');
				m.append("Environment");

				LoadEnvironment(m, env);
			}

			std::deque<std::string> e;

			for (const auto &iter : env)
			{
				std::string tmp;
				tmp.append(iter.first);
				tmp.push_back(0);
				tmp.append(iter.second);
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
