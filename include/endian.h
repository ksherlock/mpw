#ifndef __mpw_endian_h__
#define __mpw_endian_h__

#include "config.h"

#if defined(HAVE_ENDIAN_H)
#include <endian.h>
#elif defined(HAVE_SYS_ENDIAN_H)
#include <sys/endian.h>
#elif defined(HAVE_MACHINE_ENDIAN_H)
#include <machine/endian.h>
#else
#error missing endian.h
#endif

#endif

