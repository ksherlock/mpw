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
#include "CpuModule_Memory.h"
#include "CpuModule.h"
#include "CpuModule_DisassemblerFunc.h"

typedef uint32_t (*cpuDisFunc)(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands);  

static uint32_t cpuDisGetSourceMode(uint16_t opcode)
{
  return (uint32_t) (opcode >> 3) & 7;
}

static uint32_t cpuDisGetDestinationMode(uint16_t opcode)
{
  return (uint32_t) (opcode >> 6) & 7;
}

static uint32_t cpuDisGetSourceRegister(uint16_t opcode)
{
  return (uint32_t) (opcode & 7);
}

static uint32_t cpuDisGetDestinationRegister(uint16_t opcode)
{
  return (uint32_t) (opcode >> 9) & 7;
}

static uint32_t cpuDisGetEaNo(uint32_t eamode, uint32_t eareg)
{
  return (eamode < 7) ? eamode : (eamode + eareg);
}

static uint32_t cpuDisGetBit(uint16_t opcode, uint32_t bit)
{
  return (uint32_t) ((opcode >> bit) & 1);
}

static int32_t cpuDisGetLowByteSignExtend(uint16_t opc)
{
  return (int32_t)(int8_t)opc;
}

static uint32_t cpuDisGetSize(uint16_t opcode)
{
  uint32_t result = 0;
  switch ((opcode >> 6) & 3)
  {
  case 0: result = 8; break;
  case 1: result = 16; break;
  case 2: result = 32; break;
  case 3: result = 64; break;
  }
  return result;
}

static uint32_t cpuDisGetSize2(uint16_t opcode)
{
  uint32_t result = 0;
  switch ((opcode >> 8) & 1)
  {
  case 0: result = 16; break;
  case 1: result = 32; break;
  }
  return result;
}
static uint32_t cpuDisGetSize3(uint16_t opc)
{
  if ((opc & 0x3000) == 0x1000) return 8;
  else if ((opc & 0x3000) == 0x3000) return 16;
  return 32;
}

static uint32_t cpuDisGetSize4(uint16_t opc)
{
  if ((opc & 0x600) == 0x200) return 8;
  else if ((opc & 0x600) == 0x400) return 16;
  return 32;
}

static char cpuDisSizeChar(uint32_t size)
{
  return (char) ((size == 8) ? 'B' : ((size == 16) ? 'W' : 'L'));
}

static char *cpu_dis_btab[16] = {"RA", "SR", "HI", "LS", "CC", "CS", "NE", "EQ", "VC", "VS",
"PL", "MI", "GE", "LT", "GT", "LE"};

static uint32_t cpuDisGetBranchType(uint16_t opc)
{
  return (opc >> 8) & 0xf;
}

/*===========================================================================
Disassembly of the address-modes
Parameters:
uint32_t reg  - The register used in the address-mode
uint32_t pcp  - The address of the next byte not used.
char *st - The string to write the dissassembly to.
uint32_t *pos - The position in the string where the hex-words used are written
Returnvalue:
PC after possible extension words
===========================================================================*/

static char* cpuDisEoS(char *s)
{
  return s + strlen(s);
}

static void cpuDisCommaAppend(char *s)
{
  strcat(s, ",");
}

static void cpuDisWordAppend(uint32_t data, char *sdata)
{
  sprintf(cpuDisEoS(sdata), " %.4X", data);
}  

static void cpuDisLongAppend(uint32_t data, char *sdata)
{
  sprintf(cpuDisEoS(sdata), " %.8X", data);
}  

static uint32_t cpuDis05(uint32_t regno, uint32_t pcp, char *sdata, char *soperands)
{
  uint32_t disp = memoryReadWord(pcp);

  cpuDisWordAppend(disp, sdata);
  sprintf(cpuDisEoS(soperands), "$%.4X(A%1u)", disp, regno);
  return pcp + 2;
}

static uint32_t cpuDis06Brief(uint32_t regno, uint32_t pcp, uint32_t ext, BOOLE is_pc_indirect, char *sdata, char *soperands)
{
  char *scale[4] = {"", "*2", "*4", "*8"};
  char indexregtype = (char) ((ext & 0x8000) ? 'A' : 'D');
  char indexsize = (char) ((ext & 0x0800) ? 'L' : 'W');
  uint32_t indexregno = (ext >> 12) & 7;
  uint32_t offset = ext & 0xff;
  uint32_t scalefactor = (ext >> 9) & 3;

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

static uint32_t cpuDis06Od(uint32_t pcp, BOOLE wordsize, char *sdata, char *soperands)
{
  uint32_t od;

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

static uint32_t cpuDis06Extended(uint32_t regno, uint32_t pcp, uint32_t ext, BOOLE is_pc_indirect, char *sdata, char *soperands)
{
  char *scale[4] = {"", "*2", "*4", "*8"};
  char indexregtype = (char) ((ext & 0x8000) ? 'A' : 'D');
  char indexsize = (char)((ext & 0x0800) ? 'L' : 'W');
  uint32_t indexregno = (ext >> 12) & 7;
  uint32_t scalefactor = (ext >> 9) & 3;
  uint32_t iis = ext & 0x0007;
  uint32_t bdsize = (ext >> 4) & 3;
  uint32_t bd;
  BOOLE is = !!(ext & 0x0040);
  BOOLE bs = !!(ext & 0x0080);
  char baseregstr[32];
  char indexstr[32];
  char basedispstr[32];
  char outerdispstr[32];

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


static uint32_t cpuDis06(uint32_t regno, uint32_t pcp, char *sdata, char *soperands)
{
  uint32_t ext = memoryReadWord(pcp);

  if (cpuGetModelMajor() < 2 || !(ext & 0x0100))
    return cpuDis06Brief(regno, pcp + 2, ext, FALSE, sdata, soperands);
  else
    return cpuDis06Extended(regno, pcp + 2, ext, FALSE, sdata, soperands);
}

static uint32_t cpuDis70(uint32_t pcp, char *sdata, char *soperands)
{
  uint32_t absadr = memoryReadWord(pcp);

  cpuDisWordAppend(absadr, sdata);
  sprintf(cpuDisEoS(soperands), "$%.4X", absadr);
  return pcp + 2;
}

static uint32_t cpuDis71(uint32_t pcp, char *sdata, char *soperands)
{
  uint32_t absadr = memoryReadLong(pcp);

  cpuDisLongAppend(absadr, sdata);
  sprintf(cpuDisEoS(soperands), "$%.8X", absadr);
  return pcp + 4;
}

static uint32_t cpuDis72(uint32_t pcp, char *sdata, char *soperands)
{
  uint32_t disp = memoryReadWord(pcp);

  cpuDisWordAppend(disp, sdata);
  sprintf(cpuDisEoS(soperands), "$%.4X(PC)", disp);
  return pcp + 2;
}

static uint32_t cpuDis73(uint32_t pcp, char *sdata, char *soperands)
{
  uint32_t ext = memoryReadWord(pcp);

  if (cpuGetModelMajor() < 2 || !(ext & 0x0100))
    return cpuDis06Brief(0, pcp + 2, ext, TRUE, sdata, soperands);
  else
    return cpuDis06Extended(0, pcp + 2, ext, TRUE, sdata, soperands);
}

static uint32_t cpuDis74(uint32_t size, uint32_t pcp, char *sdata, char *soperands)
{
  uint32_t imm;

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

static void cpuDisRegCat(BOOLE is_datareg, uint32_t regno, char *soperands)
{
  size_t i = strlen(soperands);

  soperands[i++] = (char) ((is_datareg) ? 'D' : 'A');
  soperands[i++] = (char) (regno + 0x30);
  soperands[i] = '\0';
}

static void cpuDisIndRegCat(uint32_t mode, uint32_t regno, char *soperands)
{
  size_t i = strlen(soperands);

  if (mode == 4) soperands[i++] = '-';
  soperands[i++] = '(';
  soperands[i++] = 'A';
  soperands[i++] = (char) (regno + 0x30);
  soperands[i++] = ')';
  if (mode == 3) soperands[i++] = '+';
  soperands[i] = '\0';
}

static uint32_t cpuDisAdrMode(uint32_t eamode, uint32_t earegno, uint32_t pcp, uint32_t size, char *sdata, char *soperands)
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

static uint32_t cpu_dis_btX_trans[4] = {3, 0, 1, 2};
static char *cpu_dis_bnr[4] = {"CHG","CLR","SET","TST"};

static uint32_t cpuDisBtX(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  BOOLE is_reg = cpuDisGetBit(opc, 8);
  uint32_t eareg = cpuDisGetSourceRegister(opc);
  uint32_t eamode = cpuDisGetEaNo(cpuDisGetSourceMode(opc), eareg);
  uint32_t bitreg = cpuDisGetDestinationRegister(opc);
  char sizech = (eamode == 0) ? 'L' : 'B';

  sprintf(sinstruction, "B%s.%c", cpu_dis_bnr[cpu_dis_btX_trans[(opc >> 6) & 3]], sizech);
  if (!is_reg)
  {
    uint32_t imm = memoryReadWord(prc + 2);
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

static char *cpu_dis_anr[6] = {"ADD","SUB","CMP","AND","EOR","OR"};

static uint32_t cpuDisArith1(uint32_t prc, uint16_t opc, uint32_t nr, char *sdata, char *sinstruction, char *soperands)
{
  uint32_t eareg = cpuDisGetSourceRegister(opc);
  uint32_t dreg = cpuDisGetDestinationRegister(opc);
  uint32_t o = cpuDisGetBit(opc, 8);
  uint32_t size = cpuDisGetSize(opc);
  uint32_t eamode = cpuDisGetEaNo(cpuDisGetSourceMode(opc), eareg);

  sprintf(sinstruction, "%s.%c", cpu_dis_anr[nr], cpuDisSizeChar(size));
  prc = cpuDisAdrMode((o) ? 0 : eamode, (o) ? dreg : eareg, prc + 2, size, sdata, soperands);
  cpuDisCommaAppend(soperands);
  prc = cpuDisAdrMode((o) ? eamode : 0, (o) ? eareg : dreg, prc, size, sdata, soperands);
  return prc;
}

/* Common disassembly for ADDA, SUBA, CMPA */

static uint32_t cpuDisArith2(uint32_t prc, uint16_t opc, uint32_t nr, char *sdata, char *sinstruction, char *soperands)
{
  uint32_t eareg = cpuDisGetSourceRegister(opc);
  uint32_t dreg = cpuDisGetDestinationRegister(opc);
  uint32_t eamode = cpuDisGetEaNo(cpuDisGetSourceMode(opc), eareg);
  uint32_t size = cpuDisGetSize2(opc);

  sprintf(sinstruction, "%sA.%c", cpu_dis_anr[nr], cpuDisSizeChar(size));
  prc = cpuDisAdrMode(eamode, eareg, prc + 2, size, sdata, soperands);
  cpuDisCommaAppend(soperands);
  cpuDisAdrMode(1, dreg, prc, size, sdata, soperands);
  return prc;
}

/* Common disassembly for ADDI, SUBI, CMPI, ANDI, EORI, ORI */

static uint32_t cpuDisArith3(uint32_t prc, uint16_t opc, uint32_t nr, char *sdata, char *sinstruction, char *soperands)
{
  uint32_t eareg = cpuDisGetSourceRegister(opc);
  uint32_t eamode = cpuDisGetEaNo(cpuDisGetSourceMode(opc), eareg);
  uint32_t size = cpuDisGetSize(opc);

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

static uint32_t cpuDisArith4(uint32_t prc, uint16_t opc, uint32_t nr, char *sdata, char *sinstruction, char *soperands)
{
  uint32_t eareg = cpuDisGetSourceRegister(opc);
  uint32_t eamode = cpuDisGetEaNo(cpuDisGetSourceMode(opc), eareg);
  uint32_t size = cpuDisGetSize(opc);
  uint32_t imm = cpuDisGetDestinationRegister(opc);
  if (imm == 0)
  {
    imm = 8;
  }
  sprintf(sinstruction, "%sQ.%c", cpu_dis_anr[nr], cpuDisSizeChar(size));
  sprintf(soperands, "#$%.1u,", imm);
  return cpuDisAdrMode(eamode, eareg, prc + 2, size, sdata, soperands);
}

/* Common disassembly for ADDX, SUBX, ABCD, SBCD, CMPM */

static char *cpu_dis_a5nr[5] = {"ADDX","SUBX","ABCD","SBCD","CMPM"};

static uint32_t cpuDisArith5(uint32_t prc, uint16_t opc, uint32_t nr, char *sinstruction, char *soperands)
{
  uint32_t bit3 = cpuDisGetBit(opc, 3);
  char *minus = ((nr == 4) || !bit3) ? "" : "-";
  char *plus = ((nr == 4) && !bit3) ? "+" : "";

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

static char *cpu_dis_shnr[4] = {"AS", "LS", "RO", "ROX"};

static uint32_t cpuDisShift(uint32_t prc, uint16_t opc, uint32_t nr, char *sdata, char *sinstruction, char *soperands)
{
  uint32_t eareg = cpuDisGetSourceRegister(opc);
  uint32_t eamode = cpuDisGetEaNo(cpuDisGetSourceMode(opc), eareg);
  uint32_t size = cpuDisGetSize(opc);
  char rl = (char) ((cpuDisGetBit(opc, 8)) ? 'L' : 'R');

  if (size == 64)
  {
    sprintf(sinstruction, "%s%c.W", cpu_dis_shnr[nr], rl);
    sprintf(soperands, "#$1,");
    prc = cpuDisAdrMode(eamode, eareg, prc + 2, 16, sdata, soperands);
  }
  else
  {
    char sc = cpuDisSizeChar(size);
    uint32_t dreg = cpuDisGetDestinationRegister(opc);
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

static char *cpu_dis_unanr[10] = {"CLR","NEG","NOT","TST","JMP","JSR","PEA","TAS","NCBD","NEGX"};

static uint32_t cpuDisUnary(uint32_t prc, uint16_t opc, uint32_t nr, char *sdata, char *sinstruction, char *soperands)
{
  uint32_t eareg = cpuDisGetSourceRegister(opc);
  uint32_t eamode = cpuDisGetEaNo(cpuDisGetSourceMode(opc), eareg);
  uint32_t size = cpuDisGetSize(opc);
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

static char *cpu_dis_singnr[6] = {"NOP","RESET","RTE","RTR","RTS","TRAPV"};

static uint32_t cpuDisSingle(uint32_t prc, uint32_t nr, char *sinstruction)
{
  sprintf(sinstruction, "%s", cpu_dis_singnr[nr]);
  return prc + 2;
}

/* Common disassembly for CHK, DIVS, DIVU, LEA, MULS, MULU */

static char *cpu_dis_var1nr[6] = {"CHK","DIVS","DIVU","LEA","MULS","MULU"};

static uint32_t cpuDisVarious1(uint32_t prc, uint16_t opc, uint32_t nr, char *sdata, char *sinstruction, char *soperands)
{
  uint32_t eareg = cpuDisGetSourceRegister(opc);
  uint32_t eamode = cpuDisGetEaNo(cpuDisGetSourceMode(opc), eareg);
  uint32_t dreg = cpuDisGetDestinationRegister(opc);

  sprintf(sinstruction, "%s.%c", cpu_dis_var1nr[nr], (nr == 3) ? 'L' : 'W');
  prc = cpuDisAdrMode(eamode, eareg, prc + 2, 16, sdata, soperands);
  cpuDisCommaAppend(soperands);
  return cpuDisAdrMode((nr != 3) ? 0 : 1, dreg, prc, 16, sdata, soperands);
}

/* Common disassembly for SWAP, UNLK */

static char *cpu_dis_var2nr[2] = {"SWAP","UNLK"};

static uint32_t cpuDisVarious2(uint32_t prc, uint16_t opc, uint32_t nr, char *sinstruction, char *soperands)
{
  char regtype = (char) ((nr == 0) ? 'D' : 'A');
  sprintf(sinstruction, "%s", cpu_dis_var2nr[nr]);
  sprintf(soperands, "%c%1X", regtype, cpuDisGetSourceRegister(opc));
  return prc + 2;
}

static uint32_t cpuDisIllegal(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  sprintf(sinstruction, "ILLEGAL");
  return prc + 2;
}

static uint32_t cpuDisAbcd(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  return cpuDisArith5(prc, opc, 2, sinstruction, soperands);
}

static uint32_t cpuDisAdd(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  return cpuDisArith1(prc, opc, 0, sdata, sinstruction, soperands);
}

static uint32_t cpuDisAdda(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  return cpuDisArith2(prc, opc, 0, sdata, sinstruction, soperands);
}

static uint32_t cpuDisAddi(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  return cpuDisArith3(prc, opc, 0, sdata, sinstruction, soperands);
}

static uint32_t cpuDisAddq(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  return cpuDisArith4(prc, opc, 0, sdata, sinstruction, soperands);
}

static uint32_t cpuDisAddx(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  return cpuDisArith5(prc, opc, 0, sinstruction, soperands);
}

static uint32_t cpuDisAnd(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  return cpuDisArith1(prc, opc, 3, sdata, sinstruction, soperands);
}

static uint32_t cpuDisAndi(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  return cpuDisArith3(prc, opc, 3, sdata, sinstruction, soperands);
}

static uint32_t cpuDisAsx(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  return cpuDisShift(prc, opc, 0, sdata, sinstruction, soperands);
}

static uint32_t cpuDisBcc(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  uint32_t disp2;
  uint32_t adr;
  int32_t disp = cpuDisGetLowByteSignExtend(opc);

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
    prc += 2;
  }
  else
  {
    adr = prc + 2 + disp;
  }
  sprintf(cpuDisEoS(soperands), "$%8.8X", adr);
  return prc + 2;
}

static uint32_t cpuDisBt(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  return cpuDisBtX(prc, opc, sdata, sinstruction, soperands);
}

static uint32_t cpuDisChk(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  return cpuDisVarious1(prc, opc, 0, sdata, sinstruction, soperands);
}

static uint32_t cpuDisClr(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  return cpuDisUnary(prc, opc, 0, sdata, sinstruction, soperands);
}

static uint32_t cpuDisCmp(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  return cpuDisArith1(prc, opc, 2, sdata, sinstruction, soperands);
}  

static uint32_t cpuDisCmpa(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  return cpuDisArith2(prc, opc, 2, sdata, sinstruction, soperands);
}

static uint32_t cpuDisCmpi(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  return cpuDisArith3(prc, opc, 2, sdata, sinstruction, soperands);
}

static uint32_t cpuDisCmpm(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  return cpuDisArith5(prc, opc, 4, sinstruction, soperands);
}

static uint32_t cpuDisDBcc(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  uint32_t bratype = cpuDisGetBranchType(opc);

  prc += 2;
  uint32_t offset = memoryReadWord(prc);
  uint32_t adr = (offset > 32767) ? (prc + offset - 65536) : (prc + offset);
  cpuDisWordAppend(offset, sdata);
  sprintf(sinstruction, "DB%s", (bratype == 0) ? "T" : ((bratype == 1) ? "F" : cpu_dis_btab[bratype]));
  sprintf(soperands, "D%1u,$%6.6X", cpuDisGetSourceRegister(opc), adr);
  return prc + 2;
}

static uint32_t cpuDisDivs(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  return cpuDisVarious1(prc, opc, 1, sdata, sinstruction, soperands);
}

static uint32_t cpuDisDivu(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  return cpuDisVarious1(prc, opc, 2, sdata, sinstruction, soperands);
}

static uint32_t cpuDisEor(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  return cpuDisArith1(prc, opc, 4, sdata, sinstruction, soperands);
}

static uint32_t cpuDisEori(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  return cpuDisArith3(prc, opc, 4, sdata, sinstruction, soperands);
}

static uint32_t cpuDisExg(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  uint32_t o = (opc & 0x00f8) >> 3;

  sprintf(sinstruction, "EXG.L");
  sprintf(soperands, (o == 8) ? "D%d,D%d" : ((o == 9) ? "A%d,A%d" : "A%d,D%d"), cpuDisGetSourceRegister(opc), cpuDisGetDestinationRegister(opc));
  return prc + 2;
}

static uint32_t cpuDisExt(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  sprintf(sinstruction, "EXT.%c", (cpuDisGetBit(opc, 6) == 0) ? 'W' : 'L');
  sprintf(soperands, "D%u", cpuDisGetSourceRegister(opc));
  return prc + 2;
}

static uint32_t cpuDisJmp(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  return cpuDisUnary(prc, opc, 4, sdata, sinstruction, soperands);
}

static uint32_t cpuDisJsr(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  return cpuDisUnary(prc, opc, 5, sdata, sinstruction, soperands);
}

static uint32_t cpuDisLea(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  return cpuDisVarious1(prc, opc, 3, sdata, sinstruction, soperands);
}

static uint32_t cpuDisLink(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  uint32_t imm = memoryReadWord(prc + 2);

  cpuDisWordAppend(imm, sdata);
  sprintf(sinstruction, "LINK");
  sprintf(soperands, "A%1u,#$%.4X", cpuDisGetSourceRegister(opc), imm);
  return prc + 4;
}

static uint32_t cpuDisLsx(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  return cpuDisShift(prc, opc, 1, sdata, sinstruction, soperands);
}

static uint32_t cpuDisMove(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  uint32_t srcreg = cpuDisGetSourceRegister(opc);
  uint32_t srcmode = cpuDisGetEaNo(cpuDisGetSourceMode(opc), srcreg);
  uint32_t dstreg = cpuDisGetDestinationRegister(opc);
  uint32_t dstmode = cpuDisGetEaNo(cpuDisGetDestinationMode(opc), dstreg);
  uint32_t size = cpuDisGetSize3(opc);

  sprintf(sinstruction, "MOVE.%c", cpuDisSizeChar(size)); 
  prc = cpuDisAdrMode(srcmode, srcreg, prc + 2, size, sdata, soperands);
  cpuDisCommaAppend(soperands);
  return cpuDisAdrMode(dstmode, dstreg, prc, size, sdata, soperands);
}

static uint32_t cpuDisMoveToCcr(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  uint32_t eareg = cpuDisGetSourceRegister(opc);

  sprintf(sinstruction, "MOVE.B"); 
  prc = cpuDisAdrMode(cpuDisGetEaNo(cpuDisGetSourceMode(opc), eareg), eareg, prc + 2, 8, sdata, soperands);
  strcat(soperands, ",CCR");
  return prc;
}

static uint32_t cpuDisMoveToSr(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  uint32_t eareg = cpuDisGetSourceRegister(opc);

  sprintf(sinstruction, "MOVE.W"); 
  prc = cpuDisAdrMode(cpuDisGetEaNo(cpuDisGetSourceMode(opc), eareg), eareg, prc + 2, 16, sdata, soperands);
  strcat(soperands, ",SR");
  return prc;
}

static uint32_t cpuDisMoveFromSr(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  uint32_t eareg = cpuDisGetSourceRegister(opc);

  sprintf(sinstruction, "MOVE.W");
  sprintf(soperands, "SR,");
  prc = cpuDisAdrMode(cpuDisGetEaNo(cpuDisGetSourceMode(opc), eareg), eareg, prc + 2, 16, sdata, soperands);
  return prc;
}

static uint32_t cpuDisMoveUsp(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  sprintf(sinstruction, "MOVE.L");
  sprintf(soperands, (cpuDisGetBit(opc, 3)) ? "USP,A%1d" : "A%1d,USP", cpuDisGetSourceRegister(opc));
  return prc + 2;
}

static uint32_t cpuDisMovea(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  uint32_t eareg = cpuDisGetSourceRegister(opc);
  uint32_t dreg = cpuDisGetDestinationRegister(opc);
  uint32_t eamode = cpuDisGetEaNo(cpuDisGetSourceMode(opc), eareg);
  uint32_t size = cpuDisGetSize3(opc);

  sprintf(sinstruction, "MOVEA.%c", cpuDisSizeChar(size));
  prc = cpuDisAdrMode(eamode, eareg, prc + 2, size, sdata, soperands);
  cpuDisCommaAppend(soperands);
  return cpuDisAdrMode(1, dreg, prc, size, sdata, soperands);
}

static void cpuDisMovemRegmaskStrCat(uint32_t regmask, char *s, BOOLE predec)
{
  char tmp[2][16];

  for (uint32_t j = 0; j < 2; j++)
  {
    char* tmpp = tmp[j];
    for (uint32_t i = (8 * j); i < (8 + (8*j)); i++)
    {
      if (regmask & (1<<i))
      {
	*tmpp++ = (char) (0x30 + ((predec) ? ((7 + 8*j) - i) : (i - j*8)));
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

static uint32_t cpuDisMovem(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  uint32_t eareg = cpuDisGetSourceRegister(opc);
  uint32_t eamode = cpuDisGetEaNo(cpuDisGetSourceMode(opc), eareg);
  uint32_t dir = cpuDisGetBit(opc, 10);

  uint32_t size = (!cpuDisGetBit(opc, 6)) ? 16 : 32;
  uint32_t regmask = memoryReadWord(prc + 2);
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

static uint32_t cpuDisMovep(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  uint32_t adrregno = cpuDisGetSourceRegister(opc);
  uint32_t dataregno = cpuDisGetDestinationRegister(opc);
  uint32_t opmode = cpuDisGetDestinationMode(opc);
  char sizech = (opmode & 1) ? 'L' : 'W';
  BOOLE to_mem = (opmode & 2);
  uint32_t disp = memoryReadWord(prc + 2);

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

static uint32_t cpuDisMoveq(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  sprintf(sinstruction, "MOVEQ.L");
  sprintf(soperands, "#$%8.8X,D%u", cpuDisGetLowByteSignExtend(opc), cpuDisGetDestinationRegister(opc));
  return prc + 2;
}

static uint32_t cpuDisMuls(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  return cpuDisVarious1(prc, opc, 4, sdata, sinstruction, soperands);
}

static uint32_t cpuDisMulu(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  return cpuDisVarious1(prc, opc, 5, sdata, sinstruction, soperands);
}

static uint32_t cpuDisNbcd(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  return cpuDisUnary(prc, opc, 8, sdata, sinstruction, soperands);
}

static uint32_t cpuDisNeg(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  return cpuDisUnary(prc, opc, 1, sdata, sinstruction, soperands);
}

static uint32_t cpuDisNegx(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  return cpuDisUnary(prc, opc, 9, sdata, sinstruction, soperands);
}

static uint32_t cpuDisNop(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  return cpuDisSingle(prc, 0, sinstruction);
}

static uint32_t cpuDisNot(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  return cpuDisUnary(prc, opc, 2, sdata, sinstruction, soperands);
}

static uint32_t cpuDisOr(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  return cpuDisArith1(prc, opc, 5, sdata, sinstruction, soperands);
}

static uint32_t cpuDisOri(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  return cpuDisArith3(prc, opc, 5, sdata, sinstruction, soperands);
}

static uint32_t cpuDisPea(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  return cpuDisUnary(prc, opc, 6, sdata, sinstruction, soperands);
}

static uint32_t cpuDisReset(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  return cpuDisSingle(prc, 1, sinstruction);
}

static uint32_t cpuDisRox(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  return cpuDisShift(prc, opc, 2, sdata, sinstruction, soperands);
}

static uint32_t cpuDisRoxx(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  return cpuDisShift(prc, opc, 3, sdata, sinstruction, soperands);
}

static uint32_t cpuDisRte(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  return cpuDisSingle(prc, 2, sinstruction);
}

static uint32_t cpuDisRtr(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  return cpuDisSingle(prc, 3, sinstruction);
}

static uint32_t cpuDisRts(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  return cpuDisSingle(prc, 4, sinstruction);
}

static uint32_t cpuDisSbcd(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  return cpuDisArith5(prc, opc, 3, sinstruction, soperands);
}

static uint32_t cpuDisScc(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  uint32_t eareg = cpuDisGetSourceRegister(opc);
  uint32_t bratype = cpuDisGetBranchType(opc);

  sprintf(sinstruction, "S%s.B", (bratype == 0) ? "T" : ((bratype == 1) ? "F" : cpu_dis_btab[bratype]));
  return cpuDisAdrMode(cpuDisGetEaNo(cpuDisGetSourceMode(opc), eareg), eareg, prc + 2, 8, sdata, soperands);
}

static uint32_t cpuDisStop(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  uint32_t imm = memoryReadWord(prc + 2);
  cpuDisWordAppend(imm, sdata);
  sprintf(sinstruction, "STOP");
  sprintf(soperands, "#$%.4X", imm);
  return prc + 4;
}

static uint32_t cpuDisSub(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  return cpuDisArith1(prc, opc, 1, sdata, sinstruction, soperands);
}

static uint32_t cpuDisSuba(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  return cpuDisArith2(prc, opc, 1, sdata, sinstruction, soperands);
}

static uint32_t cpuDisSubi(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  return cpuDisArith3(prc, opc, 1, sdata, sinstruction, soperands);
}

static uint32_t cpuDisSubq(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  return cpuDisArith4(prc, opc, 1, sdata, sinstruction, soperands);
}

static uint32_t cpuDisSubx(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  return cpuDisArith5(prc, opc, 1, sinstruction, soperands);
}

static uint32_t cpuDisSwap(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  return cpuDisVarious2(prc, opc, 0, sinstruction, soperands);
}

static uint32_t cpuDisTas(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  return cpuDisUnary(prc, opc, 7, sdata, sinstruction, soperands);
}

static uint32_t cpuDisTrap(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  sprintf(sinstruction, "TRAP");
  sprintf(soperands, "#$%1X", opc & 0xf);
  return prc + 2;
}

static uint32_t cpuDisTrapv(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  return cpuDisSingle(prc, 5, sinstruction);
}

static uint32_t cpuDisTst(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  return cpuDisUnary(prc, opc, 3, sdata, sinstruction, soperands);
}

static uint32_t cpuDisUnlk(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  return cpuDisVarious2(prc, opc, 1, sinstruction, soperands);
}

static uint32_t cpuDisBkpt(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  sprintf(sinstruction, "BKPT");
  sprintf(soperands, "#$%1X", cpuDisGetSourceRegister(opc));
  return prc + 2;
}

static char *cpu_dis_bftxt[8] = {"TST ","EXTU","CHG ","EXTS","CLR ","FFO ","SET ","INS "};

static uint32_t cpuDisBf(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  uint32_t eareg = cpuDisGetSourceRegister(opc);
  uint32_t eamode = cpuDisGetEaNo(cpuDisGetSourceMode(opc), eareg);
  uint32_t ext = memoryReadWord(prc + 2);
  uint32_t n = (opc >> 8) & 7;
  uint32_t offset = (ext & 0x7c0) >> 6;
  uint32_t width = ext & 0x1f;
  char stmp[16];

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

static uint32_t cpuDisCas(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  uint32_t eareg = cpuDisGetSourceRegister(opc);
  uint32_t ext = memoryReadWord(prc + 2);
  uint32_t size = cpuDisGetSize4(opc);

  cpuDisWordAppend(ext, sdata);
  if ((opc & 0x3f) == 0x3c)
  { /* CAS2 */
    uint32_t ext2 = memoryReadWord(prc + 4);
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

static uint32_t cpuDisChkl(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  uint32_t eareg = cpuDisGetSourceRegister(opc);

  sprintf(sinstruction, "CHK.L");
  prc = cpuDisAdrMode(cpuDisGetEaNo(cpuDisGetSourceMode(opc), eareg), eareg, prc + 2, 32, sdata, soperands);
  cpuDisCommaAppend(soperands);
  return cpuDisAdrMode(0, cpuDisGetDestinationRegister(opc), prc + 2, 32, sdata, soperands);
}

static uint32_t cpuDisChk2(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  uint32_t eareg = cpuDisGetSourceRegister(opc);
  uint32_t ext = memoryReadWord(prc + 2);
  uint32_t size = cpuDisGetSize4(opc);
  char stmp[16];

  cpuDisWordAppend(ext, sdata);
  sprintf(sinstruction, "%s.%c", (ext & 0x800) ? "CHK2" : "CMP2", cpuDisSizeChar(size));
  prc = cpuDisAdrMode(cpuDisGetEaNo(cpuDisGetSourceMode(opc), eareg), eareg, prc + 4, size, sdata, soperands);
  sprintf(stmp, ",%s%u", (ext & 0x8000) ? "A" : "D", (ext>>12) & 7);
  strcat(soperands, stmp);
  return prc;
}

static uint32_t cpuDisDivl(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  uint32_t eareg = cpuDisGetSourceRegister(opc);
  uint32_t ext = memoryReadWord(prc + 2);
  char stmp[16];

  cpuDisWordAppend(ext, sdata);
  uint32_t dq = (ext >> 12) & 7;
  uint32_t dr = ext & 7;
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

static uint32_t cpuDisExtb(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  sprintf(sinstruction, "EXTB.L");
  sprintf(soperands, "D%u", cpuDisGetSourceRegister(opc));
  return prc + 2;
}

static uint32_t cpuDisLinkl(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  uint32_t disp = memoryReadLong(prc + 2);
  cpuDisLongAppend(disp, sdata);
  sprintf(sinstruction, "LINK.L");
  sprintf(soperands, "A%u, #$%.8X", cpuDisGetSourceRegister(opc), disp);
  return prc + 6;
}

static uint32_t cpuDisMoveFromCcr(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  uint32_t eareg = cpuDisGetSourceRegister(opc);
  sprintf(sinstruction, "MOVE.W");
  sprintf(soperands, "CCR,");
  return cpuDisAdrMode(cpuDisGetEaNo(cpuDisGetSourceMode(opc), eareg), eareg, prc + 2, 16, sdata, soperands);
}

static uint32_t cpuDisMovec(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  uint32_t extw = memoryReadWord(prc + 2);
  char stmp[16];

  cpuDisWordAppend(extw, sdata);
  sprintf(sinstruction, "MOVEC.L");
  if (opc & 1)
  { /* To control register */
    sprintf(stmp, "%s%u,", (extw & 0x8000) ? "A" : "D", (extw>>12) & 7); 
    strcat(soperands, stmp);
  }
  uint32_t creg = extw & 0xfff;
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

static uint32_t cpuDisMoves(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  uint32_t eareg = cpuDisGetSourceRegister(opc);
  uint32_t ext = memoryReadWord(prc + 2);
  uint32_t size = cpuDisGetSize(opc);
  char stmp[16];

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

static uint32_t cpuDisMull(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  uint32_t eareg = cpuDisGetSourceRegister(opc);
  uint32_t ext = memoryReadWord(prc + 2);
  char stmp[16];

  uint32_t dl = (ext >> 12) & 7;
  uint32_t dh = ext & 7;
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

static uint32_t cpuDisPack(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  uint32_t sreg = cpuDisGetSourceRegister(opc);
  uint32_t dreg = cpuDisGetDestinationRegister(opc);
  uint32_t mode = (opc & 8)>>1;
  uint32_t adjw = memoryReadWord(prc + 2);
  char tmp[16];

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

static void cpuDisPflush030PrintFc(char *soperands, uint32_t fcode)
{
  char stmp[16];
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

static uint32_t cpuDisPflush030(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  uint32_t eareg = cpuDisGetSourceRegister(opc);
  uint32_t eamode = cpuDisGetEaNo(cpuDisGetSourceMode(opc), eareg);
  uint32_t ext = memoryReadWord(prc + 2);
  uint32_t mode = (ext >> 10) & 7;
  uint32_t fcode = ext & 0x1f;
  uint32_t mask = ext & 0x1f;
  uint32_t op = (ext >> 13) & 7;
  char stmp[16]; 

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

static uint32_t cpuDisPflush040(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  uint32_t reg = cpuDisGetSourceRegister(opc);
  uint32_t opmode = (opc & 0x18)>>3;

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

static uint32_t cpuDisPtest040(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  uint32_t reg = cpuDisGetSourceRegister(opc);
  uint32_t rw = cpuDisGetBit(opc, 5);

  sprintf(sinstruction, "PTEST%c", (rw) ? 'R' : 'W');
  sprintf(soperands, "(A%u)", reg);
  return prc + 2;
}

static uint32_t cpuDisRtd(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  uint32_t extw = memoryReadWord(prc + 2);
  cpuDisWordAppend(extw, sdata);
  sprintf(sinstruction, "RTD");
  sprintf(soperands, "#%.4X", extw);
  return prc + 4;
}

static uint32_t cpuDisTrapcc(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  uint32_t bratype = cpuDisGetBranchType(opc);
  uint32_t ext = 0;
  uint32_t op = (opc & 7) - 2;

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

static uint32_t cpuDisUnpk(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  uint32_t sreg = cpuDisGetSourceRegister(opc);
  uint32_t dreg = cpuDisGetDestinationRegister(opc);
  uint32_t mode = (opc & 8)>>1;
  uint32_t adjw = memoryReadWord(prc + 2);
  char tmp[16];

  cpuDisWordAppend(adjw, sdata);
  sprintf(sinstruction, "UNPK");
  prc = cpuDisAdrMode(mode, sreg, prc + 4, 16, sdata, soperands);
  cpuDisCommaAppend(soperands);
  prc = cpuDisAdrMode(mode, dreg, prc, 16, sdata, soperands);
  sprintf(tmp, ",#$%.4X", adjw);
  strcat(soperands, tmp);
  return prc;
}

static uint32_t cpuDisCallm(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
{
  uint32_t eareg = cpuDisGetSourceRegister(opc);
  uint32_t ext = memoryReadWord(prc + 2);
  cpuDisWordAppend(ext, sdata);
  sprintf(soperands, "#%u,", ext & 0xff);
  return cpuDisAdrMode(cpuDisGetEaNo(cpuDisGetSourceMode(opc), eareg), eareg, prc + 4, 16, sdata, soperands);
}

static uint32_t cpuDisRtm(uint32_t prc, uint16_t opc, char *sdata, char *sinstruction, char *soperands)
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

uint32_t cpuDisOpcode(uint32_t disasm_pc, char *saddress, char *sdata, char *sinstruction, char *soperands)
{
  uint16_t opcode = (uint16_t) memoryReadWord(disasm_pc);
  sprintf(saddress, "$%.8X", disasm_pc);
  sprintf(sdata, "%.4X", opcode);
  return cpu_dis_index[cpu_dis_func_tab[opcode]](disasm_pc, opcode, sdata, sinstruction, soperands);
}
