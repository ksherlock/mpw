/* @(#) $Id: CpuModule_StackFrameGen.c,v 1.3 2011-07-18 17:22:55 peschau Exp $ */
/*=========================================================================*/
/* Fellow                                                                  */
/* 68000 stack frame generation                                            */
/*                                                                         */
/* Author: Petter Schau                                                    */
/*                                                                         */
/* Copyright (C) 1991, 1992, 1996 Free Software Foundation, Inc.           */
/*                                                                         */
/* This program is free software; you can redistribute it and/or modify    */
/* it under the terms of the GNU General Public License as published by    */
/* the Free Software Foundation; either version 2, or (at your option)     */
/* any later version.                                                      */
/*                                                                         */
/* This program is distributed in the hope that it will be useful,         */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of          */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           */
/* GNU General Public License for more details.                            */
/*                                                                         */
/* You should have received a copy of the GNU General Public License       */
/* along with this program; if not, write to the Free Software Foundation, */
/* Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.          */
/*=========================================================================*/
#include "defs.h"
#include "fellow.h"
#include "fmem.h"

#include "CpuModule.h"
#include "CpuModule_Internal.h"

/* Exception stack frame jmptables */
typedef void(*cpuStackFrameGenFunc)(UWO, ULO);
static cpuStackFrameGenFunc cpu_stack_frame_gen_func[64];

static void cpuSetStackFrameGenFunc(ULO vector_no, cpuStackFrameGenFunc func)
{
  cpu_stack_frame_gen_func[vector_no] = func;
}

/*========================================================================
  Group 1 Frame format

  000:	All, except bus and address error
  ========================================================================*/

static void cpuFrameGroup1(UWO vector_offset, ULO pcPtr)
{
  // save PC
  cpuSetAReg(7, cpuGetAReg(7) - 4);
  memoryWriteLong(pcPtr, cpuGetAReg(7));

  // save SR
  cpuSetAReg(7, cpuGetAReg(7) - 2);
  memoryWriteWord((UWO)cpuGetSR(), cpuGetAReg(7));
}

/*========================================================================
  Group 2 Frame format
 
  000:	Bus and address error
 
  memory_fault_address contains the violating address
  memory_fault_read is TRUE if the access was a read
 ========================================================================*/
	
static void cpuFrameGroup2(UWO vector_offset, ULO pcPtr)
{
  // save PC
  cpuSetAReg(7, cpuGetAReg(7) - 4);
  memoryWriteLong(pcPtr, cpuGetAReg(7));

  // save SR
  cpuSetAReg(7, cpuGetAReg(7) - 2);
  memoryWriteWord((UWO)cpuGetSR(), cpuGetAReg(7));

  // fault address, skip ireg
  cpuSetAReg(7, cpuGetAReg(7) - 6);
  memoryWriteLong(memory_fault_address, cpuGetAReg(7));

  cpuSetAReg(7, cpuGetAReg(7) - 2);
  memoryWriteLong(memory_fault_read << 4, cpuGetAReg(7));
}

static void cpuFrame4Words(UWO frame_code, UWO vector_offset, ULO pc)
{
  // save vector_offset word
  cpuSetAReg(7, cpuGetAReg(7) - 2);
  memoryWriteWord(frame_code | vector_offset, cpuGetAReg(7));

  // save PC
  cpuSetAReg(7, cpuGetAReg(7) - 4);
  memoryWriteLong(pc, cpuGetAReg(7));

  // save SR
  cpuSetAReg(7, cpuGetAReg(7) - 2);
  memoryWriteWord((UWO)cpuGetSR(), cpuGetAReg(7));
}


/*========================================================================
  Frame format $0, four word frame
 
  Stack words:
  ------------
  SR
  PCHI
  PCLO
  0000 Vector offset (4 upper bits are frame no., rest is vector offset)
 
  010:	All, except bus and address errors
  020:	Irq, Format error, Trap #N, Illegal inst., A-line, F-line,
 	Priv. violation, copr preinst
  030:	Same as for 020
  ========================================================================*/

static void cpuFrame0(UWO vector_offset, ULO pc)
{
  cpuFrame4Words(0x0000, vector_offset, pc);
}

/*========================================================================
  Frame format $1, 4 word throwaway frame
 
  Stack words:
  ------------
  SR
  PCHI
  PCLO
  0000 Vector offset (4 upper bits are frame no., rest is Vvctor offset)
 
  020:	Irq, second frame created
  030:	Same as for 020
  040:	Same as for 020
  ========================================================================*/

void cpuFrame1(UWO vector_offset, ULO pc)
{
  cpuFrame4Words(0x1000, vector_offset, pc);
}

/*========================================================================
  Frame format $2
 
  020:	chk, chk2, cpTrapcc, trapcc, trapv, trace, zero divide, MMU config,
 	copr postinst 
  030:	Same as for 020
  040:	chk, chk2, FTrapcc, trapcc, trapv, trace, zero divide, address error,
 	Unimplemented FPU inst. 
  060:	Same as for 040
  ========================================================================*/

static void cpuFrame2(UWO vector_offset, ULO pc)
{
  // save inst address
  cpuSetAReg(7, cpuGetAReg(7) - 4);
  memoryWriteLong(cpuGetOriginalPC(), cpuGetAReg(7));
  cpuFrame4Words(0x2000, vector_offset, pc);
}

/*========================================================================
  Frame format $8
 
  010:	Bus and address error
 
  ========================================================================*/

static void cpuFrame8(UWO vector_offset, ULO pc)
{
  cpuSetAReg(7, cpuGetAReg(7) - 50);
  cpuFrame4Words(0x8000, vector_offset, pc);
}

/*========================================================================
  Frame format $A
 
  020:	Address or bus-error on instruction boundrary
  030:	Same as for 020
 
  Will not set any values beyond the format/offset word
  Fellow will always generate this frame for bus/address errors
  ========================================================================*/

static void cpuFrameA(UWO vector_offset, ULO pc)
{
  // save vector_offset offset
  cpuSetAReg(7, cpuGetAReg(7) - 24);
  cpuFrame4Words(0xa000, vector_offset, pc);
}

void cpuStackFrameGenerate(UWO vector_offset, ULO pc)
{
  cpu_stack_frame_gen_func[vector_offset>>2](vector_offset, pc);
}

/*==================================*/
/* Initialize stack frame jmptables */
/*==================================*/

static void cpuStackFrameInitSetDefaultFunc(cpuStackFrameGenFunc default_func)
{
  ULO i;
  for (i = 0; i < 64; i++)
    cpuSetStackFrameGenFunc(i, default_func);
}

static void cpuStackFrameInit000(void)
{
  cpuStackFrameInitSetDefaultFunc(cpuFrameGroup1);
  cpuSetStackFrameGenFunc(2, cpuFrameGroup2); /* 2 - Bus error */
  cpuSetStackFrameGenFunc(3, cpuFrameGroup2); /* 3 - Address error */
}

static void cpuStackFrameInit010(void)
{
  cpuStackFrameInitSetDefaultFunc(cpuFrame0);
  cpuSetStackFrameGenFunc(2, cpuFrame8); /* 2 - Bus error */
  cpuSetStackFrameGenFunc(3, cpuFrame8); /* 3 - Address error */
}

static void cpuStackFrameInit020(void)
{
  cpuStackFrameInitSetDefaultFunc(cpuFrame0);
  cpuSetStackFrameGenFunc(2, cpuFrameA);  /* 2  - Bus Error */
  cpuSetStackFrameGenFunc(3, cpuFrameA);  /* 3  - Addrss Error */
  cpuSetStackFrameGenFunc(5, cpuFrame2);  /* 5  - Zero Divide */
  cpuSetStackFrameGenFunc(6, cpuFrame2);  /* 6  - CHK, CHK2 */
  cpuSetStackFrameGenFunc(7, cpuFrame2);  /* 7  - TRAPV, TRAPcc, cpTRAPcc */
  cpuSetStackFrameGenFunc(9, cpuFrame2);  /* 9  - Trace */
}

static void cpuStackFrameInit030(void)
{
  cpuStackFrameInitSetDefaultFunc(cpuFrame0);
  cpuSetStackFrameGenFunc(2, cpuFrameA);  /* 2  - Bus Error */
  cpuSetStackFrameGenFunc(3, cpuFrameA);  /* 3  - Addrss Error */
  cpuSetStackFrameGenFunc(5, cpuFrame2);  /* 5  - Zero Divide */
  cpuSetStackFrameGenFunc(6, cpuFrame2);  /* 6  - CHK, CHK2 */
  cpuSetStackFrameGenFunc(7, cpuFrame2);  /* 7  - TRAPV, TRAPcc, cpTRAPcc */
  cpuSetStackFrameGenFunc(9, cpuFrame2);  /* 9  - Trace */
}

void cpuStackFrameInit(void)
{
  switch (cpuGetModelMajor())
  {
    case 0:
      cpuStackFrameInit000();
      break;
    case 1:
      cpuStackFrameInit010();
      break;
    case 2:
      cpuStackFrameInit020();
      break;
    case 3:
      cpuStackFrameInit030();
      break;
  }
}
