#ifndef __mpw_config_h__
#define __mpw_config_h__

#ifdef __APPLE__
	#include <machine/endian.h>

	#if __ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__ >= 1050
	#define HAVE_STAT_ST_BIRTHTIME
	#endif

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
