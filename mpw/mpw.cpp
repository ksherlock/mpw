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

#include "mpw.h"
#include "mpw_errno.h"
#include "mpw_internal.h"

#include <vector>
#include <string>
#include <cstring>
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


namespace MPW {

	// for dup counts, etc.
	//std::vector<int> FDTable;

	uint32_t MacProgramInfo = 0;

	std::unordered_map<std::string, std::string> Environment;

}


namespace MPW
{

	bool Trace = false;



	static bool isdir(const std::string &path)
	{
		struct stat st;
		if (stat(path.c_str(), &st) < 0) return false;
		return S_ISDIR(st.st_mode);
	}

	std::string RootDirPathForFile(const std::string &file)
	{
		std::string dir(RootDir());
		if (dir.length() && dir.back() != '/') dir.push_back('/');
		dir.append(file);

		return dir;
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

	uint16_t InitEnvironment(const std::vector<std::string> &defines)
	{
		void EnvLoadFile(const std::string &envfile);
		void EnvLoadArray(const std::vector<std::string> &data);
		void EnvLoadEnv();


		std::string m(RootDir());
		if (!m.empty())
		{
			std::string mm = ToolBox::UnixToMac(m);
			if (mm.back() != ':') mm.push_back(':');

			Environment.emplace(std::string("MPW"), mm);
		}

		EnvLoadEnv(); // should do this first since it could set MPW??

		if (defines.size())
			EnvLoadArray(defines);

		if (!m.empty())
		{
			std::string path(RootDirPathForFile("Environment.text"));
			EnvLoadFile(path);
		}

		return 0;
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

			auto tmp = MM::Native::NewPtr(size, true);
			if (tmp.error()) return tmp.error();
			argvptr = tmp.value();


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

		// environment
		{
			Environment.emplace(std::string("Command"), command);

			std::deque<std::string> e;

			for (const auto &iter : Environment)
			{
				std::string tmp;
				tmp.append(iter.first);
				tmp.push_back(0);
				tmp.append(iter.second);
				e.emplace_back(std::move(tmp));
			}


			uint32_t size = 0;
			for (const std::string &s : e)
			{
				int l = s.length() + 1;
				if (l & 0x01) l++;
				size = size + l + 4;
			}

			size += 4; // space for null terminator.

			auto tmp = MM::Native::NewPtr(size, true);
			if (tmp.error()) return tmp.error();
			envptr = tmp.value();

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

			auto tmp = MM::Native::NewPtr(size, true);
			if (tmp.error()) return tmp.error();
			fptr = tmp.value();

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

			auto tmp = MM::Native::NewPtr(size, true);
			if (tmp.error()) return tmp.error();
			devptr = tmp.value();

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

			auto tmp = MM::Native::NewPtr(size, true);
			if (tmp.error()) return tmp.error();
			ioptr = tmp.value();

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



		auto tmp = MM::Native::NewPtr(8 + 0x30, true);
		if (tmp.error()) return tmp.error();
		uint32_t mpi = tmp.value();

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
