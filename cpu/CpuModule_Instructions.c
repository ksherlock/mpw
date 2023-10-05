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


/*============================================================================*/
/* profiling help functions                                                   */
/*============================================================================*/

#if 0
#ifndef X64
static __inline void cpuTscBefore(int64_t* a)
{
  int64_t local_a = *a;
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

static __inline void cpuTscAfter(int64_t* a, int64_t* b, int32_t* c)
{
  int64_t local_a = *a;
  int64_t local_b = *b;
  uint32_t local_c = *c;

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

void cpuUpdateSr(uint32_t new_sr)
{
  BOOLE supermode_was_set = cpuGetFlagSupervisor();
  BOOLE master_was_set = (cpuGetModelMajor() >= 2) && cpuGetFlagMaster();

  BOOLE supermode_is_set = !!(new_sr & 0x2000);
  BOOLE master_is_set = (cpuGetModelMajor() >= 2) && !!(new_sr & 0x1000);

  uint32_t runlevel_old = (cpuGetSR() >> 8) & 7;
  uint32_t runlevel_new = (new_sr >> 8) & 7;

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

  if (cpuGetModelMajor() < 2)
  {
    new_sr &= 0xa71f;
  }
  else
  {
    new_sr &= 0xf71f;
  }

  cpuSetSR(new_sr);

  if (runlevel_old != runlevel_new)
  {
    cpuCheckPendingInterrupts();
  }
}

static void cpuIllegal(void)
{
  uint16_t opcode = memoryReadWord(cpuGetPC() - 2);
  if ((opcode & 0xf000) == 0xf000)
  {
    cpuThrowFLineException();
  }
  else if ((opcode & 0xf000) == 0xa000)
  {
    #if 0
    if ((cpuGetPC() & 0xff0000) == 0xf00000)
    {
      call_calltrap(opcode & 0xfff);
      cpuInitializeFromNewPC(cpuGetPC());
      cpuSetInstructionTime(512);
    }
    else
    {
      cpuThrowALineException();
    }
    #endif
    // MPW - always do a-line exception (tool traps).
    cpuThrowALineException();
  }
  else
  {
    cpuThrowIllegalInstructionException(FALSE);
  }
}

/// <summary>
/// Illegal instruction handler.
/// </summary>
static void cpuIllegalInstruction(uint32_t *opcode_data)
{
  cpuIllegal(); 
}

/// <summary>
/// BKPT
/// </summary>
static void cpuBkpt(uint32_t vector)
{
  cpuThrowIllegalInstructionExceptionFromBreakpoint();
}

/// <summary>
/// Adds bytes src1 to src2. Sets all flags.
/// </summary>
/// <returns>The result</returns>
static uint8_t cpuAddB(uint8_t src2, uint8_t src1)
{
  uint8_t res = src2 + src1;
  cpuSetFlagsAdd(cpuIsZeroB(res), cpuMsbB(res), cpuMsbB(src2), cpuMsbB(src1));
  return res;
}

/// <summary>
/// Adds words src1 to src2. Sets all flags.
/// </summary>
/// <returns>The result</returns>
static uint16_t cpuAddW(uint16_t src2, uint16_t src1)
{
  uint16_t res = src2 + src1;
  cpuSetFlagsAdd(cpuIsZeroW(res), cpuMsbW(res), cpuMsbW(src2), cpuMsbW(src1));
  return res;
}

/// <summary>
/// Adds dwords src1 to src2. Sets all flags.
/// </summary>
/// <returns>The result</returns>
static uint32_t cpuAddL(uint32_t src2, uint32_t src1)
{
  uint32_t res = src2 + src1;
  cpuSetFlagsAdd(cpuIsZeroL(res), cpuMsbL(res), cpuMsbL(src2), cpuMsbL(src1));
  return res;
}

/// <summary>
/// Adds src1 to src2 (For address registers). No flags.
/// </summary>
/// <returns>The result</returns>
static uint32_t cpuAddaW(uint32_t src2, uint32_t src1)
{
  return src2 + src1;
}

/// <summary>
/// Adds src1 to src2 (For address registers). No flags.
/// </summary>
/// <returns>The result</returns>
static uint32_t cpuAddaL(uint32_t src2, uint32_t src1)
{
  return src2 + src1;
}

/// <summary>
/// Subtracts src1 from src2. Sets all flags.
/// </summary>
/// <returns>The result</returns>
static uint8_t cpuSubB(uint8_t src2, uint8_t src1)
{
  uint8_t res = src2 - src1;
  cpuSetFlagsSub(cpuIsZeroB(res), cpuMsbB(res), cpuMsbB(src2), cpuMsbB(src1));
  return res;
}

/// <summary>
/// Subtracts src1 from src2. Sets all flags.
/// </summary>
/// <returns>The result</returns>
static uint16_t cpuSubW(uint16_t src2, uint16_t src1)
{
  uint16_t res = src2 - src1;
  cpuSetFlagsSub(cpuIsZeroW(res), cpuMsbW(res), cpuMsbW(src2), cpuMsbW(src1));
  return res;
}

/// <summary>
/// Subtracts src1 from src2. Sets all flags.
/// </summary>
/// <returns>The result</returns>
static uint32_t cpuSubL(uint32_t src2, uint32_t src1)
{
  uint32_t res = src2 - src1;
  cpuSetFlagsSub(cpuIsZeroL(res), cpuMsbL(res), cpuMsbL(src2), cpuMsbL(src1));
  return res;
}

/// <summary>
/// Subtracts src1 from src2 (For address registers). No flags.
/// </summary>
/// <returns>The result</returns>
static uint32_t cpuSubaW(uint32_t src2, uint32_t src1)
{
  return src2 - src1;
}

/// <summary>
/// Subtracts src1 from src2 (For address registers). No flags.
/// </summary>
/// <returns>The result</returns>
static uint32_t cpuSubaL(uint32_t src2, uint32_t src1)
{
  return src2 - src1;
}

/// <summary>
/// Subtracts src1 from src2. Sets all flags.
/// </summary>
static void cpuCmpB(uint8_t src2, uint8_t src1)
{
  uint8_t res = src2 - src1;
  cpuSetFlagsCmp(cpuIsZeroB(res), cpuMsbB(res), cpuMsbB(src2), cpuMsbB(src1));
}

/// <summary>
/// Subtracts src1 from src2. Sets all flags.
/// </summary>
static void cpuCmpW(uint16_t src2, uint16_t src1)
{
  uint16_t res = src2 - src1;
  cpuSetFlagsCmp(cpuIsZeroW(res), cpuMsbW(res), cpuMsbW(src2), cpuMsbW(src1));
}

/// <summary>
/// Subtracts src1 from src2. Sets all flags.
/// </summary>
static void cpuCmpL(uint32_t src2, uint32_t src1)
{
  uint32_t res = src2 - src1;
  cpuSetFlagsCmp(cpuIsZeroL(res), cpuMsbL(res), cpuMsbL(src2), cpuMsbL(src1));
}

/// <summary>
/// Ands src1 to src2. Sets NZ00 flags.
/// </summary>
/// <returns>The result</returns>
static uint8_t cpuAndB(uint8_t src2, uint8_t src1)
{
  uint8_t res = src2 & src1;
  cpuSetFlagsNZ00NewB(res);
  return res;
}

/// <summary>
/// Ands src1 to src2. Sets NZ00 flags.
/// </summary>
/// <returns>The result</returns>
static uint16_t cpuAndW(uint16_t src2, uint16_t src1)
{
  uint16_t res = src2 & src1;
  cpuSetFlagsNZ00NewW(res);
  return res;
}

/// <summary>
/// Ands src1 to src2. Sets NZ00 flags.
/// </summary>
/// <returns>The result</returns>
static uint32_t cpuAndL(uint32_t src2, uint32_t src1)
{
  uint32_t res = src2 & src1;
  cpuSetFlagsNZ00NewL(res);
  return res;
}

/// <summary>
/// Eors src1 to src2. Sets NZ00 flags.
/// </summary>
/// <returns>The result</returns>
static uint8_t cpuEorB(uint8_t src2, uint8_t src1)
{
  uint8_t res = src2 ^ src1;
  cpuSetFlagsNZ00NewB(res);
  return res;
}

/// <summary>
/// Eors src1 to src2. Sets NZ00 flags.
/// </summary>
/// <returns>The result</returns>
static uint16_t cpuEorW(uint16_t src2, uint16_t src1)
{
  uint16_t res = src2 ^ src1;
  cpuSetFlagsNZ00NewW(res);
  return res;
}

/// <summary>
/// Eors src1 to src2. Sets NZ00 flags.
/// </summary>
/// <returns>The result</returns>
static uint32_t cpuEorL(uint32_t src2, uint32_t src1)
{
  uint32_t res = src2 ^ src1;
  cpuSetFlagsNZ00NewL(res);
  return res;
}

/// <summary>
/// Ors src1 to src2. Sets NZ00 flags.
/// </summary>
/// <returns>The result</returns>
static uint8_t cpuOrB(uint8_t src2, uint8_t src1)
{
  uint8_t res = src2 | src1;
  cpuSetFlagsNZ00NewB(res);
  return res;
}

/// <summary>
/// Ors src1 to src2. Sets NZ00 flags.
/// </summary>
/// <returns>The result</returns>
static uint16_t cpuOrW(uint16_t src2, uint16_t src1)
{
  uint16_t res = src2 | src1;
  cpuSetFlagsNZ00NewW(res);
  return res;
}

/// <summary>
/// Ors src1 to src2. Sets NZ00 flags.
/// </summary>
/// <returns>The result</returns>
static uint32_t cpuOrL(uint32_t src2, uint32_t src1)
{
  uint32_t res = src2 | src1;
  cpuSetFlagsNZ00NewL(res);
  return res;
}

/// <summary>
/// Changes bit in src. Sets Z flag.
/// </summary>
/// <returns>The result</returns>
static uint8_t cpuBchgB(uint8_t src, uint8_t bit)
{
  uint8_t bit_mask = 1 << (bit & 7);
  cpuSetZFlagBitOpsB(src & bit_mask);
  return src ^ bit_mask;
}

/// <summary>
/// Changes bit in src. Sets Z flag.
/// </summary>
/// <returns>The result</returns>
static uint32_t cpuBchgL(uint32_t src, uint32_t bit)
{
  uint32_t bit_mask = 1 << (bit & 31);
  cpuSetZFlagBitOpsL(src & bit_mask);
  return src ^ bit_mask;
}

/// <summary>
/// Clears bit in src. Sets Z flag.
/// </summary>
/// <returns>The result</returns>
static uint8_t cpuBclrB(uint8_t src, uint8_t bit)
{
  uint8_t bit_mask = 1 << (bit & 7);
  cpuSetZFlagBitOpsB(src & bit_mask);
  return src & ~bit_mask;
}

/// <summary>
/// Clears bit in src. Sets Z flag.
/// </summary>
/// <returns>The result</returns>
static uint32_t cpuBclrL(uint32_t src, uint32_t bit)
{
  uint32_t bit_mask = 1 << (bit & 31);
  cpuSetZFlagBitOpsL(src & bit_mask);
  return src & ~bit_mask;
}

/// <summary>
/// Sets bit in src. Sets Z flag.
/// </summary>
/// <returns>The result</returns>
static uint8_t cpuBsetB(uint8_t src, uint8_t bit)
{
  uint8_t bit_mask = 1 << (bit & 7);
  cpuSetZFlagBitOpsB(src & bit_mask);
  return src | bit_mask;
}

/// <summary>
/// Sets bit in src. Sets Z flag.
/// </summary>
/// <returns>The result</returns>
static uint32_t cpuBsetL(uint32_t src, uint32_t bit)
{
  uint32_t bit_mask = 1 << (bit & 31);
  cpuSetZFlagBitOpsL(src & bit_mask);
  return src | bit_mask;
}

/// <summary>
/// Tests bit in src. Sets Z flag.
/// </summary>
static void cpuBtstB(uint8_t src, uint8_t bit)
{
  uint8_t bit_mask = 1 << (bit & 7);
  cpuSetZFlagBitOpsB(src & bit_mask);
}

/// <summary>
/// Tests bit in src. Sets Z flag.
/// </summary>
static void cpuBtstL(uint32_t src, uint32_t bit)
{
  uint32_t bit_mask = 1 << (bit & 31);
  cpuSetZFlagBitOpsL(src & bit_mask);
}

/// <summary>
/// Set flags for clr operation.  X0100.
/// </summary>
static void cpuClr(void)
{
  cpuSetFlags0100();
}

/// <summary>
/// Neg src1. Sets sub flags.
/// </summary>
/// <returns>The result</returns>
static uint8_t cpuNegB(uint8_t src1)
{
  uint8_t res = (uint8_t)-(int8_t)src1;
  cpuSetFlagsNeg(cpuIsZeroB(res), cpuMsbB(res), cpuMsbB(src1));
  return res;
}

/// <summary>
/// Neg src1. Sets sub flags.
/// </summary>
/// <returns>The result</returns>
static uint16_t cpuNegW(uint16_t src1)
{
  uint16_t res = (uint16_t)-(int16_t)src1;
  cpuSetFlagsNeg(cpuIsZeroW(res), cpuMsbW(res), cpuMsbW(src1));
  return res;
}

/// <summary>
/// Neg src1. Sets sub flags.
/// </summary>
/// <returns>The result</returns>
static uint32_t cpuNegL(uint32_t src1)
{
  uint32_t res = (uint32_t)-(int32_t)src1;
  cpuSetFlagsNeg(cpuIsZeroL(res), cpuMsbL(res), cpuMsbL(src1));
  return res;
}

/// <summary>
/// Negx src1.
/// </summary>
/// <returns>The result</returns>
static uint8_t cpuNegxB(uint8_t src1)
{
  int8_t x = (cpuGetFlagX()) ? 1 : 0;
  uint8_t res = (uint8_t)-(int8_t)src1 - x;
  cpuSetFlagsNegx(cpuIsZeroB(res), cpuMsbB(res), cpuMsbB(src1));
  return res;
}

/// <summary>
/// Negx src1.
/// </summary>
/// <returns>The result</returns>
static uint16_t cpuNegxW(uint16_t src1)
{
  int16_t x = (cpuGetFlagX()) ? 1 : 0;
  uint16_t res = (uint16_t)-(int16_t)src1 - x;
  cpuSetFlagsNegx(cpuIsZeroW(res), cpuMsbW(res), cpuMsbW(src1));
  return res;
}

/// <summary>
/// Negx src1.
/// </summary>
/// <returns>The result</returns>
static uint32_t cpuNegxL(uint32_t src1)
{
  int32_t x = (cpuGetFlagX()) ? 1 : 0;
  uint32_t res = (uint32_t)-(int32_t)src1 - x;
  cpuSetFlagsNegx(cpuIsZeroL(res), cpuMsbL(res), cpuMsbL(src1));
  return res;
}

/// <summary>
/// Not src1. 
/// </summary>
/// <returns>The result</returns>
static uint8_t cpuNotB(uint8_t src1)
{
  uint8_t res = ~src1;
  cpuSetFlagsNZ00NewB(res);
  return res;
}

/// <summary>
/// Not src1. 
/// </summary>
/// <returns>The result</returns>
static uint16_t cpuNotW(uint16_t src1)
{
  uint16_t res = ~src1;
  cpuSetFlagsNZ00NewW(res);
  return res;
}

/// <summary>
/// Not src.
/// </summary>
/// <returns>The result</returns>
static uint32_t cpuNotL(uint32_t src)
{
  uint32_t res = ~src;
  cpuSetFlagsNZ00NewL(res);
  return res;
}

/// <summary>
/// Tas src. 
/// </summary>
/// <returns>The result</returns>
static uint8_t cpuTas(uint8_t src)
{
  cpuSetFlagsNZ00NewB(src);
  return src | 0x80;
}

/// <summary>
/// Tst res. 
/// </summary>
static void cpuTestB(uint8_t res)
{
  cpuSetFlagsNZ00NewB(res);
}

/// <summary>
/// Tst res. 
/// </summary>
static void cpuTestW(uint16_t res)
{
  cpuSetFlagsNZ00NewW(res);
}

/// <summary>
/// Tst res. 
/// </summary>
static void cpuTestL(uint32_t res)
{
  cpuSetFlagsNZ00NewL(res);
}

/// <summary>
/// PEA ea. 
/// </summary>
static void cpuPeaL(uint32_t ea)
{
  cpuSetAReg(7, cpuGetAReg(7) - 4);
  memoryWriteLong(ea, cpuGetAReg(7));
}

/// <summary>
/// JMP ea. 
/// </summary>
static void cpuJmp(uint32_t ea)
{
  cpuInitializeFromNewPC(ea);
}

/// <summary>
/// JSR ea. 
/// </summary>
static void cpuJsr(uint32_t ea)
{
  cpuSetAReg(7, cpuGetAReg(7) - 4);
  memoryWriteLong(cpuGetPC(), cpuGetAReg(7));
  cpuInitializeFromNewPC(ea);
}

/// <summary>
/// Move res
/// </summary>
/// <returns>The result</returns>
static void cpuMoveB(uint8_t res)
{
  cpuSetFlagsNZ00NewB(res);
}

/// <summary>
/// Move res
/// </summary>
/// <returns>The result</returns>
static void cpuMoveW(uint16_t res)
{
  cpuSetFlagsNZ00NewW(res);
}

/// <summary>
/// Move res 
/// </summary>
/// <returns>The result</returns>
static void cpuMoveL(uint32_t res)
{
  cpuSetFlagsNZ00NewL(res);
}

/// <summary>
/// Bra byte offset. 
/// </summary>
static void cpuBraB(uint32_t offset)
{
  cpuInitializeFromNewPC(cpuGetPC() + offset);
  cpuSetInstructionTime(10);
}

/// <summary>
/// Bra word offset. 
/// </summary>
static void cpuBraW(void)
{
  uint32_t tmp_pc = cpuGetPC();
  cpuInitializeFromNewPC(tmp_pc + cpuGetNextWordSignExt());
  cpuSetInstructionTime(10);
}

/// <summary>
/// Bra long offset. 
/// </summary>
static void cpuBraL(void)
{
  if (cpuGetModelMajor() < 2) cpuBraB(0xffffffff);
  else
  {
    uint32_t tmp_pc = cpuGetPC();
    cpuInitializeFromNewPC(tmp_pc + cpuGetNextLong());
    cpuSetInstructionTime(4);
  }
}

/// <summary>
/// Bsr byte offset. 
/// </summary>
static void cpuBsrB(uint32_t offset)
{
  cpuSetAReg(7, cpuGetAReg(7) - 4);
  memoryWriteLong(cpuGetPC(), cpuGetAReg(7));
  cpuInitializeFromNewPC(cpuGetPC() + offset);
  cpuSetInstructionTime(18);
}

/// <summary>
/// Bsr word offset. 
/// </summary>
static void cpuBsrW(void)
{
  uint32_t tmp_pc = cpuGetPC();
  uint32_t offset = cpuGetNextWordSignExt();
  cpuSetAReg(7, cpuGetAReg(7) - 4);
  memoryWriteLong(cpuGetPC(), cpuGetAReg(7));
  cpuInitializeFromNewPC(tmp_pc + offset);
  cpuSetInstructionTime(18);
}

/// <summary>
/// Bsr long offset. (020+) 
/// </summary>
static void cpuBsrL(void)
{
  if (cpuGetModelMajor() < 2) cpuBsrB(0xffffffff);
  else
  {
    uint32_t tmp_pc = cpuGetPC();
    uint32_t offset = cpuGetNextLong();
    cpuSetAReg(7, cpuGetAReg(7) - 4);
    memoryWriteLong(cpuGetPC(), cpuGetAReg(7));
    cpuInitializeFromNewPC(tmp_pc + offset);
    cpuSetInstructionTime(4);
  }
}

/// <summary>
/// Bcc byte offset. 
/// </summary>
static void cpuBccB(BOOLE cc, uint32_t offset)
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
    uint32_t tmp_pc = cpuGetPC();
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
      uint32_t tmp_pc = cpuGetPC();
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
static void cpuDbcc(BOOLE cc, uint32_t reg)
{
  if (!cc)
  {
    int16_t val = (int16_t)cpuGetDRegWord(reg);
    val--;
    cpuSetDRegWord(reg, val);
    if (val != -1)
    {
      uint32_t tmp_pc = cpuGetPC();
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
static void cpuAndCcrB(void)
{
  uint16_t imm = cpuGetNextWord();
  cpuSetSR(cpuGetSR() & (0xffe0 | (imm & 0x1f)));
  cpuSetInstructionTime(20);
}

/// <summary>
/// And #imm, sr 
/// </summary>
static void cpuAndSrW(void)
{
  if (cpuGetFlagSupervisor())
  {
    uint16_t imm = cpuGetNextWord();
    cpuUpdateSr(cpuGetSR() & imm);
    cpuSetInstructionTime(20);
  }
  else
  {
    cpuThrowPrivilegeViolationException();
  }
}

/// <summary>
/// Or #imm, ccr 
/// </summary>
static void cpuOrCcrB(void)
{
  uint16_t imm = cpuGetNextWord();
  cpuSetSR(cpuGetSR() | (imm & 0x1f));
  cpuSetInstructionTime(20);
}

/// <summary>
/// Or #imm, sr 
/// </summary>
static void cpuOrSrW(void)
{
  if (cpuGetFlagSupervisor())
  {
    uint16_t imm = cpuGetNextWord();
    cpuUpdateSr(cpuGetSR() | imm);
    cpuSetInstructionTime(20);
  }
  else
  {
    cpuThrowPrivilegeViolationException();
  }
}

/// <summary>
/// Eor #imm, ccr 
/// </summary>
static void cpuEorCcrB(void)
{
  uint16_t imm = cpuGetNextWord();
  cpuSetSR(cpuGetSR() ^ (imm & 0x1f));
  cpuSetInstructionTime(20);
}

/// <summary>
/// Eor #imm, sr 
/// </summary>
static void cpuEorSrW(void)
{
  if (cpuGetFlagSupervisor())
  {
    uint16_t imm = cpuGetNextWord();
    cpuUpdateSr(cpuGetSR() ^ imm);
    cpuSetInstructionTime(20);
  }
  else
  {
    cpuThrowPrivilegeViolationException();
  }
}

/// <summary>
/// Move ea, ccr 
/// </summary>
static void cpuMoveToCcr(uint16_t src)
{
  cpuSetSR((cpuGetSR() & 0xff00) | (src & 0x1f));
}

/// <summary>
/// Move <ea>, sr 
/// </summary>
static void cpuMoveToSr(uint16_t src)
{
  // Supervisor privilege for move to sr is checked in the instruction wrappers
  cpuUpdateSr(src);
}

/// <summary>
/// Move ccr, ea 
/// </summary>
static uint16_t cpuMoveFromCcr(void)
{
  return cpuGetSR() & 0x1f;
}

/// <summary>
/// Move <ea>, sr 
/// </summary>
static uint16_t cpuMoveFromSr(void)
{
  // Supervisor privilege for move from sr is checked in the instruction wrappers, privileged on 010+
  return (uint16_t) cpuGetSR();
}

/// <summary>
/// Scc byte.
/// </summary>
static uint8_t cpuScc(uint32_t cc)
{
  return (cpuCalculateConditionCode(cc)) ? 0xff : 0;
}

/// <summary>
/// Rts 
/// </summary>
static void cpuRts(void)
{
  cpuInitializeFromNewPC(memoryReadLong(cpuGetAReg(7)));
  cpuSetAReg(7, cpuGetAReg(7) + 4);
  cpuSetInstructionTime(16);
}

/// <summary>
/// Rtr 
/// </summary>
static void cpuRtr(void)
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
static void cpuNop(void)
{
  cpuSetInstructionTime(4);
}

/// <summary>
/// Trapv
/// </summary>
static void cpuTrapv(void)
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
static void cpuMulL(uint32_t src1, uint16_t extension)
{
  uint32_t dl = (extension >> 12) & 7;
  if (extension & 0x0800) // muls.l
  {
    int64_t result = ((int64_t)(int32_t) src1) * ((int64_t)(int32_t)cpuGetDReg(dl));
    if (extension & 0x0400) // 32bx32b=64b
    {  
      uint32_t dh = extension & 7;
      cpuSetDReg(dl, (uint32_t)result);
      cpuSetDReg(dh, (uint32_t)(result >> 32));
      cpuSetFlagsNZ00New64(result);
    }
    else // 32bx32b=32b
    {
      BOOLE o;
      if (result >= 0)
	o = (result & 0xffffffff80000000) != 0;
      else
	o = (result & 0xffffffff80000000) != 0xffffffff80000000;
      cpuSetDReg(dl, (uint32_t)result);
      cpuSetFlagsNZVC(((uint32_t)result) == 0, !!(result & 0x80000000), o, FALSE);
    }
  }
  else // mulu.l
  {
    uint64_t result = ((uint64_t) src1) * ((uint64_t) cpuGetDReg(dl));
    if (extension & 0x0400) // 32bx32b=64b
    {  
      uint32_t dh = extension & 7;
      cpuSetDReg(dl, (uint32_t)result);
      cpuSetDReg(dh, (uint32_t)(result >> 32));
      cpuSetFlagsNZ00New64(result);
    }
    else // 32bx32b=32b
    {
      cpuSetDReg(dl, (uint32_t)result);
      cpuSetFlagsNZVC((uint32_t)result == 0, !!(result & 0x80000000), (result >> 32) != 0, FALSE);
    }
  }
  cpuSetInstructionTime(4);
}

uint8_t cpuMuluTime[256];
uint8_t cpuMulsTime[512];

void cpuCreateMuluTimeTable(void)
{
  for (uint32_t i = 0; i < 256; i++)
  {
    uint32_t j = 0;
    for (uint32_t k = 0; k < 8; k++)
      if (((i>>k) & 1) == 1)
	j++;
    cpuMuluTime[i] = (uint8_t) j*2;
  }
}

void cpuCreateMulsTimeTable(void)
{
  for (uint32_t i = 0; i < 512; i++)
  {
    uint32_t j = 0;
    for (uint32_t k = 0; k < 9; k++)
      if ((((i>>k) & 3) == 1) || (((i>>k) & 3) == 2))
	j++; 
    cpuMulsTime[i] = (uint8_t) j*2;
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
static uint32_t cpuMulsW(uint16_t src2, uint16_t src1, uint32_t eatime)
{
  uint32_t res = (uint32_t)(((int32_t)(int16_t)src2)*((int32_t)(int16_t)src1));
  cpuSetFlagsNZ00NewL(res);
  cpuSetInstructionTime(38 + eatime + cpuMulsTime[(src1 << 1) & 0x1ff] + cpuMulsTime[src1 >> 7]);
  return res;
}

/// <summary>
/// Mulu.w
/// </summary>
static uint32_t cpuMuluW(uint16_t src2, uint16_t src1, uint32_t eatime)
{
  uint32_t res = ((uint32_t)src2)*((uint32_t)src1);
  cpuSetFlagsNZ00NewL(res);
  cpuSetInstructionTime(38 + eatime + cpuMuluTime[src1 & 0xff] + cpuMuluTime[src1 >> 8]);
  return res;
}

/// <summary>
/// Divsw, src1 / src2
/// </summary>
static void cpuDivsW(uint32_t dst, uint16_t src1, uint32_t destination_reg, uint32_t instruction_time)
{
  if (src1 == 0)
  {
    // Alcatraz odyssey assumes that PC in this exception points after the instruction.
    cpuSetFlagsNZVC(TRUE, FALSE, FALSE, FALSE);
    cpuThrowDivisionByZeroException();
  }
  else
  {
    uint32_t result;
    int32_t x = (int32_t) dst;
    int32_t y = (int32_t)(int16_t) src1;
    int32_t res = x / y;
    int32_t rem = x % y;
    if (res > 32767 || res < -32768)
    {
      result = dst;
      cpuSetFlagsNZVC(FALSE, TRUE, TRUE, FALSE);
    }
    else
    {
      result = (rem << 16) | (res & 0xffff);
      cpuSetFlagsNZVC(cpuIsZeroW((uint16_t) res), cpuMsbW((uint16_t) res), FALSE, FALSE);
    }
    cpuSetDReg(destination_reg, result);
    cpuSetInstructionTime(instruction_time);
  }
}

/// <summary>
/// Divuw, src1 / src2
/// </summary>
static void cpuDivuW(uint32_t dst, uint16_t src1, uint32_t destination_reg, uint32_t instruction_time)
{
  if (src1 == 0)
  {
    // Alcatraz odyssey assumes that PC in this exception points after the instruction.
    cpuSetFlagsNZVC((dst & 0xffff0000) == 0, (dst & 0x80000000) == 0x80000000, FALSE, FALSE);
    cpuThrowDivisionByZeroException();
  }
  else
  {
    uint32_t result;
    uint32_t x = dst;
    uint32_t y = (uint32_t) src1;
    uint32_t res = x / y;
    uint32_t rem = x % y;
    if (res > 65535)
    {
      result = dst;
      cpuSetFlagsNZVC(FALSE, TRUE, TRUE, FALSE);
    }
    else
    {
      result = (rem << 16) | (res & 0xffff);
      cpuSetFlagsNZVC(cpuIsZeroW((uint16_t) res), cpuMsbW((uint16_t) res), FALSE, FALSE);
    }
    cpuSetDReg(destination_reg, result);
    cpuSetInstructionTime(instruction_time);
  }
}

static void cpuDivL(uint32_t divisor, uint32_t ext, uint32_t instruction_time)
{
  if (divisor != 0)
  {
    uint32_t dq_reg = (ext>>12) & 7; /* Get operand registers, size and sign */
    uint32_t dr_reg = ext & 7;
    BOOLE size64 = (ext>>10) & 1;
    BOOLE sign = (ext>>11) & 1;
    BOOLE resultsigned = FALSE, restsigned = FALSE;
    uint64_t x, y;
    int64_t x_signed; 

    if (sign)
    { 
      if (size64) x_signed = (int64_t) ((uint64_t) cpuGetDReg(dq_reg)) | (((int64_t) cpuGetDReg(dr_reg))<<32);
      else x_signed = (int64_t) (int32_t) cpuGetDReg(dq_reg);
      int64_t y_signed = (int64_t)(int32_t)divisor;

      if (y_signed < 0)
      {
	y = (uint64_t) -y_signed;
	resultsigned = !resultsigned;
      }
      else y = y_signed;
      if (x_signed < 0)
      {
	x = (uint64_t) -x_signed;
	resultsigned = !resultsigned;
	restsigned = TRUE;
      }
      else x = (uint64_t) x_signed;
    }
    else
    {
      if (size64) x = ((uint64_t) cpuGetDReg(dq_reg)) | (((uint64_t) cpuGetDReg(dr_reg))<<32);
      else x = (uint64_t) cpuGetDReg(dq_reg);
      y = (uint64_t) divisor;
    }

    uint64_t result = x / y;
    uint64_t rest = x % y;

    if (sign)
    {
      if ((resultsigned && result > 0x80000000) || (!resultsigned && result > 0x7fffffff))
      {
	/* Overflow */
	cpuSetFlagsVC(TRUE, FALSE);
      }
      else
      {
	int64_t result_signed = (resultsigned) ? (-(int64_t)result) : ((int64_t)result);
	int64_t rest_signed = (restsigned) ? (-(int64_t)rest) : ((int64_t)rest);
	cpuSetDReg(dr_reg, (uint32_t) rest_signed);
	cpuSetDReg(dq_reg, (uint32_t) result_signed);
	cpuSetFlagsNZ00NewL((uint32_t) result_signed);
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
	cpuSetDReg(dr_reg, (uint32_t) rest);
	cpuSetDReg(dq_reg, (uint32_t) result);
	cpuSetFlagsNZ00NewL((uint32_t) result);
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
static uint8_t cpuLslB(uint8_t dst, uint32_t sh, uint32_t cycles)
{
  uint8_t res;
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
static uint16_t cpuLslW(uint16_t dst, uint32_t sh, uint32_t cycles)
{
  uint16_t res;
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
static uint32_t cpuLslL(uint32_t dst, uint32_t sh, uint32_t cycles)
{
  uint32_t res;
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
static uint8_t cpuLsrB(uint8_t dst, uint32_t sh, uint32_t cycles)
{
  uint8_t res;
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
static uint16_t cpuLsrW(uint16_t dst, uint32_t sh, uint32_t cycles)
{
  uint16_t res;
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
static uint32_t cpuLsrL(uint32_t dst, uint32_t sh, uint32_t cycles)
{
  uint32_t res;
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
static uint8_t cpuAslB(uint8_t dst, uint32_t sh, uint32_t cycles)
{
  int8_t res;
  sh &= 0x3f;
  if (sh == 0)
  {
    cpuSetFlagsShiftZero(cpuIsZeroB(dst), cpuMsbB(dst));
    res = (int8_t) dst;
  }
  else if (sh >= 8)
  {
    res = 0;
    cpuSetFlagsShift(TRUE, FALSE, (sh == 8) ? (dst & 1) : FALSE, dst != 0);
  }
  else
  {
    uint8_t mask = 0xff << (7-sh);
    uint8_t out = dst & mask;
    res = ((int8_t)dst) << sh;

    // Overflow calculation: 
    // 1. The msb of the result and original are different
    // 2. Or the bits shifted out were not all the same as the msb of the original
    BOOLE n_result = cpuMsbB(res);
    BOOLE n_original = cpuMsbB(dst);
    BOOLE msb_changed = (n_result != n_original) || ((n_original) ? (out != mask) : (out != 0));

    cpuSetFlagsShift(cpuIsZeroB(res), n_result, dst & (0x80>>(sh-1)), msb_changed);
  }
  cpuSetInstructionTime(cycles + sh*2);
  return (uint8_t) res;
}

/// <summary>
/// Aslw
/// </summary>
static uint16_t cpuAslW(uint16_t dst, uint32_t sh, uint32_t cycles)
{
  int16_t res;
  sh &= 0x3f;
  if (sh == 0)
  {
    cpuSetFlagsShiftZero(cpuIsZeroW(dst), cpuMsbW(dst));
    res = (int16_t) dst;
  }
  else if (sh >= 16)
  {
    res = 0;
    cpuSetFlagsShift(TRUE, FALSE, (sh == 16) ? (dst & 1) : FALSE, dst != 0);
  }
  else
  {
    uint16_t mask = 0xffff << (15-sh);
    uint16_t out = dst & mask;
    res = ((int16_t)dst) << sh;

    // Overflow calculation: 
    // 1. The msb of the result and original are different
    // 2. Or the bits shifted out were not all the same as the msb of the original
    BOOLE n_result = cpuMsbW(res);
    BOOLE n_original = cpuMsbW(dst);
    BOOLE msb_changed = (n_result != n_original) || ((n_original) ? (out != mask) : (out != 0));

    cpuSetFlagsShift(cpuIsZeroW(res), n_result, dst & (0x8000>>(sh-1)), msb_changed);
  }
  cpuSetInstructionTime(cycles + sh*2);
  return (uint16_t) res;
}

/// <summary>
/// Asll
/// </summary>
static uint32_t cpuAslL(uint32_t dst, uint32_t sh, uint32_t cycles)
{
  int32_t res;
  sh &= 0x3f;
  if (sh == 0)
  {
    cpuSetFlagsShiftZero(cpuIsZeroL(dst), cpuMsbL(dst));
    res = (int32_t) dst;
  }
  else if (sh >= 32)
  {
    res = 0;
    cpuSetFlagsShift(TRUE, FALSE, (sh == 32) ? (dst & 1) : FALSE, dst != 0);
  }
  else
  {
    uint32_t mask = 0xffffffff << (31-sh);
    uint32_t out = dst & mask;
    res = ((int32_t)dst) << sh;

    // Overflow calculation: 
    // 1. The msb of the result and original are different
    // 2. Or the bits shifted out were not all the same as the msb of the original
    BOOLE n_result = cpuMsbL(res);
    BOOLE n_original = cpuMsbL(dst);
    BOOLE msb_changed = (n_result != n_original) || ((n_original) ? (out != mask) : (out != 0));

    cpuSetFlagsShift(cpuIsZeroL(res), n_result, dst & (0x80000000>>(sh-1)), msb_changed);
  }
  cpuSetInstructionTime(cycles + sh*2);
  return (uint32_t) res;
}

/// <summary>
/// Asrb
/// </summary>
static uint8_t cpuAsrB(uint8_t dst, uint32_t sh, uint32_t cycles)
{
  int8_t res;
  sh &= 0x3f;
  if (sh == 0)
  {
    cpuSetFlagsShiftZero(cpuIsZeroB(dst), cpuMsbB(dst));
    res = (int8_t) dst;
  }
  else if (sh >= 8)
  {
    res = (cpuMsbB(dst)) ? 0xff : 0;
    cpuSetFlagsShift(cpuIsZeroB(res), cpuMsbB(res), cpuMsbB(res), FALSE);
  }
  else
  {
    res = ((int8_t)dst) >> sh;
    cpuSetFlagsShift(cpuIsZeroB(res), cpuMsbB(res), dst & (1<<(sh-1)), FALSE);
  }
  cpuSetInstructionTime(cycles + sh*2);
  return (uint8_t) res;
}

/// <summary>
/// Asrw
/// </summary>
static uint16_t cpuAsrW(uint16_t dst, uint32_t sh, uint32_t cycles)
{
  int16_t res;
  sh &= 0x3f;
  if (sh == 0)
  {
    cpuSetFlagsShiftZero(cpuIsZeroW(dst), cpuMsbW(dst));
    res = (int16_t) dst;
  }
  else if (sh >= 16)
  {
    res = (cpuMsbW(dst)) ? 0xffff : 0;
    cpuSetFlagsShift(cpuIsZeroW(res), cpuMsbW(res), cpuMsbW(res), FALSE);
  }
  else
  {
    res = ((int16_t)dst) >> sh;
    cpuSetFlagsShift(cpuIsZeroW(res), cpuMsbW(res), dst & (1<<(sh-1)), FALSE);
  }
  cpuSetInstructionTime(cycles + sh*2);
  return (uint16_t) res;
}

/// <summary>
/// Asrl
/// </summary>
static uint32_t cpuAsrL(uint32_t dst, uint32_t sh, uint32_t cycles)
{
  int32_t res;
  sh &= 0x3f;

  if (sh == 0)
  {
    cpuSetFlagsShiftZero(cpuIsZeroL(dst), cpuMsbL(dst));
    res = (int32_t) dst;
  }
  else if (sh >= 32)
  {
    res = (cpuMsbL(dst)) ? 0xffffffff : 0;
    cpuSetFlagsShift(cpuIsZeroL(res), cpuMsbL(res), cpuMsbL(res), FALSE);
  }
  else
  {
    res = ((int32_t)dst) >> sh;
    cpuSetFlagsShift(cpuIsZeroL(res), cpuMsbL(res), dst & (1<<(sh-1)), FALSE);
  }
  cpuSetInstructionTime(cycles + sh*2);
  return (uint32_t) res;
}

/// <summary>
/// Rolb
/// </summary>
static uint8_t cpuRolB(uint8_t dst, uint32_t sh, uint32_t cycles)
{
  uint8_t res;
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
static uint16_t cpuRolW(uint16_t dst, uint32_t sh, uint32_t cycles)
{
  uint16_t res;
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
static uint32_t cpuRolL(uint32_t dst, uint32_t sh, uint32_t cycles)
{
  uint32_t res;
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
static uint8_t cpuRorB(uint8_t dst, uint32_t sh, uint32_t cycles)
{
  uint8_t res;
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
static uint16_t cpuRorW(uint16_t dst, uint32_t sh, uint32_t cycles)
{
  uint16_t res;
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
static uint32_t cpuRorL(uint32_t dst, uint32_t sh, uint32_t cycles)
{
  uint32_t res;
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
static uint8_t cpuRoxlB(uint8_t dst, uint32_t sh, uint32_t cycles)
{
  BOOLE x = cpuGetFlagX();
  uint8_t res = dst;
  sh &= 0x3f;
  for (uint32_t i = 0; i < sh; ++i)
  {
    BOOLE x_temp = cpuMsbB(res);
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
static uint16_t cpuRoxlW(uint16_t dst, uint32_t sh, uint32_t cycles)
{
  BOOLE x = cpuGetFlagX();
  uint16_t res = dst;
  sh &= 0x3f;
  for (uint32_t i = 0; i < sh; ++i)
  {
    BOOLE x_temp = cpuMsbW(res);
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
static uint32_t cpuRoxlL(uint32_t dst, uint32_t sh, uint32_t cycles)
{
  BOOLE x = cpuGetFlagX();
  uint32_t res = dst;
  sh &= 0x3f;
  for (uint32_t i = 0; i < sh; ++i)
  {
    BOOLE x_temp = cpuMsbL(res);
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
static uint8_t cpuRoxrB(uint8_t dst, uint32_t sh, uint32_t cycles)
{
  BOOLE x = cpuGetFlagX();
  uint8_t res = dst;
  sh &= 0x3f;
  for (uint32_t i = 0; i < sh; ++i)
  {
    BOOLE x_temp = res & 1;
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
static uint16_t cpuRoxrW(uint16_t dst, uint32_t sh, uint32_t cycles)
{
  BOOLE x = cpuGetFlagX();
  uint16_t res = dst;
  sh &= 0x3f;
  for (uint32_t i = 0; i < sh; ++i)
  {
    BOOLE x_temp = res & 1;
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
static uint32_t cpuRoxrL(uint32_t dst, uint32_t sh, uint32_t cycles)
{
  BOOLE x = cpuGetFlagX();
  uint32_t res = dst;
  sh &= 0x3f;
  for (uint32_t i = 0; i < sh; ++i)
  {
    BOOLE x_temp = res & 1;
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
static void cpuStop(uint16_t flags)
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
static void cpuReset(void)
{
  cpuCallResetExceptionFunc();
  cpuSetInstructionTime(132);
}

/// <summary>
/// Rtd
/// </summary>
static void cpuRtd(void)
{
  uint32_t displacement = cpuGetNextWordSignExt();
  cpuInitializeFromNewPC(memoryReadLong(cpuGetAReg(7))); 
  cpuSetAReg(7, cpuGetAReg(7) + 4 + displacement);
  cpuSetInstructionTime(4);
}

static uint32_t cpuRteStackInc[16] = {0, 0, 4, 4, 8, 0, 0, 52, 50, 10, 24, 84, 16, 18, 0, 0};

/// <summary>
/// Rte
/// </summary>
static void cpuRte(void)
{
  if (cpuGetFlagSupervisor())
  {
    BOOLE redo = TRUE;
    do
    {
      uint16_t newsr = memoryReadWord(cpuGetAReg(7));
      cpuSetAReg(7, cpuGetAReg(7) + 2);

      cpuInitializeFromNewPC(memoryReadLong(cpuGetAReg(7)));
      cpuSetAReg(7, cpuGetAReg(7) + 4);

      if (cpuGetModelMajor() > 0)
      {
	uint32_t frame_type = (memoryReadWord(cpuGetAReg(7)) >> 12) & 0xf;
	cpuSetAReg(7, cpuGetAReg(7) + 2);
	cpuSetAReg(7, cpuGetAReg(7) + cpuRteStackInc[frame_type]);
	redo = (frame_type == 1 && cpuGetModelMajor() >= 2 && cpuGetModelMajor() < 6);
      }
      else redo = FALSE;

      cpuUpdateSr(newsr); // Because we can go from isp to msp here.

    } while (redo);

    cpuSetInstructionTime(20);
  }
  else
  {
    cpuThrowPrivilegeViolationException();
  }
}

/// <summary>
/// Swap
/// </summary>
static void cpuSwap(uint32_t reg)
{
  uint32_t res = cpuJoinWordToLong((uint16_t)cpuGetDReg(reg), (uint16_t) (cpuGetDReg(reg) >> 16));
  cpuSetDReg(reg, res);
  cpuSetFlagsNZ00NewL(res);
  cpuSetInstructionTime(4);
}

/// <summary>
/// Unlk
/// </summary>
static void cpuUnlk(uint32_t reg)
{
  cpuSetAReg(7, cpuGetAReg(reg));
  uint32_t value = memoryReadLong(cpuGetAReg(7));
  cpuSetAReg(7, cpuGetAReg(7) + 4);
  cpuSetAReg(reg, value);
  cpuSetInstructionTime(12);
}

/// <summary>
/// Link
/// </summary>
static void cpuLinkW(uint32_t reg)
{
  uint32_t disp = cpuGetNextWordSignExt();
  uint32_t value = cpuGetAReg(reg);
  cpuSetAReg(7, cpuGetAReg(7) - 4);
  memoryWriteLong(value, cpuGetAReg(7));
  cpuSetAReg(reg, cpuGetAReg(7));
  cpuSetAReg(7, cpuGetAReg(7) + disp);
  cpuSetInstructionTime(16);
}

/// <summary>
/// Link.
/// 68020, 68030 and 68040 only.
/// </summary>
static void cpuLinkL(uint32_t reg)
{
  uint32_t disp = cpuGetNextLong();
  uint32_t value = cpuGetAReg(reg);
  cpuSetAReg(7, cpuGetAReg(7) - 4);
  memoryWriteLong(value, cpuGetAReg(7));
  cpuSetAReg(reg, cpuGetAReg(7));
  cpuSetAReg(7, cpuGetAReg(7) + disp);
  cpuSetInstructionTime(4);
}

/// <summary>
/// Ext.w (byte to word)
/// </summary>
static void cpuExtW(uint32_t reg)
{
  uint16_t res = cpuGetDRegByteSignExtWord(reg);
  cpuSetDRegWord(reg, res);
  cpuSetFlagsNZ00NewW(res);
  cpuSetInstructionTime(4);
}

/// <summary>
/// Ext.l (word to long)
/// </summary>
static void cpuExtL(uint32_t reg)
{
  uint32_t res = cpuGetDRegWordSignExtLong(reg);
  cpuSetDReg(reg, res);
  cpuSetFlagsNZ00NewL(res);
  cpuSetInstructionTime(4);
}

/// <summary>
/// ExtB.l (byte to long) (020+)
/// </summary>
static void cpuExtBL(uint32_t reg)
{
  uint32_t res = cpuGetDRegByteSignExtLong(reg);
  cpuSetDReg(reg, res);
  cpuSetFlagsNZ00NewL(res);
  cpuSetInstructionTime(4);
}

/// <summary>
/// Exg Rx,Ry
/// </summary>
static void cpuExgAll(uint32_t reg1_type, uint32_t reg1, uint32_t reg2_type, uint32_t reg2)
{
  uint32_t tmp = cpuGetReg(reg1_type, reg1);
  cpuSetReg(reg1_type, reg1, cpuGetReg(reg2_type, reg2));
  cpuSetReg(reg2_type, reg2, tmp);
  cpuSetInstructionTime(6);
}

/// <summary>
/// Exg Dx,Dy
/// </summary>
static void cpuExgDD(uint32_t reg1, uint32_t reg2)
{
  cpuExgAll(0, reg1, 0, reg2);
}

/// <summary>
/// Exg Ax,Ay
/// </summary>
static void cpuExgAA(uint32_t reg1, uint32_t reg2)
{
  cpuExgAll(1, reg1, 1, reg2);
}

/// <summary>
/// Exg Dx,Ay
/// </summary>
static void cpuExgDA(uint32_t reg1, uint32_t reg2)
{
  cpuExgAll(0, reg1, 1, reg2);
}

/// <summary>
/// Movem.w regs, -(Ax)
/// Order: d0-d7,a0-a7   a7 first
/// </summary>
static void cpuMovemwPre(uint16_t regs, uint32_t reg)
{
  uint32_t cycles = 8;
  uint32_t dstea = cpuGetAReg(reg);
  uint32_t index = 1;
  int32_t j;

  int32_t i = 1;
  for (j = 7; j >= 0; j--)
  {
    if (regs & index)
    {
      dstea -= 2;
      if (cpuGetModelMajor() >= 2 && j == reg)
      {
	memoryWriteWord((uint16_t)dstea, dstea);
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
  cpuSetAReg(reg, dstea);
  cpuSetInstructionTime(cycles);
}

/// <summary>
/// Movem.l regs, -(Ax)
/// Order: d0-d7,a0-a7   a7 first
/// </summary>
static void cpuMovemlPre(uint16_t regs, uint32_t reg)
{
  uint32_t cycles = 8;
  uint32_t dstea = cpuGetAReg(reg);
  uint32_t index = 1;
  int32_t j;

  int32_t i = 1;
  for (j = 7; j >= 0; j--)
  {
    if (regs & index)
    {
      dstea -= 4;
      if (cpuGetModelMajor() >= 2 && j == reg)
      {
	memoryWriteLong(dstea, dstea);
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

  cpuSetAReg(reg, dstea);
  cpuSetInstructionTime(cycles);
}

/// <summary>
/// Movem.w (Ax)+, regs
/// Order: a7-a0,d7-d0   d0 first
/// </summary>
static void cpuMovemwPost(uint16_t regs, uint32_t reg)
{
  uint32_t cycles = 12;
  uint32_t dstea = cpuGetAReg(reg);
  uint32_t index = 1;

  for (uint32_t i = 0; i < 2; ++i)
  {
    for (uint32_t j = 0; j < 8; ++j)
    {
      if (regs & index)
      {
	// Each word, for both data and address registers, is sign-extended before stored.
	cpuSetReg(i, j, (uint32_t)(int32_t)(int16_t) memoryReadWord(dstea));
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
static void cpuMovemlPost(uint16_t regs, uint32_t reg)
{
  uint32_t cycles = 12;
  uint32_t dstea = cpuGetAReg(reg);
  uint32_t index = 1;

  for (uint32_t i = 0; i < 2; ++i)
  {
    for (uint32_t j = 0; j < 8; ++j)
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
static void cpuMovemwEa2R(uint16_t regs, uint32_t ea, uint32_t eacycles)
{
  uint32_t cycles = eacycles;
  uint32_t dstea = ea;
  uint32_t index = 1;

  for (uint32_t i = 0; i < 2; ++i)
  {
    for (uint32_t j = 0; j < 8; ++j)
    {
      if (regs & index)
      {
	// Each word, for both data and address registers, is sign-extended before stored.
	cpuSetReg(i, j, (uint32_t)(int32_t)(int16_t) memoryReadWord(dstea));
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
static void cpuMovemlEa2R(uint16_t regs, uint32_t ea, uint32_t eacycles)
{
  uint32_t cycles = eacycles;
  uint32_t dstea = ea;
  uint32_t index = 1;

  for (uint32_t i = 0; i < 2; ++i)
  {
    for (uint32_t j = 0; j < 8; ++j)
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
static void cpuMovemwR2Ea(uint16_t regs, uint32_t ea, uint32_t eacycles)
{
  uint32_t cycles = eacycles;
  uint32_t dstea = ea;
  uint32_t index = 1;

  for (uint32_t i = 0; i < 2; ++i)
  {
    for (uint32_t j = 0; j < 8; ++j)
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
static void cpuMovemlR2Ea(uint16_t regs, uint32_t ea, uint32_t eacycles)
{
  uint32_t cycles = eacycles;
  uint32_t dstea = ea;
  uint32_t index = 1;
  uint32_t i, j;

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
static void cpuTrap(uint32_t vectorno)
{
  // PC written to the exception frame must be pc + 2, the address of the next instruction.
  cpuThrowTrapException(vectorno);
}

/// <summary>
/// move.l  Ax,Usp
/// </summary>
static void cpuMoveToUsp(uint32_t reg)
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
static void cpuMoveFromUsp(uint32_t reg)
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
static void cpuCmpMB(uint32_t regx, uint32_t regy)
{
  uint8_t src = memoryReadByte(cpuEA03(regy, 1));
  uint8_t dst = memoryReadByte(cpuEA03(regx, 1));
  uint8_t res = dst - src;
  cpuSetFlagsCmp(cpuIsZeroB(res), cpuMsbB(res), cpuMsbB(dst), cpuMsbB(src));
  cpuSetInstructionTime(12);
}

/// <summary>
/// cmp.w (Ay)+,(Ax)+
/// </summary>
static void cpuCmpMW(uint32_t regx, uint32_t regy)
{
  uint16_t src = memoryReadWord(cpuEA03(regy, 2));
  uint16_t dst = memoryReadWord(cpuEA03(regx, 2));
  uint16_t res = dst - src;
  cpuSetFlagsCmp(cpuIsZeroW(res), cpuMsbW(res), cpuMsbW(dst), cpuMsbW(src));
  cpuSetInstructionTime(12);
}

/// <summary>
/// cmp.l (Ay)+,(Ax)+
/// </summary>
static void cpuCmpML(uint32_t regx, uint32_t regy)
{
  uint32_t src = memoryReadLong(cpuEA03(regy, 4));
  uint32_t dst = memoryReadLong(cpuEA03(regx, 4));
  uint32_t res = dst - src;
  cpuSetFlagsCmp(cpuIsZeroL(res), cpuMsbL(res), cpuMsbL(dst), cpuMsbL(src));
  cpuSetInstructionTime(20);
}

/// <summary>
/// chk.w ea, dn  (upper bound, value to test)
/// Undocumented features:
/// Z is set from the register operand,
/// V and C is always cleared.
/// </summary>
static void cpuChkW(uint16_t value, uint16_t ub, uint32_t instructionTime)
{
  cpuSetFlagZ(value == 0);
  cpuClearFlagsVC();
  if (((int16_t)value) < 0)
  {
    cpuSetFlagN(TRUE);
    cpuThrowChkException();
  }
  else if (((int16_t)value) > ((int16_t)ub))
  {
    cpuSetFlagN(FALSE);
    cpuThrowChkException();
  }
  else
  {
    cpuSetFlagN(FALSE);
    cpuSetInstructionTime(instructionTime);
  }
}

/// <summary>
/// chk.l Dx, ea
/// 68020+
/// Undocumented features:
/// Z is set from the register operand,
/// V and C is always cleared.
/// </summary>
static void cpuChkL(uint32_t value, uint32_t ub, uint32_t instructionTime)
{
  cpuSetFlagZ(value == 0);
  cpuClearFlagsVC();
  if (((int32_t)value) < 0)
  {
    cpuSetFlagN(TRUE);
    cpuThrowChkException();
  }
  else if (((int32_t)value) > ((int32_t)ub))
  {
    cpuSetFlagN(FALSE);
    cpuThrowChkException();
  }
  else
  {
    cpuSetInstructionTime(instructionTime);
  }
}

/// <summary>
/// addx.b dx,dy
/// </summary>
static uint8_t cpuAddXB(uint8_t dst, uint8_t src)
{
  uint8_t res = dst + src + ((cpuGetFlagX()) ? 1:0);
  cpuSetFlagsAddX(cpuIsZeroB(res), cpuMsbB(res), cpuMsbB(dst), cpuMsbB(src));
  return res;
}

/// <summary>
/// addx.w dx,dy
/// </summary>
static uint16_t cpuAddXW(uint16_t dst, uint16_t src)
{
  uint16_t res = dst + src + ((cpuGetFlagX()) ? 1:0);
  cpuSetFlagsAddX(cpuIsZeroW(res), cpuMsbW(res), cpuMsbW(dst), cpuMsbW(src));
  return res;
}

/// <summary>
/// addx.l dx,dy
/// </summary>
static uint32_t cpuAddXL(uint32_t dst, uint32_t src)
{
  uint32_t res = dst + src + ((cpuGetFlagX()) ? 1:0);
  cpuSetFlagsAddX(cpuIsZeroL(res), cpuMsbL(res), cpuMsbL(dst), cpuMsbL(src));
  return res;
}

/// <summary>
/// subx.b dx,dy
/// </summary>
static uint8_t cpuSubXB(uint8_t dst, uint8_t src)
{
  uint8_t res = dst - src - ((cpuGetFlagX()) ? 1:0);
  cpuSetFlagsSubX(cpuIsZeroB(res), cpuMsbB(res), cpuMsbB(dst), cpuMsbB(src));
  return res;
}

/// <summary>
/// subx.w dx,dy
/// </summary>
static uint16_t cpuSubXW(uint16_t dst, uint16_t src)
{
  uint16_t res = dst - src - ((cpuGetFlagX()) ? 1:0);
  cpuSetFlagsSubX(cpuIsZeroW(res), cpuMsbW(res), cpuMsbW(dst), cpuMsbW(src));
  return res;
}

/// <summary>
/// subx.l dx,dy
/// </summary>
static uint32_t cpuSubXL(uint32_t dst, uint32_t src)
{
  uint32_t res = dst - src - ((cpuGetFlagX()) ? 1:0);
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
static uint8_t cpuAbcdB(uint8_t dst, uint8_t src)
{
  uint8_t xflag = (cpuGetFlagX()) ? 1 : 0;
  uint16_t low_nibble = (dst & 0xf) + (src & 0xf) + xflag;
  uint16_t high_nibble = ((uint16_t)(dst & 0xf0)) + ((uint16_t)(src & 0xf0));
  uint16_t result_unadjusted = low_nibble + high_nibble;
  uint16_t result_bcd = result_unadjusted;

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
  return (uint8_t)result_bcd;
}

/// <summary>
/// sbcd.b src,dst
/// nbcd.b src   (set dst=0)
/// Implemented using the information from:
///     68000 Undocumented Behavior Notes
///                Fourth Edition
///    by Bart Trzynadlowski, May 12, 2003
/// </summary>
static uint8_t cpuSbcdB(uint8_t dst, uint8_t src)
{
  uint16_t xflag = (cpuGetFlagX()) ? 1:0;
  uint16_t result_plain_binary = (uint16_t)dst - (uint16_t)src - xflag;
  uint16_t low_nibble = (uint16_t)(dst & 0xf) - (uint16_t)(src & 0xf) - xflag;
  uint16_t high_nibble = ((uint16_t)(dst & 0xf0)) - ((uint16_t)(src & 0xf0));
  uint16_t result_unadjusted = low_nibble + high_nibble;
  uint16_t result_bcd = result_unadjusted;

  if ((int16_t)result_plain_binary < 0)
  {
    result_bcd -= 0x60;
  }

  if (((int16_t)low_nibble) < 0)
  {
    result_bcd -= 6;
    result_plain_binary -= 6;
  }

  BOOLE borrow = ((int16_t)result_plain_binary < 0);
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
  return (uint8_t) result_bcd;
}

/// <summary>
/// nbcd.b dst
/// </summary>
static uint8_t cpuNbcdB(uint8_t dst)
{
  return cpuSbcdB(0, dst);
}

// Bit field functions
typedef struct cpuBfData
{
  int32_t offset;
  uint32_t width;
  uint32_t normalized_offset;

  uint32_t base_address;
  int32_t base_address_byte_offset;
  uint32_t base_address_byte_count;

  uint32_t field;
  uint64_t field_mask;
  uint32_t dn;
  uint64_t field_memory;
} cpuBfData;

static int32_t cpuGetBfOffset(uint16_t ext, bool offsetIsDataRegister)
{
  int32_t offset = (ext >> 6) & 0x1f;
  if (offsetIsDataRegister)
  {
    offset = (int32_t) cpuGetDReg(offset & 7);
  }
  return offset;
}

static uint32_t cpuGetBfWidth(uint16_t ext, bool widthIsDataRegister)
{
  uint32_t width = (ext & 0x1f);
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

static uint32_t cpuRotateLeft(uint32_t value, uint32_t shift)
{
  return (value << shift) | (value >> (32 - shift));
}

static uint32_t cpuRotateRight(uint32_t value, uint32_t shift)
{
  return (value >> shift) | (value << (32 - shift));
}

static void cpuSetBfField(cpuBfData *bf_data, uint32_t ea_or_reg, bool has_ea)
{
  if (has_ea)
  {
    uint32_t shift = bf_data->base_address_byte_count*8 - bf_data->normalized_offset - bf_data->width;
    uint64_t field_value = (bf_data->field_memory & ~(bf_data->field_mask << shift)) | (((uint64_t)bf_data->field) << shift);
    uint32_t address = bf_data->base_address + bf_data->base_address_byte_offset;

    for (int i = bf_data->base_address_byte_count - 1; i >= 0; --i)
    {
      uint8_t field_byte = (field_value >> (i*8)) & 0xff;
      memoryWriteByte(field_byte, address);
      ++address;
    }
  }
  else
  {
    // Field from Dn wraps around if offset + width > 32
    uint32_t offsetPlusWidth = (bf_data->offset & 0x1f) + bf_data->width;
    if (offsetPlusWidth > 32)
    {
      uint32_t rotate = offsetPlusWidth - 32;
      uint32_t reg_value = (cpuGetDReg(ea_or_reg) & ~cpuRotateRight((uint32_t)bf_data->field_mask, rotate)) | cpuRotateRight(bf_data->field, rotate);
      cpuSetDReg(ea_or_reg, reg_value);
    }
    else
    {
      uint32_t reg_shift = 32 - offsetPlusWidth;
      uint32_t reg_value = (cpuGetDReg(ea_or_reg) & ~(bf_data->field_mask << reg_shift)) | (bf_data->field << reg_shift);
      cpuSetDReg(ea_or_reg, reg_value);
    }
  }
}

void cpuBfDecodeExtWordAndGetField(cpuBfData *bf_data, uint32_t ea_or_reg, bool has_dn, bool has_ea, uint16_t ext)
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

    uint32_t field = 0;
    uint64_t field_memory = 0;
    uint32_t address = bf_data->base_address + bf_data->base_address_byte_offset;
    uint32_t shift = (8 - bf_data->normalized_offset - bf_data->width) & 7;
    for (int i = bf_data->base_address_byte_count - 1; i >= 0; --i)
    {
      uint64_t value = ((uint64_t)memoryReadByte(address)) << (8 * i);
      field_memory |= value;
      field |= (value >> shift);
      ++address;
    }

    bf_data->field_memory = field_memory;
    bf_data->field = field;
  }
  else
  {
    // Field from Dn wraps around if offset + width > 32
    const uint32_t offsetPlusWidth = (bf_data->offset & 0x1f) + bf_data->width;
    if (offsetPlusWidth > 32)
    {
      bf_data->field = cpuRotateLeft(cpuGetDReg(ea_or_reg), offsetPlusWidth - 32);
    }
    else
    {
      bf_data->field = cpuGetDReg(ea_or_reg) >> (32 - offsetPlusWidth);
    }
  }
  bf_data->field &= bf_data->field_mask;
}

/// <summary>
/// bfchg common logic
/// </summary>
static void cpuBfChgCommon(uint32_t ea_or_reg, bool has_ea, uint16_t ext)
{
  cpuBfData bf_data;
  cpuBfDecodeExtWordAndGetField(&bf_data, ea_or_reg, false, has_ea, ext);
  cpuSetFlagsNZVC(bf_data.field == 0, bf_data.field & (1 << (bf_data.width - 1)), FALSE, FALSE);

  bf_data.field = (~bf_data.field) & (uint32_t)bf_data.field_mask;

  cpuSetBfField(&bf_data, ea_or_reg, has_ea);
}

// bfchg dx {offset:width}
static void cpuBfChgReg(uint32_t regno, uint16_t ext)
{
  cpuBfChgCommon(regno, false, ext);
}

/// <summary>
/// bfchg ea {offset:width}
/// </summary>
static void cpuBfChgEa(uint32_t ea, uint16_t ext)
{
  cpuBfChgCommon(ea, true, ext);
}

/// <summary>
/// bfclr common logic
/// </summary>
static void cpuBfClrCommon(uint32_t ea_or_reg, bool has_ea, uint16_t ext)
{
  cpuBfData bf_data;
  cpuBfDecodeExtWordAndGetField(&bf_data, ea_or_reg, false, has_ea, ext);

  cpuSetFlagsNZVC(bf_data.field == 0, bf_data.field & (1 << (bf_data.width - 1)), FALSE, FALSE);

  bf_data.field = 0;

  cpuSetBfField(&bf_data, ea_or_reg, has_ea);
}

/// <summary>
/// bfclr dx {offset:width}
/// </summary>
static void cpuBfClrReg(uint32_t regno, uint16_t ext)
{
  cpuBfClrCommon(regno, false, ext);
}

/// <summary>
/// bfclr ea {offset:width}
/// </summary>
static void cpuBfClrEa(uint32_t ea, uint16_t ext)
{
  cpuBfClrCommon(ea, true, ext);
}

/// <summary>
/// bfexts common logic
/// </summary>
static void cpuBfExtsCommon(uint32_t ea_or_reg, bool has_ea, uint16_t ext)
{
  cpuBfData bf_data;
  cpuBfDecodeExtWordAndGetField(&bf_data, ea_or_reg, true, has_ea, ext);

  BOOLE n_flag = bf_data.field & (1 << (bf_data.width - 1));

  cpuSetFlagsNZVC(bf_data.field == 0, n_flag, FALSE, FALSE);

  if (n_flag)
  {
    bf_data.field = (uint32_t)((~bf_data.field_mask) | bf_data.field);
  }
  // Destination is always Dn
  cpuSetDReg(bf_data.dn, bf_data.field);
}

/// <summary>
/// bfexts dx {offset:width}, Dn
/// </summary>
static void cpuBfExtsReg(uint32_t regno, uint16_t ext)
{
  cpuBfExtsCommon(regno, false, ext);
}

/// <summary>
/// bfexts ea {offset:width}, Dn
/// </summary>
static void cpuBfExtsEa(uint32_t ea, uint16_t ext)
{
  cpuBfExtsCommon(ea, true, ext);
}

/// <summary>
/// bfextu ea {offset:width}, Dn
/// </summary>
static void cpuBfExtuCommon(uint32_t ea_or_reg, bool has_ea, uint16_t ext)
{
  cpuBfData bf_data;
  cpuBfDecodeExtWordAndGetField(&bf_data, ea_or_reg, true, has_ea, ext);
  cpuSetFlagsNZVC(bf_data.field == 0, bf_data.field & (1 << (bf_data.width - 1)), FALSE, FALSE);
  // Destination is always Dn
  cpuSetDReg(bf_data.dn, bf_data.field);
}

/// <summary>
/// bfextu dx {offset:width}, Dn
/// </summary>
static void cpuBfExtuReg(uint32_t regno, uint16_t ext)
{
  cpuBfExtuCommon(regno, false, ext);
}

/// <summary>
/// bfextu ea {offset:width}, Dn
/// </summary>
static void cpuBfExtuEa(uint32_t ea, uint16_t ext)
{
  cpuBfExtuCommon(ea, true, ext);
}

/// <summary>
/// bfffo common logic
/// </summary>
static void cpuBfFfoCommon(uint32_t val, bool has_ea, uint16_t ext)
{
  cpuBfData bf_data;
  uint32_t i;
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
static void cpuBfFfoReg(uint32_t regno, uint16_t ext)
{
  cpuBfFfoCommon(regno, false, ext);
}

/// <summary>
/// bfffo ea {offset:width}, Dn
/// </summary>
static void cpuBfFfoEa(uint32_t ea, uint16_t ext)
{
  cpuBfFfoCommon(ea, true, ext);
}

/// <summary>
/// bfins common logic
/// </summary>
static void cpuBfInsCommon(uint32_t ea_or_reg, bool has_ea, uint16_t ext)
{
  cpuBfData bf_data;
  cpuBfDecodeExtWordAndGetField(&bf_data, ea_or_reg, true, has_ea, ext);

  bf_data.field = cpuGetDReg(bf_data.dn) & bf_data.field_mask;

  // Flags are set according to the inserted value
  cpuSetFlagsNZVC(bf_data.field == 0, bf_data.field & (1 << (bf_data.width - 1)), FALSE, FALSE);

  cpuSetBfField(&bf_data, ea_or_reg, has_ea);
}

/// <summary>
/// bfins Dn, ea {offset:width}
/// </summary>
static void cpuBfInsReg(uint32_t regno, uint16_t ext)
{
  cpuBfInsCommon(regno, false, ext);
}

/// <summary>
/// bfins Dn, ea {offset:width}
/// </summary>
static void cpuBfInsEa(uint32_t ea, uint16_t ext)
{
  cpuBfInsCommon(ea, true, ext);
}

/// <summary>
/// bfset common logic
/// </summary>
static void cpuBfSetCommon(uint32_t ea_or_reg, bool has_ea, uint16_t ext)
{
  cpuBfData bf_data;
  cpuBfDecodeExtWordAndGetField(&bf_data, ea_or_reg, false, has_ea, ext);
  cpuSetFlagsNZVC(bf_data.field == 0, bf_data.field & (1 << (bf_data.width - 1)), FALSE, FALSE);

  bf_data.field = (uint32_t)bf_data.field_mask;

  cpuSetBfField(&bf_data, ea_or_reg, has_ea);
}

/// <summary>
/// bfset dx {offset:width}
/// </summary>
static void cpuBfSetReg(uint32_t regno, uint16_t ext)
{
  cpuBfSetCommon(regno, false, ext);
}

/// <summary>
/// bfset ea {offset:width}
/// </summary>
static void cpuBfSetEa(uint32_t ea, uint16_t ext)
{
  cpuBfSetCommon(ea, true, ext);
}

/// <summary>
/// bftst common logic
/// </summary>
static void cpuBfTstCommon(uint32_t ea_or_reg, bool has_ea, uint16_t ext)
{
  cpuBfData bf_data;
  cpuBfDecodeExtWordAndGetField(&bf_data, ea_or_reg, false, has_ea, ext);
  cpuSetFlagsNZVC(bf_data.field == 0, bf_data.field & (1 << (bf_data.width - 1)), FALSE, FALSE);
}

/// <summary>
/// bftst dx {offset:width}
/// </summary>
static void cpuBfTstReg(uint32_t regno, uint16_t ext)
{
  cpuBfTstCommon(regno, false, ext);
}

/// <summary>
/// bftst ea {offset:width}
/// </summary>
static void cpuBfTstEa(uint32_t ea, uint16_t ext)
{
  cpuBfTstCommon(ea, true, ext);
}

/// <summary>
/// movep.w (d16, Ay), Dx
/// </summary>
static void cpuMovepWReg(uint32_t areg, uint32_t dreg)
{
  uint32_t ea = cpuGetAReg(areg) + cpuGetNextWordSignExt();
  cpuSetDRegWord(dreg, cpuJoinByteToWord(memoryReadByte(ea), memoryReadByte(ea + 2)));
  cpuSetInstructionTime(16);
}

/// <summary>
/// movep.l (d16, Ay), Dx
/// </summary>
static void cpuMovepLReg(uint32_t areg, uint32_t dreg)
{
  uint32_t ea = cpuGetAReg(areg) + cpuGetNextWordSignExt();
  cpuSetDReg(dreg, cpuJoinByteToLong(memoryReadByte(ea), memoryReadByte(ea + 2), memoryReadByte(ea + 4), memoryReadByte(ea + 6)));
  cpuSetInstructionTime(24);
}

/// <summary>
/// movep.w Dx, (d16, Ay)
/// </summary>
static void cpuMovepWEa(uint32_t areg, uint32_t dreg)
{
  uint32_t ea = cpuGetAReg(areg) + cpuGetNextWordSignExt();
  memoryWriteByte((uint8_t) (cpuGetDReg(dreg) >> 8), ea);
  memoryWriteByte(cpuGetDRegByte(dreg), ea + 2);
  cpuSetInstructionTime(16);
}

/// <summary>
/// movep.l Dx, (d16, Ay)
/// </summary>
static void cpuMovepLEa(uint32_t areg, uint32_t dreg)
{
  uint32_t ea = cpuGetAReg(areg) + cpuGetNextWordSignExt();
  memoryWriteByte((uint8_t)(cpuGetDReg(dreg) >> 24), ea);
  memoryWriteByte((uint8_t)(cpuGetDReg(dreg) >> 16), ea + 2);
  memoryWriteByte((uint8_t)(cpuGetDReg(dreg) >> 8), ea + 4);
  memoryWriteByte(cpuGetDRegByte(dreg), ea + 6);
  cpuSetInstructionTime(24);
}

/// <summary>
/// pack Dx, Dy, #adjustment
/// </summary>
static void cpuPackReg(uint32_t xreg, uint32_t yreg)
{
  uint16_t adjustment = cpuGetNextWord();
  uint16_t src = cpuGetDRegWord(xreg) + adjustment;
  cpuSetDRegByte(yreg, (uint8_t) (((src >> 4) & 0xf0) | (src & 0xf)));
  cpuSetInstructionTime(4);
}

/// <summary>
/// pack -(Ax), -(Ay), #adjustment
/// </summary>
static void cpuPackEa(uint32_t xreg, uint32_t yreg)
{
  uint16_t adjustment = cpuGetNextWord();
  uint16_t src = memoryReadWord(cpuEA04(xreg, 2));
  uint16_t result = src + adjustment;
  memoryWriteByte((uint8_t) (((result >> 4) & 0xf0) | (result & 0xf)), cpuEA04(yreg, 1));
  cpuSetInstructionTime(4);
}

/// <summary>
/// unpk Dx, Dy, #adjustment
/// </summary>
static void cpuUnpkReg(uint32_t xreg, uint32_t yreg)
{
  uint16_t adjustment = cpuGetNextWord();
  uint8_t b1 = cpuGetDRegByte(xreg);
  uint16_t result = ((((uint16_t)(b1 & 0xf0)) << 4) | ((uint16_t)(b1 & 0xf))) + adjustment;
  cpuSetDRegWord(yreg, result);
  cpuSetInstructionTime(4);
}

/// <summary>
/// unpk -(Ax), -(Ay), #adjustment
/// </summary>
static void cpuUnpkEa(uint32_t xreg, uint32_t yreg)
{
  uint16_t adjustment = cpuGetNextWord();
  uint8_t b1 = memoryReadByte(cpuEA04(xreg, 1));
  uint16_t result = ((((uint16_t)(b1 & 0xf0)) << 4) | ((uint16_t)(b1 & 0xf))) + adjustment;
  memoryWriteWord(result, cpuEA04(yreg, 2));
  cpuSetInstructionTime(4);
}

/// <summary>
/// movec
/// </summary>
static void cpuMoveCFrom(void)
{
  if (cpuGetFlagSupervisor())
  {
    uint16_t extension = (uint16_t) cpuGetNextWord();
    uint32_t da = (extension >> 15) & 1;
    uint32_t regno = (extension >> 12) & 7;
    uint32_t ctrl_regno = extension & 0xfff;
    if (cpuGetModelMajor() == 1)
    {
      switch (ctrl_regno)
      {
	case 0x000: cpuSetReg(da, regno, cpuGetSfc()); break;
	case 0x001: cpuSetReg(da, regno, cpuGetDfc()); break;
	case 0x800: cpuSetReg(da, regno, cpuGetUspDirect()); break; // In supervisor mode, usp is up to date.
	case 0x801: cpuSetReg(da, regno, cpuGetVbr()); break;
	default:  cpuThrowIllegalInstructionException(FALSE); return;	  // Illegal instruction
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
	default:  cpuThrowIllegalInstructionException(FALSE); return;	  // Illegal instruction
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
	default:  cpuThrowIllegalInstructionException(FALSE); return;	  // Illegal instruction
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
static void cpuMoveCTo(void)
{
  if (cpuGetFlagSupervisor())
  {
    uint16_t extension = (uint16_t) cpuGetNextWord();
    uint32_t da = (extension >> 15) & 1;
    uint32_t regno = (extension >> 12) & 7;
    uint32_t ctrl_regno = extension & 0xfff;
    if (cpuGetModelMajor() == 1)
    {
      switch (ctrl_regno)
      {
	case 0x000: cpuSetSfc(cpuGetReg(da, regno) & 7); break;
	case 0x001: cpuSetDfc(cpuGetReg(da, regno) & 7); break;
	case 0x800: cpuSetUspDirect(cpuGetReg(da, regno)); break;
	case 0x801: cpuSetVbr(cpuGetReg(da, regno)); break;
	default:  cpuThrowIllegalInstructionException(FALSE); return;	  // Illegal instruction
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
	default:  cpuThrowIllegalInstructionException(FALSE); return;	  // Illegal instruction
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
	default:  cpuThrowIllegalInstructionException(FALSE); return;	  // Illegal instruction
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
static void cpuMoveSB(uint32_t ea, uint16_t extension)
{
  if (cpuGetFlagSupervisor())
  {
    uint32_t da = (extension >> 15) & 1;
    uint32_t regno = (extension >> 12) & 7;
    if (extension & 0x0800) // From Rn to ea (in dfc)
    {
      memoryWriteByte((uint8_t)cpuGetReg(da, regno), ea);
    }
    else  // From ea to Rn (in sfc)
    {
      uint8_t data = memoryReadByte(ea);
      if (da == 0)
      {
	cpuSetDRegByte(regno, data);
      }
      else
      {
	cpuSetAReg(regno, (uint32_t)(int32_t)(int8_t) data);
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
static void cpuMoveSW(uint32_t ea, uint16_t extension)
{
  if (cpuGetFlagSupervisor())
  {
    uint32_t da = (extension >> 15) & 1;
    uint32_t regno = (extension >> 12) & 7;
    if (extension & 0x0800) // From Rn to ea (in dfc)
    {
      memoryWriteWord((uint16_t)cpuGetReg(da, regno), ea);
    }
    else  // From ea to Rn (in sfc)
    {
      uint16_t data = memoryReadWord(ea);
      if (da == 0)
      {
	cpuSetDRegWord(regno, data);
      }
      else
      {
	cpuSetAReg(regno, (uint32_t)(int32_t)(int16_t) data);
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
static void cpuMoveSL(uint32_t ea, uint16_t extension)
{
  if (cpuGetFlagSupervisor())
  {
    uint32_t da = (extension >> 15) & 1;
    uint32_t regno = (extension >> 12) & 7;
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
static void cpuTrapcc(uint32_t cc)
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
static void cpuTrapccW(uint32_t cc)
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
static void cpuTrapccL(uint32_t cc)
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
static void cpuCasB(uint32_t ea, uint16_t extension)
{
  uint8_t dst = memoryReadByte(ea);
  uint32_t cmp_regno = extension & 7;
  uint8_t res = dst - cpuGetDRegByte(cmp_regno);

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
static void cpuCasW(uint32_t ea, uint16_t extension)
{
  uint16_t dst = memoryReadWord(ea);
  uint32_t cmp_regno = extension & 7;
  uint16_t res = dst - cpuGetDRegWord(cmp_regno);

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
static void cpuCasL(uint32_t ea, uint16_t extension)
{
  uint32_t dst = memoryReadLong(ea);
  uint32_t cmp_regno = extension & 7;
  uint32_t res = dst - cpuGetDReg(cmp_regno);

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
static void cpuCas2W(void)
{
  uint16_t extension1 = cpuGetNextWord();
  uint16_t extension2 = cpuGetNextWord();
  uint32_t ea1 = cpuGetReg(extension1 >> 15, (extension1 >> 12) & 7);
  uint32_t ea2 = cpuGetReg(extension2 >> 15, (extension2 >> 12) & 7);
  uint16_t dst1 = memoryReadWord(ea1);
  uint16_t dst2 = memoryReadWord(ea2);
  uint32_t cmp1_regno = extension1 & 7;
  uint32_t cmp2_regno = extension2 & 7;
  uint16_t res1 = dst1 - cpuGetDRegWord(cmp1_regno);
  uint16_t res2 = dst2 - cpuGetDRegWord(cmp2_regno);

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
    if (cmp1_regno != cmp2_regno)
    {
      cpuSetDRegWord(cmp2_regno, dst2);
    }
  }
  cpuSetInstructionTime(4);
}

/// <summary>
/// cas2.l Dc1:Dc2,Du1:Du2,(Rn1):(Rn2)
/// </summary>
static void cpuCas2L(void)
{
  uint16_t extension1 = cpuGetNextWord();
  uint16_t extension2 = cpuGetNextWord();
  uint32_t ea1 = cpuGetReg(extension1 >> 15, (extension1 >> 12) & 7);
  uint32_t ea2 = cpuGetReg(extension2 >> 15, (extension2 >> 12) & 7);
  uint32_t dst1 = memoryReadLong(ea1);
  uint32_t dst2 = memoryReadLong(ea2);
  uint32_t cmp1_regno = extension1 & 7;
  uint32_t cmp2_regno = extension2 & 7;
  uint32_t res1 = dst1 - cpuGetDReg(cmp1_regno);
  uint32_t res2 = dst2 - cpuGetDReg(cmp2_regno);

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
    if (cmp1_regno != cmp2_regno)
    {
      cpuSetDReg(cmp2_regno, dst2);
    }
  }
  cpuSetInstructionTime(4);
}

/// <summary>
/// Common code for chk2 ea, Rn / cmp2 ea, Rn
/// </summary>
static void cpuChk2Cmp2(uint32_t lb, uint32_t ub, uint32_t val, BOOLE is_chk2)
{
  BOOLE z = (val == lb || val == ub);
  BOOLE c = ((lb <= ub) && (val < lb || val > ub)) || ((lb > ub) && (val < lb) && (val > ub));
  BOOLE v = (ub < lb) && (val > ub) && (val < lb);
  cpuSetFlagZ(z);
  cpuSetFlagC(c);
  cpuSetFlagV(v);
  cpuSetInstructionTime(4);
  if (is_chk2 && c)
  {
    cpuThrowChkException();
  }
}

/// <summary>
/// chk2.b ea, Rn / cmp2.b ea, Rn
/// </summary>
static void cpuChkCmp2B(uint32_t ea, uint16_t extension)
{
  uint32_t da = (uint32_t) (extension >> 15);
  uint32_t rn = (uint32_t) (extension >> 12) & 7;
  BOOLE is_chk2 = (extension & 0x0800);
  if (da == 1)
  {
    cpuChk2Cmp2((uint32_t)(int32_t)(int8_t)memoryReadByte(ea), (uint32_t)(int32_t)(int8_t)memoryReadByte(ea + 1), cpuGetAReg(rn), is_chk2);
  }
  else
  {
    cpuChk2Cmp2((uint32_t)memoryReadByte(ea), (uint32_t)memoryReadByte(ea + 1), (uint32_t)(uint8_t)cpuGetDReg(rn), is_chk2);
  }
}

/// <summary>
/// chk2.w ea, Rn / cmp2.w ea, Rn
/// </summary>
static void cpuChkCmp2W(uint32_t ea, uint16_t extension)
{
  uint32_t da = (uint32_t) (extension >> 15);
  uint32_t rn = (uint32_t) (extension >> 12) & 7;
  BOOLE is_chk2 = (extension & 0x0800);
  if (da == 1)
  {
    cpuChk2Cmp2((uint32_t)(int32_t)(int16_t)memoryReadWord(ea), (uint32_t)(int32_t)(int16_t)memoryReadWord(ea + 1), cpuGetAReg(rn), is_chk2);
  }
  else
  {
    cpuChk2Cmp2((uint32_t)memoryReadWord(ea), (uint32_t)memoryReadWord(ea + 2), (uint32_t)(uint16_t)cpuGetDReg(rn), is_chk2);
  }
}

/// <summary>
/// chk2.l ea, Rn / cmp2.l ea, Rn
/// </summary>
static void cpuChkCmp2L(uint32_t ea, uint16_t extension)
{
  uint32_t da = (uint32_t) (extension >> 15);
  uint32_t rn = (uint32_t) (extension >> 12) & 7;
  BOOLE is_chk2 = (extension & 0x0800);
  cpuChk2Cmp2(memoryReadLong(ea), memoryReadLong(ea + 4), cpuGetReg(da, rn), is_chk2);
}

/// <summary>
/// callm
/// Since this is a coprocessor instruction, this is NOP.
/// This will likely fail, but anything we do here will be wrong anyhow.
/// </summary>
static void cpuCallm(uint32_t ea, uint16_t extension)
{
  cpuSetInstructionTime(4);
}

/// <summary>
/// rtm
/// Since this is a coprocessor instruction, this is NOP.
/// This will likely fail, but anything we do here will be wrong anyhow.
/// </summary>
static void cpuRtm(uint32_t da, uint32_t regno)
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
static void cpuPflush030(uint32_t ea, uint16_t extension)
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
      uint32_t mode = (extension >> 10) & 7;
      uint32_t mask = (extension >> 5) & 7;
      uint32_t fc = extension & 0x1f;
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
static void cpuPflush040(uint32_t opmode, uint32_t regno)
{
  if (cpuGetFlagSupervisor())
  {
    if (cpuGetModelMajor() != 2)	// This is NOP on 68EC040
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
static void cpuPtest040(uint32_t rw, uint32_t regno)
{
  if (cpuGetFlagSupervisor())
  {
    if (cpuGetModelMajor() != 2)	// This is NOP on 68EC040
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
  for (uint32_t opcode = 0; opcode < 65536; opcode++)
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

uint32_t irq_arrival_time = -1;
extern uint32_t busGetCycle();

uint32_t cpuExecuteInstruction(void)
{
  if (cpuGetRaiseInterrupt())
  {
    cpuSetUpInterrupt(cpuGetRaiseInterruptLevel());
    cpuCheckPendingInterrupts();
    return 44;
  }
  else
  {
    uint32_t oldSr = cpuGetSR();
    cpuSetInstructionAborted(false);

#ifdef CPU_INSTRUCTION_LOGGING
    cpuCallInstructionLoggingFunc();
#endif

    cpuSetOriginalPC(cpuGetPC()); // Store pc and opcode for exception logging
    uint16_t opcode = cpuGetNextWord();

#ifdef CPU_INSTRUCTION_LOGGING
    cpuSetCurrentOpcode(opcode);
#endif

    cpuSetInstructionTime(0);

    cpu_opcode_data_current[opcode].instruction_func(cpu_opcode_data_current[opcode].data);

    if (oldSr & 0xc000 && !cpuGetInstructionAborted())
    {
      // This instruction was traced
      uint32_t cycles = cpuGetInstructionTime();
      cpuThrowTraceException();
      cpuSetInstructionTime(cpuGetInstructionTime() + cycles);
    }
    return cpuGetInstructionTime();
  }
}
