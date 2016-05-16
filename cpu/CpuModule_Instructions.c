/* @(#) $Id: CpuModule_Instructions.c,v 1.12 2013-01-08 18:55:48 peschau Exp $ */
/*=========================================================================*/
/* Fellow                                                                  */
/* CPU 68k functions                                                       */
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


static cpuLineExceptionFunc cpu_a_line_exception_func = NULL;
static cpuLineExceptionFunc cpu_f_line_exception_func = NULL;

void cpuSetALineExceptionFunc(cpuLineExceptionFunc func)
{
  cpu_a_line_exception_func = func;
}

void cpuSetFLineExceptionFunc(cpuLineExceptionFunc func)
{
  cpu_f_line_exception_func = func;
}

/*============================================================================*/
/* profiling help functions                                                   */
/*============================================================================*/


#if 0
#ifndef X64
static __inline void cpuTscBefore(LLO* a)
{
  LLO local_a = *a;
  __asm 
  {
      push    eax
      push    edx
      push    ecx
      mov     ecx,10h
      rdtsc
      pop     ecx
      mov     dword ptr [local_a], eax
      mov     dword ptr [local_a + 4], edx
      pop     edx
      pop     eax
  }
  *a = local_a;
}

static __inline void cpuTscAfter(LLO* a, LLO* b, ULO* c)
{
  LLO local_a = *a;
  LLO local_b = *b;
  ULO local_c = *c;

  __asm 
  {
      push    eax
      push    edx
      push    ecx
      mov     ecx, 10h
      rdtsc
      pop     ecx
      sub     eax, dword ptr [local_a]
      sbb     edx, dword ptr [local_a + 4]
      add     dword ptr [local_b], eax
      adc     dword ptr [local_b + 4], edx
      inc     dword ptr [local_c]
      pop     edx
      pop     eax
  }
  *a = local_a;
  *b = local_b;
  *c = local_c;
}
#endif
#endif

/* Maintains the integrity of the super/user state */

void cpuUpdateSr(ULO new_sr)
{
  BOOLE supermode_was_set = cpuGetFlagSupervisor();
  BOOLE master_was_set = (cpuGetModelMajor() >= 2) && cpuGetFlagMaster();

  BOOLE supermode_is_set = !!(new_sr & 0x2000);
  BOOLE master_is_set = (cpuGetModelMajor() >= 2) && !!(new_sr & 0x1000);

  ULO runlevel_old = (cpuGetSR() >> 8) & 7;
  ULO runlevel_new = (new_sr >> 8) & 7;

  if (!supermode_was_set)
  {
    cpuSetUspDirect(cpuGetAReg(7));
  }
  else if (master_was_set)
  {
    cpuSetMspDirect(cpuGetAReg(7));
  }
  else
  {
    cpuSetSspDirect(cpuGetAReg(7));
  }

  if (!supermode_is_set)
  {
    cpuSetAReg(7, cpuGetUspDirect());
  }
  else if (master_is_set)
  {
    cpuSetAReg(7, cpuGetMspDirect());
  }
  else
  {
    cpuSetAReg(7, cpuGetSspDirect());
  }

  cpuSetSR(new_sr);

  if (runlevel_old != runlevel_new)
  {
    cpuCheckPendingInterrupts();
  }
}

static void cpuIllegal(void)
{
  UWO opcode = memoryReadWord(cpuGetPC() - 2);
  if ((opcode & 0xf000) == 0xf000)
  {
    if (cpu_f_line_exception_func)
    {
      cpu_f_line_exception_func(opcode);
      cpuInitializeFromNewPC(cpuGetPC());
      cpuSetInstructionTime(512);
    }
    else
    {
    cpuThrowFLineException();
  }
	}
  else if ((opcode & 0xa000) == 0xa000)
  {
    #if 0
    if ((cpuGetPC() & 0xff0000) == 0xf00000)
    {
      call_calltrap(opcode & 0xfff);
      cpuInitializeFromNewPC(cpuGetPC());
      cpuSetInstructionTime(512);
    }
    #else
    if (cpu_a_line_exception_func)
    {
      cpu_a_line_exception_func(opcode);
      cpuInitializeFromNewPC(cpuGetPC());
      cpuSetInstructionTime(512);
    }
    #endif
    else
    {
      cpuThrowALineException();
    }
  }
  else
  {
    cpuThrowIllegalInstructionException(FALSE);
  }
}

/// <summary>
/// Illegal instruction handler.
/// </summary>
static void cpuIllegalInstruction(ULO *opcode_data)
{
  cpuIllegal(); 
}

/// <summary>
/// BKPT
/// </summary>
static void cpuBkpt(ULO vector)
{
  cpuIllegal();
}

/// <summary>
/// Adds bytes src1 to src2. Sets all flags.
/// </summary>
/// <returns>The result</returns>
static UBY cpuAddB(UBY src2, UBY src1)
{
  UBY res = src2 + src1;
  cpuSetFlagsAdd(cpuIsZeroB(res), cpuMsbB(res), cpuMsbB(src2), cpuMsbB(src1));
  return res;
}

/// <summary>
/// Adds words src1 to src2. Sets all flags.
/// </summary>
/// <returns>The result</returns>
static UWO cpuAddW(UWO src2, UWO src1)
{
  UWO res = src2 + src1;
  cpuSetFlagsAdd(cpuIsZeroW(res), cpuMsbW(res), cpuMsbW(src2), cpuMsbW(src1));
  return res;
}

/// <summary>
/// Adds dwords src1 to src2. Sets all flags.
/// </summary>
/// <returns>The result</returns>
static ULO cpuAddL(ULO src2, ULO src1)
{
  ULO res = src2 + src1;
  cpuSetFlagsAdd(cpuIsZeroL(res), cpuMsbL(res), cpuMsbL(src2), cpuMsbL(src1));
  return res;
}

/// <summary>
/// Adds src1 to src2 (For address registers). No flags.
/// </summary>
/// <returns>The result</returns>
static ULO cpuAddaW(ULO src2, ULO src1)
{
  return src2 + src1;
}

/// <summary>
/// Adds src1 to src2 (For address registers). No flags.
/// </summary>
/// <returns>The result</returns>
static ULO cpuAddaL(ULO src2, ULO src1)
{
  return src2 + src1;
}

/// <summary>
/// Subtracts src1 from src2. Sets all flags.
/// </summary>
/// <returns>The result</returns>
static UBY cpuSubB(UBY src2, UBY src1)
{
  UBY res = src2 - src1;
  cpuSetFlagsSub(cpuIsZeroB(res), cpuMsbB(res), cpuMsbB(src2), cpuMsbB(src1));
  return res;
}

/// <summary>
/// Subtracts src1 from src2. Sets all flags.
/// </summary>
/// <returns>The result</returns>
static UWO cpuSubW(UWO src2, UWO src1)
{
  UWO res = src2 - src1;
  cpuSetFlagsSub(cpuIsZeroW(res), cpuMsbW(res), cpuMsbW(src2), cpuMsbW(src1));
  return res;
}

/// <summary>
/// Subtracts src1 from src2. Sets all flags.
/// </summary>
/// <returns>The result</returns>
static ULO cpuSubL(ULO src2, ULO src1)
{
  ULO res = src2 - src1;
  cpuSetFlagsSub(cpuIsZeroL(res), cpuMsbL(res), cpuMsbL(src2), cpuMsbL(src1));
  return res;
}

/// <summary>
/// Subtracts src1 from src2 (For address registers). No flags.
/// </summary>
/// <returns>The result</returns>
static ULO cpuSubaW(ULO src2, ULO src1)
{
  return src2 - src1;
}

/// <summary>
/// Subtracts src1 from src2 (For address registers). No flags.
/// </summary>
/// <returns>The result</returns>
static ULO cpuSubaL(ULO src2, ULO src1)
{
  return src2 - src1;
}

/// <summary>
/// Subtracts src1 from src2. Sets all flags.
/// </summary>
static void cpuCmpB(UBY src2, UBY src1)
{
  UBY res = src2 - src1;
  cpuSetFlagsCmp(cpuIsZeroB(res), cpuMsbB(res), cpuMsbB(src2), cpuMsbB(src1));
}

/// <summary>
/// Subtracts src1 from src2. Sets all flags.
/// </summary>
static void cpuCmpW(UWO src2, UWO src1)
{
  UWO res = src2 - src1;
  cpuSetFlagsCmp(cpuIsZeroW(res), cpuMsbW(res), cpuMsbW(src2), cpuMsbW(src1));
}

/// <summary>
/// Subtracts src1 from src2. Sets all flags.
/// </summary>
static void cpuCmpL(ULO src2, ULO src1)
{
  ULO res = src2 - src1;
  cpuSetFlagsCmp(cpuIsZeroL(res), cpuMsbL(res), cpuMsbL(src2), cpuMsbL(src1));
}

/// <summary>
/// Ands src1 to src2. Sets NZ00 flags.
/// </summary>
/// <returns>The result</returns>
static UBY cpuAndB(UBY src2, UBY src1)
{
  UBY res = src2 & src1;
  cpuSetFlagsNZ00NewB(res);
  return res;
}

/// <summary>
/// Ands src1 to src2. Sets NZ00 flags.
/// </summary>
/// <returns>The result</returns>
static UWO cpuAndW(UWO src2, UWO src1)
{
  UWO res = src2 & src1;
  cpuSetFlagsNZ00NewW(res);
  return res;
}

/// <summary>
/// Ands src1 to src2. Sets NZ00 flags.
/// </summary>
/// <returns>The result</returns>
static ULO cpuAndL(ULO src2, ULO src1)
{
  ULO res = src2 & src1;
  cpuSetFlagsNZ00NewL(res);
  return res;
}

/// <summary>
/// Eors src1 to src2. Sets NZ00 flags.
/// </summary>
/// <returns>The result</returns>
static UBY cpuEorB(UBY src2, UBY src1)
{
  UBY res = src2 ^ src1;
  cpuSetFlagsNZ00NewB(res);
  return res;
}

/// <summary>
/// Eors src1 to src2. Sets NZ00 flags.
/// </summary>
/// <returns>The result</returns>
static UWO cpuEorW(UWO src2, UWO src1)
{
  UWO res = src2 ^ src1;
  cpuSetFlagsNZ00NewW(res);
  return res;
}

/// <summary>
/// Eors src1 to src2. Sets NZ00 flags.
/// </summary>
/// <returns>The result</returns>
static ULO cpuEorL(ULO src2, ULO src1)
{
  ULO res = src2 ^ src1;
  cpuSetFlagsNZ00NewL(res);
  return res;
}

/// <summary>
/// Ors src1 to src2. Sets NZ00 flags.
/// </summary>
/// <returns>The result</returns>
static UBY cpuOrB(UBY src2, UBY src1)
{
  UBY res = src2 | src1;
  cpuSetFlagsNZ00NewB(res);
  return res;
}

/// <summary>
/// Ors src1 to src2. Sets NZ00 flags.
/// </summary>
/// <returns>The result</returns>
static UWO cpuOrW(UWO src2, UWO src1)
{
  UWO res = src2 | src1;
  cpuSetFlagsNZ00NewW(res);
  return res;
}

/// <summary>
/// Ors src1 to src2. Sets NZ00 flags.
/// </summary>
/// <returns>The result</returns>
static ULO cpuOrL(ULO src2, ULO src1)
{
  ULO res = src2 | src1;
  cpuSetFlagsNZ00NewL(res);
  return res;
}

/// <summary>
/// Changes bit in src. Sets Z flag.
/// </summary>
/// <returns>The result</returns>
static UBY cpuBchgB(UBY src, UBY bit)
{
  UBY bit_mask = 1 << (bit & 7);
  cpuSetZFlagBitOpsB(src & bit_mask);
  return src ^ bit_mask;
}

/// <summary>
/// Changes bit in src. Sets Z flag.
/// </summary>
/// <returns>The result</returns>
static ULO cpuBchgL(ULO src, ULO bit)
{
  ULO bit_mask = 1 << (bit & 31);
  cpuSetZFlagBitOpsL(src & bit_mask);
  return src ^ bit_mask;
}

/// <summary>
/// Clears bit in src. Sets Z flag.
/// </summary>
/// <returns>The result</returns>
static UBY cpuBclrB(UBY src, UBY bit)
{
  UBY bit_mask = 1 << (bit & 7);
  cpuSetZFlagBitOpsB(src & bit_mask);
  return src & ~bit_mask;
}

/// <summary>
/// Clears bit in src. Sets Z flag.
/// </summary>
/// <returns>The result</returns>
static ULO cpuBclrL(ULO src, ULO bit)
{
  ULO bit_mask = 1 << (bit & 31);
  cpuSetZFlagBitOpsL(src & bit_mask);
  return src & ~bit_mask;
}

/// <summary>
/// Sets bit in src. Sets Z flag.
/// </summary>
/// <returns>The result</returns>
static UBY cpuBsetB(UBY src, UBY bit)
{
  UBY bit_mask = 1 << (bit & 7);
  cpuSetZFlagBitOpsB(src & bit_mask);
  return src | bit_mask;
}

/// <summary>
/// Sets bit in src. Sets Z flag.
/// </summary>
/// <returns>The result</returns>
static ULO cpuBsetL(ULO src, ULO bit)
{
  ULO bit_mask = 1 << (bit & 31);
  cpuSetZFlagBitOpsL(src & bit_mask);
  return src | bit_mask;
}

/// <summary>
/// Tests bit in src. Sets Z flag.
/// </summary>
static void cpuBtstB(UBY src, UBY bit)
{
  UBY bit_mask = 1 << (bit & 7);
  cpuSetZFlagBitOpsB(src & bit_mask);
}

/// <summary>
/// Tests bit in src. Sets Z flag.
/// </summary>
static void cpuBtstL(ULO src, ULO bit)
{
  ULO bit_mask = 1 << (bit & 31);
  cpuSetZFlagBitOpsL(src & bit_mask);
}

/// <summary>
/// Set flags for clr operation.  X0100.
/// </summary>
static void cpuClr()
{
  cpuSetFlags0100();
}

/// <summary>
/// Neg src1. Sets sub flags.
/// </summary>
/// <returns>The result</returns>
static UBY cpuNegB(UBY src1)
{
  UBY res = (UBY)-(BYT)src1;
  cpuSetFlagsNeg(cpuIsZeroB(res), cpuMsbB(res), cpuMsbB(src1));
  return res;
}

/// <summary>
/// Neg src1. Sets sub flags.
/// </summary>
/// <returns>The result</returns>
static UWO cpuNegW(UWO src1)
{
  UWO res = (UWO)-(WOR)src1;
  cpuSetFlagsNeg(cpuIsZeroW(res), cpuMsbW(res), cpuMsbW(src1));
  return res;
}

/// <summary>
/// Neg src1. Sets sub flags.
/// </summary>
/// <returns>The result</returns>
static ULO cpuNegL(ULO src1)
{
  ULO res = (ULO)-(LON)src1;
  cpuSetFlagsNeg(cpuIsZeroL(res), cpuMsbL(res), cpuMsbL(src1));
  return res;
}

/// <summary>
/// Negx src1.
/// </summary>
/// <returns>The result</returns>
static UBY cpuNegxB(UBY src1)
{
  BYT x = (cpuGetFlagX()) ? 1 : 0;
  UBY res = (UBY)-(BYT)src1 - x;
  cpuSetFlagsNegx(cpuIsZeroB(res), cpuMsbB(res), cpuMsbB(src1));
  return res;
}

/// <summary>
/// Negx src1.
/// </summary>
/// <returns>The result</returns>
static UWO cpuNegxW(UWO src1)
{
  WOR x = (cpuGetFlagX()) ? 1 : 0;
  UWO res = (UWO)-(WOR)src1 - x;
  cpuSetFlagsNegx(cpuIsZeroW(res), cpuMsbW(res), cpuMsbW(src1));
  return res;
}

/// <summary>
/// Negx src1.
/// </summary>
/// <returns>The result</returns>
static ULO cpuNegxL(ULO src1)
{
  LON x = (cpuGetFlagX()) ? 1 : 0;
  ULO res = (ULO)-(LON)src1 - x;
  cpuSetFlagsNegx(cpuIsZeroL(res), cpuMsbL(res), cpuMsbL(src1));
  return res;
}

/// <summary>
/// Not src1. 
/// </summary>
/// <returns>The result</returns>
static UBY cpuNotB(UBY src1)
{
  UBY res = ~src1;
  cpuSetFlagsNZ00NewB(res);
  return res;
}

/// <summary>
/// Not src1. 
/// </summary>
/// <returns>The result</returns>
static UWO cpuNotW(UWO src1)
{
  UWO res = ~src1;
  cpuSetFlagsNZ00NewW(res);
  return res;
}

/// <summary>
/// Not src.
/// </summary>
/// <returns>The result</returns>
static ULO cpuNotL(ULO src)
{
  ULO res = ~src;
  cpuSetFlagsNZ00NewL(res);
  return res;
}

/// <summary>
/// Tas src. 
/// </summary>
/// <returns>The result</returns>
static UBY cpuTas(UBY src)
{
  cpuSetFlagsNZ00NewB(src);
  return src | 0x80;
}

/// <summary>
/// Tst res. 
/// </summary>
static void cpuTestB(UBY res)
{
  cpuSetFlagsNZ00NewB(res);
}

/// <summary>
/// Tst res. 
/// </summary>
static void cpuTestW(UWO res)
{
  cpuSetFlagsNZ00NewW(res);
}

/// <summary>
/// Tst res. 
/// </summary>
static void cpuTestL(ULO res)
{
  cpuSetFlagsNZ00NewL(res);
}

/// <summary>
/// PEA ea. 
/// </summary>
static void cpuPeaL(ULO ea)
{
  cpuSetAReg(7, cpuGetAReg(7) - 4);
  memoryWriteLong(ea, cpuGetAReg(7));
}

/// <summary>
/// JMP ea. 
/// </summary>
static void cpuJmp(ULO ea)
{
  cpuInitializeFromNewPC(ea);
}

/// <summary>
/// JSR ea. 
/// </summary>
static void cpuJsr(ULO ea)
{
  cpuSetAReg(7, cpuGetAReg(7) - 4);
  memoryWriteLong(cpuGetPC(), cpuGetAReg(7));
  cpuInitializeFromNewPC(ea);
}

/// <summary>
/// Move res
/// </summary>
/// <returns>The result</returns>
static void cpuMoveB(UBY res)
{
  cpuSetFlagsNZ00NewB(res);
}

/// <summary>
/// Move res
/// </summary>
/// <returns>The result</returns>
static void cpuMoveW(UWO res)
{
  cpuSetFlagsNZ00NewW(res);
}

/// <summary>
/// Move res 
/// </summary>
/// <returns>The result</returns>
static void cpuMoveL(ULO res)
{
  cpuSetFlagsNZ00NewL(res);
}

/// <summary>
/// Bra byte offset. 
/// </summary>
static void cpuBraB(ULO offset)
{
  cpuInitializeFromNewPC(cpuGetPC() + offset);
  cpuSetInstructionTime(10);
}

/// <summary>
/// Bra word offset. 
/// </summary>
static void cpuBraW()
{
  ULO tmp_pc = cpuGetPC();
  cpuInitializeFromNewPC(tmp_pc + cpuGetNextWordSignExt());
  cpuSetInstructionTime(10);
}

/// <summary>
/// Bra long offset. 
/// </summary>
static void cpuBraL()
{
  if (cpuGetModelMajor() < 2) cpuBraB(0xffffffff);
  else
  {
    ULO tmp_pc = cpuGetPC();
    cpuInitializeFromNewPC(tmp_pc + cpuGetNextLong());
    cpuSetInstructionTime(4);
  }
}

/// <summary>
/// Bsr byte offset. 
/// </summary>
static void cpuBsrB(ULO offset)
{
  cpuSetAReg(7, cpuGetAReg(7) - 4);
  memoryWriteLong(cpuGetPC(), cpuGetAReg(7));
  cpuInitializeFromNewPC(cpuGetPC() + offset);
  cpuSetInstructionTime(18);
}

/// <summary>
/// Bsr word offset. 
/// </summary>
static void cpuBsrW()
{
  ULO tmp_pc = cpuGetPC();
  ULO offset = cpuGetNextWordSignExt();
  cpuSetAReg(7, cpuGetAReg(7) - 4);
  memoryWriteLong(cpuGetPC(), cpuGetAReg(7));
  cpuInitializeFromNewPC(tmp_pc + offset);
  cpuSetInstructionTime(18);
}

/// <summary>
/// Bsr long offset. (020+) 
/// </summary>
static void cpuBsrL()
{
  if (cpuGetModelMajor() < 2) cpuBsrB(0xffffffff);
  else
  {
    ULO tmp_pc = cpuGetPC();
    ULO offset = cpuGetNextLong();
    cpuSetAReg(7, cpuGetAReg(7) - 4);
    memoryWriteLong(cpuGetPC(), cpuGetAReg(7));
    cpuInitializeFromNewPC(tmp_pc + offset);
    cpuSetInstructionTime(4);
  }
}

/// <summary>
/// Bcc byte offset. 
/// </summary>
static void cpuBccB(BOOLE cc, ULO offset)
{
  if (cc)
  {
    cpuInitializeFromNewPC(cpuGetPC() + offset);
    cpuSetInstructionTime(10);
  }
  else
  {
    cpuSetInstructionTime(8);
  }
}

/// <summary>
/// Bcc word offset. 
/// </summary>
static void cpuBccW(BOOLE cc)
{
  if (cc)
  {
    ULO tmp_pc = cpuGetPC();
    cpuInitializeFromNewPC(tmp_pc + cpuGetNextWordSignExt());
    cpuSetInstructionTime(10);
  }
  else
  {
    cpuSkipNextWord();
    cpuSetInstructionTime(12);
  }
}

/// <summary>
/// Bcc long offset. (020+)
/// </summary>
static void cpuBccL(BOOLE cc)
{
  if (cpuGetModelMajor() < 2) cpuBccB(cc, 0xffffffff);
  else
  {
    if (cc)
    {
      ULO tmp_pc = cpuGetPC();
      cpuInitializeFromNewPC(tmp_pc + cpuGetNextLong());
    }
    else
    {
      cpuSkipNextLong();
    }
    cpuSetInstructionTime(4);
  }
}

/// <summary>
/// DBcc word offset. 
/// </summary>
static void cpuDbcc(BOOLE cc, ULO reg)
{
  if (!cc)
  {
    WOR val = (WOR)cpuGetDRegWord(reg);
    val--;
    cpuSetDRegWord(reg, val);
    if (val != -1)
    {
      ULO tmp_pc = cpuGetPC();
      cpuInitializeFromNewPC(tmp_pc + cpuGetNextWordSignExt());
      cpuSetInstructionTime(10);
    }
    else
    {
      cpuSkipNextWord();
      cpuSetInstructionTime(14);
    }
  }
  else
  {
    cpuSkipNextWord();
    cpuSetInstructionTime(12);
  }
}

/// <summary>
/// And #imm, ccr 
/// </summary>
static void cpuAndCcrB()
{
  UWO imm = cpuGetNextWord();
  cpuSetSR(cpuGetSR() & (0xffe0 | (imm & 0x1f)));
}

/// <summary>
/// And #imm, sr 
/// </summary>
static void cpuAndSrW()
{
  if (cpuGetFlagSupervisor())
  {
    UWO imm = cpuGetNextWord();
    cpuUpdateSr(cpuGetSR() & imm);
  }
  else
  {
    cpuThrowPrivilegeViolationException();
  }
}

/// <summary>
/// Or #imm, ccr 
/// </summary>
static void cpuOrCcrB()
{
  UWO imm = cpuGetNextWord();
  cpuSetSR(cpuGetSR() | (imm & 0x1f));
}

/// <summary>
/// Or #imm, sr 
/// </summary>
static void cpuOrSrW()
{
  if (cpuGetFlagSupervisor())
  {
    UWO imm = cpuGetNextWord();
    cpuUpdateSr(cpuGetSR() | imm);
  }
  else
  {
    cpuThrowPrivilegeViolationException();
  }
}

/// <summary>
/// Eor #imm, ccr 
/// </summary>
static void cpuEorCcrB()
{
  UWO imm = cpuGetNextWord();
  cpuSetSR(cpuGetSR() ^ (imm & 0x1f));
}

/// <summary>
/// Eor #imm, sr 
/// </summary>
static void cpuEorSrW()
{
  if (cpuGetFlagSupervisor())
  {
    UWO imm = cpuGetNextWord();
    cpuUpdateSr(cpuGetSR() ^ imm);
  }
  else
  {
    cpuThrowPrivilegeViolationException();
  }
}

/// <summary>
/// Move ea, ccr 
/// </summary>
static void cpuMoveToCcr(UWO src)
{
  cpuSetSR((cpuGetSR() & 0xff00) | (src & 0x1f));
}

/// <summary>
/// Move <ea>, sr 
/// </summary>
static void cpuMoveToSr(UWO src)
{
  if (cpuGetFlagSupervisor())
  {
    cpuUpdateSr(src);
  }
  else
  {
    cpuThrowPrivilegeViolationException();
  }
}

/// <summary>
/// Move ccr, ea 
/// </summary>
static UWO cpuMoveFromCcr()
{
  return cpuGetSR() & 0x1f;
}

/// <summary>
/// Move <ea>, sr 
/// </summary>
static UWO cpuMoveFromSr()
{
  if (cpuGetModelMajor() == 0 || (cpuGetModelMajor() > 0 && cpuGetFlagSupervisor()))
  {
    return (UWO) cpuGetSR();
  }
  else
  {
    cpuThrowPrivilegeViolationException();
  } 
  return 0;
}

/// <summary>
/// Scc byte. 
/// </summary>
static UBY cpuScc(ULO cc)
{
  return (cpuCalculateConditionCode(cc)) ? 0xff : 0;
}

/// <summary>
/// Rts 
/// </summary>
static void cpuRts()
{
  cpuInitializeFromNewPC(memoryReadLong(cpuGetAReg(7)));
  cpuSetAReg(7, cpuGetAReg(7) + 4);
  cpuSetInstructionTime(16);
}

/// <summary>
/// Rtr 
/// </summary>
static void cpuRtr()
{
  cpuSetSR((cpuGetSR() & 0xffe0) | (memoryReadWord(cpuGetAReg(7)) & 0x1f));
  cpuSetAReg(7, cpuGetAReg(7) + 2);
  cpuInitializeFromNewPC(memoryReadLong(cpuGetAReg(7)));
  cpuSetAReg(7, cpuGetAReg(7) + 4);
  cpuSetInstructionTime(20);
}

/// <summary>
/// Nop 
/// </summary>
static void cpuNop()
{
  cpuSetInstructionTime(4);
}

/// <summary>
/// Trapv
/// </summary>
static void cpuTrapv()
{
  if (cpuGetFlagV())
  {
    cpuThrowTrapVException();
    return;
  }
  cpuSetInstructionTime(4);
}

/// <summary>
/// Muls/u.l
/// </summary>
static void cpuMulL(ULO src1, UWO extension)
{
  ULO dl = (extension >> 12) & 7;
  if (extension & 0x0800) // muls.l
  {
    LLO result = ((LLO)(LON) src1) * ((LLO)(LON)cpuGetDReg(dl));
    if (extension & 0x0400) // 32bx32b=64b
    {  
      ULO dh = extension & 7;
      cpuSetDReg(dh, (ULO)(result >> 32));
      cpuSetDReg(dl, (ULO)result);
      cpuSetFlagsNZ00New64(result);
    }
    else // 32bx32b=32b
    {
      BOOLE o;
      if (result >= 0)
  o = (result & 0xffffffff00000000) != 0;
      else
  o = (result & 0xffffffff00000000) != 0xffffffff00000000;
      cpuSetDReg(dl, (ULO)result);
      cpuSetFlagsNZVC(result == 0, result < 0, o, FALSE);
    }
  }
  else // mulu.l
  {
    ULL result = ((ULL) src1) * ((ULL) cpuGetDReg(dl));
    if (extension & 0x0400) // 32bx32b=64b
    {  
      ULO dh = extension & 7;
      cpuSetDReg(dh, (ULO)(result >> 32));
      cpuSetDReg(dl, (ULO)result);
      cpuSetFlagsNZ00New64(result);
    }
    else // 32bx32b=32b
    {
      cpuSetDReg(dl, (ULO)result);
      cpuSetFlagsNZVC(result == 0, !!(result & 0x80000000), (result >> 32) != 0, FALSE);
    }
  }
  cpuSetInstructionTime(4);
}

UBY cpuMuluTime[256];
UBY cpuMulsTime[512];

void cpuCreateMuluTimeTable(void)
{
  ULO i, j, k;

  for (i = 0; i < 256; i++)
  {
    j = 0;
    for (k = 0; k < 8; k++)
      if (((i>>k) & 1) == 1)
  j++;
    cpuMuluTime[i] = (UBY) j*2;
  }
}

void cpuCreateMulsTimeTable(void)
{
  ULO i, j, k;

  for (i = 0; i < 512; i++)
  {
    j = 0;
    for (k = 0; k < 9; k++)
      if ((((i>>k) & 3) == 1) || (((i>>k) & 3) == 2))
  j++; 
    cpuMulsTime[i] = (UBY) j*2;
  }
}

void cpuCreateMulTimeTables(void)
{
  cpuCreateMuluTimeTable();
  cpuCreateMulsTimeTable();
}

/// <summary>
/// Muls.w
/// </summary>
static ULO cpuMulsW(UWO src2, UWO src1, ULO eatime)
{
  ULO res = (ULO)(((LON)(WOR)src2)*((LON)(WOR)src1));
  cpuSetFlagsNZ00NewL(res);
  cpuSetInstructionTime(38 + eatime + cpuMulsTime[(src1 << 1) & 0x1ff] + cpuMulsTime[src1 >> 7]);
  return res;
}

/// <summary>
/// Mulu.w
/// </summary>
static ULO cpuMuluW(UWO src2, UWO src1, ULO eatime)
{
  ULO res = ((ULO)src2)*((ULO)src1);
  cpuSetFlagsNZ00NewL(res);
  cpuSetInstructionTime(38 + eatime + cpuMuluTime[src1 & 0xff] + cpuMuluTime[src1 >> 8]);
  return res;
}

/// <summary>
/// Divsw, src1 / src2
/// </summary>
static void cpuDivsW(ULO dst, UWO src1, ULO destination_reg, ULO instruction_time)
{
  if (src1 == 0)
  {
    // Alcatraz odyssey assumes that PC in this exception points after the instruction.
    cpuThrowDivisionByZeroException();
  }
  else
  {
    ULO result;
    LON x = (LON) dst;
    LON y = (LON)(WOR) src1;
    LON res = x / y;
    LON rem = x % y;
    if (res > 32767 || res < -32768)
    {
      result = dst;
      cpuSetFlagsVC(TRUE, FALSE);
    }
    else
    {
      result = (rem << 16) | (res & 0xffff);
      cpuSetFlagsNZVC(cpuIsZeroW((UWO) res), cpuMsbW((UWO) res), FALSE, FALSE);
    }
    cpuSetDReg(destination_reg, result);
    cpuSetInstructionTime(instruction_time);
  }
}

/// <summary>
/// Divuw, src1 / src2
/// </summary>
static void cpuDivuW(ULO dst, UWO src1, ULO destination_reg, ULO instruction_time)
{
  if (src1 == 0)
  {
    // Alcatraz odyssey assumes that PC in this exception points after the instruction.
    cpuThrowDivisionByZeroException();
  }
  else
  {
    ULO result;
    ULO x = dst;
    ULO y = (ULO) src1;
    ULO res = x / y;
    ULO rem = x % y;
    if (res > 65535)
    {
      result = dst;
      cpuSetFlagsVC(TRUE, FALSE);
    }
    else
    {
      result = (rem << 16) | (res & 0xffff);
      cpuSetFlagsNZVC(cpuIsZeroW((UWO) res), cpuMsbW((UWO) res), FALSE, FALSE);
    }
    cpuSetDReg(destination_reg, result);
    cpuSetInstructionTime(instruction_time);
  }
}

static void cpuDivL(ULO divisor, ULO ext, ULO instruction_time)
{
  if (divisor != 0)
  {
    ULO dq_reg = (ext>>12) & 7; /* Get operand registers, size and sign */
    ULO dr_reg = ext & 7;
    BOOLE size64 = (ext>>10) & 1;
    BOOLE sign = (ext>>11) & 1;
    BOOLE resultsigned = FALSE, restsigned = FALSE;
    ULL result, rest;
    ULL x, y;
    LLO x_signed, y_signed; 

    if (sign)
    { 
      if (size64) x_signed = ((LLO) (LON) cpuGetDReg(dq_reg)) | (((LLO) cpuGetDReg(dr_reg))<<32);
      else x_signed = (LLO) (LON) cpuGetDReg(dq_reg);
      y_signed = (LLO) (LON) divisor;

      if (y_signed < 0)
      {
  y = (ULL) -y_signed;
  resultsigned = !resultsigned;
      }
      else y = y_signed;
      if (x_signed < 0)
      {
  x = (ULL) -x_signed;
  resultsigned = !resultsigned;
  restsigned = TRUE;
      }
      else x = (ULL) x_signed;
    }
    else
    {
      if (size64) x = ((ULL) cpuGetDReg(dq_reg)) | (((ULL) cpuGetDReg(dr_reg))<<32);
      else x = (ULL) cpuGetDReg(dq_reg);
      y = (ULL) divisor;
    }

    result = x / y;
    rest = x % y;

    if (sign)
    {
      if ((resultsigned && result > 0x80000000) || (!resultsigned && result > 0x7fffffff))
      {
  /* Overflow */
  cpuSetFlagsVC(TRUE, FALSE);
      }
      else
      {
  LLO result_signed = (resultsigned) ? (-(LLO)result) : ((LLO)result);
  LLO rest_signed = (restsigned) ? (-(LLO)rest) : ((LLO)rest);
  cpuSetDReg(dr_reg, (ULO) rest_signed);
  cpuSetDReg(dq_reg, (ULO) result_signed);
  cpuSetFlagsNZ00NewL((ULO) result_signed);
      }
    }
    else
    {
      if (result > 0xffffffff)
      {
  /* Overflow */
  cpuSetFlagsVC(TRUE, FALSE);
      }
      else
      {
  cpuSetDReg(dr_reg, (ULO) rest);
  cpuSetDReg(dq_reg, (ULO) result);
  cpuSetFlagsNZ00NewL((ULO) result);
      }
    }
    cpuSetInstructionTime(instruction_time);
  }
  else
  {
    cpuThrowDivisionByZeroException();
  }
}

/// <summary>
/// Lslb
/// </summary>
static UBY cpuLslB(UBY dst, ULO sh, ULO cycles)
{
  UBY res;
  sh &= 0x3f;
  if (sh == 0)
  {
    cpuSetFlagsShiftZero(cpuIsZeroB(dst), cpuMsbB(dst));
    res = dst;
  }
  else if (sh >= 8)
  {
    res = 0;
    cpuSetFlagsShift(TRUE, FALSE, (sh == 8) ? (dst & 1) : FALSE, FALSE);
  }
  else
  {
    res = dst << sh;
    cpuSetFlagsShift(cpuIsZeroB(res), cpuMsbB(res), dst & (0x80>>(sh-1)), FALSE);
  }
  cpuSetInstructionTime(cycles + sh*2);
  return res;
}

/// <summary>
/// Lslw
/// </summary>
static UWO cpuLslW(UWO dst, ULO sh, ULO cycles)
{
  UWO res;
  sh &= 0x3f;
  if (sh == 0)
  {
    cpuSetFlagsShiftZero(cpuIsZeroW(dst), cpuMsbW(dst));
    res = dst;
  }
  else if (sh >= 16)
  {
    res = 0;
    cpuSetFlagsShift(TRUE, FALSE, (sh == 16) ? (dst & 1) : FALSE, FALSE);
  }
  else
  {
    res = dst << sh;
    cpuSetFlagsShift(cpuIsZeroW(res), cpuMsbW(res), dst & (0x8000>>(sh-1)), FALSE);
  }
  cpuSetInstructionTime(cycles + sh*2);
  return res;
}

/// <summary>
/// Lsll
/// </summary>
static ULO cpuLslL(ULO dst, ULO sh, ULO cycles)
{
  ULO res;
  sh &= 0x3f;
  if (sh == 0)
  {
    cpuSetFlagsShiftZero(cpuIsZeroL(dst), cpuMsbL(dst));
    res = dst;
  }
  else if (sh >= 32)
  {
    res = 0;
    cpuSetFlagsShift(TRUE, FALSE, (sh == 32) ? (dst & 1) : FALSE, FALSE);
  }
  else
  {
    res = dst << sh;
    cpuSetFlagsShift(cpuIsZeroL(res), cpuMsbL(res), dst & (0x80000000>>(sh-1)), FALSE);
  }
  cpuSetInstructionTime(cycles + sh*2);
  return res;
}

/// <summary>
/// Lsrb
/// </summary>
static UBY cpuLsrB(UBY dst, ULO sh, ULO cycles)
{
  UBY res;
  sh &= 0x3f;
  if (sh == 0)
  {
    cpuSetFlagsShiftZero(cpuIsZeroB(dst), cpuMsbB(dst));
    res = dst;
  }
  else if (sh >= 8)
  {
    res = 0;
    cpuSetFlagsShift(TRUE, FALSE, (sh == 8) ? cpuMsbB(dst) : FALSE, FALSE);
  }
  else
  {
    res = dst >> sh;
    cpuSetFlagsShift(cpuIsZeroB(res), FALSE, dst & (1<<(sh-1)), FALSE);
  }
  cpuSetInstructionTime(cycles + sh*2);
  return res;
}

/// <summary>
/// Lsrw
/// </summary>
static UWO cpuLsrW(UWO dst, ULO sh, ULO cycles)
{
  UWO res;
  sh &= 0x3f;
  if (sh == 0)
  {
    cpuSetFlagsShiftZero(cpuIsZeroW(dst), cpuMsbW(dst));
    res = dst;
  }
  else if (sh >= 16)
  {
    res = 0;
    cpuSetFlagsShift(TRUE, FALSE, (sh == 16) ? cpuMsbW(dst) : FALSE, FALSE);
  }
  else
  {
    res = dst >> sh;
    cpuSetFlagsShift(cpuIsZeroW(res), FALSE, dst & (1<<(sh-1)), FALSE);
  }
  cpuSetInstructionTime(cycles + sh*2);
  return res;
}

/// <summary>
/// Lsrl
/// </summary>
static ULO cpuLsrL(ULO dst, ULO sh, ULO cycles)
{
  ULO res;
  sh &= 0x3f;
  if (sh == 0)
  {
    cpuSetFlagsShiftZero(cpuIsZeroL(dst), cpuMsbL(dst));
    res = dst;
  }
  else if (sh >= 32)
  {
    res = 0;
    cpuSetFlagsShift(TRUE, FALSE, (sh == 32) ? cpuMsbL(dst) : FALSE, FALSE);
  }
  else
  {
    res = dst >> sh;
    cpuSetFlagsShift(cpuIsZeroL(res), FALSE, dst & (1<<(sh-1)), FALSE);
  }
  cpuSetInstructionTime(cycles + sh*2);
  return res;
}

/// <summary>
/// Aslb
/// </summary>
static UBY cpuAslB(UBY dst, ULO sh, ULO cycles)
{
  BYT res;
  sh &= 0x3f;
  if (sh == 0)
  {
    cpuSetFlagsShiftZero(cpuIsZeroB(dst), cpuMsbB(dst));
    res = (BYT) dst;
  }
  else if (sh >= 8)
  {
    res = 0;
    cpuSetFlagsShift(TRUE, FALSE, (sh == 8) ? (dst & 1) : FALSE, dst != 0);
  }
  else
  {
    UBY mask = 0xff << (7-sh);
    UBY out = dst & mask;
    res = ((BYT)dst) << sh;

    // Overflow calculation: 
    // 1. The msb of the result and original are different
    // 2. Or the bits shifted out were not all the same as the msb of the original
    BOOLE n_result = cpuMsbB(res);
    BOOLE n_original = cpuMsbB(dst);
    BOOLE msb_changed = (n_result != n_original) || ((n_original) ? (out != mask) : (out != 0));

    cpuSetFlagsShift(cpuIsZeroB(res), n_result, dst & (0x80>>(sh-1)), msb_changed);
  }
  cpuSetInstructionTime(cycles + sh*2);
  return (UBY) res;
}

/// <summary>
/// Aslw
/// </summary>
static UWO cpuAslW(UWO dst, ULO sh, ULO cycles)
{
  WOR res;
  sh &= 0x3f;
  if (sh == 0)
  {
    cpuSetFlagsShiftZero(cpuIsZeroW(dst), cpuMsbW(dst));
    res = (WOR) dst;
  }
  else if (sh >= 16)
  {
    res = 0;
    cpuSetFlagsShift(TRUE, FALSE, (sh == 16) ? (dst & 1) : FALSE, dst != 0);
  }
  else
  {
    UWO mask = 0xffff << (15-sh);
    UWO out = dst & mask;
    res = ((WOR)dst) << sh;

    // Overflow calculation: 
    // 1. The msb of the result and original are different
    // 2. Or the bits shifted out were not all the same as the msb of the original
    BOOLE n_result = cpuMsbW(res);
    BOOLE n_original = cpuMsbW(dst);
    BOOLE msb_changed = (n_result != n_original) || ((n_original) ? (out != mask) : (out != 0));

    cpuSetFlagsShift(cpuIsZeroW(res), n_result, dst & (0x8000>>(sh-1)), msb_changed);
  }
  cpuSetInstructionTime(cycles + sh*2);
  return (UWO) res;
}

/// <summary>
/// Asll
/// </summary>
static ULO cpuAslL(ULO dst, ULO sh, ULO cycles)
{
  LON res;
  sh &= 0x3f;
  if (sh == 0)
  {
    cpuSetFlagsShiftZero(cpuIsZeroL(dst), cpuMsbL(dst));
    res = (LON) dst;
  }
  else if (sh >= 32)
  {
    res = 0;
    cpuSetFlagsShift(TRUE, FALSE, (sh == 32) ? (dst & 1) : FALSE, dst != 0);
  }
  else
  {
    ULO mask = 0xffffffff << (31-sh);
    ULO out = dst & mask;
    res = ((LON)dst) << sh;

    // Overflow calculation: 
    // 1. The msb of the result and original are different
    // 2. Or the bits shifted out were not all the same as the msb of the original
    BOOLE n_result = cpuMsbL(res);
    BOOLE n_original = cpuMsbL(dst);
    BOOLE msb_changed = (n_result != n_original) || ((n_original) ? (out != mask) : (out != 0));

    cpuSetFlagsShift(cpuIsZeroL(res), n_result, dst & (0x80000000>>(sh-1)), msb_changed);
  }
  cpuSetInstructionTime(cycles + sh*2);
  return (ULO) res;
}

/// <summary>
/// Asrb
/// </summary>
static UBY cpuAsrB(UBY dst, ULO sh, ULO cycles)
{
  BYT res;
  sh &= 0x3f;
  if (sh == 0)
  {
    cpuSetFlagsShiftZero(cpuIsZeroB(dst), cpuMsbB(dst));
    res = (BYT) dst;
  }
  else if (sh >= 8)
  {
    res = (cpuMsbB(dst)) ? 0xff : 0;
    cpuSetFlagsShift(cpuIsZeroB(res), cpuMsbB(res), cpuMsbB(res), FALSE);
  }
  else
  {
    res = ((BYT)dst) >> sh;
    cpuSetFlagsShift(cpuIsZeroB(res), cpuMsbB(res), dst & (1<<(sh-1)), FALSE);
  }
  cpuSetInstructionTime(cycles + sh*2);
  return (UBY) res;
}

/// <summary>
/// Asrw
/// </summary>
static UWO cpuAsrW(UWO dst, ULO sh, ULO cycles)
{
  WOR res;
  sh &= 0x3f;
  if (sh == 0)
  {
    cpuSetFlagsShiftZero(cpuIsZeroW(dst), cpuMsbW(dst));
    res = (WOR) dst;
  }
  else if (sh >= 16)
  {
    res = (cpuMsbW(dst)) ? 0xffff : 0;
    cpuSetFlagsShift(cpuIsZeroW(res), cpuMsbW(res), cpuMsbW(res), FALSE);
  }
  else
  {
    res = ((WOR)dst) >> sh;
    cpuSetFlagsShift(cpuIsZeroW(res), cpuMsbW(res), dst & (1<<(sh-1)), FALSE);
  }
  cpuSetInstructionTime(cycles + sh*2);
  return (UWO) res;
}

/// <summary>
/// Asrl
/// </summary>
static ULO cpuAsrL(ULO dst, ULO sh, ULO cycles)
{
  LON res;
  sh &= 0x3f;

  if (sh == 0)
  {
    cpuSetFlagsShiftZero(cpuIsZeroL(dst), cpuMsbL(dst));
    res = (LON) dst;
  }
  else if (sh >= 32)
  {
    res = (cpuMsbL(dst)) ? 0xffffffff : 0;
    cpuSetFlagsShift(cpuIsZeroL(res), cpuMsbL(res), cpuMsbL(res), FALSE);
  }
  else
  {
    res = ((LON)dst) >> sh;
    cpuSetFlagsShift(cpuIsZeroL(res), cpuMsbL(res), dst & (1<<(sh-1)), FALSE);
  }
  cpuSetInstructionTime(cycles + sh*2);
  return (ULO) res;
}

/// <summary>
/// Rolb
/// </summary>
static UBY cpuRolB(UBY dst, ULO sh, ULO cycles)
{
  UBY res;
  sh &= 0x3f;
  cycles += sh*2;
  if (sh == 0)
  {
    cpuSetFlagsShiftZero(cpuIsZeroB(dst), cpuMsbB(dst));
    res = dst;
  }
  else
  {
    sh &= 7;
    res = (dst << sh) | (dst >> (8-sh));
    cpuSetFlagsRotate(cpuIsZeroB(res), cpuMsbB(res), res & 1);
  }
  cpuSetInstructionTime(cycles);
  return res;
}

/// <summary>
/// Rolw
/// </summary>
static UWO cpuRolW(UWO dst, ULO sh, ULO cycles)
{
  UWO res;
  sh &= 0x3f;
  cycles += sh*2;
  if (sh == 0)
  {
    cpuSetFlagsShiftZero(cpuIsZeroW(dst), cpuMsbW(dst));
    res = dst;
  }
  else
  {
    sh &= 15;
    res = (dst << sh) | (dst >> (16-sh));
    cpuSetFlagsRotate(cpuIsZeroW(res), cpuMsbW(res), res & 1);
  }
  cpuSetInstructionTime(cycles);
  return res;
}

/// <summary>
/// Roll
/// </summary>
static ULO cpuRolL(ULO dst, ULO sh, ULO cycles)
{
  ULO res;
  sh &= 0x3f;
  cycles += sh*2;
  if (sh == 0)
  {
    cpuSetFlagsShiftZero(cpuIsZeroL(dst), cpuMsbL(dst));
    res = dst;
  }
  else
  {
    sh &= 31;
    res = (dst << sh) | (dst >> (32-sh));
    cpuSetFlagsRotate(cpuIsZeroL(res), cpuMsbL(res), res & 1);
  }
  cpuSetInstructionTime(cycles);
  return res;
}

/// <summary>
/// Rorb
/// </summary>
static UBY cpuRorB(UBY dst, ULO sh, ULO cycles)
{
  UBY res;
  sh &= 0x3f;
  cycles += sh*2;
  if (sh == 0)
  {
    cpuSetFlagsShiftZero(cpuIsZeroB(dst), cpuMsbB(dst));
    res = dst;
  }
  else
  {
    sh &= 7;
    res = (dst >> sh) | (dst << (8-sh));
    cpuSetFlagsRotate(cpuIsZeroB(res), cpuMsbB(res), cpuMsbB(res));
  }
  cpuSetInstructionTime(cycles);
  return res;
}

/// <summary>
/// Rorw
/// </summary>
static UWO cpuRorW(UWO dst, ULO sh, ULO cycles)
{
  UWO res;
  sh &= 0x3f;
  cycles += sh*2;
  if (sh == 0)
  {
    cpuSetFlagsShiftZero(cpuIsZeroW(dst), cpuMsbW(dst));
    res = dst;
  }
  else
  {
    sh &= 15;
    res = (dst >> sh) | (dst << (16-sh));
    cpuSetFlagsRotate(cpuIsZeroW(res), cpuMsbW(res), cpuMsbW(res));
  }
  cpuSetInstructionTime(cycles);
  return res;
}

/// <summary>
/// Rorl
/// </summary>
static ULO cpuRorL(ULO dst, ULO sh, ULO cycles)
{
  ULO res;
  sh &= 0x3f;
  cycles += sh*2;
  if (sh == 0)
  {
    cpuSetFlagsShiftZero(cpuIsZeroL(dst), cpuMsbL(dst));
    res = dst;
  }
  else
  {
    sh &= 31;
    res = (dst >> sh) | (dst << (32-sh));
    cpuSetFlagsRotate(cpuIsZeroL(res), cpuMsbL(res), cpuMsbL(res));
  }
  cpuSetInstructionTime(cycles);
  return res;
}

/// <summary>
/// Roxlb
/// </summary>
static UBY cpuRoxlB(UBY dst, ULO sh, ULO cycles)
{
  BOOLE x = cpuGetFlagX();
  BOOLE x_temp;
  UBY res = dst;
  ULO i;
  sh &= 0x3f;
  for (i = 0; i < sh; ++i)
  {
    x_temp = cpuMsbB(res);
    res = (res << 1) | ((x) ? 1:0);
    x = x_temp;
  }
  cpuSetFlagsRotateX(cpuGetZFlagB(res), cpuGetNFlagB(res), (x) ? 0x11 : 0);
  cpuSetInstructionTime(cycles + sh*2);
  return res;
}

/// <summary>
/// Roxlw
/// </summary>
static UWO cpuRoxlW(UWO dst, ULO sh, ULO cycles)
{
  BOOLE x = cpuGetFlagX();
  BOOLE x_temp;
  UWO res = dst;
  ULO i;
  sh &= 0x3f;
  for (i = 0; i < sh; ++i)
  {
    x_temp = cpuMsbW(res);
    res = (res << 1) | ((x) ? 1:0);
    x = x_temp;
  }
  cpuSetFlagsRotateX(cpuGetZFlagW(res), cpuGetNFlagW(res), (x) ? 0x11 : 0);
  cpuSetInstructionTime(cycles + sh*2);
  return res;
}

/// <summary>
/// Roxll
/// </summary>
static ULO cpuRoxlL(ULO dst, ULO sh, ULO cycles)
{
  BOOLE x = cpuGetFlagX();
  BOOLE x_temp;
  ULO res = dst;
  ULO i;
  sh &= 0x3f;
  for (i = 0; i < sh; ++i)
  {
    x_temp = cpuMsbL(res);
    res = (res << 1) | ((x) ? 1:0);
    x = x_temp;
  }
  cpuSetFlagsRotateX(cpuGetZFlagL(res), cpuGetNFlagL(res), (x) ? 0x11 : 0);
  cpuSetInstructionTime(cycles + sh*2);
  return res;
}

/// <summary>
/// Roxrb
/// </summary>
static UBY cpuRoxrB(UBY dst, ULO sh, ULO cycles)
{
  BOOLE x = cpuGetFlagX();
  BOOLE x_temp;
  UBY res = dst;
  ULO i;
  sh &= 0x3f;
  for (i = 0; i < sh; ++i)
  {
    x_temp = res & 1;
    res = (res >> 1) | ((x) ? 0x80:0);
    x = x_temp;
  }
  cpuSetFlagsRotateX(cpuGetZFlagB(res), cpuGetNFlagB(res), (x) ? 0x11 : 0);
  cpuSetInstructionTime(cycles + sh*2);
  return res;
}

/// <summary>
/// Roxrw
/// </summary>
static UWO cpuRoxrW(UWO dst, ULO sh, ULO cycles)
{
  BOOLE x = cpuGetFlagX();
  BOOLE x_temp;
  UWO res = dst;
  ULO i;
  sh &= 0x3f;
  for (i = 0; i < sh; ++i)
  {
    x_temp = res & 1;
    res = (res >> 1) | ((x) ? 0x8000:0);
    x = x_temp;
  }
  cpuSetFlagsRotateX(cpuGetZFlagW(res), cpuGetNFlagW(res), (x) ? 0x11 : 0);
  cpuSetInstructionTime(cycles + sh*2);
  return res;
}

/// <summary>
/// Roxrl
/// </summary>
static ULO cpuRoxrL(ULO dst, ULO sh, ULO cycles)
{
  BOOLE x = cpuGetFlagX();
  BOOLE x_temp;
  ULO res = dst;
  ULO i;
  sh &= 0x3f;
  for (i = 0; i < sh; ++i)
  {
    x_temp = res & 1;
    res = (res >> 1) | ((x) ? 0x80000000:0);
    x = x_temp;
  }
  cpuSetFlagsRotateX(cpuGetZFlagL(res), cpuGetNFlagL(res), (x) ? 0x11 : 0);
  cpuSetInstructionTime(cycles + sh*2);
  return res;
}

/// <summary>
/// Stop
/// </summary>
static void cpuStop(UWO flags)
{
  if (cpuGetFlagSupervisor())
  {
    cpuSetStop(TRUE);
    cpuUpdateSr(flags);
    cpuSetInstructionTime(4);
  }
  else
  {
    cpuThrowPrivilegeViolationException();
  }
}

/// <summary>
/// Reset
/// </summary>
static void cpuReset()
{
  cpuCallResetExceptionFunc();
  cpuSetInstructionTime(132);
}

/// <summary>
/// Rtd
/// </summary>
static void cpuRtd()
{
  ULO displacement = cpuGetNextWordSignExt();
  cpuInitializeFromNewPC(memoryReadLong(cpuGetAReg(7))); 
  cpuSetAReg(7, cpuGetAReg(7) + 4 + displacement);
  cpuSetInstructionTime(4);
}

static ULO cpuRteStackInc[16] = {0, 0, 4, 4, 8, 0, 0, 52, 50, 10, 24, 84, 16, 18, 0, 0};

/// <summary>
/// Rte
/// </summary>
static void cpuRte()
{
  if (cpuGetFlagSupervisor())
  {
    BOOLE redo = TRUE;
    UWO newsr;
    do
    {
      newsr = memoryReadWord(cpuGetAReg(7));
      cpuSetAReg(7, cpuGetAReg(7) + 2);

      cpuInitializeFromNewPC(memoryReadLong(cpuGetAReg(7)));
      cpuSetAReg(7, cpuGetAReg(7) + 4);

      if (cpuGetModelMajor() > 0)
      {
  ULO frame_type = (memoryReadWord(cpuGetAReg(7)) >> 12) & 0xf;
  cpuSetAReg(7, cpuGetAReg(7) + 2);
  cpuSetAReg(7, cpuGetAReg(7) + cpuRteStackInc[frame_type]);
  redo = (frame_type == 1 && cpuGetModelMajor() >= 2 && cpuGetModelMajor() < 6);
      }
      else redo = FALSE;

      cpuUpdateSr(newsr); // Because we can go from isp to msp here.

    } while (redo);
  }
  else
  {
    cpuThrowPrivilegeViolationException();
  }
  cpuSetInstructionTime(20);
}

/// <summary>
/// Swap
/// </summary>
static void cpuSwap(ULO reg)
{
  ULO res = cpuJoinWordToLong((UWO)cpuGetDReg(reg), (UWO) (cpuGetDReg(reg) >> 16));
  cpuSetDReg(reg, res);
  cpuSetFlagsNZ00NewL(res);
  cpuSetInstructionTime(4);
}

/// <summary>
/// Unlk
/// </summary>
static void cpuUnlk(ULO reg)
{
  cpuSetAReg(7, cpuGetAReg(reg));
  cpuSetAReg(reg, memoryReadLong(cpuGetAReg(7)));
  cpuSetAReg(7, cpuGetAReg(7) + 4);
  cpuSetInstructionTime(12);
}

/// <summary>
/// Link
/// </summary>
static void cpuLinkW(ULO reg)
{
  ULO disp = cpuGetNextWordSignExt();
  cpuSetAReg(7, cpuGetAReg(7) - 4);
  memoryWriteLong(cpuGetAReg(reg), cpuGetAReg(7));
  cpuSetAReg(reg, cpuGetAReg(7));
  cpuSetAReg(7, cpuGetAReg(7) + disp);
  cpuSetInstructionTime(16);
}

/// <summary>
/// Link.
/// 68020, 68030 and 68040 only.
/// </summary>
static void cpuLinkL(ULO reg)
{
  ULO disp = cpuGetNextLong();
  cpuSetAReg(7, cpuGetAReg(7) - 4);
  memoryWriteLong(cpuGetAReg(reg), cpuGetAReg(7));
  cpuSetAReg(reg, cpuGetAReg(7));
  cpuSetAReg(7, cpuGetAReg(7) + disp);
  cpuSetInstructionTime(4);
}

/// <summary>
/// Ext.w (byte to word)
/// </summary>
static void cpuExtW(ULO reg)
{
  UWO res = cpuGetDRegByteSignExtWord(reg);
  cpuSetDRegWord(reg, res);
  cpuSetFlagsNZ00NewW(res);
  cpuSetInstructionTime(4);
}

/// <summary>
/// Ext.l (word to long)
/// </summary>
static void cpuExtL(ULO reg)
{
  ULO res = cpuGetDRegWordSignExtLong(reg);
  cpuSetDReg(reg, res);
  cpuSetFlagsNZ00NewL(res);
  cpuSetInstructionTime(4);
}

/// <summary>
/// ExtB.l (byte to long) (020+)
/// </summary>
static void cpuExtBL(ULO reg)
{
  ULO res = cpuGetDRegByteSignExtLong(reg);
  cpuSetDReg(reg, res);
  cpuSetFlagsNZ00NewL(res);
  cpuSetInstructionTime(4);
}

/// <summary>
/// Exg Rx,Ry
/// </summary>
static void cpuExgAll(ULO reg1_type, ULO reg1, ULO reg2_type, ULO reg2)
{
  ULO tmp = cpuGetReg(reg1_type, reg1);
  cpuSetReg(reg1_type, reg1, cpuGetReg(reg2_type, reg2));
  cpuSetReg(reg2_type, reg2, tmp);
  cpuSetInstructionTime(6);
}

/// <summary>
/// Exg Dx,Dy
/// </summary>
static void cpuExgDD(ULO reg1, ULO reg2)
{
  cpuExgAll(0, reg1, 0, reg2);
}

/// <summary>
/// Exg Ax,Ay
/// </summary>
static void cpuExgAA(ULO reg1, ULO reg2)
{
  cpuExgAll(1, reg1, 1, reg2);
}

/// <summary>
/// Exg Dx,Ay
/// </summary>
static void cpuExgDA(ULO reg1, ULO reg2)
{
  cpuExgAll(0, reg1, 1, reg2);
}

/// <summary>
/// Movem.w regs, -(Ax)
/// Order: d0-d7,a0-a7   a7 first
/// </summary>
static void cpuMovemwPre(UWO regs, ULO reg)
{
  ULO cycles = 8;
  ULO dstea = cpuGetAReg(reg);
  ULO index = 1;
  LON i, j;
  BOOLE ea_reg_seen = FALSE;
  ULO ea_reg_ea = 0;

  i = 1;
  for (j = 7; j >= 0; j--)
  {
    if (regs & index)
    {
      dstea -= 2;
      if (cpuGetModelMajor() >= 2 && j == reg)
      {
  ea_reg_seen = TRUE;
  ea_reg_ea = dstea;
      }
      else
      {
  memoryWriteWord(cpuGetRegWord(i, j), dstea);
      }
      cycles += 4;
    }
    index = index << 1;
  }

  i = 0;
  for (j = 7; j >= 0; j--)
  {
    if (regs & index)
    {
      dstea -= 2;
      memoryWriteWord(cpuGetRegWord(i, j), dstea);
      cycles += 4;
    }
    index = index << 1;
  }
  if (ea_reg_seen)
  {
    memoryWriteWord((UWO)dstea, ea_reg_ea);
  }
  cpuSetAReg(reg, dstea);
  cpuSetInstructionTime(cycles);
}

/// <summary>
/// Movem.l regs, -(Ax)
/// Order: d0-d7,a0-a7   a7 first
/// </summary>
static void cpuMovemlPre(UWO regs, ULO reg)
{
  ULO cycles = 8;
  ULO dstea = cpuGetAReg(reg);
  ULO index = 1;
  LON i, j;
  BOOLE ea_reg_seen = FALSE;
  ULO ea_reg_ea = 0;

  i = 1;
  for (j = 7; j >= 0; j--)
  {
    if (regs & index)
    {
      dstea -= 4;
      if (cpuGetModelMajor() >= 2 && j == reg)
      {
  ea_reg_seen = TRUE;
  ea_reg_ea = dstea;
      }
      else
      {
  memoryWriteLong(cpuGetReg(i, j), dstea);
      }
      cycles += 8;
    }
    index = index << 1;
  }

  i = 0;
  for (j = 7; j >= 0; j--)
  {
    if (regs & index)
    {
      dstea -= 4;
      memoryWriteLong(cpuGetReg(i, j), dstea);
      cycles += 8;
    }
    index = index << 1;
  }

  if (ea_reg_seen)
  {
    memoryWriteLong(dstea, ea_reg_ea);
  }
  cpuSetAReg(reg, dstea);
  cpuSetInstructionTime(cycles);
}

/// <summary>
/// Movem.w (Ax)+, regs
/// Order: a7-a0,d7-d0   d0 first
/// </summary>
static void cpuMovemwPost(UWO regs, ULO reg)
{
  ULO cycles = 12;
  ULO dstea = cpuGetAReg(reg);
  ULO index = 1;
  ULO i, j;

  for (i = 0; i < 2; ++i)
  {
    for (j = 0; j < 8; ++j)
    {
      if (regs & index)
      {
  // Each word, for both data and address registers, is sign-extended before stored.
  cpuSetReg(i, j, (ULO)(LON)(WOR) memoryReadWord(dstea));
  dstea += 2;
  cycles += 4;
      }
      index = index << 1;
    }
  }
  cpuSetAReg(reg, dstea);
  cpuSetInstructionTime(cycles);
}

/// <summary>
/// Movem.l (Ax)+, regs
/// Order: a7-a0,d7-d0   d0 first
/// </summary>
static void cpuMovemlPost(UWO regs, ULO reg)
{
  ULO cycles = 12;
  ULO dstea = cpuGetAReg(reg);
  ULO index = 1;
  ULO i, j;

  for (i = 0; i < 2; ++i)
  {
    for (j = 0; j < 8; ++j)
    {
      if (regs & index)
      {
  cpuSetReg(i, j, memoryReadLong(dstea));
  dstea += 4;
  cycles += 8;
      }
      index = index << 1;
    }
  }
  cpuSetAReg(reg, dstea);
  cpuSetInstructionTime(cycles);
}

/// <summary>
/// Movem.w <Control>, regs
/// Order: a7-a0,d7-d0   d0 first
/// </summary>
static void cpuMovemwEa2R(UWO regs, ULO ea, ULO eacycles)
{
  ULO cycles = eacycles;
  ULO dstea = ea;
  ULO index = 1;
  ULO i, j;

  for (i = 0; i < 2; ++i)
  {
    for (j = 0; j < 8; ++j)
    {
      if (regs & index)
      {
  // Each word, for both data and address registers, is sign-extended before stored.
  cpuSetReg(i, j, (ULO)(LON)(WOR) memoryReadWord(dstea));
  dstea += 2;
  cycles += 4;
      }
      index = index << 1;
    }
  }
  cpuSetInstructionTime(cycles);
}

/// <summary>
/// Movem.l <Control>, regs
/// Order: a7-a0,d7-d0   d0 first
/// </summary>
static void cpuMovemlEa2R(UWO regs, ULO ea, ULO eacycles)
{
  ULO cycles = eacycles;
  ULO dstea = ea;
  ULO index = 1;
  ULO i, j;

  for (i = 0; i < 2; ++i)
  {
    for (j = 0; j < 8; ++j)
    {
      if (regs & index)
      {
  cpuSetReg(i, j, memoryReadLong(dstea));
  dstea += 4;
  cycles += 8;
      }
      index = index << 1;
    }
  }
  cpuSetInstructionTime(cycles);
}

/// <summary>
/// Movem.w regs, <Control>
/// Order: a7-a0,d7-d0   d0 first
/// </summary>
static void cpuMovemwR2Ea(UWO regs, ULO ea, ULO eacycles)
{
  ULO cycles = eacycles;
  ULO dstea = ea;
  ULO index = 1;
  ULO i, j;

  for (i = 0; i < 2; ++i)
  {
    for (j = 0; j < 8; ++j)
    {
      if (regs & index)
      {
  memoryWriteWord(cpuGetRegWord(i, j), dstea);
  dstea += 2;
  cycles += 4;
      }
      index = index << 1;
    }
  }
  cpuSetInstructionTime(cycles);
}

/// <summary>
/// Movem.l regs, <Control>
/// Order: a7-a0,d7-d0   d0 first
/// </summary>
static void cpuMovemlR2Ea(UWO regs, ULO ea, ULO eacycles)
{
  ULO cycles = eacycles;
  ULO dstea = ea;
  ULO index = 1;
  ULO i, j;

  for (i = 0; i < 2; ++i)
  {
    for (j = 0; j < 8; ++j)
    {
      if (regs & index)
      {
  memoryWriteLong(cpuGetReg(i, j), dstea);
  dstea += 4;
  cycles += 8;
      }
      index = index << 1;
    }
  }
  cpuSetInstructionTime(cycles);
}

/// <summary>
/// Trap #vectorno
/// </summary>
static void cpuTrap(ULO vectorno)
{
  // PC written to the exception frame must be pc + 2, the address of the next instruction.
  cpuThrowTrapException(vectorno);
}

/// <summary>
/// move.l  Ax,Usp
/// </summary>
static void cpuMoveToUsp(ULO reg)
{
  if (cpuGetFlagSupervisor())
  {
    // In supervisor mode, usp does not affect a7
    cpuSetUspDirect(cpuGetAReg(reg));
    cpuSetInstructionTime(4);
  }
  else
  {
    cpuThrowPrivilegeViolationException();
  }
}

/// <summary>
/// move.l  Usp,Ax
/// </summary>
static void cpuMoveFromUsp(ULO reg)
{
  if (cpuGetFlagSupervisor())
  {
    // In supervisor mode, usp is up to date
    cpuSetAReg(reg, cpuGetUspDirect());
    cpuSetInstructionTime(4);
  }
  else
  {
    cpuThrowPrivilegeViolationException();
  }
}

/// <summary>
/// cmp.b (Ay)+,(Ax)+
/// </summary>
static void cpuCmpMB(ULO regx, ULO regy)
{
  UBY src = memoryReadByte(cpuEA03(regy, 1));
  UBY dst = memoryReadByte(cpuEA03(regx, 1));
  UBY res = dst - src;
  cpuSetFlagsCmp(cpuIsZeroB(res), cpuMsbB(res), cpuMsbB(dst), cpuMsbB(src));
  cpuSetInstructionTime(12);
}

/// <summary>
/// cmp.w (Ay)+,(Ax)+
/// </summary>
static void cpuCmpMW(ULO regx, ULO regy)
{
  UWO src = memoryReadWord(cpuEA03(regy, 2));
  UWO dst = memoryReadWord(cpuEA03(regx, 2));
  UWO res = dst - src;
  cpuSetFlagsCmp(cpuIsZeroW(res), cpuMsbW(res), cpuMsbW(dst), cpuMsbW(src));
  cpuSetInstructionTime(12);
}

/// <summary>
/// cmp.l (Ay)+,(Ax)+
/// </summary>
static void cpuCmpML(ULO regx, ULO regy)
{
  ULO src = memoryReadLong(cpuEA03(regy, 4));
  ULO dst = memoryReadLong(cpuEA03(regx, 4));
  ULO res = dst - src;
  cpuSetFlagsCmp(cpuIsZeroL(res), cpuMsbL(res), cpuMsbL(dst), cpuMsbL(src));
  cpuSetInstructionTime(20);
}

/// <summary>
/// chk.w Dx, ea
/// Undocumented features:
/// Z is set from the register operand,
/// V and C is always cleared.
/// </summary>
static void cpuChkW(UWO value, UWO ub)
{
  cpuSetFlagZ(value == 0);
  cpuClearFlagsVC();
  if (((WOR)value) < 0)
  {
    cpuSetFlagN(TRUE);
    cpuThrowChkException();
  }
  else if (((WOR)value) > ((WOR)ub))
  {
    cpuSetFlagN(FALSE);
    cpuThrowChkException();
  }
}

/// <summary>
/// chk.l Dx, ea
/// 68020+
/// Undocumented features:
/// Z is set from the register operand,
/// V and C is always cleared.
/// </summary>
static void cpuChkL(ULO value, ULO ub)
{
  cpuSetFlagZ(value == 0);
  cpuClearFlagsVC();
  if (((LON)value) < 0)
  {
    cpuSetFlagN(TRUE);
    cpuThrowChkException();
  }
  else if (((LON)value) > ((LON)ub))
  {
    cpuSetFlagN(FALSE);
    cpuThrowChkException();
  }
}

/// <summary>
/// addx.b dx,dy
/// </summary>
static UBY cpuAddXB(UBY dst, UBY src)
{
  UBY res = dst + src + ((cpuGetFlagX()) ? 1:0);
  cpuSetFlagsAddX(cpuIsZeroB(res), cpuMsbB(res), cpuMsbB(dst), cpuMsbB(src));
  return res;
}

/// <summary>
/// addx.w dx,dy
/// </summary>
static UWO cpuAddXW(UWO dst, UWO src)
{
  UWO res = dst + src + ((cpuGetFlagX()) ? 1:0);
  cpuSetFlagsAddX(cpuIsZeroW(res), cpuMsbW(res), cpuMsbW(dst), cpuMsbW(src));
  return res;
}

/// <summary>
/// addx.l dx,dy
/// </summary>
static ULO cpuAddXL(ULO dst, ULO src)
{
  ULO res = dst + src + ((cpuGetFlagX()) ? 1:0);
  cpuSetFlagsAddX(cpuIsZeroL(res), cpuMsbL(res), cpuMsbL(dst), cpuMsbL(src));
  return res;
}

/// <summary>
/// subx.b dx,dy
/// </summary>
static UBY cpuSubXB(UBY dst, UBY src)
{
  UBY res = dst - src - ((cpuGetFlagX()) ? 1:0);
  cpuSetFlagsSubX(cpuIsZeroB(res), cpuMsbB(res), cpuMsbB(dst), cpuMsbB(src));
  return res;
}

/// <summary>
/// subx.w dx,dy
/// </summary>
static UWO cpuSubXW(UWO dst, UWO src)
{
  UWO res = dst - src - ((cpuGetFlagX()) ? 1:0);
  cpuSetFlagsSubX(cpuIsZeroW(res), cpuMsbW(res), cpuMsbW(dst), cpuMsbW(src));
  return res;
}

/// <summary>
/// subx.l dx,dy
/// </summary>
static ULO cpuSubXL(ULO dst, ULO src)
{
  ULO res = dst - src - ((cpuGetFlagX()) ? 1:0);
  cpuSetFlagsSubX(cpuIsZeroL(res), cpuMsbL(res), cpuMsbL(dst), cpuMsbL(src));
  return res;
}

/// <summary>
/// abcd.b src,dst
/// Implemented using the information from:
///     68000 Undocumented Behavior Notes
///                Fourth Edition
///    by Bart Trzynadlowski, May 12, 2003
/// </summary>
static UBY cpuAbcdB(UBY dst, UBY src)
{
  UBY xflag = (cpuGetFlagX()) ? 1 : 0;
  UWO low_nibble = (dst & 0xf) + (src & 0xf) + xflag;
  UWO high_nibble = ((UWO)(dst & 0xf0)) + ((UWO)(src & 0xf0));
  UWO result_unadjusted = low_nibble + high_nibble;
  UWO result_bcd = result_unadjusted;

  if (low_nibble > 9)
  {
    result_bcd += 6;
  }

  BOOLE xc_flags = (result_bcd & 0xfff0) > 0x90;
  cpuSetFlagXC(xc_flags);
  if (xc_flags)
  {
    result_bcd += 0x60;
  }

  if (result_bcd & 0xff)
  {
    cpuSetFlagZ(FALSE);
  }

  if (cpuGetModelMajor() < 4)  // 040 apparently does not set these flags
  {
    cpuSetFlagN(result_bcd & 0x80);
    cpuSetFlagV(((result_unadjusted & 0x80) == 0) && (result_bcd & 0x80));
  }
  return (UBY)result_bcd;
}

/// <summary>
/// sbcd.b src,dst
/// nbcd.b src   (set dst=0)
/// Implemented using the information from:
///     68000 Undocumented Behavior Notes
///                Fourth Edition
///    by Bart Trzynadlowski, May 12, 2003
/// </summary>
static UBY cpuSbcdB(UBY dst, UBY src)
{
  UWO xflag = (cpuGetFlagX()) ? 1:0;
  UWO result_plain_binary = (UWO)dst - (UWO)src - xflag;
  UWO low_nibble = (UWO)(dst & 0xf) - (UWO)(src & 0xf) - xflag;
  UWO high_nibble = ((UWO)(dst & 0xf0)) - ((UWO)(src & 0xf0));
  UWO result_unadjusted = low_nibble + high_nibble;
  UWO result_bcd = result_unadjusted;

  if ((WOR)result_plain_binary < 0)
  {
    result_bcd -= 0x60;
  }

  if (((WOR)low_nibble) < 0)
  {
    result_bcd -= 6;
    result_plain_binary -= 6;
  }

  BOOLE borrow = ((WOR)result_plain_binary < 0);
  cpuSetFlagXC(borrow);

  if (result_bcd & 0xff)
  {
    cpuSetFlagZ(FALSE);
  }

  if (cpuGetModelMajor() < 4)
  {
    cpuSetFlagN(result_bcd & 0x80);
    cpuSetFlagV(((result_unadjusted & 0x80) == 0x80) && !(result_bcd & 0x80));
  }
  return (UBY) result_bcd;
}

/// <summary>
/// nbcd.b dst
/// </summary>
static UBY cpuNbcdB(UBY dst)
{
  return cpuSbcdB(0, dst);
}

// Bit field functions
struct cpuBfData
{
  LON offset;
  ULO width;
  ULO normalized_offset;

  ULO base_address;
  LON base_address_byte_offset;
  ULO base_address_byte_count;

  ULO field;
  ULL field_mask;
  ULO dn;
  ULL field_memory;
};

static LON cpuGetBfOffset(UWO ext, bool offsetIsDataRegister)
{
  LON offset = (ext >> 6) & 0x1f;
  if (offsetIsDataRegister)
  {
    offset = (LON) cpuGetDReg(offset & 7);
  }
  return offset;
}

static ULO cpuGetBfWidth(UWO ext, bool widthIsDataRegister)
{
  ULO width = (ext & 0x1f);
  if (widthIsDataRegister)
  {
    width = cpuGetDReg(width & 7) & 0x1f;
  }
  if (width == 0)
  {
    width = 32;
  }
  return width;
}

static void cpuSetBfField(struct cpuBfData *bf_data, ULO ea_or_reg, bool has_ea)
{
  if (has_ea)
  {
    ULO shift = bf_data->base_address_byte_count*8 - bf_data->normalized_offset - bf_data->width;
    ULL field_value = (bf_data->field_memory & ~(bf_data->field_mask << shift)) | (bf_data->field << shift);
    ULO address = bf_data->base_address + bf_data->base_address_byte_offset;

    for (int i = bf_data->base_address_byte_count - 1; i >= 0; --i)
    {
      UBY field_byte = (field_value >> (i*8)) & 0xff;
      memoryWriteByte(field_byte, address);
      ++address;
    }
  }
  else
  {
    ULO reg_shift = (32 - bf_data->offset - bf_data->width);
    ULO reg_value = (cpuGetDReg(ea_or_reg) & ~(bf_data->field_mask << reg_shift)) | (bf_data->field << reg_shift);
    cpuSetDReg(ea_or_reg, reg_value);
  }
}

void cpuBfDecodeExtWordAndGetField(struct cpuBfData *bf_data, ULO ea_or_reg, bool has_dn, bool has_ea, UWO ext)
{
  bool offsetIsDataRegister = ((ext & 0x0800) == 0x0800);
  bool widthIsDataRegister = ((ext & 0x0020) == 0x0020);

  bf_data->offset = cpuGetBfOffset(ext, offsetIsDataRegister);
  bf_data->width = cpuGetBfWidth(ext, widthIsDataRegister);
  bf_data->field_mask = 0xffffffff >> (32 - bf_data->width);

  if (has_dn)
  {
    bf_data->dn = (ext & 0x7000) >> 12;
  }

  if (has_ea)
  {
    bf_data->base_address = ea_or_reg;  // Base address of the field, before offset is applied
    bf_data->base_address_byte_offset = (bf_data->offset >> 3); // The first byte in the field
    bf_data->normalized_offset = bf_data->offset - (bf_data->base_address_byte_offset)*8; // Offset relative to the first byte in the field
    bf_data->base_address_byte_count = (bf_data->normalized_offset + bf_data->width + 7) / 8;

    ULO field = 0;
    ULL field_memory = 0;
    ULO address = bf_data->base_address + bf_data->base_address_byte_offset;
    ULO shift = (8 - bf_data->normalized_offset - bf_data->width) & 7;
    for (int i = bf_data->base_address_byte_count - 1; i >= 0; --i)
    {
      ULL value = ((ULL)memoryReadByte(address)) << (8 * i);
      field_memory |= value;
      field |= (value >> shift);
      ++address;
    }

    bf_data->field_memory = field_memory;
    bf_data->field = field;
  }
  else
  {
    bf_data->field = cpuGetDReg(ea_or_reg) >> (32 - bf_data->offset - bf_data->width);
  }
  bf_data->field &= bf_data->field_mask;
}

/// <summary>
/// bfchg common logic
/// </summary>
static void cpuBfChgCommon(ULO ea_or_reg, bool has_ea, UWO ext)
{
  struct cpuBfData bf_data;
  cpuBfDecodeExtWordAndGetField(&bf_data, ea_or_reg, false, has_ea, ext);
  cpuSetFlagsNZVC(bf_data.field == 0, bf_data.field & (1 << (bf_data.width - 1)), FALSE, FALSE);

  bf_data.field = (~bf_data.field) & bf_data.field_mask;

  cpuSetBfField(&bf_data, ea_or_reg, has_ea);
}

// bfchg dx {offset:width}
static void cpuBfChgReg(ULO regno, UWO ext)
{
  cpuBfChgCommon(regno, false, ext);
}

/// <summary>
/// bfchg ea {offset:width}
/// </summary>
static void cpuBfChgEa(ULO ea, UWO ext)
{
  cpuBfChgCommon(ea, true, ext);
}

/// <summary>
/// bfclr common logic
/// </summary>
static void cpuBfClrCommon(ULO ea_or_reg, bool has_ea, UWO ext)
{
  struct cpuBfData bf_data;
  cpuBfDecodeExtWordAndGetField(&bf_data, ea_or_reg, false, has_ea, ext);

  cpuSetFlagsNZVC(bf_data.field == 0, bf_data.field & (1 << (bf_data.width - 1)), FALSE, FALSE);

  bf_data.field = 0;

  cpuSetBfField(&bf_data, ea_or_reg, has_ea);
}

/// <summary>
/// bfclr dx {offset:width}
/// </summary>
static void cpuBfClrReg(ULO regno, UWO ext)
{
  cpuBfClrCommon(regno, false, ext);
}

/// <summary>
/// bfclr ea {offset:width}
/// </summary>
static void cpuBfClrEa(ULO ea, UWO ext)
{
  cpuBfClrCommon(ea, true, ext);
}

/// <summary>
/// bfexts common logic
/// </summary>
static void cpuBfExtsCommon(ULO ea_or_reg, bool has_ea, UWO ext)
{
  struct cpuBfData bf_data;
  BOOLE n_flag;
  cpuBfDecodeExtWordAndGetField(&bf_data, ea_or_reg, true, has_ea, ext);

  n_flag = bf_data.field & (1 << (bf_data.width - 1));

  cpuSetFlagsNZVC(bf_data.field == 0, n_flag, FALSE, FALSE);

  if (n_flag)
  {
    bf_data.field = (ULO)((~bf_data.field_mask) | bf_data.field);
  }
  // Destination is always Dn
  cpuSetDReg(bf_data.dn, bf_data.field);
}

/// <summary>
/// bfexts dx {offset:width}, Dn
/// </summary>
static void cpuBfExtsReg(ULO regno, UWO ext)
{
  cpuBfExtsCommon(regno, false, ext);
}

/// <summary>
/// bfexts ea {offset:width}, Dn
/// </summary>
static void cpuBfExtsEa(ULO ea, UWO ext)
{
  cpuBfExtsCommon(ea, true, ext);
}

/// <summary>
/// bfextu ea {offset:width}, Dn
/// </summary>
static void cpuBfExtuCommon(ULO ea_or_reg, bool has_ea, UWO ext)
{
  struct cpuBfData bf_data;
  cpuBfDecodeExtWordAndGetField(&bf_data, ea_or_reg, true, has_ea, ext);
  cpuSetFlagsNZVC(bf_data.field == 0, bf_data.field & (1 << (bf_data.width - 1)), FALSE, FALSE);
  // Destination is always Dn
  cpuSetDReg(bf_data.dn, bf_data.field);
}

/// <summary>
/// bfextu dx {offset:width}, Dn
/// </summary>
static void cpuBfExtuReg(ULO regno, UWO ext)
{
  cpuBfExtuCommon(regno, false, ext);
}

/// <summary>
/// bfextu ea {offset:width}, Dn
/// </summary>
static void cpuBfExtuEa(ULO ea, UWO ext)
{
  cpuBfExtuCommon(ea, true, ext);
}

/// <summary>
/// bfffo common logic
/// </summary>
static void cpuBfFfoCommon(ULO val, bool has_ea, UWO ext)
{
  struct cpuBfData bf_data;
  ULO i;
  cpuBfDecodeExtWordAndGetField(&bf_data, val, true, has_ea, ext);
  cpuSetFlagsNZVC(bf_data.field == 0, bf_data.field & (1 << (bf_data.width - 1)), FALSE, FALSE);

  for (i = 0; i < bf_data.width; ++i)
  {
    if (bf_data.field & (1 << (bf_data.width - i - 1)))
      break;
  }
  cpuSetDReg(bf_data.dn, bf_data.offset + i);
}

/// <summary>
/// bfffo dx {offset:width}, Dn
/// </summary>
static void cpuBfFfoReg(ULO regno, UWO ext)
{
  cpuBfFfoCommon(regno, false, ext);
}

/// <summary>
/// bfffo ea {offset:width}, Dn
/// </summary>
static void cpuBfFfoEa(ULO ea, UWO ext)
{
  cpuBfFfoCommon(ea, true, ext);
}

/// <summary>
/// bfins common logic
/// </summary>
static void cpuBfInsCommon(ULO ea_or_reg, bool has_ea, UWO ext)
{
  struct cpuBfData bf_data;
  cpuBfDecodeExtWordAndGetField(&bf_data, ea_or_reg, true, has_ea, ext);

  bf_data.field = cpuGetDReg(bf_data.dn) & bf_data.field_mask;

  // Flags are set according to the inserted value
  cpuSetFlagsNZVC(bf_data.field == 0, bf_data.field & (1 << (bf_data.width - 1)), FALSE, FALSE);

  cpuSetBfField(&bf_data, ea_or_reg, has_ea);
}

/// <summary>
/// bfins Dn, ea {offset:width}
/// </summary>
static void cpuBfInsReg(ULO regno, UWO ext)
{
  cpuBfInsCommon(regno, false, ext);
}

/// <summary>
/// bfins Dn, ea {offset:width}
/// </summary>
static void cpuBfInsEa(ULO ea, UWO ext)
{
  cpuBfInsCommon(ea, true, ext);
}

/// <summary>
/// bfset common logic
/// </summary>
static void cpuBfSetCommon(ULO ea_or_reg, bool has_ea, UWO ext)
{
  struct cpuBfData bf_data;
  cpuBfDecodeExtWordAndGetField(&bf_data, ea_or_reg, false, has_ea, ext);
  cpuSetFlagsNZVC(bf_data.field == 0, bf_data.field & (1 << (bf_data.width - 1)), FALSE, FALSE);

  bf_data.field = (ULO)bf_data.field_mask;

  cpuSetBfField(&bf_data, ea_or_reg, has_ea);
}

/// <summary>
/// bfset dx {offset:width}
/// </summary>
static void cpuBfSetReg(ULO regno, UWO ext)
{
  cpuBfSetCommon(regno, false, ext);
}

/// <summary>
/// bfset ea {offset:width}
/// </summary>
static void cpuBfSetEa(ULO ea, UWO ext)
{
  cpuBfSetCommon(ea, true, ext);
}

/// <summary>
/// bftst common logic
/// </summary>
static void cpuBfTstCommon(ULO ea_or_reg, bool has_ea, UWO ext)
{
  struct cpuBfData bf_data;
  cpuBfDecodeExtWordAndGetField(&bf_data, ea_or_reg, false, has_ea, ext);
  cpuSetFlagsNZVC(bf_data.field == 0, bf_data.field & (1 << (bf_data.width - 1)), FALSE, FALSE);
}

/// <summary>
/// bftst dx {offset:width}
/// </summary>
static void cpuBfTstReg(ULO regno, UWO ext)
{
  cpuBfTstCommon(regno, false, ext);
}

/// <summary>
/// bftst ea {offset:width}
/// </summary>
static void cpuBfTstEa(ULO ea, UWO ext)
{
  cpuBfTstCommon(ea, true, ext);
}

/// <summary>
/// movep.w (d16, Ay), Dx
/// </summary>
static void cpuMovepWReg(ULO areg, ULO dreg)
{
  ULO ea = cpuGetAReg(areg) + cpuGetNextWordSignExt();
  cpuSetDRegWord(dreg, cpuJoinByteToWord(memoryReadByte(ea), memoryReadByte(ea + 2)));
  cpuSetInstructionTime(16);
}

/// <summary>
/// movep.l (d16, Ay), Dx
/// </summary>
static void cpuMovepLReg(ULO areg, ULO dreg)
{
  ULO ea = cpuGetAReg(areg) + cpuGetNextWordSignExt();
  cpuSetDReg(dreg, cpuJoinByteToLong(memoryReadByte(ea), memoryReadByte(ea + 2), memoryReadByte(ea + 4), memoryReadByte(ea + 6)));
  cpuSetInstructionTime(24);
}

/// <summary>
/// movep.w Dx, (d16, Ay)
/// </summary>
static void cpuMovepWEa(ULO areg, ULO dreg)
{
  ULO ea = cpuGetAReg(areg) + cpuGetNextWordSignExt();
  memoryWriteByte((UBY) (cpuGetDReg(dreg) >> 8), ea);
  memoryWriteByte(cpuGetDRegByte(dreg), ea + 2);
  cpuSetInstructionTime(16);
}

/// <summary>
/// movep.l Dx, (d16, Ay)
/// </summary>
static void cpuMovepLEa(ULO areg, ULO dreg)
{
  ULO ea = cpuGetAReg(areg) + cpuGetNextWordSignExt();
  memoryWriteByte((UBY)(cpuGetDReg(dreg) >> 24), ea);
  memoryWriteByte((UBY)(cpuGetDReg(dreg) >> 16), ea + 2);
  memoryWriteByte((UBY)(cpuGetDReg(dreg) >> 8), ea + 4);
  memoryWriteByte(cpuGetDRegByte(dreg), ea + 6);
  cpuSetInstructionTime(24);
}

/// <summary>
/// pack Dx, Dy, #adjustment
/// </summary>
static void cpuPackReg(ULO yreg, ULO xreg)
{
  UWO adjustment = cpuGetNextWord();
  UWO src = cpuGetDRegWord(xreg) + adjustment;
  cpuSetDRegByte(yreg, (UBY) (((src >> 4) & 0xf0) | (src & 0xf)));
  cpuSetInstructionTime(4);
}

/// <summary>
/// pack -(Ax), -(Ay), #adjustment
/// </summary>
static void cpuPackEa(ULO yreg, ULO xreg)
{
  UWO adjustment = cpuGetNextWord();
  UBY b1 = memoryReadByte(cpuEA04(xreg, 1));
  UBY b2 = memoryReadByte(cpuEA04(xreg, 1));
  UWO result = ((((UWO)b1) << 8) | (UWO) b2) + adjustment;
  memoryWriteByte((UBY) (((result >> 4) & 0xf0) | (result & 0xf)), cpuEA04(yreg, 1));
  cpuSetInstructionTime(4);
}

/// <summary>
/// unpk Dx, Dy, #adjustment
/// </summary>
static void cpuUnpkReg(ULO yreg, ULO xreg)
{
  UWO adjustment = cpuGetNextWord();
  UBY b1 = cpuGetDRegByte(xreg);
  UWO result = ((((UWO)(b1 & 0xf0)) << 4) | ((UWO)(b1 & 0xf))) + adjustment;
  cpuSetDRegWord(yreg, result);
  cpuSetInstructionTime(4);
}

/// <summary>
/// unpk -(Ax), -(Ay), #adjustment
/// </summary>
static void cpuUnpkEa(ULO yreg, ULO xreg)
{
  UWO adjustment = cpuGetNextWord();
  UBY b1 = memoryReadByte(cpuEA04(xreg, 1));
  UWO result = ((((UWO)(b1 & 0xf0)) << 4) | ((UWO)(b1 & 0xf))) + adjustment;
  memoryWriteByte((UBY) (result >> 8), cpuEA04(yreg, 1));
  memoryWriteByte((UBY) result, cpuEA04(yreg, 1));
  cpuSetInstructionTime(4);
}

/// <summary>
/// movec
/// </summary>
static void cpuMoveCFrom()
{
  if (cpuGetFlagSupervisor())
  {
    UWO extension = (UWO) cpuGetNextWord();
    ULO da = (extension >> 15) & 1;
    ULO regno = (extension >> 12) & 7;
    ULO ctrl_regno = extension & 0xfff;
    if (cpuGetModelMajor() == 1)
    {
      switch (ctrl_regno)
      {
  case 0x000: cpuSetReg(da, regno, cpuGetSfc()); break;
  case 0x001: cpuSetReg(da, regno, cpuGetDfc()); break;
  case 0x800: cpuSetReg(da, regno, cpuGetUspDirect()); break; // In supervisor mode, usp is up to date.
  case 0x801: cpuSetReg(da, regno, cpuGetVbr()); break;
  default:  cpuThrowIllegalInstructionException(FALSE); return;   // Illegal instruction
      }
    }
    else if (cpuGetModelMajor() == 2)
    {
      switch (ctrl_regno)
      {
  case 0x000: cpuSetReg(da, regno, cpuGetSfc()); break;
  case 0x001: cpuSetReg(da, regno, cpuGetDfc()); break;
  case 0x002: cpuSetReg(da, regno, cpuGetCacr() & 3); break;
  case 0x800: cpuSetReg(da, regno, cpuGetUspDirect()); break; // In supervisor mode, usp is up to date.
  case 0x801: cpuSetReg(da, regno, cpuGetVbr()); break;
  case 0x802: cpuSetReg(da, regno, cpuGetCaar() & 0xfc); break;
  case 0x803: cpuSetReg(da, regno, cpuGetMspAutoMap()); break;
  case 0x804: cpuSetReg(da, regno, cpuGetIspAutoMap()); break;
  default:  cpuThrowIllegalInstructionException(FALSE); return;   // Illegal instruction
      }
    }
    else if (cpuGetModelMajor() == 3)
    {
      switch (ctrl_regno)
      {
  case 0x000: cpuSetReg(da, regno, cpuGetSfc()); break;
  case 0x001: cpuSetReg(da, regno, cpuGetDfc()); break;
  case 0x002: cpuSetReg(da, regno, cpuGetCacr()); break;
  case 0x800: cpuSetReg(da, regno, cpuGetUspDirect()); break; // In supervisor mode, usp is up to date.
  case 0x801: cpuSetReg(da, regno, cpuGetVbr()); break;
  case 0x802: cpuSetReg(da, regno, cpuGetCaar() & 0xfc); break;
  case 0x803: cpuSetReg(da, regno, cpuGetMspAutoMap()); break;
  case 0x804: cpuSetReg(da, regno, cpuGetIspAutoMap()); break;
  default:  cpuThrowIllegalInstructionException(FALSE); return;   // Illegal instruction
      }
    }
  }
  else
  {
    cpuThrowPrivilegeViolationException();
    return;
  }
  cpuSetInstructionTime(4);
}

/// <summary>
/// movec
/// </summary>
static void cpuMoveCTo()
{
  if (cpuGetFlagSupervisor())
  {
    UWO extension = (UWO) cpuGetNextWord();
    ULO da = (extension >> 15) & 1;
    ULO regno = (extension >> 12) & 7;
    ULO ctrl_regno = extension & 0xfff;
    if (cpuGetModelMajor() == 1)
    {
      switch (ctrl_regno)
      {
  case 0x000: cpuSetSfc(cpuGetReg(da, regno) & 7); break;
  case 0x001: cpuSetDfc(cpuGetReg(da, regno) & 7); break;
  case 0x800: cpuSetUspDirect(cpuGetReg(da, regno)); break;
  case 0x801: cpuSetVbr(cpuGetReg(da, regno)); break;
  default:  cpuThrowIllegalInstructionException(FALSE); return;   // Illegal instruction
      }
    }
    else if (cpuGetModelMajor() == 2)
    {
      switch (ctrl_regno)
      {
  case 0x000: cpuSetSfc(cpuGetReg(da, regno) & 7); break;
  case 0x001: cpuSetDfc(cpuGetReg(da, regno) & 7); break;
  case 0x002: cpuSetCacr(cpuGetReg(da, regno) & 0x3); break;
  case 0x800: cpuSetUspDirect(cpuGetReg(da, regno)); break;
  case 0x801: cpuSetVbr(cpuGetReg(da, regno)); break;
  case 0x802: cpuSetCaar(cpuGetReg(da, regno) & 0x00fc); break;
  case 0x803: cpuSetMspAutoMap(cpuGetReg(da, regno)); break;
  case 0x804: cpuSetIspAutoMap(cpuGetReg(da, regno)); break;
  default:  cpuThrowIllegalInstructionException(FALSE); return;   // Illegal instruction
      }
    }
    else if (cpuGetModelMajor() == 3)
    {
      switch (ctrl_regno)
      {
  case 0x000: cpuSetSfc(cpuGetReg(da, regno) & 7); break;
  case 0x001: cpuSetDfc(cpuGetReg(da, regno) & 7); break;
  case 0x002: cpuSetCacr(cpuGetReg(da, regno) & 0x3313); break;
  case 0x800: cpuSetUspDirect(cpuGetReg(da, regno)); break;
  case 0x801: cpuSetVbr(cpuGetReg(da, regno)); break;
  case 0x802: cpuSetCaar(cpuGetReg(da, regno) & 0x00fc); break;
  case 0x803: cpuSetMspAutoMap(cpuGetReg(da, regno)); break;
  case 0x804: cpuSetIspAutoMap(cpuGetReg(da, regno)); break;
  default:  cpuThrowIllegalInstructionException(FALSE); return;   // Illegal instruction
      }
    }
  }
  else
  {
    cpuThrowPrivilegeViolationException();
    return;
  }
  cpuSetInstructionTime(4);
}

/// <summary>
/// moves.b Rn, ea / moves.b ea, Rn
/// </summary>
static void cpuMoveSB(ULO ea, UWO extension)
{
  if (cpuGetFlagSupervisor())
  {
    ULO da = (extension >> 15) & 1;
    ULO regno = (extension >> 12) & 7;
    if (extension & 0x0800) // From Rn to ea (in dfc)
    {
      memoryWriteByte((UBY)cpuGetReg(da, regno), ea);
    }
    else  // From ea to Rn (in sfc)
    {
      UBY data = memoryReadByte(ea);
      if (da == 0)
      {
  cpuSetDRegByte(regno, data);
      }
      else
      {
  cpuSetAReg(regno, (ULO)(LON)(BYT) data);
      }
    }
  }
  else
  {
    cpuThrowPrivilegeViolationException();
    return;
  }
  cpuSetInstructionTime(4);   
}

/// <summary>
/// moves.w Rn, ea / moves.w ea, Rn
/// </summary>
static void cpuMoveSW(ULO ea, UWO extension)
{
  if (cpuGetFlagSupervisor())
  {
    ULO da = (extension >> 15) & 1;
    ULO regno = (extension >> 12) & 7;
    if (extension & 0x0800) // From Rn to ea (in dfc)
    {
      memoryWriteWord((UWO)cpuGetReg(da, regno), ea);
    }
    else  // From ea to Rn (in sfc)
    {
      UWO data = memoryReadWord(ea);
      if (da == 0)
      {
  cpuSetDRegWord(regno, data);
      }
      else
      {
  cpuSetAReg(regno, (ULO)(LON)(WOR) data);
      }
    }
  }
  else
  {
    cpuThrowPrivilegeViolationException();
    return;
  }
  cpuSetInstructionTime(4);   
}

/// <summary>
/// moves.l Rn, ea / moves.l ea, Rn
/// </summary>
static void cpuMoveSL(ULO ea, UWO extension)
{
  if (cpuGetFlagSupervisor())
  {
    ULO da = (extension >> 15) & 1;
    ULO regno = (extension >> 12) & 7;
    if (extension & 0x0800) // From Rn to ea (in dfc)
    {
      memoryWriteLong(cpuGetReg(da, regno), ea);
    }
    else  // From ea to Rn (in sfc)
    {
      cpuSetDReg(regno, memoryReadLong(ea));
    }
  }
  else
  {
    cpuThrowPrivilegeViolationException();
    return;
  }
  cpuSetInstructionTime(4);   
}

/// <summary>
/// Trapcc
/// </summary>
static void cpuTrapcc(ULO cc)
{
  if (cc)
  {
    cpuThrowTrapVException(); // TrapV and Trapcc share the exception vector
    return;
  }
  cpuSetInstructionTime(4);   
}

/// <summary>
/// Trapcc.w #
/// </summary>
static void cpuTrapccW(ULO cc)
{
  cpuGetNextWord();
  if (cc)
  {
    cpuThrowTrapVException(); // TrapV and Trapcc share the exception vector
    return;
  }
  cpuSetInstructionTime(4);   
}

/// <summary>
/// trapcc.l #
/// </summary>
static void cpuTrapccL(ULO cc)
{
  cpuGetNextLong();
  if (cc)
  {
    cpuThrowTrapVException(); // TrapV and Trapcc share the exception vector
    return;
  }
  cpuSetInstructionTime(4);   
}

/// <summary>
/// cas.b Dc,Du, ea
/// </summary>
static void cpuCasB(ULO ea, UWO extension)
{
  UBY dst = memoryReadByte(ea);
  ULO cmp_regno = extension & 7;
  UBY res = dst - cpuGetDRegByte(cmp_regno);

  cpuSetFlagsCmp(cpuIsZeroB(res), cpuMsbB(res), cpuMsbB(dst), cpuMsbB(cpuGetDRegByte(cmp_regno)));

  if (cpuIsZeroB(res))
  {
    memoryWriteByte(cpuGetDRegByte((extension >> 6) & 7), ea);
  }
  else
  {
    cpuSetDRegByte(cmp_regno, dst);
  }
  cpuSetInstructionTime(4);
}

/// <summary>
/// cas.w Dc,Du, ea
/// </summary>
static void cpuCasW(ULO ea, UWO extension)
{
  UWO dst = memoryReadWord(ea);
  ULO cmp_regno = extension & 7;
  UWO res = dst - cpuGetDRegWord(cmp_regno);

  cpuSetFlagsCmp(cpuIsZeroW(res), cpuMsbW(res), cpuMsbW(dst), cpuMsbW(cpuGetDRegWord(cmp_regno)));

  if (cpuIsZeroW(res))
  {
    memoryWriteWord(cpuGetDRegWord((extension >> 6) & 7), ea);
  }
  else
  {
    cpuSetDRegWord(cmp_regno, dst);
  }
  cpuSetInstructionTime(4);
}

/// <summary>
/// cas.l Dc,Du, ea
/// </summary>
static void cpuCasL(ULO ea, UWO extension)
{
  ULO dst = memoryReadLong(ea);
  ULO cmp_regno = extension & 7;
  ULO res = dst - cpuGetDReg(cmp_regno);

  cpuSetFlagsCmp(cpuIsZeroL(res), cpuMsbL(res), cpuMsbL(dst), cpuMsbL(cpuGetDReg(cmp_regno)));

  if (cpuIsZeroL(res))
  {
    memoryWriteLong(cpuGetDReg((extension >> 6) & 7), ea);
  }
  else
  {
    cpuSetDReg(cmp_regno, dst);
  }
  cpuSetInstructionTime(4);
}

/// <summary>
/// cas2.w Dc1:Dc2,Du1:Du2,(Rn1):(Rn2)
/// </summary>
static void cpuCas2W()
{
  UWO extension1 = cpuGetNextWord();
  UWO extension2 = cpuGetNextWord();
  ULO ea1 = cpuGetReg(extension1 >> 15, (extension1 >> 12) & 7);
  ULO ea2 = cpuGetReg(extension2 >> 15, (extension2 >> 12) & 7);
  UWO dst1 = memoryReadWord(ea1);
  UWO dst2 = memoryReadWord(ea2);
  ULO cmp1_regno = extension1 & 7;
  ULO cmp2_regno = extension2 & 7;
  UWO res1 = dst1 - cpuGetDRegWord(cmp1_regno);
  UWO res2 = dst2 - cpuGetDRegWord(cmp2_regno);

  if (cpuIsZeroW(res1))
  {
    cpuSetFlagsCmp(cpuIsZeroW(res2), cpuMsbW(res2), cpuMsbW(dst2), cpuMsbW(cpuGetDRegWord(cmp2_regno)));
  }
  else
  {
    cpuSetFlagsCmp(cpuIsZeroW(res1), cpuMsbW(res1), cpuMsbW(dst1), cpuMsbW(cpuGetDRegWord(cmp1_regno)));
  }

  if (cpuIsZeroW(res1) && cpuIsZeroW(res2))
  {
    memoryWriteWord(cpuGetDRegWord((extension1 >> 6) & 7), ea1);
    memoryWriteWord(cpuGetDRegWord((extension2 >> 6) & 7), ea2);
  }
  else
  {
    cpuSetDRegWord(cmp1_regno, dst1);
    cpuSetDRegWord(cmp2_regno, dst2);
  }
  cpuSetInstructionTime(4);
}

/// <summary>
/// cas2.l Dc1:Dc2,Du1:Du2,(Rn1):(Rn2)
/// </summary>
static void cpuCas2L()
{
  UWO extension1 = cpuGetNextWord();
  UWO extension2 = cpuGetNextWord();
  ULO ea1 = cpuGetReg(extension1 >> 15, (extension1 >> 12) & 7);
  ULO ea2 = cpuGetReg(extension2 >> 15, (extension2 >> 12) & 7);
  ULO dst1 = memoryReadLong(ea1);
  ULO dst2 = memoryReadLong(ea2);
  ULO cmp1_regno = extension1 & 7;
  ULO cmp2_regno = extension2 & 7;
  ULO res1 = dst1 - cpuGetDReg(cmp1_regno);
  ULO res2 = dst2 - cpuGetDReg(cmp2_regno);

  if (cpuIsZeroL(res1))
  {
    cpuSetFlagsCmp(cpuIsZeroL(res2), cpuMsbL(res2), cpuMsbL(dst2), cpuMsbL(cpuGetDReg(cmp2_regno)));
  }
  else
  {
    cpuSetFlagsCmp(cpuIsZeroL(res1), cpuMsbL(res1), cpuMsbL(dst1), cpuMsbL(cpuGetDReg(cmp1_regno)));
  }

  if (cpuIsZeroL(res1) && cpuIsZeroL(res2))
  {
    memoryWriteLong(cpuGetDReg((extension1 >> 6) & 7), ea1);
    memoryWriteLong(cpuGetDReg((extension2 >> 6) & 7), ea2);
  }
  else
  {
    cpuSetDReg(cmp1_regno, dst1);
    cpuSetDReg(cmp2_regno, dst2);
  }
  cpuSetInstructionTime(4);
}

/// <summary>
/// Common code for chk2 ea, Rn / cmp2 ea, Rn
/// </summary>
static void cpuChkCmp(ULO lb, ULO ub, ULO val, BOOLE is_chk2)
{
  BOOLE z = (val == lb || val == ub);
  BOOLE c = ((lb <= ub) && (val < lb || val > ub)) || ((lb > ub) && (val < lb) && (val > ub)); 
  cpuSetFlagZ(z);
  cpuSetFlagC(c);
  cpuSetInstructionTime(4);
  if (is_chk2 && c)
  {
    cpuThrowChkException();
  }
}

/// <summary>
/// chk2.b ea, Rn / cmp2.b ea, Rn
/// </summary>
static void cpuChkCmp2B(ULO ea, UWO extension)
{
  ULO da = (ULO) (extension >> 15);
  ULO rn = (ULO) (extension >> 12) & 7;
  BOOLE is_chk2 = (extension & 0x0800);
  if (da == 1)
  {
    cpuChkCmp((ULO)(LON)(BYT)memoryReadByte(ea), (ULO)(LON)(BYT)memoryReadByte(ea + 1), cpuGetAReg(rn), is_chk2);
  }
  else
  {
    cpuChkCmp((ULO)memoryReadByte(ea), (ULO)memoryReadByte(ea + 1), (ULO)(UBY)cpuGetDReg(rn), is_chk2);
  }
}

/// <summary>
/// chk2.w ea, Rn / cmp2.w ea, Rn
/// </summary>
static void cpuChkCmp2W(ULO ea, UWO extension)
{
  ULO da = (ULO) (extension >> 15);
  ULO rn = (ULO) (extension >> 12) & 7;
  BOOLE is_chk2 = (extension & 0x0800);
  if (da == 1)
  {
    cpuChkCmp((ULO)(LON)(WOR)memoryReadWord(ea), (ULO)(LON)(WOR)memoryReadWord(ea + 1), cpuGetAReg(rn), is_chk2);
  }
  else
  {
    cpuChkCmp((ULO)memoryReadWord(ea), (ULO)memoryReadWord(ea + 2), (ULO)(UWO)cpuGetDReg(rn), is_chk2);
  }
}

/// <summary>
/// chk2.l ea, Rn / cmp2.l ea, Rn
/// </summary>
static void cpuChkCmp2L(ULO ea, UWO extension)
{
  ULO da = (ULO) (extension >> 15);
  ULO rn = (ULO) (extension >> 12) & 7;
  BOOLE is_chk2 = (extension & 0x0800);
  cpuChkCmp(memoryReadLong(ea), memoryReadLong(ea + 4), cpuGetReg(da, rn), is_chk2);
}

/// <summary>
/// callm
/// Since this is a coprocessor instruction, this is NOP.
/// This will likely fail, but anything we do here will be wrong anyhow.
/// </summary>
static void cpuCallm(ULO ea, UWO extension)
{
  cpuSetInstructionTime(4);
}

/// <summary>
/// rtm
/// Since this is a coprocessor instruction, this is NOP.
/// This will likely fail, but anything we do here will be wrong anyhow.
/// </summary>
static void cpuRtm(ULO da, ULO regno)
{
  cpuSetInstructionTime(4);
}

/// <summary>
/// 68030 version only.
///
/// Extension word: 001xxx00xxxxxxxx
/// pflusha
/// pflush fc, mask
/// pflush fc, mask, ea
///
/// Extension word: 001000x0000xxxxx
/// ploadr fc, ea
/// ploadw fc, ea
///
/// Extension word: 010xxxxx00000000 (SRp, CRP, TC)
/// Extension word: 011000x000000000 (MMU status register)
/// Extension word: 000xxxxx00000000 (TT)
/// pmove mrn, ea
/// pmove ea, mrn
/// pmovefd ea, mrn
///
/// Extension word: 100xxxxxxxxxxxxx
/// ptestr fc, ea, #level
/// ptestr fc, ea, #level, An
/// ptestw fc, ea, #level
/// ptestw fc, ea, #level, An
///
/// Since this is a coprocessor instruction, this is NOP.
/// </summary>
static void cpuPflush030(ULO ea, UWO extension)
{
  if (cpuGetFlagSupervisor())
  {
    if ((extension & 0xfde0) == 0x2000)
    {
      // ploadr, ploadw
    }
    else if ((extension & 0xe300) == 0x2000)
    {
      // pflusha, pflush
      ULO mode = (extension >> 10) & 7;
      ULO mask = (extension >> 5) & 7;
      ULO fc = extension & 0x1f;
    }
  }
  else
  {
    cpuThrowPrivilegeViolationException();
    return;
  }
  cpuSetInstructionTime(4);
}

/// <summary>
/// pflusha
/// pflush fc, mask
/// pflush fc, mask, ea
///
/// 68040 version only.
///
/// Since this is a coprocessor instruction, this is NOP.
/// </summary>
static void cpuPflush040(ULO opmode, ULO regno)
{
  if (cpuGetFlagSupervisor())
  {
    if (cpuGetModelMajor() != 2)  // This is NOP on 68EC040
    {
      switch (opmode)
      {
  case 0: //PFLUSHN (An)
    break;
  case 1: //PFLUSH (An)
    break;
  case 2: //PFLUSHAN
    break;
  case 3: //PFLUSHA
    break;
      }
    }
  }
  else
  {
    cpuThrowPrivilegeViolationException();
    return;
  }
  cpuSetInstructionTime(4);
}

/// <summary>
/// ptestr (An)
/// ptestw (An)
///
/// 68040 version only.
///
/// Since this is a coprocessor instruction, this is NOP.
/// </summary>
static void cpuPtest040(ULO rw, ULO regno)
{
  if (cpuGetFlagSupervisor())
  {
    if (cpuGetModelMajor() != 2)  // This is NOP on 68EC040
    {
      if (rw == 0)
      {
  // ptestr
      }
      else 
      {
  // ptestw
      }
    }
  } 
  else
  {
    cpuThrowPrivilegeViolationException();
    return;
  }
  cpuSetInstructionTime(4);
}

#include "CpuModule_Decl.h"
#include "CpuModule_Data.h"
#include "CpuModule_Profile.h"
#include "CpuModule_Code.h"

cpuOpcodeData cpu_opcode_data_current[65536];

void cpuMakeOpcodeTableForModel(void)
{
  ULO opcode;
  for (opcode = 0; opcode < 65536; opcode++)
  {
    if (cpu_opcode_model_mask[opcode] & cpuGetModelMask())
    {
      cpu_opcode_data_current[opcode] = cpu_opcode_data[opcode];
    }
    else
    {
      cpu_opcode_data_current[opcode].instruction_func = cpuIllegalInstruction;
      cpu_opcode_data_current[opcode].data[0] = 0;
      cpu_opcode_data_current[opcode].data[1] = 0;
      cpu_opcode_data_current[opcode].data[2] = 0;
    }
  }
}

ULO irq_arrival_time = -1;
extern ULO busGetCycle();

ULO cpuExecuteInstruction(void)
{
  if (cpuGetRaiseInterrupt())
  {
    cpuSetUpInterrupt(cpuGetRaiseInterruptLevel());
    cpuCheckPendingInterrupts();
    return 44;
  }
  else
  {
    ULO oldSr = cpuGetSR();
    UWO opcode;

#ifdef CPU_INSTRUCTION_LOGGING
    cpuCallInstructionLoggingFunc();
#endif

    cpuSetOriginalPC(cpuGetPC()); // Store pc and opcode for exception logging
    opcode = cpuGetNextWord();

#ifdef CPU_INSTRUCTION_LOGGING
    cpuSetCurrentOpcode(opcode);
#endif

    cpuSetInstructionTime(0);

  cpu_opcode_data_current[opcode].instruction_func(
      cpu_opcode_data_current[opcode].data);
    if (oldSr & 0xc000)
    {
      // This instruction was traced
      ULO cycles = cpuGetInstructionTime();
      cpuThrowTraceException();
      cpuSetInstructionTime(cpuGetInstructionTime() + cycles);
    }
    return cpuGetInstructionTime();
  }
}
