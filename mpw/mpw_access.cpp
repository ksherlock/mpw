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
#include "mpw_internal.h"
#include "mpw_errno.h"

#include <algorithm>
#include <memory>
#include <string>


#include <cstdio>
#include <cstring>
#include <cerrno>

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/paths.h>

#include <cpu/defs.h>
#include <cpu/fmem.h>
#include <cpu/cpuModule.h>

#include <toolbox/toolbox.h>
#include <toolbox/os.h>
#include <toolbox/os_internal.h>

#include <macos/errors.h>

/*
 * access return errors are |= 0x40000000.  Not entirely sure why...
 * may return an errno or an oserr, too, apparently.
 */


namespace MPW
{



	uint32_t ftrap_set_font_info(uint32_t name, uint32_t parm)
	{
		// set_font_info(const char *name, uint32_t fontSize);

		std::string sname = ToolBox::ReadCString(name, true);
		uint32_t fontSize = parm;

		Log("     set_font_info(%s, %04x)\n", sname.c_str(), fontSize);

		return 0x40000000 | kEINVAL;
	}

	uint32_t ftrap_get_font_info(uint32_t name, uint32_t parm)
	{
		// get_font_info(const char *name, uint32 *fontSize)

		std::string sname = ToolBox::ReadCString(name, true);

		Log("     get_font_info(%s, %04x)\n", sname.c_str(), parm);

		// default to 9pt
		if (parm) memoryWriteLong(9, parm);
		return 0;
	}


	uint32_t ftrap_get_tab_info(uint32_t name, uint32_t parm)
	{
		// get_tab_info(const char *name, uint32_t *tabSize)

		// hard code for now.
		// Could check xattr for actual value.
		// That would be rather pointless unless some editor respected
		// it.
		// Could check an environment variable.

		std::string sname = ToolBox::ReadCString(name, true);

		Log("     get_tab_info(%s)\n", sname.c_str());


		if (parm) memoryWriteLong(8, parm);
		return 0;
	}

	uint32_t ftrap_set_tab_info(uint32_t name, uint32_t parm)
	{
		// set_tab_info(const char *name, uint32_t tabSize)

		std::string sname = ToolBox::ReadCString(name, true);
		uint32_t tabSize = parm;

		Log("     set_tab_info(%s, %04x)\n", sname.c_str(), tabSize);

		// setxattr?
		return 0x40000000 | kEINVAL;
	}


	uint32_t ftrap_delete(uint32_t name)
	{
		std::string sname;
		int rv;

		sname = ToolBox::ReadCString(name, true);

		Log("     delete(%s)\n", sname.c_str());

		rv = ::unlink(sname.c_str());
		if (rv < 0) return 0x40000000 | mpw_errno_from_errno();

		return 0;
	}


	uint32_t ftrap_rename(uint32_t src, uint32_t dest)
	{
		std::string sname;
		std::string dname;
		int rv;

		sname = ToolBox::ReadCString(src, true);
		dname = ToolBox::ReadCString(dest, true);

		Log("     rename(%s, %s)\n", sname.c_str(), dname.c_str());
		rv = rename(sname.c_str(), dname.c_str());
		if (rv < 0) return 0x40000000 | mpw_errno_from_errno();

		return 0;
	}


/*


	MPW's open logic pseudo code:

	if (flags & 0x1000) { // undocumented - use old tool calls
		oserr = flags & O_RSRC ? PBOPENRF() : PBOPEN();
	} else {
		oserr = flags & O_RSRC ? PBHOPENRF() : PBHOPEN();
	}
	if (!oserr) {
		if ((flags & (O_CREAT | O_EXCL)) == (O_CREAT | O_EXCL)) {
			errno = EEXIST;
			return;
		}
		return;
	}
	if (oserr == file not found) {
		if (flags & O_CREAT) {
			oserr = PBCreate();
			if (!oserr) {
				oserr = flag & O_RSRC ? PBOpenRF() : PBOpen();
			} 
	}

	PBGETFCBINFO();
	if (file size) {
		if (flags & O_TRUNC) {
			oserr = PBSetEOF();
		}
		if (!permission check) {
			errno = EPERM;
			PBClose();
	}





*/
	uint32_t ftrap_open(uint32_t name, uint32_t parm)
	{
		uint32_t d0;
		int fd;
		std::string sname;

		MPWFile f;
		int nativeFlags = 0;
		std::memset(&f, 0, sizeof(f));

		f.flags = memoryReadWord(parm);


		nativeFlags = 0;
		switch (f.flags & 0x03)
		{
			case 0x01:
				nativeFlags = O_RDONLY;
				break;
			case 0x02:
				nativeFlags = O_WRONLY;
				break;
			case 0x00: // ????
			case 0x03:
				nativeFlags = O_RDWR;
				break;
		}

		if (f.flags & kO_APPEND) nativeFlags |= O_APPEND;
		if (f.flags & kO_CREAT) nativeFlags |= O_CREAT;
		if (f.flags & kO_TRUNC) nativeFlags |= O_TRUNC;
		if (f.flags & kO_EXCL) nativeFlags |= O_EXCL;


		sname = ToolBox::ReadCString(name, true);
		std::string xname = sname;

		Log("     open(%s, %04x)\n", sname.c_str(), f.flags);


		if (f.flags & kO_RSRC) {

			// O_CREAT and O_EXCL apply to the file, not the fork.
			int flags = O_RDONLY | (nativeFlags & (O_CREAT | O_EXCL));

			int parent = ::open(sname.c_str(), flags, 0666);

			fd = -1;
			if (parent >= 0) {

				sname.append(_PATH_RSRCFORKSPEC);

				nativeFlags &= ~O_EXCL;
				// APFS, etc - resource fork doesn't automatically exist so 
				// need O_CREAT.
				if ((nativeFlags & O_ACCMODE) != O_RDONLY) nativeFlags |= O_CREAT;
				fd = ::open(sname.c_str(), nativeFlags, 0666);
				close(parent);
			}

		} else {
			fd = ::open(sname.c_str(), nativeFlags, 0666);
		}

		if (fd < 0)
		{
			// return an errno.
			d0 = 0x40000000 | mpw_errno_from_errno();
			f.error = MacOS::ioErr;
			f.cookie = 0;
		}
		else
		{
			d0 = 0;
			f.error = 0;
			f.cookie = fd;


			// adjust the binary flags...
			// some apps are good about this but
			// dumpobj, makelib, linkiigs don't set O_BINARY (but should)
			// MPW Assembler sets O_BINARY (but shouldn't)

			if (OS::IsTextFile(sname)) f.flags &= ~kO_BINARY;
			if (OS::IsBinaryFile(sname)) f.flags |= kO_BINARY;

			if (f.flags & kO_RSRC) f.flags |= kO_BINARY;

			auto &e = OS::Internal::FDEntry::allocate(fd, std::move(xname));
			e.text = !(f.flags & kO_BINARY);
			e.resource = f.flags & kO_RSRC;
		}

		memoryWriteWord(f.flags, parm + 0);
		memoryWriteWord(f.error, parm + 2);
		memoryWriteLong(f.cookie, parm + 8);

		return d0;
	}


	void ftrap_access(uint16_t trap)
	{
		// open a file, rename a file, or delete a file.
		std::string sname;
		uint32_t d0;

		uint32_t sp = cpuGetAReg(7);

		// hmmm not sure if 3 or 4 parameters.

		uint32_t name = memoryReadLong(sp + 4);
		uint32_t op = memoryReadLong(sp + 8);
		uint32_t parm = memoryReadLong(sp + 12);

		Log("%04x Access(%08x, %04x, %08x)\n", trap, name, op, parm);

		switch (op)
		{
		case kF_OPEN:
			d0 = ftrap_open(name, parm);
			break;

		case kF_DELETE:
			d0 = ftrap_delete(name);
			break;

		case kF_RENAME:
			d0 = ftrap_rename(name, parm);
			break;

		case kF_GTABINFO:
			d0 = ftrap_get_tab_info(name, parm);
			break;

		case kF_STABINFO:
			d0 = ftrap_set_tab_info(name, parm);
			break;

		case kF_GFONTINFO:
			d0 = ftrap_get_font_info(name, parm);
			break;

		case kF_SFONTINFO:
			d0 = ftrap_set_font_info(name, parm);
			break;


		default:
			d0 = 0x40000000 | kEINVAL;
			fprintf(stderr, "faccess - unsupported op %04x\n", op);
			exit(1);
		}

		cpuSetDReg(0, d0);
	}



}
