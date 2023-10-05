/*=========================================================================*/
/* Fellow                                                                  */
/* 68000 internal state                                                    */
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

/* M68k registers */
static uint32_t cpu_regs[2][8]; /* 0 - data, 1 - address */
static uint32_t cpu_pc;
static uint32_t cpu_usp;
static uint32_t cpu_ssp;
static uint32_t cpu_msp;
static uint32_t cpu_sfc;
static uint32_t cpu_dfc;
uint32_t cpu_sr; // Not static because flags calculation use it extensively
static uint32_t cpu_vbr;
static uint16_t cpu_prefetch_word;
static uint32_t cpu_cacr;
static uint32_t cpu_caar;

/* Irq management */
static BOOLE cpu_raise_irq;
static uint32_t cpu_raise_irq_level;

/* Reset values */
static uint32_t cpu_initial_pc;
static uint32_t cpu_initial_sp;

/* Flag set if CPU is stopped */
static BOOLE cpu_stop;

/* The current CPU model */
static uint32_t cpu_model_major = -1;
static uint32_t cpu_model_minor;
static uint8_t cpu_model_mask;

/* For exception handling */
#ifdef CPU_INSTRUCTION_LOGGING

static uint16_t cpu_current_opcode;

#endif

static uint32_t cpu_original_pc;
static bool cpu_instruction_aborted;

/* Number of cycles taken by the last intstruction */
static uint32_t cpu_instruction_time;

/* Getters and setters */

void cpuSetDReg(uint32_t i, uint32_t value) {cpu_regs[0][i] = value;}
uint32_t cpuGetDReg(uint32_t i) {return cpu_regs[0][i];}

void cpuSetAReg(uint32_t i, uint32_t value) {cpu_regs[1][i] = value;}
uint32_t cpuGetAReg(uint32_t i) {return cpu_regs[1][i];}

void cpuSetReg(uint32_t da, uint32_t i, uint32_t value) {cpu_regs[da][i] = value;}
uint32_t cpuGetReg(uint32_t da, uint32_t i) {return cpu_regs[da][i];}

/// <summary>
/// Get the supervisor bit from sr.
/// </summary>
BOOLE cpuGetFlagSupervisor(void)
{
  return cpu_sr & 0x2000;
}

/// <summary>
/// Get the master/irq state bit from sr.
/// </summary>
BOOLE cpuGetFlagMaster(void)
{
  return cpu_sr & 0x1000;
}

void cpuSetUspDirect(uint32_t usp) {cpu_usp = usp;}
uint32_t cpuGetUspDirect() {return cpu_usp;}
uint32_t cpuGetUspAutoMap() {return (cpuGetFlagSupervisor()) ? cpuGetUspDirect() : cpuGetAReg(7);}

void cpuSetSspDirect(uint32_t ssp) {cpu_ssp = ssp;}
uint32_t cpuGetSspDirect() {return cpu_ssp;}
uint32_t cpuGetSspAutoMap() {return (cpuGetFlagSupervisor()) ? cpuGetAReg(7) : cpuGetSspDirect();}

void cpuSetMspDirect(uint32_t msp) {cpu_msp = msp;}
uint32_t cpuGetMspDirect() {return cpu_msp;}

/// <summary>
/// Returns the master stack pointer.
/// </summary>
uint32_t cpuGetMspAutoMap(void)
{
  if (cpuGetFlagSupervisor() && cpuGetFlagMaster())
  {
    return cpuGetAReg(7);
  }
  return cpuGetMspDirect();
}
    
/// <summary>
/// Sets the master stack pointer.
/// </summary>
void cpuSetMspAutoMap(uint32_t new_msp)
{
  if (cpuGetFlagSupervisor() && cpuGetFlagMaster())
  {
    cpuSetAReg(7, new_msp);
  }
  else
  {
    cpuSetMspDirect(new_msp);
  }
}
    
/// <summary>
/// Returns the interrupt stack pointer. ssp is used as isp.
/// </summary>
uint32_t cpuGetIspAutoMap(void)
{
  if (cpuGetFlagSupervisor() && !cpuGetFlagMaster())
  {
    return cpuGetAReg(7);
  }
  return cpuGetSspDirect();
}
    
/// <summary>
/// Sets the interrupt stack pointer. ssp is used as isp.
/// </summary>
void cpuSetIspAutoMap(uint32_t new_isp)
{
  if (cpuGetFlagSupervisor() && !cpuGetFlagMaster())
  {
    cpuSetAReg(7, new_isp);
  }
  else
  {
    cpuSetSspDirect(new_isp);
  }
}

void cpuSetPC(uint32_t address) {cpu_pc = address;}
uint32_t cpuGetPC() {return cpu_pc;}

void cpuSetStop(BOOLE stop) {cpu_stop = stop;}
BOOLE cpuGetStop() {return cpu_stop;}

void cpuSetVbr(uint32_t vbr) {cpu_vbr = vbr;}
uint32_t cpuGetVbr() {return cpu_vbr;}

void cpuSetSfc(uint32_t sfc) {cpu_sfc = sfc;}
uint32_t cpuGetSfc() {return cpu_sfc;}

void cpuSetDfc(uint32_t dfc) {cpu_dfc = dfc;}
uint32_t cpuGetDfc() {return cpu_dfc;}

void cpuSetCacr(uint32_t cacr) {cpu_cacr = cacr;}
uint32_t cpuGetCacr() {return cpu_cacr;}

void cpuSetCaar(uint32_t caar) {cpu_caar = caar;}
uint32_t cpuGetCaar() {return cpu_caar;}

void cpuSetSR(uint32_t sr) {cpu_sr = sr;}
uint32_t cpuGetSR() {return cpu_sr;}

void cpuSetInstructionTime(uint32_t cycles) {cpu_instruction_time = cycles;}
uint32_t cpuGetInstructionTime() {return cpu_instruction_time;}

void cpuSetOriginalPC(uint32_t pc) {cpu_original_pc = pc;}
uint32_t cpuGetOriginalPC() {return cpu_original_pc;}

void cpuSetInstructionAborted(bool aborted) {cpu_instruction_aborted = aborted;}
bool cpuGetInstructionAborted() {return cpu_instruction_aborted;}

#ifdef CPU_INSTRUCTION_LOGGING

void cpuSetCurrentOpcode(uint16_t opcode) {cpu_current_opcode = opcode;}
uint16_t cpuGetCurrentOpcode() {return cpu_current_opcode;}

#endif

void cpuSetRaiseInterrupt(BOOLE raise_irq) {cpu_raise_irq = raise_irq;}
BOOLE cpuGetRaiseInterrupt() {return cpu_raise_irq;}
void cpuSetRaiseInterruptLevel(uint32_t raise_irq_level) {cpu_raise_irq_level = raise_irq_level;}
uint32_t cpuGetRaiseInterruptLevel() {return cpu_raise_irq_level;}

uint32_t cpuGetIrqLevel() {return (cpu_sr & 0x0700) >> 8;}

void cpuSetInitialPC(uint32_t pc) {cpu_initial_pc = pc;}
uint32_t cpuGetInitialPC() {return cpu_initial_pc;}

void cpuSetInitialSP(uint32_t sp) {cpu_initial_sp = sp;}
uint32_t cpuGetInitialSP() {return cpu_initial_sp;}

void cpuSetModelMask(uint8_t model_mask) {cpu_model_mask = model_mask;}
uint8_t cpuGetModelMask() {return cpu_model_mask;}

uint32_t cpuGetModelMajor() {return cpu_model_major;}
uint32_t cpuGetModelMinor() {return cpu_model_minor;}

static void cpuCalculateModelMask(void)
{
  switch (cpuGetModelMajor())
  {
    case 0:
      cpuSetModelMask(0x01);
      break;
    case 1:
      cpuSetModelMask(0x02);
      break;
    case 2:
      cpuSetModelMask(0x04);
      break;
    case 3:
      cpuSetModelMask(0x08);
      break;
  }
}

void cpuSetModel(uint32_t major, uint32_t minor)
{
  BOOLE makeOpcodeTable = (cpu_model_major != major);
  cpu_model_major = major;
  cpu_model_minor = minor;
  cpuCalculateModelMask();
  cpuStackFrameInit();
  if (makeOpcodeTable) cpuMakeOpcodeTableForModel();
}

#if 0
void cpuSetDRegWord(uint32_t regno, uint16_t val) {*((int16_t*)&cpu_regs[0][regno]) = val;}
void cpuSetDRegByte(uint32_t regno, uint8_t val) {*((uint8_t*)&cpu_regs[0][regno]) = val;}
#else
// MPW -- above assumes little endian.
void cpuSetDRegWord(uint32_t regno, uint16_t val) {cpu_regs[0][regno] &= 0xffff0000;  cpu_regs[0][regno] |= val;}
void cpuSetDRegByte(uint32_t regno, uint8_t val) {cpu_regs[0][regno] &= 0xffffff00; cpu_regs[0][regno] |= val;}
#endif

uint16_t cpuGetRegWord(uint32_t i, uint32_t regno) {return (uint16_t)cpu_regs[i][regno];}
uint16_t cpuGetDRegWord(uint32_t regno) {return (uint16_t)cpu_regs[0][regno];}
uint8_t cpuGetDRegByte(uint32_t regno) {return (uint8_t)cpu_regs[0][regno];}

uint32_t cpuGetDRegWordSignExtLong(uint32_t regno) {return cpuSignExtWordToLong(cpuGetDRegWord(regno));}
uint16_t cpuGetDRegByteSignExtWord(uint32_t regno) {return cpuSignExtByteToWord(cpuGetDRegByte(regno));}
uint32_t cpuGetDRegByteSignExtLong(uint32_t regno) {return cpuSignExtByteToLong(cpuGetDRegByte(regno));}

uint16_t cpuGetARegWord(uint32_t regno) {return (uint16_t)cpu_regs[1][regno];}
uint8_t cpuGetARegByte(uint32_t regno) {return (uint8_t)cpu_regs[1][regno];}

typedef uint16_t (*cpuGetWordFunc)(void);
typedef uint32_t (*cpuGetLongFunc)(void);

static uint16_t cpuGetNextWordInternal(void)
{
  uint16_t data = memoryReadWord(cpuGetPC() + 2);
  return data;
}

static uint32_t cpuGetNextLongInternal(void)
{
  uint32_t data = memoryReadLong(cpuGetPC() + 2);
  return data;
}

uint16_t cpuGetNextWord(void)
{
  uint16_t tmp = cpu_prefetch_word;
  cpu_prefetch_word = cpuGetNextWordInternal();
  cpuSetPC(cpuGetPC() + 2);
  return tmp;
}

uint32_t cpuGetNextWordSignExt(void)
{
  return cpuSignExtWordToLong(cpuGetNextWord());
}

uint32_t cpuGetNextLong(void)
{
  uint32_t tmp = cpu_prefetch_word << 16;
  uint32_t data = cpuGetNextLongInternal();
  cpu_prefetch_word = (uint16_t) data;
  cpuSetPC(cpuGetPC() + 4);
  return tmp | (data >> 16);
}

void cpuInitializePrefetch(void)
{
  cpu_prefetch_word = memoryReadWord(cpuGetPC());
}

void cpuClearPrefetch(void)
{
  cpu_prefetch_word = 0;
}

void cpuSkipNextWord(void)
{
  cpuSetPC(cpuGetPC() + 2);
  cpuInitializePrefetch();
}

void cpuSkipNextLong(void)
{
  cpuSetPC(cpuGetPC() + 4);
  cpuInitializePrefetch();
}

void cpuInitializeFromNewPC(uint32_t new_pc)
{
  cpuSetPC(new_pc);
  cpuInitializePrefetch();
}

void cpuSaveState(FILE *F)
{
  fwrite(&cpu_model_major, sizeof(cpu_model_major), 1, F);
  fwrite(&cpu_model_minor, sizeof(cpu_model_minor), 1, F);
  for (uint32_t i = 0; i < 2; i++)
  {
    for (uint32_t j = 0; j < 7; j++)
    {
      fwrite(&cpu_regs[i][j], sizeof(cpu_regs[i][j]), 1, F);
    }
  }
  fwrite(&cpu_pc, sizeof(cpu_pc), 1, F);
  fwrite(&cpu_usp, sizeof(cpu_usp), 1, F);
  fwrite(&cpu_ssp, sizeof(cpu_ssp), 1, F);
  fwrite(&cpu_msp, sizeof(cpu_msp), 1, F);
  fwrite(&cpu_sfc, sizeof(cpu_sfc), 1, F);
  fwrite(&cpu_dfc, sizeof(cpu_dfc), 1, F);
  fwrite(&cpu_sr, sizeof(cpu_sr), 1, F);
  fwrite(&cpu_prefetch_word, sizeof(cpu_prefetch_word), 1, F);
  fwrite(&cpu_vbr, sizeof(cpu_vbr), 1, F);
  fwrite(&cpu_cacr, sizeof(cpu_cacr), 1, F);
  fwrite(&cpu_caar, sizeof(cpu_caar), 1, F);
  fwrite(&cpu_initial_pc, sizeof(cpu_initial_pc), 1, F);
  fwrite(&cpu_initial_sp, sizeof(cpu_initial_sp), 1, F);
}

void cpuLoadState(FILE *F)
{
  fread(&cpu_model_major, sizeof(cpu_model_major), 1, F);
  fread(&cpu_model_minor, sizeof(cpu_model_minor), 1, F);
  for (uint32_t i = 0; i < 2; i++)
  {
    for (uint32_t j = 0; j < 7; j++)
    {
      fread(&cpu_regs[i][j], sizeof(cpu_regs[i][j]), 1, F);
    }
  }
  fread(&cpu_pc, sizeof(cpu_pc), 1, F);
  fread(&cpu_usp, sizeof(cpu_usp), 1, F);
  fread(&cpu_ssp, sizeof(cpu_ssp), 1, F);
  fread(&cpu_msp, sizeof(cpu_msp), 1, F);
  fread(&cpu_sfc, sizeof(cpu_sfc), 1, F);
  fread(&cpu_dfc, sizeof(cpu_dfc), 1, F);
  fread(&cpu_sr, sizeof(cpu_sr), 1, F);
  fread(&cpu_prefetch_word, sizeof(cpu_prefetch_word), 1, F);
  fread(&cpu_vbr, sizeof(cpu_vbr), 1, F);
  fread(&cpu_cacr, sizeof(cpu_cacr), 1, F);
  fread(&cpu_caar, sizeof(cpu_caar), 1, F);
  fread(&cpu_initial_pc, sizeof(cpu_initial_pc), 1, F);
  fread(&cpu_initial_sp, sizeof(cpu_initial_sp), 1, F);
  cpuSetModel(cpu_model_major, cpu_model_minor); // Recalculates stack frames etc.
}
