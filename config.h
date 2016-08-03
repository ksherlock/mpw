#ifndef __mpw_config_h__
#define __mpw_config_h__

#ifdef __APPLE__
	#include <machine/endian.h>

	#define HAVE_STAT_ST_BIRTHTIME
	#define HAVE_DIRENT_D_NAMLEN

#endif

#ifdef __sun__
#include <endian.h>


#endif


#ifdef __linux__
#include <endian.h>

#endif

#ifdef _WIN32

#endif

#endif
