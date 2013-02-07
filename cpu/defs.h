#ifndef DEFS_H
#define DEFS_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Maximum values for memory, don't change */

#define CHIPMEM 0x200000
#define FASTMEM 0x800000
#define BOGOMEM 0x1c0000
#define KICKMEM 0x080000

/* Fellow types to ensure correct sizes */

typedef uint8_t UBY;
typedef uint16_t UWO;
typedef uint32_t ULO;
typedef uint64_t ULL;
typedef int8_t BYT;
typedef int16_t WOR;
typedef int32_t LON;
typedef int64_t LLO;
typedef int BOOLE;
#define FALSE 0
#define TRUE  1
typedef char STR;

/*
#ifndef X64
#define PTR_TO_INT(i) ((ULO)i)
#endif
#ifdef X64
#define PTR_TO_INT(i) ((ULL)i)
#endif
*/

/* Filename length used throughout the code */

#define CFG_FILENAME_LENGTH 256

/*------------------------------------*/
/* The decode routines have this type */
/*------------------------------------*/

typedef void (*decoderoutinetype)(ULO,ULO);

extern UBY configromname[];

typedef union {
          ULO *lptr;
          UWO *wptr;
          UBY *bptr;
          ULO lval;
          UWO wval[2];
          UBY bval[4];
          } ptunion;

typedef void (*planar2chunkyroutine)(void);

typedef void (*playbuffer_routine)(void);
typedef void (*sound_before_emu_routine)(void);
typedef void (*sound_after_emu_routine)(void);

typedef void (*buseventfunc)(void);

#define FELLOWVERSION        "WinFellow alpha v0.5.0 build 0 (CVS)"
#define FELLOWLONGVERSION    "WinFellow Amiga Emulator alpha v0.5.0 - CVS" 
#define FELLOWNUMERICVERSION "0.5.0.0"

#ifdef __cplusplus
}
#endif

#endif
