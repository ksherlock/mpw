/* @(#) $Id: CpuModule_Disassembler.c,v 1.2 2009/07/26 22:56:07 peschau Exp $ */
/*=========================================================================*/
/* Fellow                                                                  */
/* CPU disassembly                                                         */
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
#include "fmem.h"
#include "CpuModule.h"
#include "CpuModule_DisassemblerFunc.h"

typedef ULO (*cpuDisFunc)(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands);  

static ULO cpuDisGetSourceMode(UWO opcode)
{
  return (ULO) (opcode >> 3) & 7;
}

static ULO cpuDisGetDestinationMode(UWO opcode)
{
  return (ULO) (opcode >> 6) & 7;
}

static ULO cpuDisGetSourceRegister(UWO opcode)
{
  return (ULO) (opcode & 7);
}

static ULO cpuDisGetDestinationRegister(UWO opcode)
{
  return (ULO) (opcode >> 9) & 7;
}

static ULO cpuDisGetEaNo(ULO eamode, ULO eareg)
{
  return (eamode < 7) ? eamode : (eamode + eareg);
}

static ULO cpuDisGetBit(UWO opcode, ULO bit)
{
  return (ULO) ((opcode >> bit) & 1);
}

static LON cpuDisGetLowByteSignExtend(UWO opc)
{
  return (LON)(BYT)opc;
}

static ULO cpuDisGetSize(UWO opcode)
{
  ULO result = 0;
  switch ((opcode >> 6) & 3)
  {
  case 0: result = 8; break;
  case 1: result = 16; break;
  case 2: result = 32; break;
  case 3: result = 64; break;
  }
  return result;
}

static ULO cpuDisGetSize2(UWO opcode)
{
  ULO result = 0;
  switch ((opcode >> 8) & 1)
  {
  case 0: result = 16; break;
  case 1: result = 32; break;
  }
  return result;
}
static ULO cpuDisGetSize3(UWO opc)
{
  if ((opc & 0x3000) == 0x1000) return 8;
  else if ((opc & 0x3000) == 0x3000) return 16;
  return 32;
}

static ULO cpuDisGetSize4(UWO opc)
{
  if ((opc & 0x600) == 0x200) return 8;
  else if ((opc & 0x600) == 0x400) return 16;
  return 32;
}

static STR cpuDisSizeChar(ULO size)
{
  return (STR) ((size == 8) ? 'B' : ((size == 16) ? 'W' : 'L'));
}

static STR *cpu_dis_btab[16] = {"RA", "SR", "HI", "LS", "CC", "CS", "NE", "EQ", "VC", "VS",
"PL", "MI", "GE", "LT", "GT", "LE"};

static ULO cpuDisGetBranchType(UWO opc)
{
  return (opc >> 8) & 0xf;
}

/*===========================================================================
Disassembly of the address-modes
Parameters:
ULO reg  - The register used in the address-mode
ULO pcp  - The address of the next byte not used.
STR *st - The string to write the dissassembly to.
ULO *pos - The position in the string where the hex-words used are written
Returnvalue:
PC after possible extension words
===========================================================================*/

static STR* cpuDisEoS(STR *s)
{
  return s + strlen(s);
}

static void cpuDisCommaAppend(STR *s)
{
  strcat(s, ",");
}

static void cpuDisWordAppend(ULO data, STR *sdata)
{
  sprintf(cpuDisEoS(sdata), " %.4X", data);
}  

static void cpuDisLongAppend(ULO data, STR *sdata)
{
  sprintf(cpuDisEoS(sdata), " %.8X", data);
}  

static ULO cpuDis05(ULO regno, ULO pcp, STR *sdata, STR *soperands)
{
  ULO disp = memoryReadWord(pcp);

  cpuDisWordAppend(disp, sdata);
  sprintf(cpuDisEoS(soperands), "$%.4X(A%1u)", disp, regno);
  return pcp + 2;
}

static ULO cpuDis06Brief(ULO regno, ULO pcp, ULO ext, BOOLE is_pc_indirect, STR *sdata, STR *soperands)
{
  STR *scale[4] = {"", "*2", "*4", "*8"};
  STR indexregtype = (STR) ((ext & 0x8000) ? 'A' : 'D');
  STR indexsize = (STR) ((ext & 0x0800) ? 'L' : 'W');
  ULO indexregno = (ext >> 12) & 7;
  ULO offset = ext & 0xff;
  ULO scalefactor = (ext >> 9) & 3;

  cpuDisWordAppend(ext, sdata);
  if (cpuGetModelMajor() < 2)
  {
    if (!is_pc_indirect)
    {
      sprintf(cpuDisEoS(soperands), "$%.2X(A%1u,%c%1u.%c)", offset, regno, indexregtype, indexregno, indexsize);
    }
    else
    {
      sprintf(cpuDisEoS(soperands), "$%.2X(PC,%c%1u.%c)", offset, indexregtype, indexregno, indexsize);
    }
  }
  else
  {
    if (!is_pc_indirect)
    {
      sprintf(cpuDisEoS(soperands), "$%.2X(A%1u,%c%1u.%c%s)", offset, regno, indexregtype, indexregno, indexsize, scale[scalefactor]);
    }
    else
    {
      sprintf(cpuDisEoS(soperands), "$%.2X(PC,%c%1u.%c%s)", offset, indexregtype, indexregno, indexsize, scale[scalefactor]);
    }
  }
  return pcp;
}

static ULO cpuDis06Od(ULO pcp, BOOLE wordsize, STR *sdata, STR *soperands)
{
  ULO od;

  if (wordsize)
  {
    od = memoryReadWord(pcp);
    pcp += 2;
    cpuDisWordAppend(od, sdata);
    sprintf(cpuDisEoS(soperands), "$%.4X", od);
  }
  else
  {
    od = memoryReadLong(pcp);
    pcp += 4;
    cpuDisLongAppend(od, sdata);
    sprintf(cpuDisEoS(soperands), "$%.8X", od);
  }
  return pcp;
}

static ULO cpuDis06Extended(ULO regno, ULO pcp, ULO ext, BOOLE is_pc_indirect, STR *sdata, STR *soperands)
{
  STR *scale[4] = {"", "*2", "*4", "*8"};
  STR indexregtype = (STR) ((ext & 0x8000) ? 'A' : 'D');
  STR indexsize = (STR)((ext & 0x0800) ? 'L' : 'W');
  ULO indexregno = (ext >> 12) & 7;
  ULO scalefactor = (ext >> 9) & 3;
  ULO iis = ext & 0x0007;
  ULO bdsize = (ext >> 4) & 3;
  ULO bd;
  BOOLE is = !!(ext & 0x0040);
  BOOLE bs = !!(ext & 0x0080);
  STR baseregstr[32];
  STR indexstr[32];
  STR basedispstr[32];
  STR outerdispstr[32];

  baseregstr[0] = '\0';
  indexstr[0] = '\0';
  basedispstr[0] = '\0';
  outerdispstr[0] = '\0';

  /* Print extension word */

  cpuDisWordAppend(ext, sdata);

  /* Base register */

  if (bs)
  { /* Base register suppress */
    baseregstr[0] = '\0';
  }
  else
  {    /* Base register included */
    if (is_pc_indirect)
    {
      strcpy(baseregstr, "PC");
    }
    else
    {
      sprintf(baseregstr, "A%u", regno);
    }
  }

  /* Index register */

  if (is)
  { /* Index suppress */
    indexstr[0] = '\0';
  }
  else
  {    /* Index included */
    sprintf(indexstr, "%c%u.%c%s", indexregtype, indexregno, indexsize, scale[scalefactor]);
  }

  /* Base displacement */

  if (bdsize < 2)
  {
    basedispstr[0] = '\0';
  }
  else if (bdsize == 2)
  {
    bd = memoryReadWord(pcp);
    pcp += 2;
    cpuDisWordAppend(bd, sdata);
    sprintf(basedispstr, "$%.4X", bd);
  }
  else if (bdsize == 3)
  {
    bd = memoryReadLong(pcp);
    pcp += 4;
    cpuDisLongAppend(bd, sdata);
    sprintf(basedispstr, "$%.8X", bd);
  }

  switch (iis)
  { /* Evaluate and add index operand */
  case 0:           /* No memory indirect action */
    sprintf(cpuDisEoS(soperands), "(%s,%s,%s)", basedispstr, baseregstr, indexstr);
    break;
  case 1:           /* Preindexed with null outer displacement */
    sprintf(cpuDisEoS(soperands), "([%s,%s,%s])", basedispstr, baseregstr, indexstr);
    break;
  case 2:           /* Preindexed with word outer displacement */
    pcp = cpuDis06Od(pcp, TRUE, sdata, outerdispstr);
    sprintf(cpuDisEoS(soperands), "([%s,%s,%s],%s)", basedispstr, baseregstr, indexstr, outerdispstr);
    break;
  case 3:           /* Preindexed with long outer displacement */
    pcp = cpuDis06Od(pcp, TRUE, sdata, outerdispstr);
    sprintf(cpuDisEoS(soperands), "([%s,%s,%s],%s)", basedispstr, baseregstr, indexstr, outerdispstr);
    break;
  case 4:           /* Reserved ie. Illegal */
    sprintf(cpuDisEoS(soperands), "RESERVED/ILLEGAL");
    break;
  case 5:           /* Postindexed with null outer displacement */
    if (is)
    {
      sprintf(cpuDisEoS(soperands), "RESERVED/ILLEGAL");
    }
    else
    {
      sprintf(cpuDisEoS(soperands), "([%s,%s],%s)", basedispstr, baseregstr, indexstr);
    }
    break;
  case 6:           /* Postindexed with word outer displacement */
    if (is)
    {
      sprintf(cpuDisEoS(soperands), "RESERVED/ILLEGAL");
    }
    else
    {
      pcp = cpuDis06Od(pcp, TRUE, sdata, outerdispstr);
      sprintf(cpuDisEoS(soperands), "([%s,%s],%s,%s)", basedispstr, baseregstr, indexstr, outerdispstr);
    }
    break;
  case 7:           /* Postindexed with long outer displacement */
    if (is)
    {
      sprintf(cpuDisEoS(soperands), "RESERVED/ILLEGAL");
    }
    else
    {
      pcp = cpuDis06Od(pcp, TRUE, sdata, outerdispstr);
      sprintf(cpuDisEoS(soperands), "([%s,%s],%s,%s)", basedispstr, baseregstr, indexstr, outerdispstr);
    }
    break;
  }
  return pcp;
}


static ULO cpuDis06(ULO regno, ULO pcp, STR *sdata, STR *soperands)
{
  ULO ext = memoryReadWord(pcp);

  if (cpuGetModelMajor() < 2 || !(ext & 0x0100))
    return cpuDis06Brief(regno, pcp + 2, ext, FALSE, sdata, soperands);
  else
    return cpuDis06Extended(regno, pcp + 2, ext, FALSE, sdata, soperands);
}

static ULO cpuDis70(ULO pcp, STR *sdata, STR *soperands)
{
  ULO absadr = memoryReadWord(pcp);

  cpuDisWordAppend(absadr, sdata);
  sprintf(cpuDisEoS(soperands), "$%.4X", absadr);
  return pcp + 2;
}

static ULO cpuDis71(ULO pcp, STR *sdata, STR *soperands)
{
  ULO absadr = memoryReadLong(pcp);

  cpuDisLongAppend(absadr, sdata);
  sprintf(cpuDisEoS(soperands), "$%.8X", absadr);
  return pcp + 4;
}

static ULO cpuDis72(ULO pcp, STR *sdata, STR *soperands)
{
  ULO disp = memoryReadWord(pcp);

  cpuDisWordAppend(disp, sdata);
  sprintf(cpuDisEoS(soperands), "$%.4X(PC)", disp);
  return pcp + 2;
}

static ULO cpuDis73(ULO pcp, STR *sdata, STR *soperands)
{
  ULO ext = memoryReadWord(pcp);

  if (cpuGetModelMajor() < 2 || !(ext & 0x0100))
    return cpuDis06Brief(0, pcp + 2, ext, TRUE, sdata, soperands);
  else
    return cpuDis06Extended(0, pcp + 2, ext, TRUE, sdata, soperands);
}

static ULO cpuDis74(ULO size, ULO pcp, STR *sdata, STR *soperands)
{
  ULO imm;

  if (size == 8)
  {
    imm = memoryReadWord(pcp);
    cpuDisWordAppend(imm, sdata);
    sprintf(cpuDisEoS(soperands), "#$%.2X", imm & 0x00ff);
    return pcp + 2;
  }
  else if (size == 16)
  {
    imm = memoryReadWord(pcp);
    cpuDisWordAppend(imm, sdata);
    sprintf(cpuDisEoS(soperands), "#$%.4X", imm);
    return pcp + 2;
  }
  else
  {
    imm = memoryReadLong(pcp);
    cpuDisLongAppend(imm, sdata);
    sprintf(cpuDisEoS(soperands), "#$%.8X", imm);
    return pcp + 4;
  }
}

static void cpuDisRegCat(BOOLE is_datareg, ULO regno, STR *soperands)
{
  size_t i = strlen(soperands);

  soperands[i++] = (STR) ((is_datareg) ? 'D' : 'A');
  soperands[i++] = (STR) (regno + 0x30);
  soperands[i] = '\0';
}

static void cpuDisIndRegCat(ULO mode, ULO regno, STR *soperands)
{
  size_t i = strlen(soperands);

  if (mode == 4) soperands[i++] = '-';
  soperands[i++] = '(';
  soperands[i++] = 'A';
  soperands[i++] = (STR) (regno + 0x30);
  soperands[i++] = ')';
  if (mode == 3) soperands[i++] = '+';
  soperands[i] = '\0';
}

static ULO cpuDisAdrMode(ULO eamode, ULO earegno, ULO pcp, ULO size, STR *sdata, STR *soperands)
{
  switch (eamode)
  {
  case 0:
  case 1:  cpuDisRegCat(eamode == 0, earegno, soperands); break;
  case 2:
  case 3:
  case 4:  cpuDisIndRegCat(eamode, earegno, soperands); break;
  case 5:  return cpuDis05(earegno, pcp, sdata, soperands);
  case 6:  return cpuDis06(earegno, pcp, sdata, soperands);
  case 7:  return cpuDis70(pcp, sdata, soperands);
  case 8:  return cpuDis71(pcp, sdata, soperands);
  case 9:  return cpuDis72(pcp, sdata, soperands);
  case 10: return cpuDis73(pcp, sdata, soperands);
  case 11: return cpuDis74(size, pcp, sdata, soperands);
  default: return pcp;
  }
  return pcp;
}

/*=============================*/
/* Common disassembly routines */
/*=============================*/

/* Common disassembly for BCHG, BCLR, BSET, BTST */

static ULO cpu_dis_btX_trans[4] = {3, 0, 1, 2};
static STR *cpu_dis_bnr[4] = {"CHG","CLR","SET","TST"};

static ULO cpuDisBtX(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  BOOLE is_reg = cpuDisGetBit(opc, 8);
  ULO eareg = cpuDisGetSourceRegister(opc);
  ULO eamode = cpuDisGetEaNo(cpuDisGetSourceMode(opc), eareg);
  ULO bitreg = cpuDisGetDestinationRegister(opc);
  STR sizech = (eamode == 0) ? 'L' : 'B';

  sprintf(sinstruction, "B%s.%c", cpu_dis_bnr[cpu_dis_btX_trans[(opc >> 6) & 3]], sizech);
  if (!is_reg)
  {
    ULO imm = memoryReadWord(prc + 2);
    cpuDisWordAppend(imm, sdata);
    sprintf(soperands, "#$%.4X,", imm & ((eamode == 0) ? 0x1f : 7));
    prc += 2;
  }
  else
  {
    sprintf(soperands, "D%1X,", bitreg);
  }
  return cpuDisAdrMode(eamode, eareg, prc + 2, 8, sdata, soperands);
}

/* Common disassembly for ADD, SUB, CMP, AND, EOR, OR */

static STR *cpu_dis_anr[6] = {"ADD","SUB","CMP","AND","EOR","OR"};

static ULO cpuDisArith1(ULO prc, UWO opc, ULO nr, STR *sdata, STR *sinstruction, STR *soperands)
{
  ULO eareg = cpuDisGetSourceRegister(opc);
  ULO dreg = cpuDisGetDestinationRegister(opc);
  ULO o = cpuDisGetBit(opc, 8);
  ULO size = cpuDisGetSize(opc);
  ULO eamode = cpuDisGetEaNo(cpuDisGetSourceMode(opc), eareg);

  sprintf(sinstruction, "%s.%c", cpu_dis_anr[nr], cpuDisSizeChar(size));
  prc = cpuDisAdrMode((o) ? 0 : eamode, (o) ? dreg : eareg, prc + 2, size, sdata, soperands);
  cpuDisCommaAppend(soperands);
  prc = cpuDisAdrMode((o) ? eamode : 0, (o) ? eareg : dreg, prc, size, sdata, soperands);
  return prc;
}

/* Common disassembly for ADDA, SUBA, CMPA */

static ULO cpuDisArith2(ULO prc, UWO opc, ULO nr, STR *sdata, STR *sinstruction, STR *soperands)
{
  ULO eareg = cpuDisGetSourceRegister(opc);
  ULO dreg = cpuDisGetDestinationRegister(opc);
  ULO eamode = cpuDisGetEaNo(cpuDisGetSourceMode(opc), eareg);
  ULO size = cpuDisGetSize2(opc);

  sprintf(sinstruction, "%sA.%c", cpu_dis_anr[nr], cpuDisSizeChar(size));
  prc = cpuDisAdrMode(eamode, eareg, prc + 2, size, sdata, soperands);
  cpuDisCommaAppend(soperands);
  cpuDisAdrMode(1, dreg, prc, size, sdata, soperands);
  return prc;
}

/* Common disassembly for ADDI, SUBI, CMPI, ANDI, EORI, ORI */

static ULO cpuDisArith3(ULO prc, UWO opc, ULO nr, STR *sdata, STR *sinstruction, STR *soperands)
{
  ULO eareg = cpuDisGetSourceRegister(opc);
  ULO eamode = cpuDisGetEaNo(cpuDisGetSourceMode(opc), eareg);
  ULO size = cpuDisGetSize(opc);

  sprintf(sinstruction, "%sI.%c", cpu_dis_anr[nr], cpuDisSizeChar(size));
  prc = cpuDisAdrMode(11, 4, prc + 2, size, sdata, soperands);
  cpuDisCommaAppend(soperands);
  if ((nr > 2) && (eamode == 11))
  {
    strcat(soperands, (size == 8) ? "CCR" : "SR");
  }
  else
  {
    prc = cpuDisAdrMode(eamode, eareg, prc, size, sdata, soperands);
  }
  return prc;
}

/* Common disassembly for ADDQ, SUBQ */

static ULO cpuDisArith4(ULO prc, UWO opc, ULO nr, STR *sdata, STR *sinstruction, STR *soperands)
{
  ULO eareg = cpuDisGetSourceRegister(opc);
  ULO eamode = cpuDisGetEaNo(cpuDisGetSourceMode(opc), eareg);
  ULO size = cpuDisGetSize(opc);
  ULO imm = cpuDisGetDestinationRegister(opc);
  if (imm == 0)
  {
    imm = 8;
  }
  sprintf(sinstruction, "%sQ.%c", cpu_dis_anr[nr], cpuDisSizeChar(size));
  sprintf(soperands, "#$%.1u,", imm);
  return cpuDisAdrMode(eamode, eareg, prc + 2, size, sdata, soperands);
}

/* Common disassembly for ADDX, SUBX, ABCD, SBCD, CMPM */

static STR *cpu_dis_a5nr[5] = {"ADDX","SUBX","ABCD","SBCD","CMPM"};

static ULO cpuDisArith5(ULO prc, UWO opc, ULO nr, STR *sinstruction, STR *soperands)
{
  ULO bit3 = cpuDisGetBit(opc, 3);
  STR *minus = ((nr == 4) || !bit3) ? "" : "-";
  STR *plus = ((nr == 4) && !bit3) ? "+" : "";

  sprintf(sinstruction, "%s.%c", cpu_dis_a5nr[nr], cpuDisSizeChar(cpuDisGetSize(opc)));
  sprintf(soperands,
    ((!bit3) ?
    "%sD%d%s,%sD%d%s":
  "%s(A%d)%s,%s(A%d)%s"),
    minus,
    cpuDisGetSourceRegister(opc),
    plus,
    minus,
    cpuDisGetDestinationRegister(opc),
    plus);
  return prc + 2;
}

/* Common disassembly for ASX, LSX, ROX, ROXX */

static STR *cpu_dis_shnr[4] = {"AS", "LS", "RO", "ROX"};

static ULO cpuDisShift(ULO prc, UWO opc, ULO nr, STR *sdata, STR *sinstruction, STR *soperands)
{
  ULO eareg = cpuDisGetSourceRegister(opc);
  ULO eamode = cpuDisGetEaNo(cpuDisGetSourceMode(opc), eareg);
  ULO size = cpuDisGetSize(opc);
  STR rl = (STR) ((cpuDisGetBit(opc, 8)) ? 'L' : 'R');

  if (size == 64)
  {
    sprintf(sinstruction, "%s%c.W", cpu_dis_shnr[nr], rl);
    sprintf(soperands, "#$1,");
    prc = cpuDisAdrMode(eamode, eareg, prc + 2, 16, sdata, soperands);
  }
  else
  {
    STR sc = cpuDisSizeChar(size);
    ULO dreg = cpuDisGetDestinationRegister(opc);
    if (!cpuDisGetBit(opc, 5))
    {
      if (dreg == 0)
      {
	dreg = 8;
      }
      sprintf(sinstruction, "%s%c.%c", cpu_dis_shnr[nr], rl, sc);
      sprintf(soperands, "#$%1X,D%1u", dreg, eareg);
    }
    else
    {
      sprintf(sinstruction, "%s%c.%c", cpu_dis_shnr[nr], rl, sc);
      sprintf(soperands, "D%1u,D%1u", dreg, eareg);
    }
    prc += 2;
  }
  return prc;
}

/* Common disassembly for CLR, NEG, NOT, TST, JMP, JSR, PEA, NBCD, NEGX */

static STR *cpu_dis_unanr[10] = {"CLR","NEG","NOT","TST","JMP","JSR","PEA","TAS","NCBD","NEGX"};

static ULO cpuDisUnary(ULO prc, UWO opc, ULO nr, STR *sdata, STR *sinstruction, STR *soperands)
{
  ULO eareg = cpuDisGetSourceRegister(opc);
  ULO eamode = cpuDisGetEaNo(cpuDisGetSourceMode(opc), eareg);
  ULO size = cpuDisGetSize(opc);
  if (nr < 4 || nr > 7)
  {
    sprintf(sinstruction, "%s.%c", cpu_dis_unanr[nr], cpuDisSizeChar(size));
  }
  else
  {
    sprintf(sinstruction, "%s", cpu_dis_unanr[nr]);
  }
  return cpuDisAdrMode(eamode, eareg, prc + 2, size, sdata, soperands);
}

/* Common disassembly for NOP, RESET, RTE, RTR, RTS, TRAPV */

static STR *cpu_dis_singnr[6] = {"NOP","RESET","RTE","RTR","RTS","TRAPV"};

static ULO cpuDisSingle(ULO prc, ULO nr, STR *sinstruction)
{
  sprintf(sinstruction, "%s", cpu_dis_singnr[nr]);
  return prc + 2;
}

/* Common disassembly for CHK, DIVS, DIVU, LEA, MULS, MULU */

static STR *cpu_dis_var1nr[6] = {"CHK","DIVS","DIVU","LEA","MULS","MULU"};

static ULO cpuDisVarious1(ULO prc, UWO opc, ULO nr, STR *sdata, STR *sinstruction, STR *soperands)
{
  ULO eareg = cpuDisGetSourceRegister(opc);
  ULO eamode = cpuDisGetEaNo(cpuDisGetSourceMode(opc), eareg);
  ULO dreg = cpuDisGetDestinationRegister(opc);

  sprintf(sinstruction, "%s.%c", cpu_dis_var1nr[nr], (nr == 3) ? 'L' : 'W');
  prc = cpuDisAdrMode(eamode, eareg, prc + 2, 16, sdata, soperands);
  cpuDisCommaAppend(soperands);
  return cpuDisAdrMode((nr != 3) ? 0 : 1, dreg, prc, 16, sdata, soperands);
}

/* Common disassembly for SWAP, UNLK */

static STR *cpu_dis_var2nr[2] = {"SWAP","UNLK"};

static ULO cpuDisVarious2(ULO prc, UWO opc, ULO nr, STR *sinstruction, STR *soperands)
{
  STR regtype = (STR) ((nr == 0) ? 'D' : 'A');
  sprintf(sinstruction, "%s", cpu_dis_var2nr[nr]);
  sprintf(soperands, "%c%1X", regtype, cpuDisGetSourceRegister(opc));
  return prc + 2;
}

static ULO cpuDisIllegal(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  sprintf(sinstruction, "ILLEGAL");
  return prc + 2;
}

static ULO cpuDisAbcd(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  return cpuDisArith5(prc, opc, 2, sinstruction, soperands);
}

static ULO cpuDisAdd(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  return cpuDisArith1(prc, opc, 0, sdata, sinstruction, soperands);
}

static ULO cpuDisAdda(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  return cpuDisArith2(prc, opc, 0, sdata, sinstruction, soperands);
}

static ULO cpuDisAddi(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  return cpuDisArith3(prc, opc, 0, sdata, sinstruction, soperands);
}

static ULO cpuDisAddq(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  return cpuDisArith4(prc, opc, 0, sdata, sinstruction, soperands);
}

static ULO cpuDisAddx(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  return cpuDisArith5(prc, opc, 0, sinstruction, soperands);
}

static ULO cpuDisAnd(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  return cpuDisArith1(prc, opc, 3, sdata, sinstruction, soperands);
}

static ULO cpuDisAndi(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  return cpuDisArith3(prc, opc, 3, sdata, sinstruction, soperands);
}

static ULO cpuDisAsx(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  return cpuDisShift(prc, opc, 0, sdata, sinstruction, soperands);
}

static ULO cpuDisBcc(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  ULO disp2;
  ULO adr;
  LON disp = cpuDisGetLowByteSignExtend(opc);

  sprintf(sinstruction, "B%s.%c", cpu_dis_btab[cpuDisGetBranchType(opc)], (disp == -1) ? 'L' : ((disp == 0) ? 'W' : 'B'));
  if (disp == 0)
  {
    prc += 2;
    disp2 = memoryReadWord(prc);
    cpuDisWordAppend(disp2, sdata);
    adr = (disp2 > 32767) ? (prc + disp2 - 65536) : (prc + disp2);
  }
  else if (disp == -1 && cpuGetModelMajor() >= 2)
  {
    prc += 2;
    disp2 = memoryReadLong(prc);
    cpuDisLongAppend(disp2, sdata);
    adr = prc + disp2;
  }
  else
  {
    adr = prc + 2 + disp;
  }
  sprintf(cpuDisEoS(soperands), "$%8.8X", adr);
  return prc + 2;
}

static ULO cpuDisBt(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  return cpuDisBtX(prc, opc, sdata, sinstruction, soperands);
}

static ULO cpuDisChk(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  return cpuDisVarious1(prc, opc, 0, sdata, sinstruction, soperands);
}

static ULO cpuDisClr(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  return cpuDisUnary(prc, opc, 0, sdata, sinstruction, soperands);
}

static ULO cpuDisCmp(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  return cpuDisArith1(prc, opc, 2, sdata, sinstruction, soperands);
}  

static ULO cpuDisCmpa(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  return cpuDisArith2(prc, opc, 2, sdata, sinstruction, soperands);
}

static ULO cpuDisCmpi(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  return cpuDisArith3(prc, opc, 2, sdata, sinstruction, soperands);
}

static ULO cpuDisCmpm(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  return cpuDisArith5(prc, opc, 4, sinstruction, soperands);
}

static ULO cpuDisDBcc(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  ULO offset;
  ULO adr;
  ULO bratype = cpuDisGetBranchType(opc);

  prc += 2;
  offset = memoryReadWord(prc);
  adr = (offset > 32767) ? (prc + offset - 65536) : (prc + offset);
  cpuDisWordAppend(offset, sdata);
  sprintf(sinstruction, "DB%s", (bratype == 0) ? "T" : ((bratype == 1) ? "F" : cpu_dis_btab[bratype]));
  sprintf(soperands, "D%1u,$%6.6X", cpuDisGetSourceRegister(opc), adr);
  return prc + 2;
}

static ULO cpuDisDivs(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  return cpuDisVarious1(prc, opc, 1, sdata, sinstruction, soperands);
}

static ULO cpuDisDivu(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  return cpuDisVarious1(prc, opc, 2, sdata, sinstruction, soperands);
}

static ULO cpuDisEor(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  return cpuDisArith1(prc, opc, 4, sdata, sinstruction, soperands);
}

static ULO cpuDisEori(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  return cpuDisArith3(prc, opc, 4, sdata, sinstruction, soperands);
}

static ULO cpuDisExg(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  ULO o = (opc & 0x00f8) >> 3;

  sprintf(sinstruction, "EXG.L");
  sprintf(soperands, (o == 8) ? "D%d,D%d" : ((o == 9) ? "A%d,A%d" : "A%d,D%d"), cpuDisGetSourceRegister(opc), cpuDisGetDestinationRegister(opc));
  return prc + 2;
}

static ULO cpuDisExt(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  sprintf(sinstruction, "EXT.%c", (cpuDisGetBit(opc, 6) == 0) ? 'W' : 'L');
  sprintf(soperands, "D%u", cpuDisGetSourceRegister(opc));
  return prc + 2;
}

static ULO cpuDisJmp(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  return cpuDisUnary(prc, opc, 4, sdata, sinstruction, soperands);
}

static ULO cpuDisJsr(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  return cpuDisUnary(prc, opc, 5, sdata, sinstruction, soperands);
}

static ULO cpuDisLea(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  return cpuDisVarious1(prc, opc, 3, sdata, sinstruction, soperands);
}

static ULO cpuDisLink(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  ULO imm = memoryReadWord(prc + 2);

  cpuDisWordAppend(imm, sdata);
  sprintf(sinstruction, "LINK");
  sprintf(soperands, "A%1u,#$%.4X", cpuDisGetSourceRegister(opc), imm);
  return prc + 4;
}

static ULO cpuDisLsx(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  return cpuDisShift(prc, opc, 1, sdata, sinstruction, soperands);
}

static ULO cpuDisMove(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  ULO srcreg = cpuDisGetSourceRegister(opc);
  ULO srcmode = cpuDisGetEaNo(cpuDisGetSourceMode(opc), srcreg);
  ULO dstreg = cpuDisGetDestinationRegister(opc);
  ULO dstmode = cpuDisGetEaNo(cpuDisGetDestinationMode(opc), dstreg);
  ULO size = cpuDisGetSize3(opc);

  sprintf(sinstruction, "MOVE.%c", cpuDisSizeChar(size)); 
  prc = cpuDisAdrMode(srcmode, srcreg, prc + 2, size, sdata, soperands);
  cpuDisCommaAppend(soperands);
  return cpuDisAdrMode(dstmode, dstreg, prc, size, sdata, soperands);
}

static ULO cpuDisMoveToCcr(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  ULO eareg = cpuDisGetSourceRegister(opc);

  sprintf(sinstruction, "MOVE.B"); 
  prc = cpuDisAdrMode(cpuDisGetEaNo(cpuDisGetSourceMode(opc), eareg), eareg, prc + 2, 8, sdata, soperands);
  strcat(soperands, ",CCR");
  return prc;
}

static ULO cpuDisMoveToSr(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  ULO eareg = cpuDisGetSourceRegister(opc);

  sprintf(sinstruction, "MOVE.W"); 
  prc = cpuDisAdrMode(cpuDisGetEaNo(cpuDisGetSourceMode(opc), eareg), eareg, prc + 2, 16, sdata, soperands);
  strcat(soperands, ",SR");
  return prc;
}

static ULO cpuDisMoveFromSr(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  ULO eareg = cpuDisGetSourceRegister(opc);

  sprintf(sinstruction, "MOVE.W");
  sprintf(soperands, "SR,");
  prc = cpuDisAdrMode(cpuDisGetEaNo(cpuDisGetSourceMode(opc), eareg), eareg, prc + 2, 16, sdata, soperands);
  return prc;
}

static ULO cpuDisMoveUsp(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  sprintf(sinstruction, "MOVE.L");
  sprintf(soperands, (cpuDisGetBit(opc, 3)) ? "USP,A%1d" : "A%1d,USP", cpuDisGetSourceRegister(opc));
  return prc + 2;
}

static ULO cpuDisMovea(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  ULO eareg = cpuDisGetSourceRegister(opc);
  ULO dreg = cpuDisGetDestinationRegister(opc);
  ULO eamode = cpuDisGetEaNo(cpuDisGetSourceMode(opc), eareg);
  ULO size = cpuDisGetSize3(opc);

  sprintf(sinstruction, "MOVEA.%c", cpuDisSizeChar(size));
  prc = cpuDisAdrMode(eamode, eareg, prc + 2, size, sdata, soperands);
  cpuDisCommaAppend(soperands);
  return cpuDisAdrMode(1, dreg, prc, size, sdata, soperands);
}

static void cpuDisMovemRegmaskStrCat(ULO regmask, STR *s, BOOLE predec)
{
  ULO i, j;
  STR tmp[2][16];
  STR *tmpp;

  for (j = 0; j < 2; j++)
  {
    tmpp = tmp[j];
    for (i = (8*j); i < (8 + (8*j)); i++)
    {
      if (regmask & (1<<i))
      {
	*tmpp++ = (STR) (0x30 + ((predec) ? ((7 + 8*j) - i) : (i - j*8)));
      }
    }
    *tmpp = '\0';
  }
  if (tmp[0][0] != '\0')
  {
    strcat(s, ((predec) ? "A" : "D"));
    strcat(s, tmp[0]);
  }
  if (tmp[1][0] != '\0')
  {
    strcat(s, ((predec) ? "D" : "A"));
    strcat(s, tmp[1]);
  }
}

static ULO cpuDisMovem(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  ULO eareg = cpuDisGetSourceRegister(opc);
  ULO eamode = cpuDisGetEaNo(cpuDisGetSourceMode(opc), eareg);
  ULO size;
  ULO dir = cpuDisGetBit(opc, 10);
  ULO regmask;

  size = (!cpuDisGetBit(opc, 6)) ? 16 : 32;
  regmask = memoryReadWord(prc + 2);
  cpuDisWordAppend(regmask, sdata);
  sprintf(sinstruction, "MOVEM.%c", (size == 16) ? 'W' : 'L');
  if (dir == 0 && eamode == 4)
  {  /* Register to memory, predecrement */
    cpuDisMovemRegmaskStrCat(regmask, soperands, TRUE);
    cpuDisCommaAppend(soperands);
    prc = cpuDisAdrMode(eamode, eareg, prc + 4, size, sdata, soperands);
  }     
  else if (dir)
  { /* Memory to register, any legal adressmode */
    prc = cpuDisAdrMode(eamode, eareg, prc + 4, size, sdata, soperands);
    cpuDisCommaAppend(soperands);
    cpuDisMovemRegmaskStrCat(regmask, soperands, FALSE);
  }
  else
  {  /* Register to memory, the rest of the adr.modes */
    cpuDisMovemRegmaskStrCat(regmask, soperands, FALSE);
    cpuDisCommaAppend(soperands);
    prc = cpuDisAdrMode(eamode, eareg, prc + 4, size, sdata, soperands);
  }
  return prc;
}

static ULO cpuDisMovep(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  ULO adrregno = cpuDisGetSourceRegister(opc);
  ULO dataregno = cpuDisGetDestinationRegister(opc);
  ULO opmode = cpuDisGetDestinationMode(opc);
  STR sizech = (opmode & 1) ? 'L' : 'W';
  BOOLE to_mem = (opmode & 2);
  ULO disp = memoryReadWord(prc + 2);

  cpuDisWordAppend(disp, sdata);
  sprintf(sinstruction, "MOVEP.%c", sizech);
  if (to_mem)
  {
    sprintf(soperands, "D%1u,$%.4X(A%1u)", dataregno, disp, adrregno); 
  }
  else
  {
    sprintf(soperands, "$%.4X(A%1u),D%1u", disp, adrregno, dataregno);
  }
  return prc + 4;
}

static ULO cpuDisMoveq(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  sprintf(sinstruction, "MOVEQ.L");
  sprintf(soperands, "#$%8.8X,D%u", cpuDisGetLowByteSignExtend(opc), cpuDisGetDestinationRegister(opc));
  return prc + 2;
}

static ULO cpuDisMuls(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  return cpuDisVarious1(prc, opc, 4, sdata, sinstruction, soperands);
}

static ULO cpuDisMulu(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  return cpuDisVarious1(prc, opc, 5, sdata, sinstruction, soperands);
}

static ULO cpuDisNbcd(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  return cpuDisUnary(prc, opc, 8, sdata, sinstruction, soperands);
}

static ULO cpuDisNeg(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  return cpuDisUnary(prc, opc, 1, sdata, sinstruction, soperands);
}

static ULO cpuDisNegx(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  return cpuDisUnary(prc, opc, 9, sdata, sinstruction, soperands);
}

static ULO cpuDisNop(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  return cpuDisSingle(prc, 0, sinstruction);
}

static ULO cpuDisNot(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  return cpuDisUnary(prc, opc, 2, sdata, sinstruction, soperands);
}

static ULO cpuDisOr(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  return cpuDisArith1(prc, opc, 5, sdata, sinstruction, soperands);
}

static ULO cpuDisOri(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  return cpuDisArith3(prc, opc, 5, sdata, sinstruction, soperands);
}

static ULO cpuDisPea(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  return cpuDisUnary(prc, opc, 6, sdata, sinstruction, soperands);
}

static ULO cpuDisReset(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  return cpuDisSingle(prc, 1, sinstruction);
}

static ULO cpuDisRox(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  return cpuDisShift(prc, opc, 2, sdata, sinstruction, soperands);
}

static ULO cpuDisRoxx(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  return cpuDisShift(prc, opc, 3, sdata, sinstruction, soperands);
}

static ULO cpuDisRte(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  return cpuDisSingle(prc, 2, sinstruction);
}

static ULO cpuDisRtr(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  return cpuDisSingle(prc, 3, sinstruction);
}

static ULO cpuDisRts(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  return cpuDisSingle(prc, 4, sinstruction);
}

static ULO cpuDisSbcd(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  return cpuDisArith5(prc, opc, 3, sinstruction, soperands);
}

static ULO cpuDisScc(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  ULO eareg = cpuDisGetSourceRegister(opc);
  ULO bratype = cpuDisGetBranchType(opc);

  sprintf(sinstruction, "S%s.B", (bratype == 0) ? "T" : ((bratype == 1) ? "F" : cpu_dis_btab[bratype]));
  return cpuDisAdrMode(cpuDisGetEaNo(cpuDisGetSourceMode(opc), eareg), eareg, prc + 2, 8, sdata, soperands);
}

static ULO cpuDisStop(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  ULO imm = memoryReadWord(prc + 2);
  cpuDisWordAppend(imm, sdata);
  sprintf(sinstruction, "STOP");
  sprintf(soperands, "#$%.4X", imm);
  return prc + 4;
}

static ULO cpuDisSub(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  return cpuDisArith1(prc, opc, 1, sdata, sinstruction, soperands);
}

static ULO cpuDisSuba(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  return cpuDisArith2(prc, opc, 1, sdata, sinstruction, soperands);
}

static ULO cpuDisSubi(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  return cpuDisArith3(prc, opc, 1, sdata, sinstruction, soperands);
}

static ULO cpuDisSubq(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  return cpuDisArith4(prc, opc, 1, sdata, sinstruction, soperands);
}

static ULO cpuDisSubx(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  return cpuDisArith5(prc, opc, 1, sinstruction, soperands);
}

static ULO cpuDisSwap(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  return cpuDisVarious2(prc, opc, 0, sinstruction, soperands);
}

static ULO cpuDisTas(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  return cpuDisUnary(prc, opc, 7, sdata, sinstruction, soperands);
}

static ULO cpuDisTrap(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  sprintf(sinstruction, "TRAP");
  sprintf(soperands, "#$%1X", opc & 0xf);
  return prc + 2;
}

static ULO cpuDisTrapv(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  return cpuDisSingle(prc, 5, sinstruction);
}

static ULO cpuDisTst(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  return cpuDisUnary(prc, opc, 3, sdata, sinstruction, soperands);
}

static ULO cpuDisUnlk(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  return cpuDisVarious2(prc, opc, 1, sinstruction, soperands);
}

static ULO cpuDisBkpt(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  sprintf(sinstruction, "BKPT");
  sprintf(soperands, "#$%1X", cpuDisGetSourceRegister(opc));
  return prc + 2;
}

static STR *cpu_dis_bftxt[8] = {"TST ","EXTU","CHG ","EXTS","CLR ","FFO ","SET ","INS "};

static ULO cpuDisBf(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  ULO eareg = cpuDisGetSourceRegister(opc);
  ULO eamode = cpuDisGetEaNo(cpuDisGetSourceMode(opc), eareg);
  ULO ext = memoryReadWord(prc + 2);
  ULO n = (opc >> 8) & 7;
  ULO offset = (ext & 0x7c0) >> 6;
  ULO width = ext & 0x1f;
  STR stmp[16];

  cpuDisWordAppend(ext, sdata);
  sprintf(sinstruction, "BF%s", cpu_dis_bftxt[n]);
  if (n == 7)
  {
    sprintf(stmp, "D%u,", (ext >> 12) & 7);
    strcat(soperands, stmp);
  }
  prc = cpuDisAdrMode(eamode, eareg, prc + 4, 16, sdata, soperands);
  if (ext & 0x800)
  {
    sprintf(stmp, "{D%u:", offset & 7);
  }
  else
  {
    sprintf(stmp, "{%u:", offset);
  }
  strcat(soperands, stmp);
  if (ext & 0x20)
  {
    sprintf(stmp, "D%u}", width & 7);
  }
  else
  {
    sprintf(stmp, "%u}", width);
  }
  strcat(soperands, stmp);
  if ((n == 1) || (n == 3) || (n == 7))
  {
    sprintf(stmp, ",D%u", (ext >> 12) & 7);
    strcat(soperands, stmp);
  }
  return prc;
}

static ULO cpuDisCas(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  ULO eareg = cpuDisGetSourceRegister(opc);
  ULO ext = memoryReadWord(prc + 2);
  ULO size = cpuDisGetSize4(opc);

  cpuDisWordAppend(ext, sdata);
  if ((opc & 0x3f) == 0x3c)
  { /* CAS2 */
    ULO ext2 = memoryReadWord(prc + 4);
    cpuDisWordAppend(ext2, sdata);
    sprintf(sinstruction, "CAS2.%c", cpuDisSizeChar(size));

    sprintf(soperands,
      "D%u:D%u,D%u:D%u,(%s%u):(%s%u)",
      ext & 7, 
      ext2 & 7,
      (ext >> 6) & 7, 
      (ext2 >> 6) & 7, 
      (ext & 0x8000) ? "A" : "D",
      (ext >> 12) & 7, 
      (ext2 & 0x8000) ? "A" : "D", 
      (ext2 >> 12) & 7);
    prc += 6;
  }
  else
  {
    sprintf(sinstruction, "CAS.%c", cpuDisSizeChar(size));
    sprintf(soperands, "D%u,D%u,", ext & 7, (ext >> 6) & 7);
    prc = cpuDisAdrMode(cpuDisGetEaNo(cpuDisGetSourceMode(opc), eareg), eareg, prc + 4, size, sdata, soperands);
  }
  return prc;
}

static ULO cpuDisChkl(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  ULO eareg = cpuDisGetSourceRegister(opc);

  sprintf(sinstruction, "CHK.L");
  prc = cpuDisAdrMode(cpuDisGetEaNo(cpuDisGetSourceMode(opc), eareg), eareg, prc + 2, 32, sdata, soperands);
  cpuDisCommaAppend(soperands);
  return cpuDisAdrMode(0, cpuDisGetDestinationRegister(opc), prc + 2, 32, sdata, soperands);
}

static ULO cpuDisChk2(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  ULO eareg = cpuDisGetSourceRegister(opc);
  ULO ext = memoryReadWord(prc + 2);
  ULO size = cpuDisGetSize4(opc);
  STR stmp[16];

  cpuDisWordAppend(ext, sdata);
  sprintf(sinstruction, "%s.%c", (ext & 0x800) ? "CHK2" : "CMP2", cpuDisSizeChar(size));
  prc = cpuDisAdrMode(cpuDisGetEaNo(cpuDisGetSourceMode(opc), eareg), eareg, prc + 4, size, sdata, soperands);
  sprintf(stmp, ",%s%u", (ext & 0x8000) ? "A" : "D", (ext>>12) & 7);
  strcat(soperands, stmp);
  return prc;
}

static ULO cpuDisDivl(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  ULO eareg = cpuDisGetSourceRegister(opc);
  ULO ext = memoryReadWord(prc + 2);
  ULO dq;
  ULO dr;
  STR stmp[16];

  cpuDisWordAppend(ext, sdata);
  dq = (ext >> 12) & 7;
  dr = ext & 7;
  sprintf(sinstruction, "DIV%c%s.L ", (ext & 0x800) ? 'S' : 'U', (ext & 0x400) ? "L" : "");
  prc = cpuDisAdrMode(cpuDisGetEaNo(cpuDisGetSourceMode(opc), eareg), eareg, prc + 4, 32, sdata, soperands);
  if (ext & 0x400)
  {
    sprintf(stmp, ",D%u:D%u", dr, dq);
  }
  else
  {
    sprintf(stmp, ",D%u", dq);
  }
  strcat(soperands, stmp);
  return prc;
}

static ULO cpuDisExtb(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  sprintf(sinstruction, "EXTB.L");
  sprintf(soperands, "D%u", cpuDisGetSourceRegister(opc));
  return prc + 2;
}

static ULO cpuDisLinkl(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  ULO disp = memoryReadLong(prc + 2);
  cpuDisLongAppend(disp, sdata);
  sprintf(sinstruction, "LINK.L");
  sprintf(soperands, "A%u, #$%.8X", cpuDisGetSourceRegister(opc), disp);
  return prc + 6;
}

static ULO cpuDisMoveFromCcr(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  ULO eareg = cpuDisGetSourceRegister(opc);
  sprintf(sinstruction, "MOVE.W");
  sprintf(soperands, "CCR,");
  return cpuDisAdrMode(cpuDisGetEaNo(cpuDisGetSourceMode(opc), eareg), eareg, prc + 2, 16, sdata, soperands);
}

static ULO cpuDisMovec(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  ULO creg;
  ULO extw = memoryReadWord(prc + 2);
  STR stmp[16];

  cpuDisWordAppend(extw, sdata);
  sprintf(sinstruction, "MOVEC.L");
  if (opc & 1)
  { /* To control register */
    sprintf(stmp, "%s%u,", (extw & 0x8000) ? "A" : "D", (extw>>12) & 7); 
    strcat(soperands, stmp);
  }
  creg = extw & 0xfff;
  if (cpuGetModelMajor() == 1 && ((creg != 0) && (creg != 1) && (creg != 0x800) &&
    (creg != 0x801))) creg = 0xfff;
  switch (creg)
  {
  case 0x000:    /* SFC, Source function code X12346*/
    strcat(soperands, "SFC");
    break;
  case 0x001:    /* DFC, Destination function code X12346 */
    strcat(soperands, "DFC");
    break;
  case 0x800:    /* USP, User stack pointer X12346 */
    strcat(soperands, "USP");
    break;
  case 0x801:    /* VBR, Vector base register X12346 */
    strcat(soperands, "VBR");
    break;
  case 0x002:    /* CACR, Cache control register XX2346 */
    strcat(soperands, "CACR");
    break;
  case 0x802:    /* CAAR, Cache adress register XX2346 */
    strcat(soperands, "CAAR");
    break;
  case 0x803:    /* MSP, Master stack pointer XX234X */
    strcat(soperands, "MSP");
    break;
  case 0x804:    /* ISP, Interrupt stack pointer XX234X */
    strcat(soperands, "ISP");
    break;
  default:
    strcat(soperands, "ILLEGAL");
    break;
  }
  if (!(opc & 1))
  { /* From control register */
    sprintf(stmp, ",%s%u", (extw & 0x8000) ? "A":"D", (extw >> 12) & 7); 
    strcat(soperands, stmp);
  }
  return prc + 4;
}

static ULO cpuDisMoves(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  ULO eareg = cpuDisGetSourceRegister(opc);
  ULO ext = memoryReadWord(prc + 2);
  ULO size = cpuDisGetSize(opc);
  STR stmp[16];

  cpuDisWordAppend(ext, sdata);
  sprintf(sinstruction, "MOVES.%c", cpuDisSizeChar(size));
  if (ext & 0x800)
  {
    sprintf(stmp, "%s%u,", (ext & 0x8000) ? "A" : "D", (ext >> 12) & 7);
    strcat(soperands, stmp);
  }
  prc = cpuDisAdrMode(cpuDisGetEaNo(cpuDisGetSourceMode(opc), eareg), eareg, prc + 4, size, sdata, soperands);
  if (!(ext & 0x800))
  {
    sprintf(stmp, ",%s%u", (ext & 0x8000) ? "A" : "D", (ext >> 12) & 7);
    strcat(soperands, stmp);
  }
  return prc;
}

static ULO cpuDisMull(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  ULO eareg = cpuDisGetSourceRegister(opc);
  ULO ext = memoryReadWord(prc + 2);
  ULO dl;
  ULO dh;
  STR stmp[16];

  dl = (ext>>12) & 7;
  dh = ext & 7;
  cpuDisWordAppend(ext, sdata);
  sprintf(sinstruction, "MUL%c.L", (ext & 0x800) ? 'S' : 'U');
  prc = cpuDisAdrMode(cpuDisGetEaNo(cpuDisGetSourceMode(opc), eareg), eareg, prc + 4, 32, sdata, soperands);
  if (ext & 0x400)
  {
    sprintf(stmp, ",D%u:D%u", dh, dl);
  }
  else
  {
    sprintf(stmp, ",D%u", dl);
  }
  strcat(soperands, stmp);
  return prc;
}

static ULO cpuDisPack(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  ULO sreg = cpuDisGetSourceRegister(opc);
  ULO dreg = cpuDisGetDestinationRegister(opc);
  ULO mode = (opc & 8)>>1;
  ULO adjw = memoryReadWord(prc + 2);
  STR tmp[16];

  cpuDisWordAppend(adjw, sdata);
  sprintf(sinstruction, "PACK");
  prc = cpuDisAdrMode(mode, sreg, prc + 4, 16, sdata, soperands);
  cpuDisCommaAppend(soperands);
  prc = cpuDisAdrMode(mode, dreg, prc, 16, sdata, soperands);
  sprintf(tmp, ",#$%.4X", adjw);
  strcat(soperands, tmp);
  return prc;
}

/* Disassemble for 030 */

static void cpuDisPflush030PrintFc(STR *soperands, ULO fcode)
{
  STR stmp[16];
  if (fcode == 0) strcat(soperands, "SFC,");
  else if (fcode == 1) strcat(soperands, "DFC,");
  else if ((fcode & 0x18) == 8)
  {
    sprintf(stmp, "D%u,", fcode & 7);
    strcat(soperands, stmp);
  }
  else if ((fcode & 0x18) == 0x10)
  {
    sprintf(stmp, "#%u,", fcode & 7);
    strcat(soperands, stmp);
  }
}

static ULO cpuDisPflush030(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  ULO eareg = cpuDisGetSourceRegister(opc);
  ULO eamode = cpuDisGetEaNo(cpuDisGetSourceMode(opc), eareg);
  ULO ext = memoryReadWord(prc + 2);
  ULO mode = (ext >> 10) & 7;
  ULO fcode = ext & 0x1f;
  ULO mask = ext & 0x1f;
  ULO op = (ext >> 13) & 7;
  STR stmp[16]; 

  cpuDisWordAppend(ext, sdata);
  if (op == 0x1)
  {
    if (mode != 0)
    {  /* PFLUSH */
      sprintf(sinstruction, "PFLUSH%s", (mode == 1) ? "A" : "");
      prc += 4;
      if (mode != 1)
      {
	cpuDisPflush030PrintFc(soperands, fcode);
	sprintf(stmp, "%.3X", mask);
	strcat(soperands, stmp);
	if (mode == 6)
	{
	  cpuDisCommaAppend(soperands);
	  prc = cpuDisAdrMode(eamode, eareg, prc, 16, sdata, soperands);
	}
      }
    }
    else
    { /* PLOAD */
      sprintf(sinstruction, "PLOAD%c", (ext & 0x200) ? 'R':'W');
      prc += 4;
      cpuDisPflush030PrintFc(soperands, fcode);
      cpuDisCommaAppend(soperands);
      prc = cpuDisAdrMode(eamode, eareg, prc, 16, sdata, soperands);
    }
  }
  else if (op == 4)
  { /* PTEST */
    sprintf(sinstruction, "PTEST");
    prc += 4;
    prc = cpuDisAdrMode(eamode, eareg, prc, 16, sdata, soperands);
  }
  else if (op == 0 || op == 2 || op == 3)
  { /* PMOVE */
    sprintf(sinstruction, "PMOVE");
    prc += 4;
    prc = cpuDisAdrMode(eamode, eareg, prc, 16, sdata, soperands);
  }
  return prc;
}

/* PFLUSH disassemble for 68040 */

static ULO cpuDisPflush040(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  ULO reg = cpuDisGetSourceRegister(opc);
  ULO opmode = (opc & 0x18)>>3;

  switch (opmode)
  {
  case 0:
    sprintf(sinstruction, "PFLUSHN");
    sprintf(soperands, "(A%u)", reg);
    break;
  case 1:
    sprintf(sinstruction, "PFLUSH");
    sprintf(soperands, "(A%u)", reg);
    break;
  case 2:
    sprintf(sinstruction, "PFLUSHAN");
    break;
  case 3:
    sprintf(sinstruction, "PFLUSHA");
    break;
  }      
  return prc + 2;
}

/* PTEST disassemble on 68040 */

static ULO cpuDisPtest040(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  ULO reg = cpuDisGetSourceRegister(opc);
  ULO rw = cpuDisGetBit(opc, 5);

  sprintf(sinstruction, "PTEST%c", (rw) ? 'R' : 'W');
  sprintf(soperands, "(A%u)", reg);
  return prc + 2;
}

static ULO cpuDisRtd(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  ULO extw = memoryReadWord(prc + 2);
  cpuDisWordAppend(extw, sdata);
  sprintf(sinstruction, "RTD");
  sprintf(soperands, "#%.4X", extw);
  return prc + 4;
}

static ULO cpuDisTrapcc(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  ULO bratype = cpuDisGetBranchType(opc);
  ULO ext = 0;
  ULO op = (opc & 7) - 2;

  sprintf(sinstruction, "TRAP%s", (bratype == 0) ? "T" : ((bratype == 1) ? "F" : cpu_dis_btab[bratype]));
  if (op == 0)
  {
    ext = memoryReadWord(prc + 2);
    prc += 2;
    cpuDisWordAppend(ext, sdata);
    strcat(sinstruction, ".W");
    sprintf(soperands, "#%.4X", ext);
  }
  else if (op == 1)
  {
    ext = memoryReadLong(prc + 2);
    prc += 4;
    cpuDisLongAppend(ext, sdata);
    strcat(sinstruction, ".L");
    sprintf(soperands, "#%.8X", ext);
  }
  return prc + 2;
}

static ULO cpuDisUnpk(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  ULO sreg = cpuDisGetSourceRegister(opc);
  ULO dreg = cpuDisGetDestinationRegister(opc);
  ULO mode = (opc & 8)>>1;
  ULO adjw = memoryReadWord(prc + 2);
  STR tmp[16];

  cpuDisWordAppend(adjw, sdata);
  sprintf(sinstruction, "UNPK");
  prc = cpuDisAdrMode(mode, sreg, prc + 4, 16, sdata, soperands);
  cpuDisCommaAppend(soperands);
  prc = cpuDisAdrMode(mode, dreg, prc, 16, sdata, soperands);
  sprintf(tmp, ",#$%.4X", adjw);
  strcat(soperands, tmp);
  return prc;
}

static ULO cpuDisCallm(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  ULO eareg = cpuDisGetSourceRegister(opc);
  ULO ext = memoryReadWord(prc + 2);
  cpuDisWordAppend(ext, sdata);
  sprintf(soperands, "#%u,", ext & 0xff);
  return cpuDisAdrMode(cpuDisGetEaNo(cpuDisGetSourceMode(opc), eareg), eareg, prc + 4, 16, sdata, soperands);
}

static ULO cpuDisRtm(ULO prc, UWO opc, STR *sdata, STR *sinstruction, STR *soperands)
{
  sprintf(sinstruction, "RTM");
  sprintf(soperands, "%c%u", (opc & 8) ? 'A':'D', cpuDisGetSourceRegister(opc));
  return prc + 2;
}

static cpuDisFunc cpu_dis_index[100] =
{
  cpuDisIllegal,  // 0
  cpuDisAbcd,
  cpuDisAdd,
  cpuDisAdda,
  cpuDisAddi,
  cpuDisAddq,	  // 5
  cpuDisAddx,
  cpuDisAnd,
  cpuDisAndi,
  cpuDisAsx,
  cpuDisBcc,	  // 10
  cpuDisBt,
  cpuDisBkpt,
  cpuDisBf,
  cpuDisCallm,
  cpuDisCas,	  // 15
  cpuDisChk,
  cpuDisChk2,
  cpuDisChkl,
  cpuDisClr,
  cpuDisCmp,	  // 20
  cpuDisCmpa,
  cpuDisCmpi,
  cpuDisCmpm,
  cpuDisDBcc,
  cpuDisDivl,	  // 25
  cpuDisDivs,
  cpuDisDivu,
  cpuDisEor,
  cpuDisEori,
  cpuDisExg,	  // 30
  cpuDisExt,
  cpuDisExtb,
  cpuDisJmp,
  cpuDisJsr,
  cpuDisLea,	  // 35
  cpuDisLink,
  cpuDisLinkl,
  cpuDisLsx,
  cpuDisMove,
  cpuDisMovea,	  // 40
  cpuDisMovec,
  cpuDisMoveFromCcr,
  cpuDisMoveFromSr,
  cpuDisMovem,
  cpuDisMovep,	  // 45
  cpuDisMoveq,
  cpuDisMoves,
  cpuDisMoveToCcr,
  cpuDisMoveToSr,
  cpuDisMoveUsp,  // 50
  cpuDisMull,
  cpuDisMuls,
  cpuDisMulu,
  cpuDisNbcd,
  cpuDisNeg,	  // 55
  cpuDisNegx,
  cpuDisNop,
  cpuDisNot,
  cpuDisOr,
  cpuDisOri,	  // 60
  cpuDisPack,
  cpuDisPea,
  cpuDisPflush030,
  cpuDisPflush040,
  cpuDisPtest040, // 65
  cpuDisReset,
  cpuDisRox,
  cpuDisRoxx,
  cpuDisRtd,
  cpuDisRte,	  // 70
  cpuDisRtm,
  cpuDisRtr,
  cpuDisRts,
  cpuDisSbcd,
  cpuDisScc,	  // 75
  cpuDisStop,
  cpuDisSub,
  cpuDisSuba,
  cpuDisSubi,
  cpuDisSubq,	  // 80
  cpuDisSubx,
  cpuDisSwap,
  cpuDisTas,
  cpuDisTrap,
  cpuDisTrapcc,	  // 85
  cpuDisTrapv,
  cpuDisTst,
  cpuDisUnlk,
  cpuDisUnpk	  // 89
};

ULO cpuDisOpcode(ULO disasm_pc, STR *saddress, STR *sdata, STR *sinstruction, STR *soperands)
{
  UWO opcode = (UWO) memoryReadWord(disasm_pc);
  sprintf(saddress, "$%.8X", disasm_pc);
  sprintf(sdata, "%.4X", opcode);
  return cpu_dis_index[cpu_dis_func_tab[opcode]](disasm_pc, opcode, sdata, sinstruction, soperands);
}
