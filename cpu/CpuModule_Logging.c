/* @(#) $Id: CpuModule_Logging.c,v 1.3 2012-08-12 16:51:02 peschau Exp $ */
/*=========================================================================*/
/* Fellow                                                                  */
/* CPU 68k logging functions                                               */
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
#include "CpuModule.h"

#ifdef CPU_INSTRUCTION_LOGGING

/* Function for logging the intruction execution */
static cpuInstructionLoggingFunc cpu_instruction_logging_func;
static cpuExceptionLoggingFunc cpu_exception_logging_func;
static cpuInterruptLoggingFunc cpu_interrupt_logging_func;

void cpuSetInstructionLoggingFunc(cpuInstructionLoggingFunc func)
{
  cpu_instruction_logging_func = func;
}

void cpuCallInstructionLoggingFunc(void)
{
  if (cpu_instruction_logging_func != NULL)
    cpu_instruction_logging_func();
}

void cpuSetExceptionLoggingFunc(cpuExceptionLoggingFunc func)
{
  cpu_exception_logging_func = func;
}

void cpuCallExceptionLoggingFunc(STR *description, ULO original_pc, UWO opcode)
{
  if (cpu_exception_logging_func != NULL)
    cpu_exception_logging_func(description, original_pc, opcode);
}

void cpuSetInterruptLoggingFunc(cpuInterruptLoggingFunc func)
{
  cpu_interrupt_logging_func = func;
}

void cpuCallInterruptLoggingFunc(ULO level, ULO vector_address)
{
  if (cpu_interrupt_logging_func != NULL)
    cpu_interrupt_logging_func(level, vector_address);
}

#endif
