/* @(#) $Id: CpuIntegration.c,v 1.10 2013/01/08 19:17:33 peschau Exp $ */
/*=========================================================================*/
/* Fellow                                                                  */
/* Initialization of 68000 core                                            */
/* Integrates the 68k emulation with custom chips			   */
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
#include "CpuIntegration.h"
#include "CpuModule_Internal.h"
#include "bus.h"
#include "fileops.h"


jmp_buf cpu_integration_exception_buffer;

/* custom chip intreq bit-number to irq-level */
static ULO cpu_integration_int_to_level[16] = {1,1,1,2, 3,3,3,4, 4,4,4,5, 5,6,6,7};
static ULO cpu_integration_irq_source;

/* Cycles spent by chips (Blitter) as a result of an instruction */
static ULO cpu_integration_chip_cycles;
static ULO cpu_integration_chip_slowdown;

/*===========================================================================*/
/* CPU properties                                                            */
/*===========================================================================*/

ULO cpu_integration_speed; // The speed as expressed in the fellow configuration settings
ULO cpu_integration_speed_multiplier; // The cycle multiplier used to adjust the cpu-speed, calculated from cpu_integration_speed
cpu_integration_models cpu_integration_model; // The cpu model as expressed in the fellow configuration settings

/*===========================================================================*/
/* CPU properties                                                            */
/*===========================================================================*/

void cpuIntegrationSetSpeed(ULO speed)
{
  cpu_integration_speed = speed;
}

ULO cpuIntegrationGetSpeed(void)
{
  return cpu_integration_speed;
}

static void cpuIntegrationSetSpeedMultiplier(ULO multiplier)
{
  cpu_integration_speed_multiplier = multiplier;
}

static ULO cpuIntegrationGetSpeedMultiplier(void)
{
  return cpu_integration_speed_multiplier;
}

static void cpuIntegrationCalculateMultiplier(void)
{
  ULO multiplier = 12;

  switch (cpuGetModelMajor())
  {
  case 0:
    multiplier = 12;
    break;
  case 1:
    multiplier = 12;
    break;
  case 2:
    multiplier = 11;
    break;
  case 3:
    multiplier = 11;
    break;
  }

  if (cpuIntegrationGetSpeed() >= 8) cpuIntegrationSetSpeedMultiplier(multiplier);
  else if (cpuIntegrationGetSpeed() >= 4) cpuIntegrationSetSpeedMultiplier(multiplier - 1);
  else if (cpuIntegrationGetSpeed() >= 2) cpuIntegrationSetSpeedMultiplier(multiplier - 2);
  else if (cpuIntegrationGetSpeed() >= 1) cpuIntegrationSetSpeedMultiplier(multiplier - 3);
  else cpuIntegrationSetSpeedMultiplier(multiplier - 4);
}

BOOLE cpuIntegrationSetModel(cpu_integration_models model)
{
  BOOLE needreset = (cpu_integration_model != model);
  cpu_integration_model = model;

  switch (cpu_integration_model) 
  {
  case M68000: cpuSetModel(0, 0); break;
  case M68010: cpuSetModel(1, 0); break;
  case M68020: cpuSetModel(2, 0); break;
  case M68030: cpuSetModel(3, 0); break;
  case M68EC20: cpuSetModel(2, 1); break;
  case M68EC30: cpuSetModel(3, 1); break;  
  }
  return needreset;
}

cpu_integration_models cpuIntegrationGetModel(void)
{
  return cpu_integration_model;
}

void cpuIntegrationSetChipCycles(ULO chip_cycles)
{
  cpu_integration_chip_cycles = chip_cycles;
}

ULO cpuIntegrationGetChipCycles(void)
{
  return cpu_integration_chip_cycles;
}

void cpuIntegrationSetChipSlowdown(ULO chip_slowdown)
{
  cpu_integration_chip_slowdown = chip_slowdown;
}

ULO cpuIntegrationGetChipSlowdown(void)
{
  return cpu_integration_chip_slowdown;
}

ULO cpuIntegrationGetChipIrqToIntLevel(ULO chip_irq)
{
  return cpu_integration_int_to_level[chip_irq];
}

void cpuIntegrationSetIrqSource(ULO irq_source)
{
  cpu_integration_irq_source = irq_source;
}

ULO cpuIntegrationGetIrqSource(void)
{
  return cpu_integration_irq_source;
}

/*=====================================================
  Checking for waiting interrupts
  =====================================================*/

static BOOLE cpuIntegrationCheckPendingInterruptsFunc(void)
{
  ULO current_cpu_level = (cpuGetSR() >> 8) & 7;
  BOOLE chip_irqs_enabled = !!(intena & 0x4000);

  if (chip_irqs_enabled)
  {
    LON highest_chip_irq;
    ULO chip_irqs = intreq & intena;

    if (chip_irqs == 0) return FALSE;

    for (highest_chip_irq = 13; highest_chip_irq >= 0; highest_chip_irq--)
    {
      if (chip_irqs & (1 << highest_chip_irq))
      {
	// Found a chip-irq that is both flagged and enabled.
	ULO highest_chip_level = cpuIntegrationGetChipIrqToIntLevel(highest_chip_irq);
	if (highest_chip_level > current_cpu_level)
	{
	  cpuSetIrqLevel(highest_chip_level);
	  cpuSetIrqAddress(memoryReadLong(cpuGetVbr() + 0x60 + highest_chip_level*4));
	  cpuIntegrationSetIrqSource(highest_chip_irq);

	  if (cpuGetStop())
	  {
	    cpuSetStop(FALSE);
	    cpuEvent.cycle = bus.cycle;
	  }
	  return TRUE;
	}
      }
    }
  }
  return FALSE;
}

void cpuIntegrationCheckPendingInterrupts(void)
{
  cpuCheckPendingInterrupts();
}

/*=========================================*/
/* Exception mid-instruction exit function */
/*=========================================*/

void cpuIntegrationMidInstructionExceptionFunc(void)
{
  longjmp(cpu_integration_exception_buffer, -1);
}

/*===================================================*/
/* Handles reset exception event from the cpu-module */
/*===================================================*/

void cpuIntegrationResetExceptionFunc(void)
{
  fellowSoftReset();
}

/*=========*/
/* Logging */
/*=========*/

#ifdef CPU_INSTRUCTION_LOGGING

FILE *CPUINSTRUCTIONLOG;
int cpu_disable_instruction_log = TRUE;

void cpuInstructionLogOpen(void)
{
  if (CPUINSTRUCTIONLOG == NULL)
  {
    char filename[MAX_PATH];
    fileopsGetGenericFileName(filename, "WinFellow", "cpuinstructions.log");
    CPUINSTRUCTIONLOG = fopen(filename, "w");
  }
}

void cpuIntegrationPrintBusCycle(void)
{
  fprintf(CPUINSTRUCTIONLOG, "%d:%.5d ", bus.frame_no, bus.cycle);
}

void cpuIntegrationInstructionLogging(void)
{
  char saddress[256], sdata[256], sinstruction[256], soperands[256];

  if (cpu_disable_instruction_log) return;
  cpuInstructionLogOpen();
  /*
  fprintf(CPUINSTRUCTIONLOG, 
	  "D0:%.8X D1:%.8X D2:%.8X D3:%.8X D4:%.8X D5:%.8X D6:%.8X D7:%.8X\n", 
	  cpuGetDReg(0),
	  cpuGetDReg(1),
	  cpuGetDReg(2),
	  cpuGetDReg(3),
	  cpuGetDReg(4),
	  cpuGetDReg(5),
	  cpuGetDReg(6),
	  cpuGetDReg(7));

  fprintf(CPUINSTRUCTIONLOG, 
	  "A0:%.8X A1:%.8X A2:%.8X A3:%.8X A4:%.8X A5:%.8X A6:%.8X A7:%.8X\n", 
	  cpuGetAReg(0),
	  cpuGetAReg(1),
	  cpuGetAReg(2),
	  cpuGetAReg(3),
	  cpuGetAReg(4),
	  cpuGetAReg(5),
	  cpuGetAReg(6),
	  cpuGetAReg(7));
	  */
  saddress[0] = '\0';
  sdata[0] = '\0';
  sinstruction[0] = '\0';
  soperands[0] = '\0';
  cpuDisOpcode(cpuGetPC(), saddress, sdata, sinstruction, soperands);
  fprintf(CPUINSTRUCTIONLOG, "SSP:%.6X USP:%.6X SP:%.4X %s %s\t%s\t%s\n", cpuGetSspDirect(), cpuGetUspDirect(), cpuGetSR(), saddress, sdata, sinstruction, soperands);
}

void cpuIntegrationExceptionLogging(STR *description, ULO original_pc, UWO opcode)
{
  if (cpu_disable_instruction_log) return;
  cpuInstructionLogOpen();

  cpuIntegrationPrintBusCycle();
  fprintf(CPUINSTRUCTIONLOG, "%s for opcode %.4X at PC %.8X from PC %.8X\n", description, opcode, original_pc, cpuGetPC());
}

STR *cpuIntegrationGetInterruptName(ULO chip_irq_no)
{
  switch (chip_irq_no)
  {
  case 0: return "TBE: Output buffer of the serial port is empty.";
  case 1: return "DSKBLK: Disk DMA transfer ended.";
  case 2: return "SOFT: Software interrupt.";
  case 3: return "PORTS: From CIA-A or expansion port.";
  case 4: return "COPER: Copper interrupt.";
  case 5: return "VERTB: Start of vertical blank.";
  case 6: return "BLIT: Blitter done.";
  case 7: return "AUD0: Audio data on channel 0.";
  case 8: return "AUD1: Audio data on channel 1.";
  case 9: return "AUD2: Audio data on channel 2.";
  case 10: return "AUD3: Audio data on channel 3.";
  case 11: return "RBF: Input buffer of the serial port full.";
  case 12: return "DSKSYN: Disk sync value recognized.";
  case 13: return "EXTER: From CIA-B or expansion port.";
  case 14: return "INTEN: BUG! Not an interrupt.";
  case 15: return "NMI: BUG! Not an interrupt.";
  }
  return "Illegal interrupt source!";
}

void cpuIntegrationInterruptLogging(ULO level, ULO vector_address)
{
  if (cpu_disable_instruction_log) return;
  cpuInstructionLogOpen();

  cpuIntegrationPrintBusCycle();
  fprintf(CPUINSTRUCTIONLOG, "Irq %d to %.6X (%s)\n", level, vector_address, cpuIntegrationGetInterruptName(cpuIntegrationGetIrqSource()));
}

#endif

void cpuIntegrationExecuteInstructionEventHandler68000Fast(void)
{
  ULO cycles;
  cycles = cpuExecuteInstruction();

  if (cpuGetStop())
  {
      cpuEvent.cycle = BUS_CYCLE_DISABLE;
  }
  else  
  {
    cpuEvent.cycle += ((cycles*cpuIntegrationGetChipSlowdown())>>1) + cpuIntegrationGetChipCycles();
  }
  cpuIntegrationSetChipCycles(0);
}

void cpuIntegrationExecuteInstructionEventHandler68000General(void)
{
  ULO cycles = 0;
  ULO time_used = 0;

  do
  {
    cycles = cpuExecuteInstruction();
    cycles = cycles*cpuIntegrationGetChipSlowdown(); // Compensate for blitter time
    time_used += (cpuIntegrationGetChipCycles()<<12) + (cycles<<cpuIntegrationGetSpeedMultiplier());
  }
  while (time_used < 8192 && !cpuGetStop());

  if (cpuGetStop())
  {
    cpuEvent.cycle = BUS_CYCLE_DISABLE;
  }
  else  
  {
    cpuEvent.cycle += (time_used>>12);
  }
  cpuIntegrationSetChipCycles(0);
}

void cpuIntegrationExecuteInstructionEventHandler68020(void)
{
  ULO time_used = 0;
  do
  {
    cpuExecuteInstruction();
    time_used += (cpuIntegrationGetChipCycles()<<12) + (4<<cpuIntegrationGetSpeedMultiplier());
  }
  while (time_used < 8192 && !cpuGetStop());

  if (cpuGetStop())
  {
    cpuEvent.cycle = BUS_CYCLE_DISABLE;
  }
  else  
  {
    cpuEvent.cycle += (time_used>>12);
  }
  cpuIntegrationSetChipCycles(0);
}

void cpuIntegrationSetDefaultConfig(void)
{
  cpuIntegrationSetModel(M68000);
  cpuIntegrationSetChipCycles(0);
  cpuIntegrationSetChipSlowdown(1);
  cpuIntegrationSetSpeed(4);

  cpuSetCheckPendingInterruptsFunc(cpuIntegrationCheckPendingInterruptsFunc);
  cpuSetMidInstructionExceptionFunc(cpuIntegrationMidInstructionExceptionFunc);
  cpuSetResetExceptionFunc(cpuIntegrationResetExceptionFunc);

#ifdef CPU_INSTRUCTION_LOGGING
  cpuSetInstructionLoggingFunc(cpuIntegrationInstructionLogging);
  cpuSetExceptionLoggingFunc(cpuIntegrationExceptionLogging);
  cpuSetInterruptLoggingFunc(cpuIntegrationInterruptLogging);
#endif
}

/*=========================*/
/* Fellow lifecycle events */
/*=========================*/

void cpuIntegrationSaveState(FILE *F)
{
  cpuSaveState(F);

  fwrite(&cpu_integration_chip_slowdown, sizeof(cpu_integration_chip_slowdown), 1, F);
  // Everything else is configuration options which will be set when the associated config-file is loaded.
}

void cpuIntegrationLoadState(FILE *F)
{
  cpuLoadState(F);

  fread(&cpu_integration_chip_slowdown, sizeof(cpu_integration_chip_slowdown), 1, F);
  // Everything else is configuration options which will be set when the associated config-file is loaded.
}

void cpuIntegrationEmulationStart(void)
{
  cpuIntegrationCalculateMultiplier();
}

void cpuIntegrationEmulationStop(void)
{
}

void cpuIntegrationHardReset(void)
{
  cpuIntegrationSetChipCycles(0);
  cpuIntegrationSetChipSlowdown(1);
  cpuSetInitialPC(memoryInitialPC());
  cpuSetInitialSP(memoryInitialSP());
  cpuHardReset();
}

void cpuIntegrationStartup(void)
{
  cpuStartup();
  cpuIntegrationSetDefaultConfig();
  cpuCreateMulTimeTables();
}

void cpuIntegrationShutdown(void)
{
  cpuProfileWrite();
}
