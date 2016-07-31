
#include "native_internal.h"
#include <sys/attr.h>
#include <sys/xattr.h>
#include <sys/stat.h>

//using MacOS::macos_error_from_errno;
//using MacOS::macos_error;
using namespace MacOS;

#if __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ < 1050
#define st_birthtime st_mtime
#endif

namespace {


	uint32_t rforksize(const std::string &path_name)
	{
		ssize_t rv;

		rv = getxattr(path_name.c_str(), XATTR_RESOURCEFORK_NAME, nullptr, 0, 0, 0);
		if (rv < 0) return 0;
		return rv;
	}

	uint32_t rforksize(int fd)
	{
		ssize_t rv;

		rv = fgetxattr(fd, XATTR_RESOURCEFORK_NAME, nullptr, 0, 0, 0);
		if (rv < 0) return 0;
		return rv;

	}

	void fixup_prodos(uint8_t *buffer) {
		if (memcmp(buffer + 4, "pdos", 4) == 0) {
			// mpw expects 'xx  ' where
			// xx are the ascii-encode hex value of the file type.
			// the hfs fst uses 'p' ftype8 auxtype16

			// todo -- but only if auxtype is $0000 ??
			if (buffer[0] == 'p' && buffer[2] == 0 && buffer[3] == 0)
			{
				static char Hex[] = "0123456789ABCDEF";

				uint8_t ftype = buffer[1];
				buffer[0] = Hex[ftype >> 4];
				buffer[1] = Hex[ftype & 0x0f];
				buffer[2] = ' ';
				buffer[3] = ' ';
			}
		}
	}

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
			fixup_prodos(buffer);
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
			fixup_prodos(buffer);
			memcpy(info, buffer, extended ? 32 : 16);
			return noErr;
		}

		/* if it's a text file, call it a text file */
		if (is_text_file_internal(path_name)) {
			memcpy(buffer, "TEXTMPS ", 8);
			memcpy(info, buffer, extended ? 32 : 16);
		}

		memcpy(info, buffer, extended ? 32 : 16);
		return noErr;

	}

	macos_error get_file_info(const std::string &path_name, file_info &fi)
	{
		struct stat st;

		if (stat(path_name.c_str(), &st) < 0)
			return macos_error_from_errno();

		fi.create_date = unix_to_mac(st.st_birthtime);
		fi.modify_date = unix_to_mac(st.st_mtime);
		fi.backup_date = unix_to_mac(st.st_mtime);

		if (S_ISDIR(st.st_mode)) {
			fi.type = file_info::directory;


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



}