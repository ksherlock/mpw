/* @(#) $Id: CpuModule.c,v 1.7 2012-08-12 16:51:02 peschau Exp $ */
/*=========================================================================*/
/* Fellow                                                                  */
/* Initialization of 68000 core                                            */
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
#include "CpuModule.h"
#include "CpuModule_Memory.h"
#include "CpuModule_Internal.h"

void cpuClearEverything(void)
{
  ULO i,j;

  for (j = 0; j < 2; j++)
    for (i = 0; i < 8; i++)
      cpuSetReg(j, i, 0);

  cpuSetUspDirect(0);
  cpuSetSspDirect(0);
  cpuSetMspDirect(0);
  cpuSetPC(0);
  cpuClearPrefetch();
  cpuSetVbr(0);
  cpuSetSR(0);
  cpuSetCacr(0);
  cpuSetCaar(0);
  cpuSetSfc(0);
  cpuSetDfc(0);
  cpuSetIrqLevel(0);
  cpuSetRaiseInterrupt(FALSE);
  cpuSetStop(FALSE);
  cpuSetInstructionTime(0);
  cpuSetOriginalPC(0);
  cpuSetInitialPC(0);
  cpuSetInitialSP(0);
  cpuSetModel(0, 0);  // Also sets model-mask
  cpuSetCheckPendingInterruptsFunc(NULL);

#ifdef CPU_INSTRUCTION_LOGGING
  cpuSetCurrentOpcode(0);
  cpuSetInstructionLoggingFunc(NULL);
  cpuSetExceptionLoggingFunc(NULL);
  cpuSetInterruptLoggingFunc(NULL);
#endif

  cpuSetMidInstructionExceptionFunc(NULL);
  cpuSetResetExceptionFunc(NULL);
}

void cpuHardReset(void)
{
  cpuThrowResetException();
  cpuSetRaiseInterrupt(FALSE);
}

void cpuStartup(void)
{
  cpuClearEverything();
}
