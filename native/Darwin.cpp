/*
 * Copyright (c) 2016, Kelvin W Sherlock
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

#include "native_internal.h"
#include <sys/attr.h>
#include <sys/xattr.h>
#include <sys/stat.h>
#include <sys/paths.h>
#include <unistd.h>
#include <fcntl.h>


//using MacOS::macos_error_from_errno;
//using MacOS::macos_error;
using namespace MacOS;

#if __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ < 1050
#define st_birthtime st_ctime
#endif

namespace {


	uint32_t rforksize(const std::string &path_name)
	{
		ssize_t rv;

		rv = getxattr(path_name.c_str(), XATTR_RESOURCEFORK_NAME, nullptr, 0, 0, 0);
		if (rv < 0) return 0;
		return rv;
	}

/*
	uint32_t rforksize(int fd)
	{
		ssize_t rv;

		rv = fgetxattr(fd, XATTR_RESOURCEFORK_NAME, nullptr, 0, 0, 0);
		if (rv < 0) return 0;
		return rv;

	}
*/


}


namespace native {

	/*

     tech note PT515
     ProDOS -> Macintosh conversion

     ProDOS             Macintosh
     Filetype    Auxtype    Creator    Filetype
     $00          $0000     'pdos'     'BINA'
     $B0 (SRC)    (any)     'pdos'     'TEXT'
     $04 (TXT)    $0000     'pdos'     'TEXT'
     $FF (SYS)    (any)     'pdos'     'PSYS'
     $B3 (S16)    (any)     'pdos'     'PS16'
     $uv          $wxyz     'pdos'     'p' $uv $wx $yz

     Programmer's Reference for System 6.0:

     ProDOS Macintosh
     File Type Auxiliary Type Creator Type File Type
     $00        $0000           “pdos”  “BINA”
     $04 (TXT)  $0000           “pdos”  “TEXT”
     $FF (SYS)  (any)           “pdos”  “PSYS”
     $B3 (S16)  $DByz           “pdos”  “p” $B3 $DB $yz
     $B3 (S16)  (any)           “pdos”  “PS16”
     $D7        $0000           “pdos”  “MIDI”
     $D8        $0000           “pdos”  “AIFF”
     $D8        $0001           “pdos”  “AIFC”
     $E0        $0005           “dCpy”  “dImg”
     $FF (SYS)  (any)           “pdos”  “PSYS”
     $uv        $wxyz           “pdos”  “p” $uv $wx $yz


	  mpw standard:
     $uv        (any)          "pdos"  printf("%02x  ",$uv)

     */


	macos_error get_finder_info(const std::string &path_name, void *info, bool extended) {

		uint8_t buffer[32];
		int rv;

		std::memset(buffer, 0, sizeof(buffer));

		rv = getxattr(path_name.c_str(), XATTR_FINDERINFO_NAME, buffer, 32, 0, 0);
		if (rv == 16 || rv == 32) {
			prodos_ftype_out(buffer);
			memcpy(info, buffer, extended ? 32 : 16);
			return noErr;
		}

		if (rv < 0) {
			switch (errno) {
				case ENOATTR:
				case ENOTSUP:
					break;
				default:
					return macos_error_from_errno();
			}
		}

		/* finder info does not exist */
		/* 1. check prodos ftype/aux type */
		uint8_t ftype;
		uint16_t atype;

		int rv1 = ::getxattr(path_name.c_str(), "prodos.FileType", &ftype, 1, 0, 0);
		int rv2 = ::getxattr(path_name.c_str(), "prodos.AuxType", &atype, 2, 0, 0);

		if (rv1 == 1 && rv2 == 2) {
			#if BYTE_ORDER == BIG_ENDIAN
			ftype = (ftype >> 8) | (ftype << 8);
			#endif



			memcpy(buffer, "pxxxpdos", 8);
			buffer[1] = (char)ftype;
			buffer[2] = (char)((atype >> 8) & 0xff);
			buffer[3] = (char)(atype & 0xff);

			switch (atype)
			{
				case 0x00:
					std::memcpy(buffer, "BINA", 4);
					break;

				case 0x04:
				case 0xb0:
					std::memcpy(buffer, "TEXT", 4);
					break;

				case 0xff:
					std::memcpy(buffer, "PSYS", 4);
					break;

				case 0xd7:
					std::memcpy(buffer, "MIDI", 4);
					break;

				case 0xd8:
					switch (atype)
					{
						case 0x0000:
							std::memcpy(buffer, "AIFF", 4);
							break;
						case 0x0001:
							std::memcpy(buffer, "AIFC", 4);
							break;
					}
					break;

				case 0xe0:
					switch (atype)
					{
						case 0x0005:
							std::memcpy(buffer, "dImgdCpy", 8);
							break;

					}

				case 0xb3:
					if (atype == 0)
					{
						std::memcpy(buffer, "PS16", 4); // verify dumpobj.
						break;
					}

			}
			prodos_ftype_out(buffer);
			memcpy(info, buffer, extended ? 32 : 16);
			return noErr;
		}

		/* if it's a text file, call it a text file */
		if (is_text_file_internal(path_name)) {
			memcpy(buffer, "TEXTMPS ", 8);
		}

		memcpy(info, buffer, extended ? 32 : 16);
		return noErr;

	}

	macos_error set_finder_info(const std::string &path_name, const void *info, bool extended) {

		uint8_t buffer[32];
		ssize_t rv;

		std::memset(buffer, 0, sizeof(buffer));

		if (extended) {
			std::memcpy(buffer, info, 32);
		} else {
			get_finder_info(path_name, buffer, true);
			std::memcpy(buffer, info, 16);
		}

		prodos_ftype_in(buffer);
		rv = setxattr(path_name.c_str(), XATTR_FINDERINFO_NAME, buffer, 32, 0, 0);
		if ( rv < 0) return macos_error_from_errno();

		return noErr;
	}


	macos_error get_file_info(const std::string &path_name, file_info &fi)
	{
		struct stat st;

		if (stat(path_name.c_str(), &st) < 0)
			return macos_error_from_errno();

		fi.create_date = unix_to_mac(st.st_birthtime);
		fi.modify_date = unix_to_mac(st.st_mtime);
		fi.backup_date = 0;

		fi.attributes = 0;

		if (S_ISDIR(st.st_mode)) {
			fi.type = file_info::directory;
			fi.attributes = 1 << 4;

			int links = st.st_nlink - 2;
			if (links < 0) links = 0;
			if (links > 65535) links = 65535;

			fi.entry_count = links;
			return noErr;
		}

		// todo -- get actual block size instead of assuming 512.  oh well!
		
		fi.type = file_info::file;
		fi.data_logical_size = st.st_size;
		fi.data_physical_size = (st.st_size + 511) & ~511;
		fi.resource_physical_size = 0;
		fi.resource_logical_size = 0;

		get_finder_info(path_name, fi.finder_info);

		ssize_t rsize = rforksize(path_name);
		if (rsize > 0) {
			fi.resource_physical_size = rsize;
			fi.resource_logical_size = (rsize + 511) & ~511;
		}

		return noErr;
	}

	macos_error set_file_info(const std::string &path_name, const file_info &fi) {

		struct stat st;

		if (stat(path_name.c_str(), &st) < 0) return macos_error_from_errno();

		if (S_ISREG(st.st_mode)) {
			auto rv = set_finder_info(path_name, fi.finder_info);
			if (rv) return rv;
		}

		time_t create_date = fi.create_date ? mac_to_unix(fi.create_date) : 0;
		time_t modify_date = fi.modify_date ? mac_to_unix(fi.modify_date) : 0;
		time_t backup_date = fi.backup_date ? mac_to_unix(fi.backup_date) : 0;

		// todo -- value of 0 == set to current date/time?

		if (create_date == st.st_birthtime) create_date = 0;
		if (modify_date == st.st_mtime) modify_date = 0;

		// try setattr list.
		int rv;
		struct attrlist list;
		unsigned i = 0;
		timespec dates[3];

		memset(&list, 0, sizeof(list));
		memset(dates, 0, sizeof(dates));

		list.bitmapcount = ATTR_BIT_MAP_COUNT;
		list.commonattr  = 0;

		if (create_date)
		{
			dates[i++].tv_sec = create_date;
			list.commonattr |= ATTR_CMN_CRTIME;
		}

		if (fi.modify_date)
		{
			dates[i++].tv_sec = modify_date;
			list.commonattr |= ATTR_CMN_MODTIME;
		}

		if (backup_date)
		{
			dates[i++].tv_sec = backup_date;
			list.commonattr |= ATTR_CMN_BKUPTIME;
		}

		if (!i) return noErr;

		rv = setattrlist(path_name.c_str(), &list, dates, i * sizeof(timespec), 0);


		if (rv < 0 && errno == ENOTSUP)
		{
			// try utimes.

			struct timeval tv[2];
			memset(tv, 0, sizeof(tv));

			rv = 0;
			if (modify_date) {
				tv[1].tv_sec = modify_date;
				rv = utimes(path_name.c_str(), tv);
			}

		}

		if (rv < 0) return macos_error_from_errno();
		return noErr;
	}


	int open_resource_fork(const std::string &path_name, int oflag) {

		std::string rname = path_name + _PATH_RSRCFORKSPEC;

		// todo -- verify behavior on non-hfs volume.
		//if ((oflag & O_ACCMODE) & O_WRONLY) oflag |= O_CREAT;
		return open(rname.c_str(), oflag, 0666);
	}


}