/* @(#) $Id: CpuModule_Exceptions.c,v 1.5 2012-08-12 16:51:02 peschau Exp $ */
/*=========================================================================*/
/* Fellow                                                                  */
/* CPU 68k exception handling functions                                    */
/*                                                                         */
/* Author: Petter Schau                                                    */
/*                                                                         */
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
#include "CpuModule_Memory.h"

#include "CpuModule.h"
#include "CpuModule_Internal.h"

/* Function for exiting from mid-instruction exceptions */
static cpuMidInstructionExceptionFunc cpu_mid_instruction_exception_func;

static void cpuCallMidInstructionExceptionFunc(void)
{
  cpu_mid_instruction_exception_func();
}

void cpuSetMidInstructionExceptionFunc(cpuMidInstructionExceptionFunc func)
{
  cpu_mid_instruction_exception_func = func;
}

/* Function for notifying the emulator about a reset */
static cpuResetExceptionFunc cpu_reset_exception_func;

void cpuCallResetExceptionFunc(void)
{
  cpu_reset_exception_func();
}

void cpuSetResetExceptionFunc(cpuResetExceptionFunc func)
{
  cpu_reset_exception_func = func;
}

static STR *cpuGetExceptionName(ULO vector_offset)
{
  char *name;

  if (vector_offset == 0x8)
    name = "Exception: 2 - Access fault";
  else if (vector_offset == 0xc)
    name = "Exception: 3 - Address error";
  else if (vector_offset == 0x10)
    name = "Exception: 4 - Illegal Instruction";
  else if (vector_offset == 0x14)
    name = "Exception: 5 - Integer division by zero";
  else if (vector_offset == 0x18) 
    name = "Exception: 6 - CHK, CHK2";
  else if (vector_offset == 0x1c) 
    name = "Exception: 7 - FTRAPcc, TRAPcc, TRAPV";
  else if (vector_offset == 0x20) 
    name = "Exception: 8 - Privilege Violation";
  else if (vector_offset == 0x24) 
    name = "Exception: 9 - Trace";
  else if (vector_offset == 0x28) 
    name = "Exception: 10 - A-Line";
  else if (vector_offset == 0x2c) 
    name = "Exception: 11 - F-Line";
  else if (vector_offset == 0x38) 
    name = "Exception: 14 - Format error";
  else if (vector_offset >= 0x80 && vector_offset <= 0xbc)
    name = "Exception: TRAP"; 
  else
    name = "Exception: Unknown";

  return name;
}

/*===============================================
  Sets up an exception
  ===============================================*/

void cpuExceptionFail(BOOLE executejmp)
{
  // Avoid endless loop that will crash the emulator.
  // The (odd) address error exception vector contained an odd address.
  cpuCallResetExceptionFunc();
  cpuHardReset();
  cpuSetInstructionTime(132);
  if (executejmp)
  {
    cpuCallMidInstructionExceptionFunc(); // Supposed to be doing setjmp/longjmp back to machine emulator code
  }
}

void cpuThrowException(ULO vector_offset, ULO pc, BOOLE executejmp)
{
  ULO vector_address;
  BOOLE is_address_error_on_sub_020 = (cpuGetModelMajor() < 2 && vector_offset == 0xc);
  BOOLE stack_is_even = !(cpuGetAReg(7) & 1);
  BOOLE vbr_is_even = !(cpuGetVbr() & 1);

  if ((is_address_error_on_sub_020 && !stack_is_even) || !vbr_is_even)
  {
    cpuExceptionFail(executejmp);
    return;
  }

#ifdef CPU_INSTRUCTION_LOGGING
  cpuCallExceptionLoggingFunc(cpuGetExceptionName(vector_offset), cpuGetOriginalPC(), cpuGetCurrentOpcode());
#endif

  cpuActivateSSP();

  stack_is_even = !(cpuGetAReg(7) & 1);

  if (is_address_error_on_sub_020 && !stack_is_even)
  {
    cpuExceptionFail(executejmp);
    return;
  }

  cpuStackFrameGenerate((UWO) vector_offset, pc);

  // read a memory position
  vector_address = memoryReadLong(cpuGetVbr() + vector_offset);
  if (is_address_error_on_sub_020 && vector_address & 1)
  {
    cpuExceptionFail(executejmp);
    return;
  }
  else
  {
    // set supervisor modus
    cpuSetSR(cpuGetSR() | 0x2000); 
    cpuSetSR(cpuGetSR() & 0x3fff);

    // restart cpu, if needed
    cpuSetStop(FALSE);

    cpuInitializeFromNewPC(vector_address);
    cpuSetInstructionTime(40);
  }

  // If the exception happened mid-instruction...
  if (executejmp)
  {
    cpuCallMidInstructionExceptionFunc(); // Supposed to be doing setjmp/longjmp back to machine emulator code
  }
}

void cpuThrowPrivilegeViolationException(void)
{
  // The saved pc points to the instruction causing the violation
  // (And the kickstart excpects pc in the stack frame to be the opcode PC.)
  cpuThrowException(0x20, cpuGetOriginalPC(), FALSE);
}

void cpuThrowIllegalInstructionException(BOOLE executejmp)
{
  // The saved pc points to the illegal instruction
  cpuThrowException(0x10, cpuGetOriginalPC(), executejmp);
}

void cpuThrowALineException(void)
{
  // The saved pc points to the a-line instruction
  cpuThrowException(0x28, cpuGetOriginalPC(), FALSE);
}

void cpuThrowFLineException(void)
{
  // The saved pc points to the f-line instruction
  cpuThrowException(0x2c, cpuGetOriginalPC(), FALSE);
}

void cpuThrowTrapVException(void)
{
  // The saved pc points to the next instruction, which is now in pc
  cpuThrowException(0x1c, cpuGetPC(), FALSE);
}

void cpuThrowDivisionByZeroException()
{
  // The saved pc points to the next instruction, which is now in pc
  cpuThrowException(0x14, cpuGetPC(), FALSE);
}

void cpuThrowTrapException(ULO vector_no)
{
  // The saved pc points to the next instruction, which is now in pc
  cpuThrowException(0x80 + vector_no*4, cpuGetPC(), FALSE);
}

void cpuThrowChkException(void)
{
  // The saved pc points to the next instruction, which is now in pc
  cpuThrowException(0x18, cpuGetPC(), FALSE);
}

void cpuThrowTraceException(void)
{
  // The saved pc points to the next instruction, which is now in pc
  cpuThrowException(0x24, cpuGetPC(), FALSE);
}

void cpuThrowAddressErrorException(void)
{
  cpuThrowException(0xc, cpuGetPC() - 2, TRUE);
}

/*=================*/
/* Reset exception */
/*=================*/

static void cpuThrowResetException000(void)
{
  cpuSetSR(cpuGetSR() & 0x271f); /* T = 0 */
  cpuSetSR(cpuGetSR() | 0x2700); /* S = 1, ilvl = 7 */
  cpuSetVbr(0);
  cpuSetSspDirect(cpuGetInitialSP());        /* ssp = fake vector 0 */
  cpuInitializeFromNewPC(cpuGetInitialPC()); /* pc = fake vector 1 */
}

static void cpuThrowResetException010(void)
{
  cpuSetSR(cpuGetSR() & 0x271f); /* T = 0 */
  cpuSetSR(cpuGetSR() | 0x2700); /* S = 1, ilvl = 7 */
  cpuSetVbr(0);
  cpuSetSspDirect(cpuGetInitialSP());        /* ssp = fake vector 0 */
  cpuInitializeFromNewPC(cpuGetInitialPC()); /* pc = fake vector 1 */
}

static void cpuThrowResetException020(void)
{
  cpuSetSR(cpuGetSR() & 0x271f); /* T1T0 = 0, M = 0 */
  cpuSetSR(cpuGetSR() | 0x2700); /* S = 1, ilvl = 7 */
  cpuSetVbr(0);
  cpuSetCacr(0);             /* E = 0, F = 0 */
  cpuSetCaar(0);
  /* Invalidate cache, we don't have one */
  cpuSetSspDirect(cpuGetInitialSP());        /* ssp = fake vector 0 */
  cpuInitializeFromNewPC(cpuGetInitialPC()); /* pc = fake vector 1 */
}

static void cpuThrowResetException030(void)
{
  cpuSetSR(cpuGetSR() & 0x271f); /* T1T0 = 0, M = 0 */
  cpuSetSR(cpuGetSR() | 0x2700); /* S = 1, ilvl = 7 */
  cpuSetVbr(0);
  cpuSetCacr(0);             /* E = 0, F = 0 */
  cpuSetCaar(0);
  /* Invalidate cache, we don't have one */
  cpuSetSspDirect(cpuGetInitialSP());        /* ssp = fake vector 0 */
  cpuInitializeFromNewPC(cpuGetInitialPC()); /* pc = fake vector 1 */
}

/*============================*/
/* Performs a Reset exception */
/*============================*/

void cpuThrowResetException(void)
{
  cpuSetStop(FALSE);
  switch (cpuGetModelMajor())
  {
  case 0:
    cpuThrowResetException000();
    break;
  case 1:
    cpuThrowResetException010();
    break;
  case 2:
    cpuThrowResetException020();
    break;
  case 3:
    cpuThrowResetException030();
    break;
  }
}
