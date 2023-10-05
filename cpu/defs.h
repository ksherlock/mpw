#pragma once

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>


/* Maximum values for memory, don't change */

#define CHIPMEM 0x200000
#define FASTMEM 0x800000
#define BOGOMEM 0x1c0000
#define KICKMEM 0x080000

typedef int BOOLE;

#define FALSE 0
#define TRUE  1

/*
#ifndef X64
#define PTR_TO_INT(i) ((uint32_t)i)
#define PTR_TO_INT_MASK_TYPE(i) ((uint32_t)i)
#endif
#ifdef X64
#define PTR_TO_INT(i) ((uint64_t)i)
#define PTR_TO_INT_MASK_TYPE(i) ((uint64_t)i)
#endif
*/

/* Filename length used throughout the code */

#define CFG_FILENAME_LENGTH 256

/*------------------------------------*/
/* The decode routines have this type */
/*------------------------------------*/

typedef void (*decoderoutinetype)(uint32_t, uint32_t);

extern uint8_t configromname[];

typedef union {
          uint32_t *lptr;
          uint16_t *wptr;
          uint8_t *bptr;
          uint32_t lval;
          uint16_t wval[2];
          uint8_t bval[4];
          } ptunion;

typedef void (*planar2chunkyroutine)(void);

typedef void (*playbuffer_routine)(void);
typedef void (*sound_before_emu_routine)(void);
typedef void (*sound_after_emu_routine)(void);

typedef void (*buseventfunc)(void);
